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
	state->Reg.b = read_byte(state->Reg.pc);
	state->Reg.pc++;
	state->Reg.m = 2;
}

// 0x07
void op_rlc_a(z80_t* state)
{
	uint8_t ci = (state->Reg.a & 0x80 ? 1 : 0);
	uint8_t co = (state->Reg.a & 0x80 ? 0x10 : 0);
	state->Reg.a = (state->Reg.a << 1) + ci;
	state->Reg.a &= 255;
	state->Reg.f = (state->Reg.f & 0xEF) + co;
	state->Reg.m = 1;
}

// 0x08
void op_ld_nn_sp(z80_t* state)
{
	uint8_t i = read_word(state->Reg.pc);
	state->Reg.pc += 2;
	write_word(i, (state->Reg.h << 8) + state->Reg.l);
	state->Reg.m = 5;
}

// 0x09
void op_add_hl_bc(z80_t* state)
{
	uint8_t hl = (state->Reg.h << 8) + state->Reg.l;
	hl += (state->Reg.b << 8) + state->Reg.c;
	
	if (hl > 65535)
		state->Reg.f |= 0x10;
	else 
		state->Reg.f &= 0xEF;

	state->Reg.h = (hl >> 8) & 255;
	state->Reg.l = hl & 255;
	state->Reg.m = 3;
}

// 0x0A
void op_ld_a_bc(z80_t* state)
{
	state->Reg.a = read_byte((state->Reg.b << 8) + state->Reg.c);
	state->Reg.m = 2;
}

// 0x0B
void op_dec_bc(z80_t* state)
{
	state->Reg.c = (state->Reg.c - 1) & 255;

	if (state->Reg.c == 255)
		state->Reg.b = (state->Reg.b - 1) & 255;
	
	state->Reg.m = 1;
}

// 0x0C
void op_inc_c(z80_t* state)
{
	state->Reg.c++;
	state->Reg.c &= 255;
	state->Reg.f = (state->Reg.c ? 0 : 0x80);
	state->Reg.m = 1;
}

// 0x0D
void op_dec_c(z80_t* state)
{
	state->Reg.c--;
	state->Reg.c &= 255;
	state->Reg.f = (state->Reg.c ? 0 : 0x80);
	state->Reg.m = 1;
}

// 0x0E
void op_ld_c_nn(z80_t* state)
{
	state->Reg.c = read_byte(state->Reg.pc);
	state->Reg.pc++;
	state->Reg.m = 2;
}

// 0x0F
void op_rrc_a(z80_t* state)
{
	uint8_t ci = (state->Reg.a & 1 ? 0x80 : 0);
	uint8_t co = (state->Reg.a & 1 ? 0x10 : 0);
	state->Reg.a = (state->Reg.a >> 1) + ci;
	state->Reg.a &= 255;
	state->Reg.f = (state->Reg.f & 0xEF) + co;
	state->Reg.m = 1;
}

// 0x10
void op_stop(z80_t* state)
{
	uint8_t i = read_byte(state->Reg.pc);
	
	if (i > 127)
		i -= ((~i + 1) & 255);

	state->Reg.pc++;
	state->Reg.m = 2;
	state->Reg.b--;
	
	if (state->Reg.b) {
		state->Reg.pc += i;
		state->Reg.m++;
	}
}

// 0x11
void op_ld_de_nn(z80_t* state)
{
	state->Reg.e = read_byte(state->Reg.pc);
	state->Reg.d = read_byte(state->Reg.pc + 1);
	state->Reg.pc += 2;
	state->Reg.m = 3;
}

// 0x12
void op_ld_de_a(z80_t* state)
{
	write_byte((state->Reg.d << 8) + state->Reg.e, state->Reg.a);
	state->Reg.m = 2;
}

// 0x13
void op_inc_de(z80_t* state)
{
	state->Reg.e = (state->Reg.e + 1) & 255;
	
	if (!state->Reg.e)
		state->Reg.d = (state->Reg.d + 1) & 255;
	
	state->Reg.m = 1;
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
	state->Reg.d = read_byte(state->Reg.pc);
	state->Reg.pc++;
	state->Reg.m = 2;

}

// 0x17
void op_rl_a(z80_t* state)
{
	uint8_t ci = (state->Reg.f & 0x10 ? 1 : 0);
	uint8_t co = (state->Reg.a & 0x80 ? 0x10 : 0);
	state->Reg.a = (state->Reg.a << 1) + ci;
	state->Reg.a &= 255;
	state->Reg.f = (state->Reg.f & 0xEF) + co;
	state->Reg.m = 1;
}

// 0x18
void op_jr(z80_t* state)
{
	uint8_t i = read_byte(state->Reg.pc);
	
	if (i > 127)
		i -= ((~i + 1) & 255);

	state->Reg.pc++;
	state->Reg.m = 2;
	state->Reg.pc += i;
	state->Reg.m++;
}

// 0x19
void op_add_hl_de(z80_t* state)
{
	uint8_t hl = (state->Reg.h << 8) + state->Reg.l;
	hl += (state->Reg.d << 8) + state->Reg.e;

	if (hl > 65535)
		state->Reg.f |= 0x10;
	else 
		state->Reg.f &= 0xEF;

	state->Reg.h = (hl >> 8) & 255;
	state->Reg.l = hl & 255;
	state->Reg.m = 3;
}

