#include <kernel/log.h>
#include <kernel/tty.h>

void kwritel(const char *msg, enum logLevel lvl)
{
    if (lvl != LOG_NONE || !lvl)
    {
        kwrite("[", 0x0F);
        switch (lvl)
        {
            case LOG_INFO:
                kwrite("INFO", 0x03);
                break;
            case LOG_SYSTEM:
                kwrite("SYSTEM", 0x05);
                break;
            case LOG_WARN:
                kwrite("WARN", 0x0E);
                break;
            case LOG_ERROR:
                kwrite("ERROR", 0x04);
                break;
            default:
                break;
        }
        kwrite("]: ", 0x0F);
    }
    kwrite(msg, 0x0F);
}

void kprintl(char *msg, enum logLevel lvl)
{
    kwritel(msg, lvl);
    kwrite("\n\r", 0x0F);
}

void kwrite(const char *msg, int color)
{
    term_setcolor(color);
    term_writestring(msg);
    term_setcolor(0x0F);
}

void kprint(char *msg, int color)
{
    kwrite(msg, color);
    kwrite("\n\r", color);
}