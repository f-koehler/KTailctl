// Stub implementations of the Go FFI functions for test_status_json.cpp test
#include <cstdlib>
#include <cstring>
#include <string_view>

namespace
{

// Allocates a copy of a string on the heap, the caller is responsible for calling std::free() on the result.
[[nodiscard]] char *mallocString(std::string_view string)
{
    // NOLINTNEXTLINE(cppcoreguidelines-no-malloc)
    char *buf = static_cast<char *>(std::malloc(string.size() + 1));
    if (buf != nullptr) {
        std::memcpy(buf, string.data(), string.size() + 1);
    }
    return buf;
}

} // namespace

extern "C" {

char *tailscale_status()
{
    return mallocString("{}");
}

char *tailscale_suggest_exit_node()
{
    return mallocString("");
}

} // extern "C"
