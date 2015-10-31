extern kernel_main
global start

MBALIGN  equ 1 << 0
MEMINFO  equ 1 << 1
FLAGS    equ MBALIGN | MEMINFO
MAGIC    equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
  align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bootstrap_stack
stack_bottom:
    resb 64
stack_top:

start:
  mov esp, stack_top

  call kernel_main

  cli
.loop:
  hlt
  jmp .loop 


