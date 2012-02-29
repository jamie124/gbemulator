// z80_t.c
#include "z80.h"
#include "mmu.h"

void init_state(z80_t* state)
{
}

// 0x00
void op_nop(z80_t* state)
{
	printf("\top_nop test\n");

	state->Reg.m = 1; 
//	state->Reg.t = 4;
}

// 0x01
void op_ld_bc_nn(z80_t* state)
{
	state->Reg.c = read_byte(state->Reg.pc);
	state->Reg.b = read_byte(state->Reg.pc+1);
	state->Reg.pc += 2;
	state->Reg.m = 3;
}

// 0x02
void op_ld_bc_a(z80_t* state)
{
	write_byte((state->Reg.b << 8) + state->Reg.c, state->Reg.a);
	state->Reg.m = 2;
}

// 0x03
void op_inc_bc(z80_t* state)
{
	state->Reg.c = (state->Reg.c + 1) & 255;
	if (!state->Reg.c)
		state->Reg.b = (state->Reg.b + 1) & 255;

	state->Reg.m = 1;
}

// 0x04
void op_inc_b(z80_t* state)
{
	state->Reg.b++;
	state->Reg.b &= 255;
	state->Reg.f = (state->Reg.b ? 0 : 0x80);
	state->Reg.m = 1;
}

// 0x05
void op_dec_b(z80_t* state)
{
	state->Reg.b--;
	state->Reg.b &= 255;
	state->Reg.f = (state->Reg.b ? 0 : 0x80);
	state->Reg.m = 1;
}

// 0x06
void op_ld_b_nn(z80_t* state)
{

}

// 0x07
void op_rlc_a(z80_t* state)
{

}

// 0x08
void op_ld_nn_sp(z80_t* state)
{

}

// 0x09
void op_add_hl_bc(z80_t* state)
{

}

// 0xA0
void op_ld_a_bc(z80_t* state)
{

}

// 0xB0
void op_dec_bc(z80_t* state)
{

}

// 0xC0
void op_inc_c(z80_t* state)
{
	state->Reg.c++;
	state->Reg.c &= 255;
	state->Reg.f = (state->Reg.c ? 0 : 0x80);
	state->Reg.m = 1;
}

// 0xD0
void op_dec_c(z80_t* state)
{
	state->Reg.c--;
	state->Reg.c &= 255;
	state->Reg.f = (state->Reg.c ? 0 : 0x80);
	state->Reg.m = 1;
}

// 0xE0
void op_ld_c_nn(z80_t* state)
{

}

// 0xF0
void op_rrc_a(z80_t* state)
{

}

// 0x10
void op_stop(z80_t* state)
{

}

// 0x11
void op_ld_de_nn(z80_t* state)
{

}

// 0x12
void op_ld_de_a(z80_t* state)
{

}

// 0x13
void op_inc_de(z80_t* state)
{

}

// 0x14
void op_inc_d(z80_t* state)
{
	state->Reg.d++;
	state->Reg.d &= 255;
	state->Reg.f = (state->Reg.d ? 0 : 0x80);
	state->Reg.m = 1;
}
 
// 0x15
void op_dec_d(z80_t* state)
{
	state->Reg.d--;
	state->Reg.d &= 255;
	state->Reg.f = (state->Reg.d ? 0 : 0x80);
	state->Reg.m = 1;
}

// 0x16
void op_ld_d_nn(z80_t* state)
{

}

// 0x17
void op_rl_a(z80_t* state)
{

}

// 0x18
void op_jr(z80_t* state)
{

}

// 0x19
void op_add_hl_de(z80_t* state)
{

}

// 0x1A
void op_ld_a_de(z80_t* state)
{

}

// 0x1B
void op_dec_de(z80_t* state)
{

}

// 0x1C
void op_inc_e(z80_t* state)
{
	state->Reg.e++;
	state->Reg.e &= 255;
	state->Reg.f = (state->Reg.e ? 0 : 0x80);
	state->Reg.m = 1;
}

// 0x1D
void op_dec_e(z80_t* state)
{
	state->Reg.e--;
	state->Reg.e &= 255;
	state->Reg.f = (state->Reg.e ? 0 : 0x80);
	state->Reg.m = 1;
}

// 0x1E
void op_ld_e_nn(z80_t* state)
{

}

// 0x1F
void op_rr_a(z80_t* state)
{

}

