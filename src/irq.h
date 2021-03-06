#ifndef _IRQ_H
#define _IRQ_H 1

void irq_remap();
void irq_install();
void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);

#endif
