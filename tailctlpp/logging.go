package main

// #include <stdlib.h>
// #include "logging.h"
// #cgo LDFLAGS: -l/workspaces/KTailctl/build/lib/libtailctlpp_logging.a -latomic
import "C"
import (
	"fmt"
	"unsafe"
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
