#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#define SET_0 0x0
#define I_CLEAR 0x00E0
#define I_RET 0x00EE

#define I_JUMP 0x1
#define I_CALL 0x2
#define I_EQ 0x3
#define I_NEQ 0x4
#define I_EQXY 0x5
#define I_SETCONST 0x6
#define I_ADD 0x7

#define SET_8 0x8
#define I_8_SET 0x0
#define I_8_OR 0x1
#define I_8_AND 0x2
#define I_8_XOR 0x3
#define I_8_ADD 0x4
#define I_8_MIN 0x5
#define I_8_RSH 0x6
#define I_8_MINYX 0x7
#define I_8_LSH 0xe

#define I_NEQXY 0x9
#define I_SETIDX 0xA
#define I_RAND 0xC
#define I_DRAW 0xD

#define SET_E 0xE
#define I_E_KEQ 0x9E
#define I_E_KNEQ 0xA1

#define SET_F 0xF
#define I_F_GETTIME 0x07
#define I_F_SETTIME 0x15
#define I_F_SETSOUND 0x18
#define I_F_SPR 0x29
#define I_F_BCD 0x33
#define I_F_STR 0x55
#define I_F_LOD 0x65

#endif
