#ifndef CPU_C
#define CPU_C

#include "definitions.h"
#include "memory.c"
#include "graphics.c"
#include "instructions.h"
#include <stdio.h>

#define GET_NIBBLE(x, n) (((x)>>((n) * 4)) & 0xF)
#define U12(x, y) (u16)(((x)<<8) | y)
#define GET_BIT(x, n) ((x)>>(n)&1)
#define GET_VX(x) GET_NIBBLE(x, 2)
#define GET_VY(x) GET_NIBBLE(x, 1)

u16 fetch() {
	u16 instruction = (RAM[PC]<<8)|(RAM[PC+1]);
	PC += 2;
	if(PC > 4095) PC -= (4095 - INITIAL_SPACE);
	return instruction;
}

void decode(u16 inst) {
	volatile byte* inst_bytes = (void*)&inst;

	volatile byte x = 0;
	volatile byte y = 0;
	volatile byte n = 0;
	volatile int vx = 0;

	switch(GET_NIBBLE(inst, 3)) {
		case SET_0:
			if(inst == I_CLEAR)
				clear_screen();
			else if((inst & 0xFFF) > 0) goto Unknown;
			break;
		case I_JUMP:
			PC = U12(GET_NIBBLE(inst, 2), inst_bytes[0]);
			break;
		/* Missing 0x2 */
		case I_EQ:
			if(V[GET_VX(inst)] == inst_bytes[0]) PC += 2;
			break;
		case I_NEQ:
			if(V[GET_VX(inst)] != inst_bytes[0]) PC += 2;
			break;
		case I_EQXY:
			if(V[GET_VX(inst)] == V[GET_VY(inst)]) PC += 2;
			break;
		case I_SETCONST:
			V[GET_VX(inst)] = inst_bytes[0];
			break;
		case I_ADD:
			V[GET_VX(inst)] += inst_bytes[0];
			break;
		case SET_8:
			switch(GET_NIBBLE(inst, 0)) {
				case 0:
					V[GET_VX(inst)] = V[GET_VY(inst)];
					break;
				case 1:
					V[GET_VX(inst)] |= V[GET_VY(inst)];
					break;
				case 2:
					V[GET_VX(inst)] &= V[GET_VY(inst)];
					break;
				case 3:
					V[GET_VX(inst)] ^= V[GET_VY(inst)];
					break;
				case 4:
					vx = V[GET_VX(inst)];
					vx += V[GET_VY(inst)];
					if(vx > 0xFFFF) {
						V[0xF] = 1;
						V[GET_VX(inst)] = vx;
					} else {
						V[0xF] = 0;
						V[GET_VX(inst)] = vx;
					}
					break;
				case 5:
					V[0xF] = V[GET_VY(inst)] >= V[GET_VX(inst)];
					V[GET_VX(inst)] -= V[GET_VY(inst)];
					break;
				case 6:
					V[0xF] = GET_BIT(V[GET_VX(inst)], 0);
					V[GET_VX(inst)] >>= 1;
					break;
				case 7:
					V[0xF] = V[GET_VY(inst)] >= V[GET_VX(inst)];
					V[GET_VX(inst)] = V[GET_VY(inst)] + V[GET_VX(inst)];
					break;
				case 0xE:
					V[0xF] = GET_BIT(V[GET_VX(inst)], 7);
					V[GET_VX(inst)] <<= 1;
					break;
			}
			break;
		case 0xA:
			I = U12(GET_NIBBLE(inst, 2), inst_bytes[0]);
			break;
		case 0xD:
			x = V[GET_VX(inst)]%64;
			y = V[GET_VY(inst)]%32;
			n = GET_NIBBLE(inst, 0);
			V[0xF] = 0;

			for(int i = 0; i < n && y < 32; i++) {
				volatile byte b = RAM[I+i];
				int xc = x;
				for(short k = 8; k > 0 && xc < 64; k--) {
					if(GET_BIT(b, k-1) == 1) {
						if(draw_point(xc, y) == 0) V[0xF] = 1;
					}
					xc++;
				}
				y++;
			}

			break;

		default:
		Unknown:
			printf("Unknown instruction: %x at address %d\n", inst, PC);
			break;
	}
	return;
}

#endif

/* 01110000
 * 00001001
 * 10100010 */
