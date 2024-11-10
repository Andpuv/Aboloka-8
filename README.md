```
| <opcode> 00rr--00 iiiiiiii -------- | dst = reg[r]
|                                     | src = imm
| <opcode> 00rrmm01 iiiiiiii -------- | dst = reg[r]
|                                     | src = reg[m] + imm
| <opcode> 00rr--10 iiiiiiii -------- | Exception
|                                     | Exception
| <opcode> 00rrmm11 iiiiiiii -------- | Exception
|                                     | Exception
|-------------------------------------|----------------------------------------
| <opcode> 01rr--00 oooooooo -------- | dst = reg[r]
|                                     | src = mem[seg[ds]][o]
| <opcode> 01rrmm01 oooooooo -------- | dst = reg[r]
|                                     | src = mem[seg[ds]][reg[m] + o]
| <opcode> 01rr--10 oooooooo -------- | dst = mem[seg[ds]][o]
|                                     | src = reg[r]
| <opcode> 01rrmm11 oooooooo -------- | dst = mem[seg[ds]][reg[m] + o]
|                                     | src = reg[r]
|-------------------------------------|----------------------------------------
| <opcode> 10rr--00 oooooooo -------- | dst = reg[r]
|                                     | src = mem[seg[ss]][o]
| <opcode> 10rrmm01 oooooooo -------- | dst = reg[r]
|                                     | src = mem[seg[ss]][reg[m] + o]
| <opcode> 10rr--10 oooooooo -------- | dst = mem[seg[ss]][o]
|                                     | src = reg[r]
| <opcode> 10rrmm11 oooooooo -------- | dst = mem[seg[ss]][reg[m] + o]
|                                     | src = reg[r]
|-------------------------------------|----------------------------------------
| <opcode> 11rrmm00 oooooooo -------- | dst = reg[r]
|                                     | src = mem[seg[es]][o]
| <opcode> 11rrmm01 oooooooo -------- | dst = reg[r]
|                                     | src = mem[seg[es]][reg[m] + o]
| <opcode> 11rrmm10 oooooooo -------- | dst = mem[seg[es]][o]
|                                     | src = reg[r]
| <opcode> 11rrmm11 oooooooo -------- | dst = mem[seg[es]][reg[m] + o]
|                                     | src = reg[r]

https://www.cs.princeton.edu/courses/archive/fall12/cos375/IA32Jcc.pdf

Memory has 256 segments, each one has 256 bytes.
Memory is 64KiB in total.

Flags      Index  Description
========== =====  ===========
SF             0  Set as the bit 7 of the result
ZF             1  Set if the result is 0
CF             2  Set if the result has a carry
OF             3  Set if the operation overflowed
RF             4  Always set (CPU is running)
IF             5  Set to 1 if interrupts are enabled
6F             6  Reserved
UF             7  Set to 1 if the CPU is in user mode

Registers  Index  Description            UF
=========  =====  =====================  ==
A              0  Kernel accumulator      0
X              1  Kernel register X       0
Y              2  Kernel register Y       0
Z              3  Kernel register Z       0
---------  -----  ---------------------  --
A'             4  User accumulator        1
X'             5  User register X         1
Y'             6  User register Y         1
Z'             7  User register Z         1

Segments   Index  Description            UF
=========  =====  =====================  ==
CS             0  Kernel Code Ssegment    0
DS             1  Kernel Data Ssegment    0
SS             2  Kernel Stack Ssegment   0
ES             3  Kernel Extra Ssegment   0
---------  -----  ---------------------  --
CS'            4  User Code Ssegment      1
DS'            5  User Data Ssegment      1
SS'            6  User Stack Ssegment     1
ES'            7  User Extra Ssegment     1

Instruction    ModRM  Opcode  Description                               Size
====================  ======  ========================================  ====
hlt                0    00    Halt the execution until interrupt 0         1
hcf                0    01    Halt and catch fire                          1
jmp far imm        0    02    Set CS:PC to ES:IMM                          2
jmp near imm       0    03    In/Decrement PC by IMM                       2
jmp far a          0    04    Set CS:PC to ES:A                            1
jmp far x          0    05    Set CS:PC to ES:X                            1
jmp far y          0    06    Set CS:PC to ES:Y                            1
jmp far z          0    07    Set CS:PC to ES:Z                            1
jmp near a         0    08    In/Decrement PC by A                         1
jmp near x         0    09    In/Decrement PC by X                         1
jmp near y         0    0A    In/Decrement PC by Y                         1
jmp near z         0    0B    In/Decrement PC by Z                         1
int imm            0    0C    Interrupt IMM                                2
rti                0    0D    Return from interrupt                        1
jmp seg:off        0    0E    Set CS:PC to SEG:OFF                         3
int #ud            0    0F    Interrupt #UD                                1
--------------------  ------  ----------------------------------------  ----
mov a, imm         0    10    Move IMM into A                              2
mov x, imm         0    11    Move IMM into X                              2
mov y, imm         0    12    Move IMM into Y                              2
mov z, imm         0    13    Move IMM into Z                              2
mov reg, imm       1    14    Move IMM into REG                            3
mov reg, reg       1    14    Move REG into REG                            2
mov reg, mem       1    14    Move MEM into REG (read memory)              3
mov mem, reg       1    14    Move REG into MEM (write memory)             3
mov a, x           0    15    Move X into A                                1
mov a, y           0    16    Move Y into A                                1
mov a, z           0    17    Move Z into A                                1
mov a, a           0    18    No operation                                 1
mov x, a           0    19    Move A into X                                1
mov y, a           0    1A    Move A into Y                                1
mov z, a           0    1B    Move A into Z                                1
xhg reg, imm       1    1C    Exchange REG with IMM                        3
xhg reg, reg       1    1C    Exchange REG with REG                        2
xhg reg, mem       1    1C    Exchange REG with MEM                        3
xhg mem, reg       1    1C    Exchange MEM with REG                        3
xhg a, x           0    1D    Exchange A with X                            1
xhg a, y           0    1E    Exchange A with Y                            1
xhg a, z           0    1F    Exchange A with Z                            1
--------------------  ------  ----------------------------------------  ----
jcc imm            0  20..2F  Conditional jump if CC is true               2
--------------------  ------  ----------------------------------------  ----
sti                0    30    Set IF to 1 (enable interrupts)              1
cli                0    31    Set IF to 0 (disable interrupts)             1
call far imm       0    32    Push CS:PC and set CS:PC to ES:IMM           2
call near imm      0    33    Push PC and in/decrement PC by IMM           2
call far a         0    34    Push CS:PC and set CS:PC to ES:A             1
call far x         0    35    Push CS:PC and set CS:PC to ES:X             1
call far y         0    36    Push CS:PC and set CS:PC to ES:Y             1
call far z         0    37    Push CS:PC and set CS:PC to ES:Z             1
call near a        0    38    Push PC and in/decrement PC by A             1
call near x        0    39    Push PC and in/decrement PC by X             1
call near y        0    3A    Push PC and in/decrement PC by Y             1
call near z        0    3B    Push PC and in/decrement PC by Z             1
ret far            0    3C    Pop CS:PC                                    1
ret near           0    3D    Pop PC                                       1
call seg:off       0    3E    Push CS:PC and set CS:PC to SEG:OFF          3
clf                0    3F    Clear SF, ZF, CF, and OF                     1
--------------------  ------  ----------------------------------------  ----
add a, imm         0    40    Add IMM to A                                 2
add a, x           0    41    Add X to A                                   1
add a, y           0    42    Add Y to A                                   1
add a, z           0    43    Add Z to A                                   1
adc a, imm         0    44    Add IMM to A (with carry)                    2
adc a, x           0    45    Add X to A (with carry)                      1
adc a, y           0    46    Add Y to A (with carry)                      1
adc a, z           0    47    Add Z to A (with carry)                      1
sub a, imm         0    48    Subtract IMM from A                          2
sub a, x           0    49    Subtract X from A                            1
sub a, y           0    4A    Subtract Y from A                            1
sub a, z           0    4B    Subtract Z from A                            1
sbb a, imm         0    4C    Subtract IMM from A (with carry)             2
sbb a, x           0    4D    Subtract X from A (with carry)               1
sbb a, y           0    4E    Subtract Y from A (with carry)               1
sbb a, z           0    4F    Subtract Z from A (with carry)               1
--------------------  ------  ----------------------------------------  ----
inc a              0    50    Increment A by 1                             1
inc x              0    51    Increment X by 1                             1
inc y              0    52    Increment Y by 1                             1
inc z              0    53    Increment Z by 1                             1
dec a              0    54    Decrement A by 1                             1
dec x              0    55    Decrement X by 1                             1
dec y              0    56    Decrement Y by 1                             1
dec z              0    57    Decrement Z by 1                             1
and a, imm         0    58    Bitwise AND between A and IMM                2
and a, x           0    59    Bitwise AND between A and X                  1
and a, y           0    5A    Bitwise AND between A and Y                  1
and a, z           0    5B    Bitwise AND between A and Z                  1
or  a, imm         0    5C    Bitwise OR  between A and IMM                2
or  a, x           0    5D    Bitwise OR  between A and X                  1
or  a, y           0    5E    Bitwise OR  between A and Y                  1
or  a, z           0    5F    Bitwise OR  between A and Z                  1
--------------------  ------  ----------------------------------------  ----
sll a, imm         0    60    Logic shift left A by IMM                    2
sll a, x           0    61    Logic shift left A by X                      1
sll a, y           0    62    Logic shift left A by Y                      1
sll a, z           0    63    Logic shift left A by Z                      1
slr a, imm         0    64    Logic shift right A by IMM                   2
slr a, x           0    65    Logic shift right A by X                     1
slr a, y           0    66    Logic shift right A by Y                     1
slr a, z           0    67    Logic shift right A by Z                     1
sar a, imm         0    68    Arithmetic shift right A by IMM              2
sar a, x           0    69    Arithmetic shift right A by X                1
sar a, y           0    6A    Arithmetic shift right A by Y                1
sar a, z           0    6B    Arithmetic shift right A by Z                1
xor a, a           0    6C    Bitwise XOR between A and A (A = 0)          1
xor a, x           0    6D    Bitwise XOR between A and X                  1
xor a, y           0    6E    Bitwise XOR between A and Y                  1
xor a, z           0    6F    Bitwise XOR between A and Z                  1
--------------------  ------  ----------------------------------------  ----
tst a, imm         0    70    Bitwise AND between A and IMM (only flags)   2
tst a, x           0    71    Bitwise AND between A and X (only flags)     1
tst a, y           0    72    Bitwise AND between A and Y (only flags)     1
tst a, z           0    73    Bitwise AND between A and Z (only flags)     1
in  a, imm         0    74    Read A from port IMM                         1
in  a, x           0    75    Read A from port X                           1
in  a, y           0    76    Read A from port Y                           1
in  a, z           0    77    Read A from port Z                           1
cmp a, imm         0    78    Subtract IMM from A (only flags)             2
cmp a, x           0    79    Subtract X from A (only flags)               1
cmp a, y           0    7A    Subtract Y from A (only flags)               1
cmp a, z           0    7B    Subtract Z from A (only flags)               1
out a, imm         0    7C    Write A to port IMM                          1
out a, x           0    7D    Write A to port X                            1
out a, y           0    7E    Write A to port Y                            1
out a, z           0    7F    Write A to port Z                            1
--------------------  ------  ----------------------------------------  ----
add reg, imm       1    80    Add IMM to REG                               3
add reg, reg       1    80    Add REG to REG                               2
add reg, mem       1    80    Add MEM to REG                               3
add mem, reg       1    80    Add REG to MEM                               3
adc reg, imm       1    81    Add IMM to REG (with carry)                  3
adc reg, reg       1    81    Add REG to REG (with carry)                  2
adc reg, mem       1    81    Add MEM to REG (with carry)                  3
adc mem, reg       1    81    Add REG to MEM (with carry)                  3
sub reg, imm       1    82    Subtract IMM from REG                        3
sub reg, reg       1    82    Subtract REG from REG                        2
sub reg, mem       1    82    Subtract MEM from REG                        3
sub mem, reg       1    82    Subtract REG from MEM                        3
sbb reg, imm       1    83    Subtract IMM from REG (with carry)           3
sbb reg, reg       1    83    Subtract REG from REG (with carry)           2
sbb reg, mem       1    83    Subtract MEM from REG (with carry)           3
sbb mem, reg       1    83    Subtract REG from MEM (with carry)           3
and reg, imm       1    84    Bitwise AND between REG and IMM              3
and reg, reg       1    84    Bitwise AND between REG and REG              2
and reg, mem       1    84    Bitwise AND between REG and MEM              3
and mem, reg       1    84    Bitwise AND between MEM and REG              3
or  reg, imm       1    85    Bitwise OR  between REG and IMM              3
or  reg, reg       1    85    Bitwise OR  between REG and REG              2
or  reg, mem       1    85    Bitwise OR  between REG and MEM              3
or  mem, reg       1    85    Bitwise OR  between MEM and REG              3
sll reg, imm       1    86    Logic shift left REG by IMM                  3
sll reg, reg       1    86    Logic shift left REG by REG                  2
sll reg, mem       1    86    Logic shift left REG by MEM                  3
sll mem, reg       1    86    Logic shift left MEM by REG                  3
slr reg, imm       1    87    Logic shift right REG by IMM                 3
slr reg, reg       1    87    Logic shift right REG by REG                 2
slr reg, mem       1    87    Logic shift right REG by MEM                 3
slr mem, reg       1    87    Logic shift right MEM by REG                 3
sar reg, imm       1    88    Arithmetic shift right REG by IMM            3
sar reg, reg       1    88    Arithmetic shift right REG by REG            2
sar reg, mem       1    88    Arithmetic shift right REG by MEM            3
sar mem, reg       1    88    Arithmetic shift right MEM by REG            3
xor reg, imm       1    89    Bitwise XOR between REG and IMM              3
xor reg, reg       1    89    Bitwise XOR between REG and REG              2
xor reg, mem       1    89    Bitwise XOR between REG and MEM              3
xor mem, reg       1    89    Bitwise XOR between MEM and REG              3
cmp reg, imm       1    8A    Subtract IMM from REG (only flags)           3
cmp reg, reg       1    8A    Subtract REG from REG (only flags)           2
cmp reg, mem       1    8A    Subtract MEM from REG (only flags)           3
cmp mem, reg       1    8A    Subtract REG from MEM (only flags)           3
tst reg, imm       1    8B    Bitwise AND between REG and IMM (only flags) 3
tst reg, reg       1    8B    Bitwise AND between REG and REG (only flags) 2
tst reg, mem       1    8B    Bitwise AND between REG and MEM (only flags) 3
tst mem, reg       1    8B    Bitwise AND between MEM and REG (only flags) 3
int #ud            0    8C    Interrupt #UD                                1
int #ud            0    8D    Interrupt #UD                                1
int #ud            0    8E    Interrupt #UD                                1
int #ud            0    8F    Interrupt #UD                                1
--------------------  ------  ----------------------------------------  ----
setcc              0  90..9F  Set A to 1 if CC is true, else 0             1
--------------------  ------  ----------------------------------------  ----
psh a              0    A0    Push A onto the stack                        1
psh x              0    A1    Push A onto the stack                        1
psh y              0    A2    Push A onto the stack                        1
psh z              0    A3    Push A onto the stack                        1
pop a              0    A4    Pop A from the stack                         1
pop x              0    A5    Pop A from the stack                         1
pop y              0    A6    Pop A from the stack                         1
pop z              0    A7    Pop A from the stack                         1
psh imm            0    A8    Push IMM onto the stack                      2
psh mem            0    A9    Push MEM onto the stack                      2
psh ds:mem         0    AA    Push MEM onto the stack                      2
psh es:mem         0    AB    Push MEM onto the stack                      2
int #ud            0    AC    Interrupt #UD                                1
pop mem            0    AD    Pop MEM from the stack                       2
pop ds:mem         0    AE    Pop MEM from the stack                       2
pop es:mem         0    AF    Pop MEM from the stack                       2
--------------------  ------  ----------------------------------------  ----
psh cs             0    B0    Push CS onto the stack                       1
psh ds             0    B1    Push DS onto the stack                       1
psh ss             0    B2    Push SS onto the stack                       1
psh es             0    B3    Push ES onto the stack                       1
pop cs             0    B4    Pop CS from the stack                        1
pop ds             0    B5    Pop DS from the stack                        1
pop ss             0    B6    Pop SS from the stack                        1
pop es             0    B7    Pop ES from the stack                        1
psh sp             0    B8    Push SP onto the stack                       1
psh sr             0    B9    Push SR onto the stack                       1
stu                0    BA    Set UF to 1                                  1
clu                0    BB    Set UF to 0                                  1
pop sp             0    BC    Pop SP from the stack                        1
pop sr             0    BD    Pop SR from the stack                        1
mks a, x, y        0    BE    Map segment X to A using Y as attributes     1
rms a              0    BF    Unmap segment A                              1
```