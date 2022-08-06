#include "cpu.h"
#include "memory.h"
#include <stdint.h>
#include <stdio.h>

int main()
{
	struct cpu cpu;
	cpu_init(&cpu, 0x1000);
        memory_write(&cpu.memory, 17, 1337);
        uint32_t val;
        memory_read(&cpu.memory, 17, &val);
        printf("%d\n", val);
        cpu_free(&cpu);
	return 0;
}
