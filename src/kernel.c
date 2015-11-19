#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "./serial.h"
#include "./gdt.h"
#include "./idt.h"
#include "./isrs.h"
#include "./irq.h"
#include "./framebuffer.h"
#include "./io.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

void keyboard_handler_main(void) {
  unsigned char status;
  char keycode;

  /* write EOI */
  outb(0x20, 0x20);

  status = inb(KEYBOARD_STATUS_PORT);
  /* Lowest bit of status will be set if buffer is not empty */
  if (status & 0x01) {
    keycode = inb(KEYBOARD_DATA_PORT);
    if(keycode < 0) {
      return;
    } else {
      char str[] = { keycode };
      terminal_writestring(str);
    }
  }
}

extern void keyboard_handler();

void kb_init(void) {
    /* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
    outb(0x21 , 0xFD);
}

void kernel_main() {
  gdt_install();
  idt_install();
  isrs_install();
  irq_remap();
  idt_set_gate(0x21, keyboard_handler, 0x08, 0x8e);
  init_serial();
  kb_init();
	terminal_initialize();

  while(1);
}

