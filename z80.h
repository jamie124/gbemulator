// Z80.h
#ifndef _z80_h
#define _z80_h

#include "dbg.h"

struct Z80 {
	struct Clock {
		int m;
		int t;
	} Clock;

	// Register
	struct Reg {
		// 8-bit register
		int a, b, c, d, e, h, l, f;
		// 16-bi register
		int pc, sp;
		// Clock
		int m, t;
	} Reg;
} Z80;

void init_state(struct Z80*);

// Add
void add_rb(struct Z80*);
void add_rc(struct Z80*);
void add_rd(struct Z80*);
void add_re(struct Z80*);
void add_rh(struct Z80*);
void add_rl(struct Z80*);
void add_ra(struct Z80*);
void add_hl(struct Z80*);
void add_n(struct Z80*);
void add_hlbc(struct Z80*);
void add_hlde(struct Z80*);
void add_hlhl(struct Z80*);
void add_hlsp(struct Z80*);
void add_spn(struct Z80*);

// Compare B to A (CP A, B)
void cp_rb(struct Z80*);

void nop(struct Z80*);

void push_bc(struct Z80* state);		// Pop registers B and C off stack (PUSH BC)
void pop_hl(struct Z80* state);			// Pop registers H and L off stack (POP HL)
void lda_mm(struct Z80* state);			// Read a byte from absolute location into A (LD A, addr)

void reset(struct Z80* state);			// Reset registers
	
// Testing
void print_state(struct Z80*);

#endif
