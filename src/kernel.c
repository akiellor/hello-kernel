#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
 
/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

#include "./serial.h"
#include "./gdt.h"
#include "./idt.h"
#include "./isrs.h"
#include "./irq.h"
#include "./framebuffer.h"
#include "./io.h"

#define IDT_SIZE 256

struct IDT_entry{
  unsigned short int offset_lowerbits;
  unsigned short int selector;
  unsigned char zero;
  unsigned char type_attr;
  unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];

extern void idt_load();

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
      write_serial(keycode);
    }
  }
}

extern void keyboard_handler();

void idt_init(void)
{
  unsigned long keyboard_address;
  unsigned long idt_address;
  unsigned long idt_ptr[2];

  /* populate IDT entry of keyboard's interrupt */
  keyboard_address = (unsigned long)keyboard_handler; 
  IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
  IDT[0x21].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
  IDT[0x21].zero = 0;
  IDT[0x21].type_attr = 0x8e; /* INTERRUPT_GATE */
  IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;
  

  /*     Ports
  *  PIC1 PIC2
  *Command 0x20 0xA0
  *Data  0x21 0xA1
  */

  /* ICW1 - begin initialization */
  outb(0x20 , 0x11);
  outb(0xA0 , 0x11);

  /* ICW2 - remap offset address of IDT */
  /*
  * In x86 protected mode, we have to remap the PICs beyond 0x20 because
  * Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
  */
  outb(0x21 , 0x20);
  outb(0xA1 , 0x28);

  /* ICW3 - setup cascading */
  outb(0x21 , 0x00);  
  outb(0xA1 , 0x00);  

  /* ICW4 - environment info */
  outb(0x21 , 0x01);
  outb(0xA1 , 0x01);
  /* Initialization finished */

  /* mask interrupts */
  outb(0x21 , 0xff);
  outb(0xA1 , 0xff);

  /* fill the IDT descriptor */
  idt_address = (unsigned long)IDT ;
  idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
  idt_ptr[1] = idt_address >> 16 ;

  idt_load(idt_ptr);
}

void kb_init(void) {
    /* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
    outb(0x21 , 0xFD);
}

void new_keyboard_handler(struct regs *r) {
  unsigned char status;
  char keycode;

  status = inb(KEYBOARD_STATUS_PORT);
  /* Lowest bit of status will be set if buffer is not empty */
  if (status & 0x01) {
    keycode = inb(KEYBOARD_DATA_PORT);
    if(keycode < 0) {
      return;
    } else {
      write_serial(keycode);
    }
  }
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

