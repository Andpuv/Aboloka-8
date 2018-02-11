======================================================================================

Aboloka-8 Reference Manual
          Created by Avuxo

======================================================================================

Aboloka-8

        The Aboloka-8 is a fantasy retro computer based off of old soviet
        home computers. The system is inspired by projects like Pico-8.
        Unlike with systems like the Pico-8 (a fantasy retro console), it
        is not a game console, and is more reminiscent the ZX Spectrum.

        The Aboloka-8 provides a custom instruction set specifically tai-
        lored to make assembly programming fun and accesible to provide
        an introduction to a generation of people who have never and pro-
        bably will never write assembly code for their actual home compu-
        ters.

        At the github repository for Aboloka-8 there are two implementat-
        ions of the specification defined in this manual and other liter-
        ature regarding the Aboloka-8. There is an implentation in C usi-
        ng SDL for graphics. The second implementation is written in JS
        using Canvas for graphics. The github is visible at Avuxo's page
        at https://github.com/avuxo/aboloka-8


:: Specifications

        Display: 128x128 pixels, monochrome
        Input: 30 key keyboard. 0-9, a-Z, shift, space, execute
        Memory size: 64KB
        Tape size: 54KB (can hold 2 tapes, both R/W)
        Internal storage: 16KB
        8-bit Registers: 4 (X,Y,Z,A -- X,Y,Z General; A accumulator)

:: Hello world!

        Below is an example of how the standard hello world would look
        in the instruction set used by Aboloka-8.

        In order to get to the point where you can write this program,
        type ed at the prompt. This will open up the standard Aboloka-8
        editor. From here, enter insertion mode at line 0 by typing i 0.

        PROGRAM hello
        ld $0002, "Hello, world!" ; load "Hello world" into 0x02 in memory
        ld $000B, #$00 ; load a null terminator into 0x0B in memory
        PRINT $02 ; print whatever is between 0x02 and null term.
        exit ; return to OS

        Note: for more info on the instruction set, look at the "program-
        ming" section later in this manual.
        
        exit and save by hitting the ESC key and typing :wq hello.a
        This will save the file as hello.a on the local 16kb internal
        storage.

        To create an executable program, type asm hello.a hello EXEC into
        the propmt. This will assemble the program and produce an execu-
        table program.

:: The OS

        The Aboloka-8 comes with a built in operating system that is aut-
        omatically loaded from the internal ROM. The operating system is
        written in Aboloka-8 assembly and is replacable if you want a di-
        fferent experience with the Aboloka.

        The OS comes with a list of built in programs that can be execut-
        ed to perform basic tasks.
           ed          the basic editor, a line based text editor
           mv          move files from x y
           asm         the assembler for all user programs
           rm          remove a file forever (overwrites with zeros)
           ld          load a tape (takes argument 0 or 1) see: tapes
           mem         print current memory usage

        Programs can be run any time that there is a $ showing (indicati-
        ng that the prompt is ready to be run).

        Arguments are provided to programs by separating them by spaces.
        ex: rm hello.a EXEC would delete the file hello.a

        The OS does not support sub-directories. Everything is stored in
        the root directory or on a tape.


        If you ever need to restart the Aboloka-8, use the F1 key on you-
        r computer. This will restart the computer and clear the RAM.

        Before a restart, ensure that you have put any code that you wis-
        h to backup up on a tape. Tapes will NOT be cleared at restart.

:: Tapes

        The Aboloka-8 comes with two virtual tape drives. These tape drives
        are interfaced with via the ld command.

        When ld is run on a drive, it begins reading code at $00. Any prog-
        gram there will be loaded.

        Tapes contain 54kb of storage for writing and distributing tapes.
        Tapes can be distributed from your host operating system using the
        .tape file format.
        
