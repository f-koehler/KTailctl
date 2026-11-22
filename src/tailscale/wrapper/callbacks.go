package main

// Bridges that let Go invoke C function pointers handed over from the C++ side.
//
// cgo cannot call a C function pointer directly, and files containing //export
// directives may only declare (not define) C functions in their preamble, so the
// definitions live here.

// #include <stdint.h>
// typedef void (*tailscale_taildrop_progress_callback)(void *userdata, int64_t bytes_sent);
// void tailscale_bridge_taildrop_progress_callback(tailscale_taildrop_progress_callback cb, void *userdata, int64_t bytes_sent)
// {
//     cb(userdata, bytes_sent);
// }
import "C"
