A little cpu emulator to be used as an assembly playground.
I'll be making an assembler too since I don't want to write instructions
in binary.

Arch specifications:

Instructions are encoded on 32 bits.
Memory can be accessed in 4byte aligned addresses.
Condition codes are available only on some instructions [C].

Processor registers:
        - r0 to r9 for general use
        - sp - stack pointer (r10)
        - lr - link register (r11)
        - fp - frame pointer (r12)
        - pc - program counter (r15)

Data types:
        - <IMMEDIATE> is an immediate value
        - <REGISTER> is a register

Processor instructions:
        MEMORY:
                - LOAD dest <REGISTER> src <REGISTER>
                - STORE src <REGISTER> dest <REGISTER>
        MOVE:
                - MOV dest <REGISTER> src <REGISTER>
                - MOV dest <REGISTER> src <IMMEDIATE>
        ARITHMETICS:
                - ADD dest <REGISTER> op1 <REGISTER> op2 <REGISTER>
                - SUB dest <REGISTER> op1 <REGISTER> op2 <REGISTER>
                - CMP op1 <REGISTER> op2 <REGISTER/IMMEDIATE>
        BRANCH:
                - J[C] address <MEMORY>

Condition codes [C]:
        - EQ: equal
        - NE: not equal
        - GT: greater than
        - GE: equal or greater than

Character output is achieved writing to address 500, which is a memory
map for a virtual output.

The makefile builds both an assembler and an emulator.

The assembler generates executable (by the emulator) binaries from mnemonics.

The emulator loads the binary and executes it. The memory mapped devices (such
as the serial output) are loaded in ram. The code is hard-loaded at address
0x0, used as the entry point.