// 0x1A
void op_ld_a_de(z80_t* state)
{
	state->Reg.a = read_byte((state->Reg.d << 8) + state->Reg.e);
	state->Reg.m = 2;
}

// 0x1B
void op_dec_de(z80_t* state)
{
	state->Reg.e = (state->Reg.e - 1) & 255;

	if (state->Reg.e == 255)
		state->Reg.d = (state->Reg.d - 1) & 255;
	
	state->Reg.m = 1;
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
	state->Reg.e = read_byte(state->Reg.pc);
	state->Reg.pc++;
	state->Reg.m = 2;
}

// 0x1F
void op_rr_a(z80_t* state)
{
	uint8_t ci = (state->Reg.f & 0x10 ? 0x80 : 0);
	uint8_t co = (state->Reg.a & 1 ? 0x10 : 0);

	state->Reg.a = (state->Reg.a >> 1) + ci;
	state->Reg.a &= 255;
	state->Reg.f = (state->Reg.f & 0xEF) + co;
	state->Reg.m = 1;
}

// 0x20
void op_jr_nz(z80_t* state)
{
	uint8_t i = read_byte(state->Reg.pc);
	
	if (i > 127)
		i -= ((~i + 1) & 255);

	state->Reg.pc++;
	state->Reg.m = 2;
	
	if ((state->Reg.f & 0x80) == 0x00) {
		state->Reg.pc += i;
		state->Reg.m++;
	}
}

// 0x21
void op_ld_hl_nnnn(z80_t* state)
{
	state->Reg.l = read_byte(state->Reg.pc);
	state->Reg.h = read_byte(state->Reg.pc + 1);
	state->Reg.pc += 2;
	state->Reg.m = 3;
}

// 0x22
void op_ldi_hl_a(z80_t* state)
{
	write_byte((state->Reg.h << 8) + state->Reg.l, state->Reg.a);
	state->Reg.l = (state->Reg.l + 1) & 255;
	
	if (!state->Reg.l)
		state->Reg.h = (state->Reg.h + 1) & 255;

	state->Reg.m = 2; 
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
	state->Reg.h = read_byte(state->Reg.pc);
	state->Reg.pc++;
	state->Reg.m = 2;
}

// 0x27
void op_daa(z80_t* state)
{
	uint8_t a = state->Reg.a;
	if ((state->Reg.f & 0x20) || ((state->Reg.a & 15) > 9))
		state->Reg.a += 6;
	
	state->Reg.f &= 0xEF;
	if ((state->Reg.f & 0x20) || (a > 0x99)) {
		state->Reg.a += 0x60;
		state->Reg.f |= 0x10;
	}

	state->Reg.m = 1;
}

// 0x28
void op_jr_z(z80_t* state)
{
	uint8_t i = read_byte(state->Reg.pc);
	if (i > 127)
		i =- ((~i + 1) &  255);

	state->Reg.pc++;
	state->Reg.m = 2;

	if ((state->Reg.f & 0x80) == 0x80) {
		state->Reg.pc += i;
		state->Reg.m++;
	}
}

// 0x29
void op_add_hl_hl(z80_t* state)
{
	uint8_t hl = (state->Reg.h << 8) + state->Reg.l;
	hl += (state->Reg.h << 8) + state->Reg.l;

	if (hl > 65535)
		state->Reg.f |= 0x10;
	else 
		state->Reg.f &= 0xEF;

	state->Reg.h = (hl >> 8) & 255;
	state->Reg.h = hl & 255;
	state->Reg.m = 3;
}

// 0x2A
void op_ldi_a_hl(z80_t* state)
{
	state->Reg.a = read_byte((state->Reg.h << 8) + state->Reg.l);
	state->Reg.l = (state->Reg.l + 1) & 255;

	if (!state->Reg.l)
		state->Reg.h = (state->Reg.h + 1) & 255;

	state->Reg.m = 2;
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
	state->Reg.l = read_byte(state->Reg.pc);
	state->Reg.pc++;
	state->Reg.m = 2;
}

// 0x2F
void op_cpl(z80_t* state)
{
	state->Reg.a ^= 255;
	state->Reg.f = (state->Reg.a ? 0 : 0x80);
	state->Reg.m = 1;
}

// 0x30
void op_jr_nc(z80_t* state)
{
	uint8_t i = read_byte(state->Reg.pc);
	if (i > 127) 
		i -= ((~i + 1) & 255);

	state->Reg.pc++;
	state->Reg.m = 2;

	if ((state->Reg.f & 0x010) == 0x00) {
		state->Reg.pc += i;
		state->Reg.m++;
	}
}

// 0x31
void op_ld_sp_nn(z80_t* state)
{
	state->Reg.sp = read_word(state->Reg.pc);
	state->Reg.pc += 2;
	state->Reg.m = 3;
}

// 0x32
void op_ldd_hl_a(z80_t* state)
{
	write_byte((state->Reg.h << 8) + state->Reg.l, state->Reg.a);

	state->Reg.l = (state->Reg.l - 1) & 255;
	
	if (state->Reg.l == 255)
		state->Reg.h = (state->Reg.h - 1) & 255;
	
	state->Reg.m = 2;
}

