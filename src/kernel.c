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

unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
  '9', '0', '-', '=', '\b', /* Backspace */
  '\t',     /* Tab */
  'q', 'w', 'e', 'r', /* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0,      /* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
 '\'', '`',   0,    /* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',      /* 49 */
  'm', ',', '.', '/',   0,        /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

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
      terminal_putchar(kbdus[keycode]);
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

