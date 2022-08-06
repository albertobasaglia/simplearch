I'm gifting myself a little cpu emulator to be used as an assembly playground.
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
        ARITHMETICS:
                - ADD dest <REGISTER> op1 <REGISTER> op2 <REGISTER/IMMEDIATE>
                - SUB dest <REGISTER> op1 <REGISTER> op2 <REGISTER/IMMEDIATE>
                - CMP op1 <REGISTER> op2 <REGISTER/IMMEDIATE>
        BRANCH:
                - J[C] address <MEMORY>

Condition codes [C]:
        - EQ: equal
        - NE: not equal
        - GT: greater than
        - GE: equal or greater than

Todo:
        - Implement some output functionality:
                - could implement an OUT instruction and and expansion port
                - or maybe memory map some video memory: cooler =D
