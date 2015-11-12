#ifndef _IDT_H
#define _IDT_H 1

/* This defines what the stack looks like after an ISR was running */
struct regs
{
  unsigned int gs, fs, es, ds;      /* pushed the segs last */
  unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
  unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
  unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

void idt_install();
void idt_set_gate(unsigned char num, void (*handler)(), unsigned short sel, unsigned char flags);


#endif
