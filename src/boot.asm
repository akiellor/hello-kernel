global start
section .text
bits 32
start:
  mov esp, stack_top
  extern kernel_main
  call kernel_main
  hlt

section .bss
stack_bottom:
    resb 64
stack_top:
