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
        - <MEMORY> is a memory address
        - <REGISTER> is a register

Processor instructions:
        MEMORY:
                - LOAD dest <REGISTER>, src <REGISTER>
                - SAVE src <REGISTER>, dest <REGISTER>
        MOVE:
                - MOV dest <REGISTER>, src <REGISTER>
        ARITHMETICS:
                - ADD dest <REGISTER>, op1 <REGISTER>, op2 <REGISTER/IMMEDIATE>
                - SUB dest <REGISTER>, op1 <REGISTER>, op2 <REGISTER/IMMEDIATE>
                - CMP op1 <REGISTER>, op2 <REGISTER/IMMEDIATE>
        BRANCH:
                - J[C] address <MEMORY>

Condition codes [C]:
        - EQ: equal
        - NE: not equal
        - GT: greater than
        - GE: equal or greater than
