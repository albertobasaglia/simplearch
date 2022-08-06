#ifndef CONFIG_H
#define CONFIG_H

#define LINEBUFFER_SIZE          30
#define MAX_OPERANDS             4
#define MAX_OVERLOADS            4

#define GENERAL_REGISTER_COUNT   10

#define ERR_INVALIDADDRESS       1
#define ERR_INVALIDINSTRUCTION   1

#define REGISTER_STACK_POINTER   10
#define REGISTER_LINK            11
#define REGISTER_FRAME_POINTER   12
#define REGISTER_PROGRAM_COUNTER 15

#endif
