#ifndef LOG_H
#define LOG_H

enum logLevel {
    LOG_NONE,
    LOG_INFO,
    LOG_SYSTEM,
    LOG_WARN,
    LOG_ERROR,
};

void kwritel(const char *msg, enum logLevel lvl);
void kprintl(char *msg, enum logLevel lvl);

void kwrite(const char *msg, int color);
void kprint(char *msg, int color);

#endif