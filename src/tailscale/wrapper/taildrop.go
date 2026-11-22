package main

// #include <stdint.h>
// typedef void (*tailscale_taildrop_progress_callback)(void *userdata, int64_t bytes_sent);
// extern void tailscale_bridge_taildrop_progress_callback(tailscale_taildrop_progress_callback cb, void *userdata, int64_t bytes_sent);
import "C"

import (
	"context"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"sync"
	"unsafe"

	"tailscale.com/tailcfg"
)

// Taildrop send jobs are identified by an ID chosen by the C++ side. A single
// job may consist of several files sent one after another, all sharing one
// cancellable context.
//
// Contexts are created lazily by whichever of tailscale_taildrop_send_file or
// tailscale_taildrop_cancel runs first, so cancelling a job before (or while)
// its first file starts is race-free. The C++ side must call
// tailscale_taildrop_release once the job is done to free the entry.
type taildropJob struct {
	ctx    context.Context
	cancel context.CancelFunc
}

var (
	taildropJobsMutex sync.Mutex
	taildropJobs      = map[uint64]*taildropJob{}
)

func taildropJobFor(jobID uint64) *taildropJob {
	taildropJobsMutex.Lock()
	defer taildropJobsMutex.Unlock()
	job, ok := taildropJobs[jobID]
	if !ok {
		ctx, cancel := context.WithCancel(context.Background())
		job = &taildropJob{ctx: ctx, cancel: cancel}
		taildropJobs[jobID] = job
	}
	return job
}

// progressReader counts the bytes consumed from the underlying reader and
// reports the running total for the current file through a C callback.
//
// Note that this measures how much of the file has been handed to the local
// tailscaled, which is a close approximation of the transfer progress since the
// daemon streams the body straight to the peer.
type progressReader struct {
	reader   io.Reader
	total    int64
	callback C.tailscale_taildrop_progress_callback
	userdata unsafe.Pointer
}

func (r *progressReader) Read(buf []byte) (int, error) {
	n, err := r.reader.Read(buf)
	if n > 0 {
		r.total += int64(n)
		if r.callback != nil {
			C.tailscale_bridge_taildrop_progress_callback(r.callback, r.userdata, C.int64_t(r.total))
		}
	}
	return n, err
}

func sendFile(ctx context.Context, peerID, path string, reader *progressReader) error {
	file, err := os.Open(filepath.Clean(path))
	if err != nil {
		return fmt.Errorf("failed to open file: %w", err)
	}
	defer func() { _ = file.Close() }()

	stat, err := file.Stat()
	if err != nil {
		return fmt.Errorf("failed to stat file: %w", err)
	}
	if !stat.Mode().IsRegular() {
		return fmt.Errorf("%s is not a regular file", path)
	}

	size := stat.Size()
	reader.reader = io.LimitReader(file, size)

	if err := client.PushFile(ctx, tailcfg.StableNodeID(peerID), size, filepath.Base(path), reader); err != nil {
		if errors.Is(err, context.Canceled) || ctx.Err() != nil {
			return errors.New("transfer cancelled")
		}
		return fmt.Errorf("failed to send file: %w", err)
	}
	return nil
}

// tailscale_taildrop_send_file sends a single file via Taildrop to the peer with the given stable node ID.
//
// The call blocks until the transfer is done and must therefore not be made from the UI thread. While the file is uploaded, callback (if not NULL) is invoked from a Go-managed thread with userdata and the number of bytes of this file sent so far.
//
// Returns NULL on success, or a malloc'd error message that the caller must free.
//
//export tailscale_taildrop_send_file
func tailscale_taildrop_send_file(jobID uint64, peerID *string, path *string, callback C.tailscale_taildrop_progress_callback, userdata unsafe.Pointer) *C.char {
	if peerID == nil || *peerID == "" {
		return C.CString("no peer given")
	}
	if path == nil || *path == "" {
		return C.CString("no file given")
	}

	job := taildropJobFor(jobID)
	if job.ctx.Err() != nil {
		return C.CString("transfer cancelled")
	}

	reader := &progressReader{callback: callback, userdata: userdata}
	if err := sendFile(job.ctx, *peerID, *path, reader); err != nil {
		log_warning(fmt.Sprintf("taildrop: sending %s to %s failed: %v", *path, *peerID, err))
		return C.CString(err.Error())
	}
	log_info(fmt.Sprintf("taildrop: sent %s to %s", *path, *peerID))
	return nil
}

// tailscale_taildrop_cancel aborts the running and all future transfers of the given job.
//
//export tailscale_taildrop_cancel
func tailscale_taildrop_cancel(jobID uint64) {
	taildropJobFor(jobID).cancel()
}

// tailscale_taildrop_release frees the bookkeeping of a finished job. It must be called once no more tailscale_taildrop_send_file calls will be made for the job.
//
//export tailscale_taildrop_release
func tailscale_taildrop_release(jobID uint64) {
	taildropJobsMutex.Lock()
	defer taildropJobsMutex.Unlock()
	if job, ok := taildropJobs[jobID]; ok {
		job.cancel()
		delete(taildropJobs, jobID)
	}
}
