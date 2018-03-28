#ifndef UTIL_H_
#define UTIL_H_

/* The number of columns. */
#define COLUMNS                 80
/* The number of lines. */
#define LINES                   24
/* The attribute of an character. */
#define ATTRIBUTE               7
/* The video memory address. */
#define VIDEO                   0xC00B8000

void clear_scr (void);
void printf (const char *format, ...);
void puts(const char *ptr_str);

#endif