// 0x20
void op_jr_nz(z80_t* state)
{

}

// 0x21
void op_ld_hl_nnnn(z80_t* state)
{

}

// 0x22
void op_ldi_hl_a(z80_t* state)
{

}

// 0x23
void op_inc_hl(z80_t* state)
{
	uint8_t i = read_byte((state->Reg.h<<8) + state->Reg.l) + 1;
	i &= 255;
	write_byte((state->Reg.h<<8) + state->Reg.l,i);
	state->Reg.f = (i ? 0 : 0x80);
	state->Reg.m = 3;
}

// 0x24
void op_inc_h(z80_t* state)
{
	state->Reg.h++;
	state->Reg.h &= 255;
	state->Reg.f = (state->Reg.h ? 0 : 0x80);
	state->Reg.m = 1;
}

// 0x25
void op_dec_h(z80_t* state)
{
	state->Reg.h--;
	state->Reg.h &= 255;
	state->Reg.f = (state->Reg.h ? 0 : 0x80);
	state->Reg.m = 1;
}

// 0x26
void op_ld_h_nn(z80_t* state)
{

}

// 0x27
void op_daa(z80_t* state)
{

}

// 0x28
void op_jr_z(z80_t* state)
{

}

// 0x29
void op_add_hl_hl(z80_t* state)
{

}

// 0x2A
void op_ldi_a_hl(z80_t* state)
{

}

// 0x2B
void op_dec_hl(z80_t* state)
{
	uint8_t i = read_byte((state->Reg.h << 8) + state->Reg.l) - 1;
	i &= 255;
	write_byte((state->Reg.h << 8) + state->Reg.l, i); state->Reg.f = (i ? 0 : 0x80);
	state->Reg.m = 3;
}

// 0x2C
void op_inc_l(z80_t* state)
{
	state->Reg.l++;
	state->Reg.l &= 255;
	state->Reg.f = (state->Reg.l ? 0 : 0x80);
	state->Reg.m = 1;
}

// 0x2D
void op_dec_l(z80_t* state)
{
	state->Reg.l--;
	state->Reg.l &= 255;
	state->Reg.f = (state->Reg.l ? 0 : 0x80);
	state->Reg.m = 1;
}

// 0x2E
void op_ld_l_nn(z80_t* state)
{

}

// 0x2F
void op_cpl(z80_t* state)
{

}

// 0x30
void op_jr_nc(z80_t* state)
{

}

// 0x31
void op_ld_sp_nn(z80_t* state)
{

}

// 0x32
void op_ldd_hl_a(z80_t* state)
{

}

// 0x33
void op_inc_sp(z80_t* state)
{

}

// 0x34
void op_inc_hl_(z80_t* state)
{

}

// 0x35
void op_dec_hl_(z80_t* state)
{

}

// 0x36
void op_ld_hl_nn(z80_t* state)
{

}

// 0x37
void op_scf(z80_t* state)
{

}

// 0x38
void op_jr_c(z80_t* state)
{

}

// 0x39
void op_add_hl_sp(z80_t* state)
{

}

// 0x3A
void op_ldd_a_hl(z80_t* state)
{

}

// 0x3B
void op_dec_sp(z80_t* state)
{

}

// 0x3C
void op_inc_a(z80_t* state)
{
	state->Reg.a++;
	state->Reg.a &= 255;
	state->Reg.f = (state->Reg.a ? 0 : 0x80);
	state->Reg.m = 1;
}

// 0x3D
void op_dec_a(z80_t* state)
{
	state->Reg.a--;
	state->Reg.a &= 255;
	state->Reg.f = (state->Reg.a ? 0 : 0x80);
	state->Reg.m = 1;
}

// 0x3E
void op_ld_a_nn(z80_t* state)
{

}

// 0x3F
void op_ccf(z80_t* state)
{

}

// 0x40
void op_ld_b_b(z80_t* state)
{

}

// 0x41
void op_ld_b_c(z80_t* state)
{

}

// 0x42
void op_ld_b_d(z80_t* state)
{

}

// 0x43
void op_ld_b_e(z80_t* state)
{

}

// 0x44
void op_ld_b_h(z80_t* state)
{

}

// 0x45
void op_ld_b_l(z80_t* state)
{

}

// 0x46
void op_ld_b_hl(z80_t* state)
{

}

