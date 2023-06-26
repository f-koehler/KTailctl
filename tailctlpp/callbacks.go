package main

// typedef void (*tailscale_send_file_callback)(unsigned long);
// void tailscale_bridge_send_file_callback(tailscale_send_file_callback cb, unsigned long bytes_sent)
// {
//     return cb(bytes_sent);
// }
import "C"