:: Instruction set

        The Aboloka-8 uses 8-bit opcodes that translate 1:1 with the mneu-
        monic instruction set.

        Any number prefixed with a $ is in hexadecimal. Any number prefix-
        xed with a # is a literal value.

        instruction           op   description                       length
        ====================  ==   ================================= ======        
        ld $ADDRESS, #$VALUE  10   load the hex VALUE into ADDRESS        4
        ldx #$VALUE           11   load the hex value VALUE into X        2
        ldy #$VALUE           12   load the hex value VALUE into Y        2
        ldz #$VALUE           13   load the hex value VALUE into Z        2
        lda #$VALUE           14   load the hex value VALUE into A        2
        ldx $ADDRESS          15   load the value at an address into X    3
        ldy $ADDRESS          16   load the value at an address into Y    3
        ldz $ADDRESS          17   load the value at an address into Z    3
        lda $ADDRESS          18   load the value at an address into A    3
        
        cpx #$VALUE           1A   compare the hex value VALUE with X     2
        cpx $ADDRESS          1B   compare X with the value at address    3
        cpy #$VALUE           1C   compare the hex value VALUE with Y     2
        cpy $ADDRESS          1D   compare Y with the value at address    3
        cpz #$VALUE           1E   compare the hex value VALUE with Z     2
        cpz $ADDRESS          1F   compare Z with the value at address    3
        
        jmp $ADDRESS          20   jump to the memory location ADDRESS    3
        jmp REG               21   jump to the register REG               2
        je  $ADDRESS          22   jump only if eq flag is set            3
        jne $ADDRESS          23   jump only if eq flag is not set        3

        call $ADDRESS         25   jump and push return addr to stack     3
        ret                   26   pop ret address off the stack and jump 1

        add #$VALUE           30   add VALUE to A                         2
        adm $ADDRESS, #$VALUE 31   add VALUE to memory location ADDRESS   4
        sub #$VALUE           32   subtract VALUE form A                  2
        sbm $ADDRESS, #$VALUE 33   sub VALUE from memory @ADDRESS         4
        inc $ADDRESS          34   add 1 to whatever value is in ADDRESS  3
        inc REG               35   add 1 to whatever is in the register   2
        dec $ADDRESS          36   subtract one from ADDRESS' value       3
        dec REG               37   subtract one from whatever is in REG   2
        
        clf                   40   clear all status flags                 1
        
        emit #$VALUE          55   echo a single ascii character          2
        emit REG              46   echo a single character in a register  2
        print $ADDRESS        57   print at ADDRESS until a 0x00 is found 3
        
        exit                  EA   exit the program                       1


:: Opcodes

        The Aboloka-8 uses 32 bit fixed-width instructions separated into 4
        bytes. However, not all instructions use all 32 bits, all unused
        bits are simply zeroed out in this case. The below diagram explains
        the bit pattern of opcodes.
        
        [opcode: 8 bits][operands:  24 bits]
                        [arg1] [arg2] [arg3]

        01 23 45 67
         |  |  |  |
         |  |  |  +----- Operand byte 3
         |  |  +-------- Operand byte 2
         |  +----------- Operand byte 1
         +-------------- The opcode (the operation)

        For example: the assembly instruction

        ld $0040, #$30

        would translate to the bit pattern

        10 00 40 30
         |  |  |  |
         |  |  |  +----- The byte being written to the address  (arg2)
         |  |  +-------- The second byte of 0x040 (the address) (arg1)
         |  +----------- The first byte of 0x0040 (the address) (arg1)
         +-------------- The ld instruction

        For an example of an instruction that doesn't use all 4 bytes:

        clf

        Would translate to

        40 00 00 00

        In machine code, only utilizing the opcode. The rest of the
        instruction is irrelevant because clf takes 0 arguments