// 0x47
void op_ld_b_a(z80_t* state)
{

}

// 0x48
void op_ld_c_b(z80_t* state)
{

}

// 0x49
void op_ld_c_c(z80_t* state)
{

}

// 0x4A
void op_ld_c_d(z80_t* state)
{

}

// 0x4B
void op_ld_c_e(z80_t* state)
{

}

// 0x4C
void op_ld_c_h(z80_t* state)
{

}

// 0x4D
void op_ld_c_l(z80_t* state)
{

}

// 0x4E
void op_ld_c_hl(z80_t* state)
{

}

// 0x4F
void op_ld_c_a(z80_t* state)
{

}



void add_rb(z80_t* state)
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


void add_rc(z80_t* state)
{
	int a = state->Reg.a;
	state->Reg.a += state->Reg.c;
	state->Reg.f = (state->Reg.a > 255) ? 0x10 : 0;
	state->Reg.a &= 255;
	if (!state->Reg.a) state->Reg.f |= 0x80;
	if ((state->Reg.a^state->Reg.c^a) & 0x10)
	state->Reg.f |= 0x20; state->Reg.m = 1;

}

void add_rd(z80_t* state)
{
	int a = state->Reg.a;
	state->Reg.a += state->Reg.d;
	state->Reg.f = (state->Reg.a > 255) ? 0x10 : 0;
	state->Reg.a &= 255;
	if (!state->Reg.a) state->Reg.f |= 0x80;
	if ((state->Reg.a^state->Reg.d^a) & 0x10)
	state->Reg.f |= 0x20; state->Reg.m = 1;
}

void add_re(z80_t* state)
{
	int a = state->Reg.a;
	state->Reg.a += state->Reg.e;
	state->Reg.f = (state->Reg.a > 255) ? 0x10 : 0;
	state->Reg.a &= 255;
	if (!state->Reg.a) state->Reg.f |= 0x80;
	if ((state->Reg.a^state->Reg.e^a) & 0x10)
	state->Reg.f |= 0x20; state->Reg.m = 1;
}

void add_rh(z80_t* state)
{
	int a = state->Reg.a;
	state->Reg.a += state->Reg.h;
	state->Reg.f = (state->Reg.a > 255) ? 0x10 : 0;
	state->Reg.a &= 255;
	if (!state->Reg.a) state->Reg.f |= 0x80;
	if ((state->Reg.a^state->Reg.h^a) & 0x10)
	state->Reg.f |= 0x20; state->Reg.m = 1;
}

void add_rl(z80_t* state)
{
	int a = state->Reg.a;
	state->Reg.a += state->Reg.l;
	state->Reg.f = (state->Reg.a > 255) ? 0x10 : 0;
	state->Reg.a &= 255;
	if (!state->Reg.a) state->Reg.f |= 0x80;
	if ((state->Reg.a^state->Reg.l^a) & 0x10)
	state->Reg.f |= 0x20; state->Reg.m = 1;
}

void add_ra(z80_t* state)
{
	int a = state->Reg.a;
	state->Reg.a += state->Reg.a;
	state->Reg.f = (state->Reg.a > 255) ? 0x10 : 0;
	state->Reg.a &= 255;
	if (!state->Reg.a) state->Reg.f |= 0x80;
	if ((state->Reg.a^state->Reg.a^a) & 0x10)
	state->Reg.f |= 0x20; state->Reg.m = 1;
}

void add_hl(z80_t* state)
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

