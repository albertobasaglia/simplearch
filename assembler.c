#include "instruction.h"
#include <stdio.h>

int main()
{
        char instruction[] = "ADD R0, R1, R2";
        instruction_encode(instruction);
	return 0;
}
