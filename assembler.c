#include "instruction.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
        char instruction[] = "ADD R0, R1, R2";
        if(argc == 2)
                instruction_encode(argv[1]);
        else
                instruction_encode(instruction);
	return 0;
}
