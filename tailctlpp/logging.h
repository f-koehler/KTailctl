#ifndef KTAILCTL_LOGGING_H
#define KTAILCTL_LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

void ktailctl_critical(const char *message);
void ktailctl_debug(const char *message);
void ktailctl_info(const char *message);
void ktailctl_warning(const char *message);

#ifdef __cplusplus
}
#endif

#endif /* KTAILCTL_LOGGING_H */
