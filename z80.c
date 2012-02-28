// Z80.c
#include "z80.h"
#include "mmu.h"

void init_state(struct Z80* state)
{
}

// Add E to A (ADD A, E)
void add_re(struct Z80* state)
{
	state->Reg.a += state->Reg.e;
	state->Reg.f = 0;
	if (!(state->Reg.a & 255)) state->Reg.f |= 0x80;	// Check for zero
	if (state->Reg.a > 255) state->Reg.f |= 0x10;	// Check for carry
	state->Reg.a &= 255;				// Mask to 8-bits
	state->Reg.m = 1; state->Reg.t = 4;		// 1 M-time taken
}

// Compare B to A (CP A, B)
void cp_rb(struct Z80* state)
{
	int i = state->Reg.e;				// Temp copy
	i -= state->Reg.b;				// Subtract b
	state->Reg.f |= 0x40;				// Set subtraction flag
	if (!(i & 255)) state->Reg.f |= 0x80;		// Check for zero
	if (i < 0) state->Reg.f |= 0x10;		// Check for underflow
	state->Reg.m = 1; state->Reg.t = 4;		// 1 M-time taken 
}

void nop(struct Z80* state)
{
	state->Reg.m = 1; 
	state->Reg.t = 4;
}

void push_bc(struct Z80* state)
{
	state->Reg.sp--;				// Drop through stack
	write_byte(state->Reg.sp, state->Reg.b);	// Write b
	state->Reg.sp--;				// Drop
	write_byte(state->Reg.sp, state->Reg.c);	// Write c
	state->Reg.m = 3; state->Reg.t = 12;		// 3 M-times taken
}

void pop_hl(struct Z80* state)
{
	state->Reg.l = read_byte(state->Reg.sp);	// Read L
	state->Reg.sp++;				// Move up stack
	state->Reg.h = read_byte(state->Reg.sp);	// Read H
	state->Reg.sp++;				// Move up stack
	state->Reg.m = 3; state->Reg.t = 12;		// 3 M-times taken
}

void lda_mm(struct Z80* state)
{
	uint16_t addr = read_word(state->Reg.pc);	// Get address from instruction
	state->Reg.pc += 2;				// Advance PC
	state->Reg.a = read_byte(addr);			// Read from address
	state->Reg.m = 4; state->Reg.t = 16;		// 4 M-times taken
}

void reset(struct Z80* state)
{
	state->Reg.a = 0; state->Reg.b = 0; state->Reg.c = 0; state->Reg.d = 0;
	state->Reg.e = 0; state->Reg.h = 0; state->Reg.l = 0; state->Reg.f = 0;
	state->Reg.sp = 0;
	state->Reg.pc = 0;	// Start execution at 0

//	state->Clock.m = 0; state->Clock.t = 0;	
}

void print_state(struct Z80* state)
{
	printf("8-Bit Registers:\na=%d, b=%d, c=%d, d=%d, e=%d, h=%d, l=%d, f=%d\n",
		state->Reg.a, state->Reg.b, state->Reg.c, state->Reg.d, state->Reg.e,
		state->Reg.h, state->Reg.l, state->Reg.f);
	printf("16-Bit Registers:\npc=%d, sp=%d\n", 
		state->Reg.pc, state->Reg.sp);
	printf("Clock:\nm=%d, t=%d\n",
		state->Reg.m, state->Reg.t);
}

