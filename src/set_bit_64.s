#set_bit_64.s
#
.section .data

.section .text

.global set_bit_64 

.type set_bit_64, @function
set_bit_64:

 movq   %rsi, %rax           # Introduce posición en el acumulador
 movq   $0, %rdx
 movq   $64, %r8             # Almacena el divisor
 divq   %r8                  # Computar base y desplazamiento resto en dx
                             # cociente en ax

 leaq   (%rdi, %rax, 8), %rbx   # Calcula la posición dentro del array
 movq   (%rbx), %rcx         # Carga la posición en el registro cx

 btsq   %rdx, %rcx           # Establece a uno el bit seleccionado

 movq   %rcx, (%rbx)         # Almacena la palabra modificada en su posición

 ret



