// z80_t.h
#ifndef _z80_h
#define _z80_h

#include "dbg.h"

typedef struct {
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
} z80_t;

void init_state(z80_t* state);

// Add
void add_rb(z80_t* state);
void add_rc(z80_t* state);
void add_rd(z80_t* state);
void add_re(z80_t* state);
void add_rh(z80_t* state);
void add_rl(z80_t* state);
void add_ra(z80_t* state);
void add_hl(z80_t* state);
void add_n(z80_t* state);
void add_hlbc(z80_t* state);
void add_hlde(z80_t* state);
void add_hlhl(z80_t* state);
void add_hlsp(z80_t* state);
void add_spn(z80_t* state);

// Compare B to A (CP A, B)
void cp_rb(z80_t* state);

void nop(z80_t* state);

void push_bc(z80_t* state);		// Pop registers B and C off stack (PUSH BC)
void pop_hl(z80_t* state);			// Pop registers H and L off stack (POP HL)
void lda_mm(z80_t* state);			// Read a byte from absolute location into A (LD A, addr)

void reset(z80_t* state);			// Reset registers
	
// Testing
void print_state(z80_t* state);

#endif
