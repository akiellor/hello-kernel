 #ifndef _SERIAL_H
#define _SERIAL_H 1

#include <stddef.h>
#include <stdint.h>

void init_serial();

int serial_received();

char read_serial();

int is_transmit_empty();

void write_serial(char a);

#endif
