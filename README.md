# Aboloka-8 Specifications

```
.==============================================================================.
| Encoding | Description                                                       |
|==========|===================================================================|
| 00rrrmmm | dst = reg[R]                                                      |
| iiiiiiii | src = reg[M] + I or I depending on the operation.                 |
|----------|-------------------------------------------------------------------|
| 01rrrmmm | dst = reg[R]                                                      |
| iiiiiiii | src = mem[S:(reg[M] + I)]                                         |
|----------|-------------------------------------------------------------------|
| 10rrrmmm | dst = mem[S:(reg[M] + I)]                                         |
| iiiiiiii | src = reg[R]                                                      |
|----------|-------------------------------------------------------------------|
| 11rrrmmm | dst = reg[R]                                                      |
|          | src = reg[M]                                                      |
'=============================================================================='

.==============================================================================.
| Encoding | Assembly-like Syntax | Description                                |
|==========|======================|============================================|
| 0 000    | AX or AX'            | Accumulator register.                      |
| 0 001    | BX or BX'            | Data register 0.                           |
| 0 010    | CX or CX'            | Data register 1.                           |
| 0 011    | DX or DX'            | Data register 2.                           |
| 0 100    | SP or SP'            | Stack pointer.                             |
| 0 101    | BP or BP'            | Base pointer.                              |
| 0 110    | DI or DI'            | Destination pointer/index.                 |
| 0 111    | SI or SI'            | Source pointer/index.                      |
|----------|----------------------|--------------------------------------------|
| 1 000    | CS or CS'            | Code segment.                              |
| 1 001    | DS or DS'            | Data segment.                              |
| 1 010    | SS or SS'            | Stack segment.                             |
| 1 011    | ES or ES'            | Extra (data/code) segment.                 |
|----------|----------------------|--------------------------------------------|
| 1 100    | IDT                  | Interrupt descriptors table register.      |
| 1 101    | MDT                  | Memory map descriptors table register.     |
| 1 110    | MRR                  | Memory refresh register.                   |
| 1 111    | CSR                  | Control and status register.               |
|----------|----------------------|--------------------------------------------|
| - ---    | IP                   | Instruction pointer.                       |
| - ---    | MCR                  | Memory refresh counter register.           |
| - ---    | IRR0, ...IRR7        | Interrupt requests registers.              |
| - ---    | IMR0, ...IMR7        | Interrupt masks registers.                 |
| - ---    | CYR0, ...CYR3        | Cycles registers.                          |
'=============================================================================='

.==============================================================================.
| Encoding | Assembly-like Syntax | Description                                |
|==========|======================|============================================|
| 00000001 | CF                   | Carry flag is set if an unsigned operation |
|          |                      | generates a carry.                         |
|----------|----------------------|--------------------------------------------|
| 00000010 | ZF                   | Zero flag is set if an operation set the   |
|          |                      | destination to zero.                       |
|----------|----------------------|--------------------------------------------|
| 00000100 | SF                   | Sign flag is set if an operation set the   |
|          |                      | bit 7 of the destination to 1.             |
|----------|----------------------|--------------------------------------------|
| 00001000 | OF                   | Overflow flag is set if a signed operation |
|          |                      | overflowed.                                |
|----------|----------------------|--------------------------------------------|
| 00010000 | IF                   | Interrupt enable flag is set to 1 if       |
|          |                      | interrupts are enabled.                    |
|----------|----------------------|--------------------------------------------|
| 00100000 | TF                   | Trap flag is set to 1 if single-step mode  |
|          |                      | is enabled.                                |
|----------|----------------------|--------------------------------------------|
| 01000000 | LF                   | Avuxo legacy mode.                         |
|----------|----------------------|--------------------------------------------|
| 10000000 | UF                   | User flag is set to 1 entering in the user |
|          |                      | mode.                                      |
'=============================================================================='

.==============================================================================.
| Control Flow - Halt, Interrupts, and Jumps                                   |
|==============================================================================|
| Encoding | Assembly-like Syntax | Description                                |
|==========|======================|============================================|
| 00000000 | HLT                  | Halt the execution, wait for IRQ 0.        |
|----------|----------------------|--------------------------------------------|
| 00000001 | HCF                  | Halt the execution, quit.                  |
|----------|----------------------|--------------------------------------------|
| 00000010 | INT I                | Interrupt the execution using              |
| iiiiiiii |                      | - (I >> 0) & 0xF as interrupt source;      |
|          |                      | - (I >> 4) & 0xF as interrupt cause.       |
|----------|----------------------|--------------------------------------------|
| 00000011 | RTI                  | Return from interrupt subroutine.          |
|----------|----------------------|--------------------------------------------|
| 00000100 | JMP NEAR I           | Jump to memory location pointed by IP      |
| iiiiiiii |                      | in/decremented by I.                       |
|----------|----------------------|--------------------------------------------|
| 00000101 | JMP FAR  I           | Jump to memory location pointed by S:I.    |
|----------|----------------------|--------------------------------------------|
| 00000110 | CLI                  | Set IF to 0.                               |
|----------|----------------------|--------------------------------------------|
| 00000111 | STI                  | Set IF to 1.                               |
|----------|----------------------|--------------------------------------------|
| 000010rr | JMP NEAR reg[R]      | Jump to memory location pointed by IP      |
|          |                      | in/decremented by reg[R].                  |
|----------|----------------------|--------------------------------------------|
| 000011rr | JMP FAR  reg[R]      | Jump to memory location pointed by         |
|          |                      | S:reg[R].                                  |
|==============================================================================|
| Control Flow - Jump to and Return from Subroutines                           |
|==============================================================================|
| Encoding | Assembly-like Syntax | Description                                |
|==========|======================|============================================|
| 00010000 | RST                  | Jump to reset interrupt service routine.   |
|----------|----------------------|--------------------------------------------|
| 00010001 | BRK                  | Jump to debugger service routine when a    |
|          |                      | breakpoint has been hit.                   |
|----------|----------------------|--------------------------------------------|
| 00010010 | SYS.JSR ACC          | Jump to system subroutine pointed by       |
|          |                      | accumulator.                               |
|----------|----------------------|--------------------------------------------|
| 00010011 | SYS.RET              | Return from system subroutine.             |
|----------|----------------------|--------------------------------------------|
| 00010100 | JSR NEAR I           | Jump to subroutine starting at memory      |
| iiiiiiii |                      | location pointed by IP in/decremented by I.|
|----------|----------------------|--------------------------------------------|
| 00010101 | JSR FAR  I           | Jump to subroutine starting at memory      |
| iiiiiiii |                      | location pointed by S:I.                   |
|----------|----------------------|--------------------------------------------|
| 00010110 | RET NEAR             | Return from subroutine invoked by a near   |
|          |                      | jump instruction.                          |
|----------|----------------------|--------------------------------------------|
| 00010111 | RET FAR              | Return from subroutine invoked by a far    |
|          |                      | jump instruction.                          |
|----------|----------------------|--------------------------------------------|
| 000110rr | JSR NEAR reg[R]      | Jump to subroutine starting at memory      |
|          |                      | location pointed by IP in/decremented by   |
|          |                      | reg[R].                                    |
|----------|----------------------|--------------------------------------------|
| 000111rr | JSR FAR  reg[R]      | Jump to subroutine starting at memory      |
|          |                      | location pointed by S:reg[R].              |
|==============================================================================|
| Control Flow - Conditional Near Jumps                                        |
|==============================================================================|
| Encoding | Assembly-like Syntax | Description                                |
|==========|======================|============================================|
| 00100000 | JNC NEAR I           | Jump to memory location pointed by IP      |
| iiiiiiii | JAE NEAR I           | in/decremented by I if CF is 0.            |
|----------|----------------------|--------------------------------------------|
| 00100001 | JC  NEAR I           | Jump to memory location pointed by IP      |
| iiiiiiii | JB  NEAR I           | in/decremented by I if CF is 1.            |
|----------|----------------------|--------------------------------------------|
| 00100010 | JNZ NEAR I           | Jump to memory location pointed by IP      |
| iiiiiiii | JNE NEAR I           | in/decremented by I if ZF is 0.            |
|----------|----------------------|--------------------------------------------|
| 00100011 | JZ  NEAR I           | Jump to memory location pointed by IP      |
| iiiiiiii | JE  NEAR I           | in/decremented by I if ZF is 1.            |
|----------|----------------------|--------------------------------------------|
| 00100100 | JNS NEAR I           | Jump to memory location pointed by IP      |
| iiiiiiii |                      | in/decremented by I if SF is 0.            |
|----------|----------------------|--------------------------------------------|
| 00100101 | JS  NEAR I           | Jump to memory location pointed by IP      |
| iiiiiiii |                      | in/decremented by I if SF is 1.            |
|----------|----------------------|--------------------------------------------|
| 00100110 | JNO NEAR I           | Jump to memory location pointed by IP      |
| iiiiiiii |                      | in/decremented by I if OF is 0.            |
|----------|----------------------|--------------------------------------------|
| 00100111 | JO  NEAR I           | Jump to memory location pointed by IP      |
| iiiiiiii |                      | in/decremented by I if OF is 1.            |
|----------|----------------------|--------------------------------------------|
| 00101000 | JA  NEAR I           | Jump to memory location pointed by IP      |
| iiiiiiii |                      | in/decremented by I if CF is 0 and ZF is 0.|
|----------|----------------------|--------------------------------------------|
| 00101001 | JBE NEAR I           | Jump to memory location pointed by IP      |
| iiiiiiii |                      | in/decremented by I if CF is 1 or ZF is 1. |
|----------|----------------------|--------------------------------------------|
| 00101010 | JG  NEAR I           | Jump to memory location pointed by IP      |
| iiiiiiii |                      | in/decremented by I if SF is equal to OF   |
|          |                      | and ZF is 0.                               |
|----------|----------------------|--------------------------------------------|
| 00101011 | JLE NEAR I           | Jump to memory location pointed by IP      |
| iiiiiiii |                      | in/decremented by I if SF is not equal to  |
|          |                      | OF or ZF is 1.                             |
|----------|----------------------|--------------------------------------------|
| 00101100 | JL  NEAR I           | Jump to memory location pointed by IP      |
| iiiiiiii |                      | in/decremented by I if SF is not equal to  |
|          |                      | OF.                                        |
|----------|----------------------|--------------------------------------------|
| 00101101 | JGE NEAR I           | Jump to memory location pointed by IP      |
| iiiiiiii |                      | in/decremented by I if SF is equal to OF.  |
|----------|----------------------|--------------------------------------------|
| 00101110 | JMP NEAR reg[R], C   | Jump to memory location pointed by IP      |
| ccccbrrr |                      | in/decremented by reg[R] if C is B.        |
|          |                      | If C is 1110b or 1111b, INT #UD is raised. |
|----------|----------------------|--------------------------------------------|
| 00101111 | JSR NEAR reg[R], C   | Jump to memory location pointed by IP      |
| ccccbrrr |                      | in/decremented by reg[R] if C is B.        |
|          |                      | If C is 1110b or 1111b, INT #UD is raised. |
|==============================================================================|
| Control Flow - Conditional Far Jumps                                         |
|==============================================================================|
| Encoding | Assembly-like Syntax | Description                                |
|==========|======================|============================================|
| 00110000 | JNC FAR  I           | Jump to memory location pointed by S:I if  |
| iiiiiiii | JAE FAR  I           | CF is 0.                                   |
|----------|----------------------|--------------------------------------------|
| 00110001 | JC  FAR  I           | Jump to memory location pointed by S:I if  |
| iiiiiiii | JB  FAR  I           | CF is 1.                                   |
|----------|----------------------|--------------------------------------------|
| 00110010 | JNZ FAR  I           | Jump to memory location pointed by S:I if  |
| iiiiiiii | JNE FAR  I           | ZF is 0.                                   |
|----------|----------------------|--------------------------------------------|
| 00110011 | JZ  FAR  I           | Jump to memory location pointed by S:I if  |
| iiiiiiii | JE  FAR  I           | ZF is 1.                                   |
|----------|----------------------|--------------------------------------------|
| 00110100 | JNS FAR  I           | Jump to memory location pointed by S:I if  |
| iiiiiiii |                      | SF is 0.                                   |
|----------|----------------------|--------------------------------------------|
| 00110101 | JS  FAR  I           | Jump to memory location pointed by S:I if  |
| iiiiiiii |                      | SF is 1.                                   |
|----------|----------------------|--------------------------------------------|
| 00110110 | JNO FAR  I           | Jump to memory location pointed by S:I if  |
| iiiiiiii |                      | OF is 0.                                   |
|----------|----------------------|--------------------------------------------|
| 00110111 | JO  FAR  I           | Jump to memory location pointed by S:I if  |
| iiiiiiii |                      | OF is 1.                                   |
|----------|----------------------|--------------------------------------------|
| 00111000 | JA  FAR  I           | Jump to memory location pointed by S:I if  |
| iiiiiiii |                      | CF is 0 and ZF is 0.                       |
|----------|----------------------|--------------------------------------------|
| 00111001 | JBE FAR  I           | Jump to memory location pointed by S:I if  |
| iiiiiiii |                      | CF is 1 or ZF is 1.                        |
|----------|----------------------|--------------------------------------------|
| 00111010 | JG  FAR  I           | Jump to memory location pointed by S:I if  |
| iiiiiiii |                      | SF is equal to OF and ZF is 0.             |
|----------|----------------------|--------------------------------------------|
| 00111011 | JLE FAR  I           | Jump to memory location pointed by S:I if  |
| iiiiiiii |                      | SF is not equal to OF or ZF is 1.          |
|----------|----------------------|--------------------------------------------|
| 00111100 | JL  FAR  I           | Jump to memory location pointed by S:I if  |
| iiiiiiii |                      | SF is not equal to OF.                     |
|----------|----------------------|--------------------------------------------|
| 00111101 | JGE FAR  I           | Jump to memory location pointed by S:I if  |
| iiiiiiii |                      | SF is equal to OF.                         |
|----------|----------------------|--------------------------------------------|
| 00111110 | JMP FAR  reg[R], C   | Jump to memory location pointed by S:reg[R]|
| ccccbrrr |                      | if C is B.                                 |
|          |                      | If C is 1110b or 1111b, INT #UD is raised. |
|----------|----------------------|--------------------------------------------|
| 00111111 | JSR FAR  reg[R], C   | Jump to memory location pointed by S:reg[R]|
| ccccbrrr |                      | if C is B.                                 |
|          |                      | If C is 1110b or 1111b, INT #UD is raised. |
|==============================================================================|
| Data Operations - Move Data from Source to Destination, Exchange Data,       |
|                   and Push onto and Pop from Stack                           |
|==============================================================================|
| Encoding | Assembly-like Syntax | Description                                |
|==========|======================|============================================|
| 01000000 | MOV reg[R],          | Move data from reg[M] in/decremented by I  |
| 00rrrmmm |     reg[M] + I       | to reg[R].                                 |
| iiiiiiii |                      |                                            |
| 01000000 | MOV reg[R],          | Move data from memory location pointed by  |
| 01rrrmmm |     mem[S:reg[M] + I]| S:reg[R] in/decremented by I to reg[R].    |
| iiiiiiii |                      |                                            |
| 01000000 | MOV mem[S:reg[M] + I]| Move data from reg[R] to memory location   |
| 10rrrmmm |     reg[R]           | pointed by S:reg[R] in/decremented by I.   |
| iiiiiiii |                      |                                            |
| 01000000 | MOV reg[R], reg[M]   | Move data from reg[M] to reg[R].           |
| 11rrrmmm |                      |                                            |
|----------|----------------------|--------------------------------------------|
| 01000001 | DS                   | Prefix: Use DS as S.                       |
|----------|----------------------|--------------------------------------------|
| 01000010 | SS                   | Prefix: Use SS as S.                       |
|----------|----------------------|--------------------------------------------|
| 01000011 | ES                   | Prefix: Use ES as S.                       |
|----------|----------------------|--------------------------------------------|
| 01000100 | PSH reg[R]           | Push reg[R] onto the stack.                |
| 00000rrr |                      |                                            |
| 01000100 | PSH seg[R]           | Push seg[R] onto the stack.                |
| 000010rr |                      |                                            |
| 01000100 | PSH IDT              | Push IDT onto the stack.                   |
| 00001100 |                      |                                            |
| 01000100 | PSH MDT              | Push MDT onto the stack.                   |
| 00001101 |                      |                                            |
| 01000100 | PSH MRR              | Push MRR onto the stack.                   |
| 00001110 |                      |                                            |
| 01000100 | PSH CSR              | Push CSR onto the stack.                   |
| 00001111 |                      |                                            |
| 01000100 | PSH IMR[R]           | Push IMR of IRQ R onto the stack.          |
| 10000rrr |                      |                                            |
| 01000100 | PSH IRR[R]           | Push IRR of IRQ R onto the stack.          |
| 10001rrr |                      |                                            |
| 01000100 | PSH MCR              | Push MCR onto the stack.                   |
| 10001000 |                      |                                            |
| 01000100 | PSH CYR[R]           | Push CYR[R] onto the stack.                |
| 100011rr |                      |                                            |
|----------|----------------------|--------------------------------------------|
| 01000101 | POP reg[R]           | Pop reg[R] from the stack.                 |
| 00000rrr |                      |                                            |
| 01000101 | POP seg[R]           | Pop seg[R] from the stack.                 |
| 000010rr |                      |                                            |
| 01001000 | POP IDT              | Pop IDT from the stack.                    |
| 00001100 |                      |                                            |
| 01001000 | POP MDT              | Pop MDT from the stack.                    |
| 00001101 |                      |                                            |
| 01001000 | POP MRR              | Pop MRR from the stack.                    |
| 00001110 |                      |                                            |
| 01001000 | POP CSR              | Pop CSR from the stack.                    |
| 00001111 |                      |                                            |
| 01001000 | POP IMR[R]           | Pop IMR of IRQ R from the stack.           |
| 10000rrr |                      |                                            |
|----------|----------------------|--------------------------------------------|
| 01000110 | PSH CTX              | Push context onto the stack.               |
|----------|----------------------|--------------------------------------------|
| 01000111 | POP CTX              | Pop context from the stack.                |
|----------|----------------------|--------------------------------------------|
| 01001rrr | XHG ACC, reg[R]      | Exchange data between accumulator and      |
|          |                      | reg[R].                                    |
|==============================================================================|
| Data Operations - Move Data from Source to Destination                       |
|==============================================================================|
| Encoding | Assembly-like Syntax | Description                                |
|==========|======================|============================================|
| 01010rrr | MOV reg[R], ACC      | Move data from accumulator to reg[R].      |
|----------|----------------------|--------------------------------------------|
| 01011rrr | MOV ACC, reg[R]      | Move data from reg[R] to accumulator.      |
|==============================================================================|
| Data Operations - Operations between Accumulator and Immediate Value, and    |
|                   I/O Operations Using Immediate Value as Port Number        |
|==============================================================================|
| Encoding | Assembly-like Syntax | Description                                |
|==========|======================|============================================|
| 01100000 | ADD ACC, I           | Add I to accumulator.                      |
| iiiiiiii |                      |                                            |
|----------|----------------------|--------------------------------------------|
| 01100001 | ADC ACC, I           | Add I + CF to accumulator.                 |
| iiiiiiii |                      |                                            |
|----------|----------------------|--------------------------------------------|
| 01100010 | SUB ACC, I           | Subtract I from accumulator.               |
| iiiiiiii |                      |                                            |
|----------|----------------------|--------------------------------------------|
| 01100011 | SBB ACC, I           | Subtract I + CF from accumulator.          |
| iiiiiiii |                      |                                            |
|----------|----------------------|--------------------------------------------|
| 01100100 | AND ACC, I           | Bitwise AND between accumulator and I.     |
| iiiiiiii |                      |                                            |
|----------|----------------------|--------------------------------------------|
| 01100101 | OR  ACC, I           | Bitwise OR between accumulator and I.      |
| iiiiiiii |                      |                                            |
|----------|----------------------|--------------------------------------------|
| 01100110 | SRL ACC, I           | Shift right accumulator logically by I.    |
| iiiiiiii |                      |                                            |
|----------|----------------------|--------------------------------------------|
| 01100111 | SLL ACC, I           | Shift left accumulator logically by I.     |
| iiiiiiii |                      |                                            |
|----------|----------------------|--------------------------------------------|
| 01101000 | SRA ACC, I           | Shift right accumulator arithmetically by  |
| iiiiiiii |                      | I.                                         |
|----------|----------------------|--------------------------------------------|
| 01101001 | XOR ACC, I           | Bitwise XOR between accumulator and I.     |
| iiiiiiii |                      |                                            |
|----------|----------------------|--------------------------------------------|
| 01101010 | CMP ACC, I           | Subtract I from accumulator without update |
| iiiiiiii |                      | the accumulator.                           |
|----------|----------------------|--------------------------------------------|
| 01101011 | SBB ACC, I           | Bitwise AND between accumulator and I      |
| iiiiiiii |                      | without update the accumulator.            |
|----------|----------------------|--------------------------------------------|
| 01101100 | IN  ACC, I           | Move data from port I to accumulator.      |
| iiiiiiii |                      |                                            |
|----------|----------------------|--------------------------------------------|
| 01101101 | OUT ACC, I           | Move data from accumulator to port I.      |
| iiiiiiii |                      |                                            |
|----------|----------------------|--------------------------------------------|
| 01101110 | LEGACY               | Go to the legacy ISA.                      |
|----------|----------------------|--------------------------------------------|
| 01101111 | CPU_ID               | Move the CPU unique ID to accumulator.     |
|==============================================================================|
| Data Operations - Increment, and Decrement Register by 1                     |
|==============================================================================|
| Encoding | Assembly-like Syntax | Description                                |
|==========|======================|============================================|
| 01110rrr | INC reg[R]           | Increment reg[R] by 1.                     |
|----------|----------------------|--------------------------------------------|
| 01111rrr | DEC reg[R]           | Decrement reg[R] by 1.                     |
|==============================================================================|
| Data Operations - Add (with and without Carry) Register to Accumulator       |
|==============================================================================|
| Encoding | Assembly-like Syntax | Description                                |
|==========|======================|============================================|
| 10000rrr | ADD ACC, reg[R]      | Add reg[R] to accumulator.                 |
|----------|----------------------|--------------------------------------------|
| 10001rrr | ADC ACC, reg[R]      | Add reg[R] + CF to accumulator.            |
|==============================================================================|
| Data Operations - Subtract (with and without Carry) Register to Accumulator  |
|==============================================================================|
| Encoding | Assembly-like Syntax | Description                                |
|==========|======================|============================================|
| 10010rrr | SUB ACC, reg[R]      | Subtract reg[R] from accumulator.          |
|----------|----------------------|--------------------------------------------|
| 10011rrr | SBB ACC, reg[R]      | Subtract reg[R] + CF from accumulator.     |
|==============================================================================|
| Data Operations - Bitwise AND and OR between Accumulator and Register        |
|==============================================================================|
| Encoding | Assembly-like Syntax | Description                                |
|==========|======================|============================================|
| 10100rrr | AND ACC, reg[R]      | Bitwise AND between accumulator and reg[R].|
|----------|----------------------|--------------------------------------------|
| 10101rrr | OR  ACC, reg[R]      | Bitwise OR between accumulator and reg[R]. |
|==============================================================================|
| Data Operations - Shift Logically Accumulator by Register Bits               |
|==============================================================================|
| Encoding | Assembly-like Syntax | Description                                |
|==========|======================|============================================|
| 10110rrr | SRL ACC, reg[R]      | Shift right accumulator logically by reg[R]|
|          |                      | bits.                                      |
|----------|----------------------|--------------------------------------------|
| 10111rrr | SLL ACC, reg[R]      | Shift left accumulator logically by reg[R] |
|                                 | bits.                                      |
|==============================================================================|
| Data Operations - Shift Arithmetically Accumulator by Register Bits, and XOR |
|                   between Accumulator and Register                           |
|==============================================================================|
| Encoding | Assembly-like Syntax | Description                                |
|==========|======================|============================================|
| 11000rrr | SRA ACC, reg[R]      | Shift right accumulator arithmetically by  |
|          |                      | reg[R] bits.                               |
|----------|----------------------|--------------------------------------------|
| 11001rrr | XOR ACC, reg[R]      | Bitwise XOR between accumulator and reg[R].|
|==============================================================================|
| Data Operations - Compare and Test Accumulator with Register                 |
|==============================================================================|
| Encoding | Assembly-like Syntax | Description                                |
|==========|======================|============================================|
| 11010rrr | CMP ACC, reg[R]      | Subtract reg[R] from accumulator without   |
|          |                      | update the accumulator.                    |
|----------|----------------------|--------------------------------------------|
| 11011rrr | TST ACC, reg[R]      | Bitwise AND between accumulator and reg[R] |
|          |                      | without update the accumulator.            |
|==============================================================================|
| Data Operations - Operations with ModRM                                      |
|==============================================================================|
| Encoding | Assembly-like Syntax | Description                                |
|==========|======================|============================================|
| 1110xxxx | INT #UD              | Undefined instruction. (TODO)              |
|==============================================================================|
| Reserved Operations                                                          |
|==============================================================================|
| 1111xxxx | INT #UD              | Undefined instruction.                     |
'=============================================================================='
```
