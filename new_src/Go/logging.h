#ifndef KTAILCTL_GO_LOGGING_H
#define KTAILCTL_GO_LOGGING_H

extern "C" {
extern void ktailctl_critical(const char *message);
extern void ktailctl_debug(const char *message);
extern void ktailctl_info(const char *message);
extern void ktailctl_warning(const char *message);
}

#endif /* KTAILCTL_GO_LOGGING_H */
