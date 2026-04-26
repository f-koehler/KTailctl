#ifndef KTAILCTL_LOGGING_H
#define KTAILCTL_LOGGING_H

#if defined(__GNUC__) || defined(__clang__)
#define KTAILCTL_WRAPPER_EXPORT __attribute__((visibility("default")))
#else
#define KTAILCTL_WRAPPER_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

KTAILCTL_WRAPPER_EXPORT void ktailctl_critical(const char *message);
KTAILCTL_WRAPPER_EXPORT void ktailctl_debug(const char *message);
KTAILCTL_WRAPPER_EXPORT void ktailctl_info(const char *message);
KTAILCTL_WRAPPER_EXPORT void ktailctl_warning(const char *message);

#ifdef __cplusplus
}
#endif

#endif /* KTAILCTL_LOGGING_H */