// 0x33
void op_inc_sp(z80_t* state)
{
	state->Reg.sp = (state->Reg.sp + 1) & 65535;

	state->Reg.m = 1;
}

// 0x34
void op_inc_hl_(z80_t* state)
{
	uint8_t i = read_byte((state->Reg.h << 8) + state->Reg.l) + 1;
	i &= 255;
	write_byte((state->Reg.h << 8) + state->Reg.l, i);
	
	state->Reg.f = (i ? 0 : 0x80);
	state->Reg.m = 3;
}

// 0x35
void op_dec_hl_(z80_t* state)
{
	uint8_t i = read_byte((state->Reg.h << 8) + state->Reg.l) -1;
	i &= 255;
	write_byte((state->Reg.h << 8) + state->Reg.l, i);
	
	state->Reg.f = (i ? 0 : 0x80);
	state->Reg.m = 3;
}

// 0x36
void op_ld_hl_nn(z80_t* state)
{
	write_byte((state->Reg.h << 8) + state->Reg.l, read_byte(state->Reg.pc));
	state->Reg.pc++;

	state->Reg.m = 3;
	
}

// 0x37
void op_scf(z80_t* state)
{
	state->Reg.f |= 0x10;
	
	state->Reg.m = 1;
}

// 0x38
void op_jr_c(z80_t* state)
{
	uint8_t i = read_byte(state->Reg.pc);
		
	if (i > 127)
		i -= ((~i + 1) & 255);

	state->Reg.pc++;
	
	if ((state->Reg.f & 0x10) == 0x10) {
		state->Reg.pc += i;
		state->Reg.m++;
	}
}

// 0x39
void op_add_hl_sp(z80_t* state)
{
	uint8_t hl = (state->Reg.h << 8) + state->Reg.l;
	hl += state->Reg.sp;

	if (hl > 65535)
		state->Reg.f |= 0x10;
	else
		state->Reg.f &= 0xEF;

	state->Reg.h = (hl >> 8) & 255;
	state->Reg.l = hl & 255;

	state->Reg.m = 3;
}

// 0x3A
void op_ldd_a_hl(z80_t* state)
{
	state->Reg.a = read_byte((state->Reg.h << 8) + state->Reg.l);
	state->Reg.l = (state->Reg.l - 1) & 255;

	if (state->Reg.l == 255)
		state->Reg.h = (state->Reg.h - 1) & 255;

	state->Reg.m = 2;
}

// 0x3B
void op_dec_sp(z80_t* state)
{
	state->Reg.sp = (state->Reg.sp - 1) & 65535;
	
	state->Reg.m = 1;
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
	state->Reg.a = read_byte(state->Reg.pc);
	state->Reg.pc++;
	state->Reg.m = 2;

}

// 0x3F
void op_ccf(z80_t* state)
{
	uint8_t ci = (state->Reg.f & 0x10 ? 0 : 0x10);
	state->Reg.f = (state->Reg.f & 0xEF) + ci;
	state->Reg.m = 1;
}

// 0x40
void op_ld_b_b(z80_t* state)
{
	state->Reg.b = state->Reg.b;
	state->Reg.m = 1;
}

// 0x41
void op_ld_b_c(z80_t* state)
{
	state->Reg.b = state->Reg.c;
	state->Reg.m = 1;
}

// 0x42
void op_ld_b_d(z80_t* state)
{
	state->Reg.b = state->Reg.d;
	state->Reg.m = 1;
}

// 0x43
void op_ld_b_e(z80_t* state)
{
	state->Reg.b = state->Reg.e;
	state->Reg.m = 1;
}

// 0x44
void op_ld_b_h(z80_t* state)
{
	state->Reg.b = state->Reg.h;
	state->Reg.m = 1;
}

// 0x45
void op_ld_b_l(z80_t* state)
{
	state->Reg.b = state->Reg.l;
	state->Reg.m = 1;
}

// 0x46
void op_ld_b_hl(z80_t* state)
{
	state->Reg.b = read_byte((state->Reg.h << 8) + state->Reg.l);
	state->Reg.m = 2;
}

// 0x47
void op_ld_b_a(z80_t* state)
{
	state->Reg.b = state->Reg.a;
	state->Reg.m = 1;
}

// 0x48
void op_ld_c_b(z80_t* state)
{
	state->Reg.c = state->Reg.b;
	state->Reg.m = 1;
}

// 0x49
void op_ld_c_c(z80_t* state)
{
	state->Reg.c = state->Reg.c;
	state->Reg.m = 1;
}

// 0x4A
void op_ld_c_d(z80_t* state)
{
	state->Reg.c = state->Reg.d;
	state->Reg.m = 1;
}

// 0x4B
void op_ld_c_e(z80_t* state)
{
	state->Reg.c = state->Reg.e;
	state->Reg.m = 1;
}

// 0x4C
void op_ld_c_h(z80_t* state)
{
	state->Reg.c = state->Reg.h;
	state->Reg.m = 1;
}

// 0x4D
void op_ld_c_l(z80_t* state)
{
	state->Reg.c = state->Reg.l;
	state->Reg.m = 1;
}

