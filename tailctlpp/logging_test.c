#include "logging.h"
#include <stdlib.h>

int main()
{
    ktailctl_critical("Critical message");
    ktailctl_debug("Debug message");
    ktailctl_info("Info message");
    ktailctl_warning("Warning message");
    return EXIT_SUCCESS;
}