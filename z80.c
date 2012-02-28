// Z80.c
#include "z80.h"
#include "mmu.h"

void init_state(struct Z80* state)
{
}

void add_rb(struct Z80* state)
{
/*
	state->Reg.a += state->Reg.b; state->Reg.f = 0;
	if (!(state->Reg.a & 255)) state->Reg.f |= 0x80;
	if (state->Reg.a > 255) state->Reg.f |= 0x10;
	state->Reg.a &= 255; state->Reg.m = 1; state->Reg.t = 4;
*/
	int a = state->Reg.a;
	state->Reg.a += state->Reg.b;
	state->Reg.f = (state->Reg.a > 255) ? 0x10 : 0;
	state->Reg.a &= 255;
	if (!state->Reg.a) state->Reg.f |= 0x80;
	if ((state->Reg.a^state->Reg.b^a) & 0x10)
	state->Reg.f |= 0x20; state->Reg.m = 1;
}

void add_rc(struct Z80* state)
{
	int a = state->Reg.a;
	state->Reg.a += state->Reg.c;
	state->Reg.f = (state->Reg.a > 255) ? 0x10 : 0;
	state->Reg.a &= 255;
	if (!state->Reg.a) state->Reg.f |= 0x80;
	if ((state->Reg.a^state->Reg.c^a) & 0x10)
	state->Reg.f |= 0x20; state->Reg.m = 1;

}

void add_rd(struct Z80* state)
{
	int a = state->Reg.a;
	state->Reg.a += state->Reg.d;
	state->Reg.f = (state->Reg.a > 255) ? 0x10 : 0;
	state->Reg.a &= 255;
	if (!state->Reg.a) state->Reg.f |= 0x80;
	if ((state->Reg.a^state->Reg.d^a) & 0x10)
	state->Reg.f |= 0x20; state->Reg.m = 1;
}

void add_re(struct Z80* state)
{
	int a = state->Reg.a;
	state->Reg.a += state->Reg.e;
	state->Reg.f = (state->Reg.a > 255) ? 0x10 : 0;
	state->Reg.a &= 255;
	if (!state->Reg.a) state->Reg.f |= 0x80;
	if ((state->Reg.a^state->Reg.e^a) & 0x10)
	state->Reg.f |= 0x20; state->Reg.m = 1;
}

void add_rh(struct Z80* state)
{
	int a = state->Reg.a;
	state->Reg.a += state->Reg.h;
	state->Reg.f = (state->Reg.a > 255) ? 0x10 : 0;
	state->Reg.a &= 255;
	if (!state->Reg.a) state->Reg.f |= 0x80;
	if ((state->Reg.a^state->Reg.h^a) & 0x10)
	state->Reg.f |= 0x20; state->Reg.m = 1;
}

void add_rl(struct Z80* state)
{
	int a = state->Reg.a;
	state->Reg.a += state->Reg.l;
	state->Reg.f = (state->Reg.a > 255) ? 0x10 : 0;
	state->Reg.a &= 255;
	if (!state->Reg.a) state->Reg.f |= 0x80;
	if ((state->Reg.a^state->Reg.l^a) & 0x10)
	state->Reg.f |= 0x20; state->Reg.m = 1;
}

void add_ra(struct Z80* state)
{
	int a = state->Reg.a;
	state->Reg.a += state->Reg.a;
	state->Reg.f = (state->Reg.a > 255) ? 0x10 : 0;
	state->Reg.a &= 255;
	if (!state->Reg.a) state->Reg.f |= 0x80;
	if ((state->Reg.a^state->Reg.a^a) & 0x10)
	state->Reg.f |= 0x20; state->Reg.m = 1;
}

void add_hl(struct Z80* state)
{
	int a = state->Reg.a;
	int m = read_byte((state->Reg.h<<8) + state->Reg.l);	
	state->Reg.a += m;
	state->Reg.f = (state->Reg.a > 255) ? 0x10 : 0;
	state->Reg.a &= 255;
	if (!state->Reg.a) state->Reg.f |= 0x80;
	if ((state->Reg.a^a^m) & 0x10) state->Reg.f |= 0x20;
	state->Reg.m = 2;
}

void add_n(struct Z80* state);

void add_hlbc(struct Z80* state);

void add_hlde(struct Z80* state);

void add_hlhl(struct Z80* state);

void add_hlsp(struct Z80* state);

void add_spn(struct Z80* state);



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

