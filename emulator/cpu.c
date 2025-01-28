#ifndef CPU_C
#define CPU_C

#include "definitions.h"
#include "memory.c"
#include "graphics.c"
#include "input.c"
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
	byte* inst_bytes = (void*)&inst;

	byte x;
	byte y;
	byte n;
	int vx;
	byte flag;

	switch(GET_NIBBLE(inst, 3)) {
		case SET_0:
			if(inst == I_CLEAR)
				clear_screen();
			else if(inst == I_RET) {
				stack_counter--;
				PC = stack[stack_counter];
				stack[stack_counter] = 0;
			} else if((inst & 0xFFF) > 0) goto Unknown;
			break;
		case I_JUMP:
			PC = U12(GET_NIBBLE(inst, 2), inst_bytes[0]);
			break;
		case I_CALL:
			stack[stack_counter] = PC;
			stack_counter++;
			PC = U12(GET_NIBBLE(inst, 2), inst_bytes[0]);
			break;
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
				case I_8_SET:
					V[GET_VX(inst)] = V[GET_VY(inst)];
					break;
				case I_8_OR:
					V[GET_VX(inst)] |= V[GET_VY(inst)];
					break;
				case I_8_AND:
					V[GET_VX(inst)] &= V[GET_VY(inst)];
					break;
				case I_8_XOR:
					V[GET_VX(inst)] ^= V[GET_VY(inst)];
					break;
				case I_8_ADD:
					V[GET_VX(inst)] += V[GET_VY(inst)];
					V[0xF] = V[GET_VX(inst)] < V[GET_VY(inst)];
					break;
				case I_8_MIN:
					flag =  V[GET_VX(inst)] >= V[GET_VY(inst)];
					V[GET_VX(inst)] -= V[GET_VY(inst)];
					V[0xF] = flag;
					break;
				case I_8_RSH:
					flag = GET_BIT(V[GET_VY(inst)], 0);
					V[GET_VX(inst)] = V[GET_VY(inst)];
					V[GET_VX(inst)] >>= 1;
					V[0xF] = flag;
					break;
				case I_8_MINYX:
					flag = V[GET_VY(inst)] >= V[GET_VX(inst)];
					V[GET_VX(inst)] = V[GET_VY(inst)] - V[GET_VX(inst)];
					V[0xF] = flag;
					break;
				case I_8_LSH:
					flag = GET_BIT(V[GET_VY(inst)], 7);
					V[GET_VX(inst)] = V[GET_VY(inst)];
					V[GET_VX(inst)] <<= 1;
					V[0xF] = flag;
					break;
			}
			break;
		case I_NEQXY:
			if(V[GET_VX(inst)] != V[GET_VY(inst)]) PC += 2;
			break;
		case I_SETIDX:
			I = U12(GET_NIBBLE(inst, 2), inst_bytes[0]);
			break;
		case I_JUMPOFF:
			PC = U12(GET_NIBBLE(inst, 2), inst_bytes[0]) + V[0x0];
			break;
		case I_RAND:
			V[GET_VX(inst)] = rand()&inst_bytes[0];
			break;
		case I_DRAW:
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

		case SET_E:
			switch(inst_bytes[0]) {
				case I_E_KEQ:
					if(GET_KEY(V[GET_VX(inst)]) == 1) PC += 2;
					break;
				case I_E_KNEQ:
					if(GET_KEY(V[GET_VX(inst)]) != 1) PC += 2;
					break;
				default:
					goto Unknown;
			}
			break;
		
		case SET_F:
			switch(inst_bytes[0]) {
				case I_F_GETTIME:
					V[GET_VX(inst)] = timer;
					break;
				case I_F_GETKEY:
					if(lastKeyPress)
						V[GET_VX(inst)] = lastKeyPress;
					else PC -= 2;
					break;
				case I_F_SETTIME:
					timer = V[GET_VX(inst)];
					break;
				case I_F_SETSOUND:
					sound_timer = V[GET_VX(inst)];
					break;
				case I_F_ADDXI:
					I += V[GET_VX(inst)];
					break;
				case I_F_SPR:
					I = FONT_ADDR + GET_NIBBLE(V[GET_VX(inst)], 0) * 5;
					break;
				case I_F_BCD:
					vx = V[GET_VX(inst)];
					RAM[I] = vx/100;
					RAM[I+1] = (vx/10)%10;
					RAM[I+2] = vx%10;
					break;
				case I_F_STR:
					for(int i = 0; i < GET_VX(inst)+1; i++) {
						RAM[I] = V[i];
						I++;
					}
					break;
				case I_F_LOD:
					for(int i = 0; i < GET_VX(inst)+1; i++) {
						V[i] = RAM[I];
						I++;
					}
					break;
				default:
					goto Unknown;
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