// 0x4E
void op_ld_c_hl(z80_t* state)
{
	state->Reg.c = read_byte((state->Reg.h << 8) + state->Reg.l);
	state->Reg.m = 2;
}

// 0x4F
void op_ld_c_a(z80_t* state)
{
	state->Reg.c = state->Reg.c;
	state->Reg.m = 1;
}

// 0x50
void op_ld_d_b(z80_t* state)
{
	state->Reg.d = state->Reg.b;
	state->Reg.m = 1;
}

// 0x51
void op_ld_d_c(z80_t* state)
{
	state->Reg.d = state->Reg.c;
	state->Reg.m = 1;
}

// 0x52
void op_ld_d_d(z80_t* state)
{
	state->Reg.d = state->Reg.d;
	state->Reg.m = 1;
}

// 0x53
void op_ld_d_e(z80_t* state)
{
	state->Reg.d = state->Reg.e;
	state->Reg.m = 1;
}

// 0x54
void op_ld_d_h(z80_t* state)
{
	state->Reg.d = state->Reg.h;
	state->Reg.m = 1;
}

// 0x55
void op_ld_d_l(z80_t* state)
{
	state->Reg.d = state->Reg.l;
	state->Reg.m = 1;
}

// 0x56
void op_ld_d_hl(z80_t* state)
{

}

// 0x57
void op_ld_d_a(z80_t* state)
{
	state->Reg.d = state->Reg.a;
	state->Reg.m = 1;
}

// 0x58
void op_ld_e_b(z80_t* state)
{
	state->Reg.e = state->Reg.b;
	state->Reg.m = 1;
}

// 0x59
void op_ld_e_c(z80_t* state)
{
	state->Reg.e = state->Reg.c;
	state->Reg.m = 1;
}

// 0x5A
void op_ld_e_d(z80_t* state)
{
	state->Reg.e = state->Reg.d;
	state->Reg.m = 1;
}

// 0x5B
void op_ld_e_e(z80_t* state)
{
	state->Reg.e = state->Reg.e;
	state->Reg.m = 1;
}

// 0x5C
void op_ld_e_h(z80_t* state)
{
	state->Reg.e = state->Reg.h;
	state->Reg.m = 1;
}

// 0x5D
void op_ld_e_l(z80_t* state)
{
	state->Reg.e = state->Reg.l;
	state->Reg.m = 1;
}

// 0x5E
void op_ld_e_hl(z80_t* state)
{

}

// 0x5F
void op_ld_e_a(z80_t* state)
{
	state->Reg.e = state->Reg.a;
	state->Reg.m = 1;
}

// 0x60
void op_ld_h_b(z80_t* state)
{
	state->Reg.h = state->Reg.b;
	state->Reg.m = 1;
}

// 0x61
void op_ld_h_c(z80_t* state)
{
	state->Reg.h = state->Reg.c;
	state->Reg.m = 1;
}

// 0x62
void op_ld_h_d(z80_t* state)
{
	state->Reg.h = state->Reg.d;
	state->Reg.m = 1;
}

// 0x63
void op_ld_h_e(z80_t* state)
{
	state->Reg.h = state->Reg.e;
	state->Reg.m = 1;
}

// 0x64
void op_ld_h_h(z80_t* state)
{
	state->Reg.h = state->Reg.h;
	state->Reg.m = 1;
}

// 0x65
void op_ld_h_l(z80_t* state)
{
	state->Reg.h = state->Reg.l;
	state->Reg.m = 1;
}

// 0x66
void op_ld_h_hl(z80_t* state)
{
	state->Reg.h = read_byte((state->Reg.h << 8) + state->Reg.l);
	state->Reg.m = 2;
}

// 0x67
void op_ld_h_a(z80_t* state)
{
	state->Reg.h = state->Reg.a;
	state->Reg.m = 1;
}

// 0x68
void op_ld_l_b(z80_t* state)
{
	state->Reg.l = state->Reg.b;
	state->Reg.m = 1;
}

// 0x69
void op_ld_l_c(z80_t* state)
{
	state->Reg.l = state->Reg.c;
	state->Reg.m = 1;
}

// 0x6A
void op_ld_l_d(z80_t* state)
{
	state->Reg.l = state->Reg.d;
	state->Reg.m = 1;
}

// 0x6B
void op_ld_l_e(z80_t* state)
{
	state->Reg.l = state->Reg.e;
	state->Reg.m = 1;
}

// 0x6C
void op_ld_l_h(z80_t* state)
{
	state->Reg.l = state->Reg.h;
	state->Reg.m = 1;
}

// 0x6D
void op_ld_l_l(z80_t* state)
{
	state->Reg.l = state->Reg.l;
	state->Reg.m = 1;
}

// 0x6E
void op_ld_l_hl(z80_t* state)
{
	state->Reg.l = read_byte((state->Reg.h << 8) + state->Reg.l);
	state->Reg.m = 2;
}

// 0x6F
void op_ld_l_a(z80_t* state)
{
	state->Reg.l = state->Reg.a;
	state->Reg.m = 1;
}

// 0x70
void op_ld_hl_b(z80_t* state)
{
	write_byte((state->Reg.h << 8) + state->Reg.l, state->Reg.b);
	state->Reg.m = 2;
}