/*
void add_n(z80_t* state);

void add_hlbc(z80_t* state);

void add_hlde(z80_t* state);

void add_hlhl(z80_t* state);

void add_hlsp(z80_t* state);

void add_spn(z80_t* state);



// Compare B to A (CP A, B)
void cp_rb(z80_t* state)
{
	int i = state->Reg.e;				// Temp copy
	i -= state->Reg.b;				// Subtract b
	state->Reg.f |= 0x40;				// Set subtraction flag
	if (!(i & 255)) state->Reg.f |= 0x80;		// Check for zero
	if (i < 0) state->Reg.f |= 0x10;		// Check for underflow
	state->Reg.m = 1; state->Reg.t = 4;		// 1 M-time taken 
}

void nop(z80_t* state)
{
	state->Reg.m = 1; 
	state->Reg.t = 4;
}

void push_bc(z80_t* state)
{
	state->Reg.sp--;				// Drop through stack
	write_byte(state->Reg.sp, state->Reg.b);	// Write b
	state->Reg.sp--;				// Drop
	write_byte(state->Reg.sp, state->Reg.c);	// Write c
	state->Reg.m = 3; state->Reg.t = 12;		// 3 M-times taken
}

void pop_hl(z80_t* state)
{
	state->Reg.l = read_byte(state->Reg.sp);	// Read L
	state->Reg.sp++;				// Move up stack
	state->Reg.h = read_byte(state->Reg.sp);	// Read H
	state->Reg.sp++;				// Move up stack
	state->Reg.m = 3; state->Reg.t = 12;		// 3 M-times taken
}

void lda_mm(z80_t* state)
{
	uint16_t addr = read_word(state->Reg.pc);	// Get address from instruction
	state->Reg.pc += 2;				// Advance PC
	state->Reg.a = read_byte(addr);			// Read from address
	state->Reg.m = 4; state->Reg.t = 16;		// 4 M-times taken
}
*/

void reset(z80_t* state)
{
	state->Reg.a = 0; state->Reg.b = 0; state->Reg.c = 0; state->Reg.d = 0;
	state->Reg.e = 0; state->Reg.h = 0; state->Reg.l = 0; state->Reg.f = 0;
	state->Reg.sp = 0;
	state->Reg.pc = 0;	// Start execution at 0

//	state->Clock.m = 0; state->Clock.t = 0;	
}

void print_state(z80_t* state)
{
	printf("8-Bit Registers:\na=%d, b=%d, c=%d, d=%d, e=%d, h=%d, l=%d, f=%d\n",
		state->Reg.a, state->Reg.b, state->Reg.c, state->Reg.d, state->Reg.e,
		state->Reg.h, state->Reg.l, state->Reg.f);
	printf("16-Bit Registers:\npc=%d, sp=%d\n", 
		state->Reg.pc, state->Reg.sp);
	printf("Clock:\nm=%d, t=%d\n",
		state->Reg.m, state->Reg.t);
}

// OpCode map
void *opcodes[256] = {
	// 0x00
	op_nop,
	op_ld_bc_nn,
	op_ld_bc_a,
	op_inc_bc,
	op_inc_b,
	op_dec_b,
	op_ld_b_nn,
	op_rlc_a,
	op_ld_nn_sp,	// 0x08
	op_add_hl_bc,
	op_ld_a_bc,
	op_dec_bc,
	op_inc_c,
	op_dec_c,
	op_ld_c_nn,
	op_rrc_a,
	op_stop,	// 0x10
	op_ld_de_nn,
	op_ld_de_a,
	op_inc_de,
	op_inc_d,
	op_dec_d,
	op_ld_d_nn,
	op_rl_a,
	op_jr,		// 0x18
	op_add_hl_de,
	op_ld_a_de,
	op_dec_de,
	op_inc_e,
	op_dec_e,
	op_ld_e_nn,
	op_rr_a,
	op_jr_nz,	// 0x20
	op_ld_hl_nnnn,
	op_ldi_hl_a,
	op_inc_hl,
	op_inc_h,
	op_dec_h,
	op_ld_h_nn,
	op_daa,
	op_jr_z,	// 0x28
	op_add_hl_hl,
	op_ldi_a_hl,
	op_dec_hl,
	op_inc_l,
	op_dec_l,
	op_ld_l_nn,
	op_cpl,
	op_jr_nc,	// 0x30
	op_ld_sp_nn,
	op_ldd_hl_a,
	op_inc_sp,
	op_inc_hl_,
	op_dec_hl_,
	op_ld_hl_nn,
	op_scf,
	op_jr_c,	// 0x38
	op_add_hl_sp,
	op_ldd_a_hl,
	op_dec_sp,
	op_inc_a,
	op_dec_a,
	op_ld_a_nn,
	op_ccf,
	op_ld_b_b,	// 0x40
	op_ld_b_c,
	op_ld_b_d,
	op_ld_b_e,
	op_ld_b_h,
	op_ld_b_l,
	op_ld_b_hl,
	op_ld_b_a,
	op_ld_c_b,	// 0x48
	op_ld_c_c,
	op_ld_c_d,
	op_ld_c_e,
	op_ld_c_h,
	op_ld_c_l,
	op_ld_c_hl,
	op_ld_c_a
	
};