:: Subroutines

        Subroutines are created using the label syntax seen in other diale-
        cts of assembly. Although in machine code the opcode jumps to a lo-
        cation in the program, when actually programming, you can use simp-
        le to use names.

        ; subroutine to add 2 numbers
        ; add (int a, in b) -> (int c)
        subroutine add:
            add X ; add A to X
            ret   ; jump back to call location

        ldX $3B  ; load 0x3B into X
        ldA $6   ; load 0x06 into A
        call add ; call the add subroutine
        emit A   ; print out the char 'A'


        When creating a subroutine, the 'call' and 'ret' instructions are
        used. call pushes the return address onto the stack and jumps to
        the subroutine, and ret returns to the location pushed to the st-
        ack. Due to direct memory access being possible, the stack is re-
        ally only used as a call stack. This call stack is only 500 bytes
        wide, so deep recursion is not advised.

        The call stack goes from $1001-$11f5, if the subroutine attempts
        to push an address any further than that, it will raise a stack
        overflow exception.

:: Labels

        An alternative to the subroutine mechanism is the ability to
        create human-readable labels to jump to. Using this mechani-
        sm, it is easy to create loops or branching paths.

        Below is an example showing both loops and branching paths using
        only the jmp and cpa instructions.

        The example also shows off a (non-required) function of labels:
            they can be used inside of subroutines. They can also be
            used outside of subroutines, but it's better to show that
            they can be used inside.

        subroutine test:
            lda #$41
            jmp repeat
        
            label repeat:
                cpa #$51
                je done
                emit A
                inc A
                jmp repeat

            label done:
                exit

        It should be noted however that labels are NOT scoped meaning
        that jumping to a label inside of another subroutine could lead
        to an issue. Just bear this in mind while using labels as it is
        the major limitation relating to them.

        In terms of naming conventions, labels should probably be named
        as follows:

        SUBNAME_ACTION

        The above example's labels would be better named as:

        repeat -> test_repeat
        done   -> test_done

        As to fit within these naming conventions. They were named as
        shown simply for readability reasons (and there can't be scop-
        ing issues in a code snippet).

:: Memory Layout

        In the Aboloka-8 there are a number of important memory regions.
        The system has no memory safety, which means that any program can
        write anywhere in memory.

        $0000-$1000 ROM for programs and OS
        $1001-$11f5 call stack
        $1001-$3FFF free memory for user use.
        $4000-$8000 graphics memory (each byte is 1 pixel).
        $3000-$30FF keyboard input memory.
        $30FF-$FA00 free memory for user use.


:: CPU Status Flags

        The Aboloka-8's CPU features an unsigned 8-bit integer holding all
        of the system's flags.

        The bits are arranged as follows:

        A B C D   E F G H
        | | | |   | | | |
        | | | |   | | | +- eq flag,  when a cmp returns true
        | | | |   | | +--- carry flag, on unsigned arithmetic with carry
        | | | |   | +----- overflow flag, used in signed arithmetic
        | | | |   +------- none (currently unused)
        | | | +----------- none (currently unused)
        | | +------------- none (currently unused)
        | +--------------- none (currently unused)
        +----------------- none (currently unused)

:: Graphics programming

        In order to write to a specific pixel, an 8-bit color value is writ-
        ten to a specific area in memory. Whatever value is in that byte wi-
        ll be drawn to the screen when it comes time for the screen to be r-
        efreshed.

        The graphics block of memory starts at $4000 and goes to $8000. Any
        8-bit color value can be drawn here. if a byte is $00 then it will
        be drawn as black. In order to clear the screen, the memory should
        simply be zeroed out.

        The following code snippet will draw a white box in the top left of
        the screen (exit the program by restarting as you will be in an in-
        finite loop).

        PROGRAM box
        label draw:
        ld $4000, #$01
        ld $4001, #$01
        ld $4081, #$01
        ld $4082, #$01
        jmp draw

        The above code functions because it loads the value $01 (white) into
        the block of memory used to draw graphics. The code constantly jumps
        to the start to make sure that it continues drawing and never exits.

        As such, the code cannot be exited without a full system restart.
