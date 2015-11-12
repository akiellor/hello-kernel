#include "./serial.h"
#include "./idt.h"

/* These are function prototypes for all of the exception
*  handlers: The first 32 entries in the IDT are reserved
*  by Intel, and are designed to service exceptions! */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

/* This is a very repetitive function... it's not hard, it's
*  just annoying. As you can see, we set the first 32 entries
*  in the IDT to the first 32 ISRs. We can't use a for loop
*  for this, because there is no way to get the function names
*  that correspond to that given entry. We set the access
*  flags to 0x8E. This means that the entry is present, is
*  running in ring 0 (kernel level), and has the lower 5 bits
*  set to the required '14', which is represented by 'E' in
*  hex. */
void isrs_install()
{
    idt_set_gate(0, isr0, 0x08, 0x8E);
    idt_set_gate(1, isr1, 0x08, 0x8E);
    idt_set_gate(2, isr2, 0x08, 0x8E);
    idt_set_gate(3, isr3, 0x08, 0x8E);
    idt_set_gate(4, isr4, 0x08, 0x8E);
    idt_set_gate(5, isr5, 0x08, 0x8E);
    idt_set_gate(6, isr6, 0x08, 0x8E);
    idt_set_gate(7, isr7, 0x08, 0x8E);

    idt_set_gate(8, isr8, 0x08, 0x8E);
    idt_set_gate(9, isr9, 0x08, 0x8E);
    idt_set_gate(10, isr10, 0x08, 0x8E);
    idt_set_gate(11, isr11, 0x08, 0x8E);
    idt_set_gate(12, isr12, 0x08, 0x8E);
    idt_set_gate(13, isr13, 0x08, 0x8E);
    idt_set_gate(14, isr14, 0x08, 0x8E);
    idt_set_gate(15, isr15, 0x08, 0x8E);

    idt_set_gate(16, isr16, 0x08, 0x8E);
    idt_set_gate(17, isr17, 0x08, 0x8E);
    idt_set_gate(18, isr18, 0x08, 0x8E);
    idt_set_gate(19, isr19, 0x08, 0x8E);
    idt_set_gate(20, isr20, 0x08, 0x8E);
    idt_set_gate(21, isr21, 0x08, 0x8E);
    idt_set_gate(22, isr22, 0x08, 0x8E);
    idt_set_gate(23, isr23, 0x08, 0x8E);

    idt_set_gate(24, isr24, 0x08, 0x8E);
    idt_set_gate(25, isr25, 0x08, 0x8E);
    idt_set_gate(26, isr26, 0x08, 0x8E);
    idt_set_gate(27, isr27, 0x08, 0x8E);
    idt_set_gate(28, isr28, 0x08, 0x8E);
    idt_set_gate(29, isr29, 0x08, 0x8E);
    idt_set_gate(30, isr30, 0x08, 0x8E);
    idt_set_gate(31, isr31, 0x08, 0x8E);
}

/* This is a simple string array. It contains the message that
*  corresponds to each and every exception. We get the correct
*  message by accessing like:
*  exception_message[interrupt_number] */
char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

/* All of our Exception handling Interrupt Service Routines will
*  point to this function. This will tell us what exception has
*  happened! Right now, we simply halt the system by hitting an
*  endless loop. All ISRs disable interrupts while they are being
*  serviced as a 'locking' mechanism to prevent an IRQ from
*  happening and messing up kernel data structures */
void fault_handler(struct regs *r)
{
    if (r->int_no < 32)
    {
        serial_writestring(exception_messages[r->int_no]);
        serial_writestring(" Exception. System Halted!\n");
        for (;;);
    }
}
