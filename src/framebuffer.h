#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H 1

void terminal_initialize();
void terminal_writestring(const char* data);
void terminal_putchar(char data);

#endif