// 0x71
void op_ld_hl_c(z80_t* state)
{
	write_byte((state->Reg.h << 8) + state->Reg.l, state->Reg.c);
	state->Reg.m = 2;
}

// 0x72
void op_ld_hl_d(z80_t* state)
{
	write_byte((state->Reg.h << 8) + state->Reg.l, state->Reg.d);
	state->Reg.m = 2;
}

// 0x73
void op_ld_hl_e(z80_t* state)
{
	write_byte((state->Reg.h << 8) + state->Reg.l, state->Reg.e);
	state->Reg.m = 2;
}

// 0x74
void op_ld_hl_h(z80_t* state)
{
	write_byte((state->Reg.h << 8) + state->Reg.l, state->Reg.h);
	state->Reg.m = 2;
}

// 0x75
void op_ld_hl_l(z80_t* state)
{
	write_byte((state->Reg.h << 8) + state->Reg.l, state->Reg.l);
	state->Reg.m = 2;
}

// 0x76
void op_halt(z80_t* state)
{
	state->halt = 1;
	state->Reg.m = 1;
}

// 0x77
void op_ld_hl_a(z80_t* state)
{
	write_byte((state->Reg.h << 8) + state->Reg.l, state->Reg.a);
	state->Reg.m = 2;
}

// 0x78
void op_ld_a_b(z80_t* state)
{
	state->Reg.a = state->Reg.a;
	state->Reg.m = 1;
}

// 0x79
void op_ld_a_c(z80_t* state)
{
	state->Reg.a = state->Reg.c;
	state->Reg.m = 1;
}

// 0x7A
void op_ld_a_d(z80_t* state)
{
	state->Reg.a = state->Reg.d;
	state->Reg.m = 1;
}

// 0x7B
void op_ld_a_e(z80_t* state)
{
	state->Reg.a = state->Reg.c;
	state->Reg.m = 1;
}

// 0x7C
void op_ld_a_h(z80_t* state)
{
	state->Reg.a = state->Reg.h;
	state->Reg.m = 1;
}

// 0x7D
void op_ld_a_l(z80_t* state)
{
	state->Reg.a = state->Reg.l;
	state->Reg.m = 1;
}

// 0x7E
void op_ld_a_hl(z80_t* state)
{
	state->Reg.a = read_byte((state->Reg.h << 8) + state->Reg.l);
	state->Reg.m = 2;
}

// 0x7F
void op_ld_a_a(z80_t* state)
{
	state->Reg.a = state->Reg.a;
	state->Reg.m = 1;
}

