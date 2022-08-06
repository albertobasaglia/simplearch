Arch specifications:

Instructions are encoded on 32 bits.
Memory can be accessed in 4byte aligned addresses.

Processor registers:
        - r0 to r9 for general use
        - sp - stack pointer
        - pc - program counter

Data types:
        - <IMMEDIATE> is an immediate value
        - <MEMORY> is a memory address
        - <REGISTER> is a register

Processor instructions:
        - LOAD <REGISTER>, <MEMORY>
        - SAVE <REGISTER>, <MEMORY>
        - MOV <REGISTER>, <REGISTER>
        - ADD <REGISTER>, <REGISTER>, <REGISTER>
        - SUB <REGISTER>, <REGISTER>, <REGISTER>
        - CMP <REGISTER>, <REGISTER>
        - CMP <REGISTER>, <IMMEDIATE>
        - J <MEMORY>

Condition codes:
        - EQ
        - NE
