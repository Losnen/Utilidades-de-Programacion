#test_bit_64.s
#
.section .data

.section .text

.global test_bit_64 

.type test_bit_64, @function
test_bit_64:

 movq   %rsi, %rax           # Introduce posición en el acumulador
 movq   $0, %rdx
 movq   $64, %r8             # Almacena el divisor
 divq   %r8                  # Computar base y desplazamiento resto en dx
                             # cociente en ax

 leaq   (%rdi, %rax, 8), %rbx   # Calcula la posición dentro del array
 movq   (%rbx), %rcx         # Carga la posición en el registro cx

 btq    %rdx, %rcx           # Pregunta por el bit

 movq   $1, %rax             # Establece a 1 el acumulador si
 jc     is_set               # Salta al final si el carry está a 1
 movq   $0, %rax             # En caso contrario retorna cero
is_set:
 ret