// 0x80
void op_add_a_b(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a += state->Reg.b;
	
	state->Reg.f = ((state->Reg.a > 255) ? 0x10 : 0);
	state->Reg.a &= 255;

	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.b ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x81
void op_add_a_c(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a += state->Reg.c;
	
	state->Reg.f = ((state->Reg.a > 255) ? 0x10 : 0);
	state->Reg.a &= 255;

	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.c ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x82
void op_add_a_d(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a += state->Reg.d;
	
	state->Reg.f = ((state->Reg.a > 255) ? 0x10 : 0);
	state->Reg.a &= 255;

	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.d ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x83
void op_add_a_e(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a += state->Reg.e;
	
	state->Reg.f = ((state->Reg.a > 255) ? 0x10 : 0);
	state->Reg.a &= 255;

	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.e ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x84
void op_add_a_h(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a += state->Reg.h;
	
	state->Reg.f = ((state->Reg.a > 255) ? 0x10 : 0);
	state->Reg.a &= 255;

	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.h ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x85
void op_add_a_l(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a += state->Reg.l;
	
	state->Reg.f = ((state->Reg.a > 255) ? 0x10 : 0);
	state->Reg.a &= 255;

	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.l ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x86
void op_add_a_hl(z80_t* state)
{
	uint8_t a = state->Reg.a;
	uint8_t m = read_byte((state->Reg.h << 8) + state->Reg.l);
	state->Reg.a += m;

	state->Reg.f = ((state->Reg.a > 255) ? 0x10 : 0);
	state->Reg.a &= 255;

	if (!state->Reg.a)
		state->Reg.f |= 0x80;
		
	if ((state->Reg.a ^ a ^ m) & 0x10)
		state->Reg.f |= 0x20;
	
	state->Reg.m = 2;
}

// 0x87
void op_add_a_a(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a += state->Reg.a;
	
	state->Reg.f = ((state->Reg.a > 255) ? 0x10 : 0);
	state->Reg.a &= 255;

	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.a ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;

}

// 0x88
void op_adc_b(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a += state->Reg.b;

	state->Reg.a += ((state->Reg.f & 0x10) ? 1 : 0);
	state->Reg.f = ((state->Reg.a > 255) ? 0x10 : 0);
	state->Reg.a &= 255;
	
	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.b ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x89
void op_adc_c(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a += state->Reg.c;

	state->Reg.a += ((state->Reg.f & 0x10) ? 1 : 0);
	state->Reg.f = ((state->Reg.a > 255) ? 0x10 : 0);
	state->Reg.a &= 255;
	
	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.c ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x8A
void op_adc_d(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a += state->Reg.d;

	state->Reg.a += ((state->Reg.f & 0x10) ? 1 : 0);
	state->Reg.f = ((state->Reg.a > 255) ? 0x10 : 0);
	state->Reg.a &= 255;
	
	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.d ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x8B
void op_adc_e(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a += state->Reg.e;

	state->Reg.a += ((state->Reg.f & 0x10) ? 1 : 0);
	state->Reg.f = ((state->Reg.a > 255) ? 0x10 : 0);
	state->Reg.a &= 255;
	
	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.e ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x8C
void op_adc_h(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a += state->Reg.h;

	state->Reg.a += ((state->Reg.f & 0x10) ? 1 : 0);
	state->Reg.f = ((state->Reg.a > 255) ? 0x10 : 0);
	state->Reg.a &= 255;
	
	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.h ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x8D
void op_adc_l(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a += state->Reg.l;

	state->Reg.a += ((state->Reg.f & 0x10) ? 1 : 0);
	state->Reg.f = ((state->Reg.a > 255) ? 0x10 : 0);
	state->Reg.a &= 255;
	
	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.l ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x8E
void op_adc_hl(z80_t* state)
{
	uint8_t a = state->Reg.a;
	uint8_t m = read_byte((state->Reg.h << 8) + state->Reg.l);

	state->Reg.a += m;
	state->Reg.a += ((state->Reg.f & 0x10) ? 1 : 0);
	state->Reg.f = ((state->Reg.a > 255) ? 0x10 : 0);
	state->Reg.a &= 255;
	
	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ m ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 2;	
}

// 0x8F
void op_adc_a(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a += state->Reg.a;

	state->Reg.a += ((state->Reg.f & 0x10) ? 1 : 0);
	state->Reg.f = ((state->Reg.a > 255) ? 0x10 : 0);
	state->Reg.a &= 255;
	
	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.a ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x90
void op_sub_b(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a -= state->Reg.b;
	
	state->Reg.f = ((state->Reg.a < 0) ? 0x50 : 0x40);
	state->Reg.a &= 255;
	
	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.b ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x91
void op_sub_c(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a -= state->Reg.c;
	
	state->Reg.f = ((state->Reg.a < 0) ? 0x50 : 0x40);
	state->Reg.a &= 255;
	
	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.c ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x92
void op_sub_d(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a -= state->Reg.d;
	
	state->Reg.f = ((state->Reg.a < 0) ? 0x50 : 0x40);
	state->Reg.a &= 255;
	
	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.d ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x93
void op_sub_e(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a -= state->Reg.e;
	
	state->Reg.f = ((state->Reg.a < 0) ? 0x50 : 0x40);
	state->Reg.a &= 255;
	
	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.e ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x94
void op_sub_h(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a -= state->Reg.h;
	
	state->Reg.f = ((state->Reg.a < 0) ? 0x50 : 0x40);
	state->Reg.a &= 255;
	
	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.h ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x95
void op_sub_l(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a -= state->Reg.l;
	
	state->Reg.f = ((state->Reg.a < 0) ? 0x50 : 0x40);
	state->Reg.a &= 255;
	
	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.l ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x96
void op_sub_hl(z80_t* state)
{
	uint8_t a = state->Reg.a;
	uint8_t m = read_byte((state->Reg.h << 8) + state->Reg.l);
	
	state->Reg.a -= m;
	state->Reg.f = ((state->Reg.a < 0) ? 0x50 : 0x40);
	state->Reg.a &= 255;

	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ m ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 2;
}

// 0x97
void op_sub_a(z80_t* state)
{
	uint8_t a = state->Reg.a;
	state->Reg.a -= state->Reg.a;
	
	state->Reg.f = ((state->Reg.a < 0) ? 0x50 : 0x40);
	state->Reg.a &= 255;
	
	if (!state->Reg.a)
		state->Reg.f |= 0x80;

	if ((state->Reg.a ^ state->Reg.a ^ a) & 0x10)
		state->Reg.f |= 0x20;

	state->Reg.m = 1;
}

// 0x98
void op_sbc_b(z80_t* state)
{

}

// 0x99
void op_sbc_c(z80_t* state)
{

}

// 0x9A
void op_sbc_d(z80_t* state)
{

}

// 0x9B
void op_sbc_e(z80_t* state)
{

}

// 0x9C
void op_sbc_h(z80_t* state)
{

}

// 0x9D
void op_sbc_l(z80_t* state)
{

}

// 0x9E
void op_sbc_hl(z80_t* state)
{

}

// 0x9F
void op_sbc_a(z80_t* state)
{

}

// 0xA0
void op_and_b(z80_t* state)
{

}

// 0xA1
void op_and_c(z80_t* state)
{

}

// 0xA2
void op_and_d(z80_t* state)
{

}

// 0xA3
void op_and_e(z80_t* state)
{

}

// 0xA4
void op_and_h(z80_t* state)
{

}

// 0xA5
void op_and_l(z80_t* state)
{

}

// 0xA6
void op_and_hl(z80_t* state)
{

}

// 0xA7
void op_and_a(z80_t* state)
{

}

// 0xA8
void op_xor_b(z80_t* state)
{

}

// 0xA9
void op_xor_c(z80_t* state)
{

}

// 0xAA
void op_xor_d(z80_t* state)
{

}

// 0xAB
void op_xor_e(z80_t* state)
{

}

// 0xAC
void op_xor_h(z80_t* state)
{

}

// 0xAD
void op_xor_l(z80_t* state)
{

}

// 0xAE
void op_xor_hl(z80_t* state)
{

}

// 0xAF
void op_xor_a(z80_t* state)
{

}

// 0xB0
void op_or_b(z80_t* state)
{

}

// 0xB1
void op_or_c(z80_t* state)
{

}

// 0xB2
void op_or_d(z80_t* state)
{

}

// 0xB3
void op_or_e(z80_t* state)
{

}

// 0xB4
void op_or_h(z80_t* state)
{

}

// 0xB5
void op_or_l(z80_t* state)
{

}

// 0xB6
void op_or_hl(z80_t* state)
{

}

// 0xB7
void op_or_a(z80_t* state)
{

}

// 0xB8
void op_cp_b(z80_t* state)
{

}

// 0xB9
void op_cp_c(z80_t* state)
{

}

// 0xBA
void op_cp_d(z80_t* state)
{

}

// 0xBB
void op_cp_e(z80_t* state)
{

}

// 0xBC
void op_cp_h(z80_t* state)
{

}

// 0xBD
void op_cp_l(z80_t* state)
{

}

// 0xBE
void op_cp_hl(z80_t* state)
{

}

// 0xBF
void op_cp_a(z80_t* state)
{

}

// 0xC0
void op_ret_nz(z80_t* state)
{

}

// 0xC1
void op_pop_bc(z80_t* state)
{

}

// 0xC2
void op_jp_nz(z80_t* state)
{

}

// 0xC3
void op_jp(z80_t* state)
{

}

// 0xC4
void op_call_nz(z80_t* state)
{

}

// 0xC5
void op_push_bc(z80_t* state)
{

}

// 0xC6
void op_add_a_nn(z80_t* state)
{

}

// 0xC7
void op_rst_0(z80_t* state)
{

}

// 0xC8
void op_ret_z(z80_t* state)
{

}

// 0xC9
void op_ret(z80_t* state)
{

}

// 0xCA
void op_jp_z(z80_t* state)
{

}

// 0xCB
void op_cb(z80_t* state)
{

}

// 0xCC
void op_call_z(z80_t* state)
{

}

// 0xCD
void op_call(z80_t* state)
{

}

// 0xCE
void op_adc_nn(z80_t* state)
{

}

// 0xCF
void op_rst_8(z80_t* state)
{

}

// 0xD0
void op_ret_nc(z80_t* state)
{

}

// 0xD1
void op_pop_de(z80_t* state)
{

}

// 0xD2
void op_jp_nc(z80_t* state)
{

}

// 0xD3

// 0xD4
void op_call_nc(z80_t* state)
{

}

// 0xD5
void op_push_de(z80_t* state)
{

}

// 0xD6
void op_sub_nn(z80_t* state)
{

}

// 0xD7
void op_rst_10(z80_t* state)
{

}

// 0xD8
void op_ret_c(z80_t* state)
{

}

// 0xD9
void op_reti(z80_t* state)
{

}

// 0xDA
void op_jp_c(z80_t* state)
{

}

// 0xDB

// 0xDC
void op_call_c(z80_t* state)
{

}

// 0xDD

// 0xDE
void op_sbc_a_nn(z80_t* state)
{

}

// 0xDF
void op_rst_18(z80_t* state)
{

}

// 0xE0
void op_ld_ffnn_a(z80_t* state)
{

}

// 0xE1
void op_pop_hl(z80_t* state)
{

}

// 0xE2
void op_ld_ffc_a(z80_t* state)
{

}

// 0xE3

// 0xE4

// 0xE5
void op_push_hl(z80_t* state)
{

}

// 0xE6
void op_and_nn(z80_t* state)
{

}

// 0xE7
void op_rst_20(z80_t* state)
{

}

// 0xE8
void op_add_sp_nn(z80_t* state)
{

}

// 0xE9
void op_jp_hl(z80_t* state)
{

}

// 0xEA
void op_ld_nnnn_a(z80_t* state)
{

}

// 0xEB

// 0xEC

// 0xED

// 0xEE
void op_xor_nn(z80_t* state)
{

}

// 0xEF
void op_rst_28(z80_t* state)
{

}

// 0xF0
void op_ld_a_ffnn(z80_t* state)
{

}

// 0xF1
void op_pop_af(z80_t* state)
{

}

// 0xF2
void op_ld_a_ffc(z80_t* state)
{

}

// 0xF3
void op_di(z80_t* state)
{

}

// 0xF4

// 0xF5
void op_push_af(z80_t* state)
{

}

// 0xF6
void op_or_nn(z80_t* state)
{

}

// 0xF7
void op_rst_30(z80_t* state)
{

}

// 0xF8
void op_hl_ld_sp_nn(z80_t* state)
{

}

// 0xF9
void op_ld_sp_hl(z80_t* state)
{

}

// 0xFA
void op_ld_a_nnnn(z80_t* state)
{

}

// 0xFB
void op_ei(z80_t* state)
{

}

// 0xFC

// 0xFD
void op_undefined(z80_t* state)
{

}

// 0xFE
void op_cp_nn(z80_t* state)
{

}

// 0xFF
void op_rst_38(z80_t* state)
{

}

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
	printf("8-Bit Registers: a=%d, b=%d, c=%d, d=%d, e=%d, h=%d, l=%d, f=%d ",
		state->Reg.a, state->Reg.b, state->Reg.c, state->Reg.d, state->Reg.e,
		state->Reg.h, state->Reg.l, state->Reg.f);
	printf("16-Bit Registers: pc=%d, sp=%d ", 
		state->Reg.pc, state->Reg.sp);
	printf("Clock: m=%d, t=%d\n",
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
	op_ld_c_a,
	op_ld_d_b,	// 0x50
	op_ld_d_c,
	op_ld_d_d,
	op_ld_d_e,
	op_ld_d_h,
	op_ld_d_l,
	op_ld_d_hl,
	op_ld_d_a,
	op_ld_e_b,	// 0x58
	op_ld_e_c,
	op_ld_e_d,
	op_ld_e_h,
	op_ld_e_l,
	op_ld_e_hl,
	op_ld_e_a,
	op_ld_h_b,	// 0x60
	op_ld_h_c,
	op_ld_h_d,
	op_ld_h_e,
	op_ld_h_h,
	op_ld_h_l,
	op_ld_h_hl,
	op_ld_h_a,
	op_ld_l_b,	// 0x68
	op_ld_l_c,
	op_ld_l_d,
	op_ld_l_e,
	op_ld_l_h,
	op_ld_l_l,
	op_ld_l_hl,
	op_ld_l_a,
	op_ld_hl_b,	// 0x70
	op_ld_hl_c,
	op_ld_hl_d,
	op_ld_hl_e,
	op_ld_hl_h,
	op_ld_hl_l,
	op_halt,
	op_ld_hl_a,
	op_ld_a_b,	// 0x78
	op_ld_a_c,
	op_ld_a_d,
	op_ld_a_e,
	op_ld_a_h,
	op_ld_a_l,
	op_ld_a_hl,
	op_ld_a_a,
	op_add_a_b,	// 0x80
	op_add_a_c,
	op_add_a_d,
	op_add_a_e,
	op_add_a_h,
	op_add_a_l,
	op_add_a_hl,
	op_add_a_a,
	op_adc_b,	// 0x88
	op_adc_c,
	op_adc_d,
	op_adc_e,
	op_adc_h,
	op_adc_l,
	op_adc_hl,
	op_adc_a,
	op_sub_b,	// 0x90
	op_sub_c,
	op_sub_d,
	op_sub_e,
	op_sub_h,
	op_sub_l,
	op_sub_hl,
	op_sub_a,
	op_sbc_b,	// 0x98
	op_sbc_c,
	op_sbc_d,
	op_sbc_e,
	op_sbc_h,
	op_sbc_l,
	op_sbc_hl,
	op_sbc_a,
	op_and_b,	// 0xA0
	op_and_c,
	op_and_d,
	op_and_e,
	op_and_h,
	op_and_l,
	op_and_hl,
	op_and_a,
	op_xor_b,	// 0xA8
	op_xor_c,
	op_xor_d,
	op_xor_e,
	op_xor_h,
	op_xor_l,
	op_xor_hl,
	op_xor_a,
	op_or_b,	// 0xB0
	op_or_c,
	op_or_d,
	op_or_e,
	op_or_h,
	op_or_l,
	op_or_hl,
	op_or_a,
	op_cp_b,	// 0xB8
	op_cp_c,
	op_cp_d,
	op_cp_e,
	op_cp_h,
	op_cp_l,
	op_cp_hl,
	op_cp_a,
	op_ret_nz,	// 0xC0
	op_pop_bc,
	op_jp_nz,
	op_jp,
	op_call_nz,
	op_push_bc,
	op_add_a_nn,
	op_rst_0,
	op_ret_z,	// 0xC8
	op_ret,
	op_jp_z,
	op_cb,
	op_call_z,
	op_call,
	op_adc_nn,
	op_rst_8,
	op_ret_nc,	// 0xD0
	op_pop_de,
	op_jp_nc,
	op_undefined,
	op_call_nz,
	op_push_de,
	op_sub_nn,
	op_rst_10,
	op_ret_c,	// 0xD8
	op_reti,
	op_jp_c,
	op_undefined,
	op_call_c,
	op_undefined,
	op_sbc_a_nn,
	op_rst_18,
	op_ld_ffnn_a,	// 0xE0
	op_pop_hl,
	op_ld_ffc_a,
	op_undefined,
	op_undefined,
	op_push_hl,
	op_and_nn,
	op_rst_20,
	op_add_sp_nn,	// 0xE8
	op_jp_hl,
	op_ld_nnnn_a,
	op_undefined,
	op_undefined,
	op_undefined,
	op_xor_nn,
	op_rst_28,
	op_ld_a_ffnn,	// 0xF0
	op_pop_af,
	op_ld_a_ffc,
	op_di,
	op_undefined,
	op_push_af,
	op_or_nn,
	op_rst_30,
	op_hl_ld_sp_nn,	// 0xF8
	op_ld_sp_hl,
	op_ld_a_nnnn,
	op_ei,
	op_undefined,
	op_undefined,
	op_cp_nn,
	op_rst_38
};


