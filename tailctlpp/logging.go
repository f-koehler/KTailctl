package main

// #include <stdlib.h>
// #include "logging.h"
// #cgo LDFLAGS: -l/home/fkoehler/code/tailctl/build/lib/libtailctlpp_logging.a
import "C"
import (
	"unsafe"
	"fmt"
)

func log_critical(message string) {
	msg := C.CString(message)
	defer C.free(unsafe.Pointer(msg))
	C.ktailctl_critical(msg)
}

func log_debug(message string) {
	msg := C.CString(message)
	defer C.free(unsafe.Pointer(msg))
	C.ktailctl_debug(msg)
}

func log_info(message string) {
	msg := C.CString(message)
	defer C.free(unsafe.Pointer(msg))
	C.ktailctl_info(msg)
}

func log_warning(message string) {
	msg := C.CString(message)
	defer C.free(unsafe.Pointer(msg))
	C.ktailctl_warning(msg)
}

func log_critical_error(err error) {
	msg := C.CString(fmt.Sprintln(err))
	defer C.free(unsafe.Pointer(msg))
	C.ktailctl_critical(msg)
}
