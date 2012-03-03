// z80_t.h
#ifndef _z80_h
#define _z80_h

#include "dbg.h"
#include <inttypes.h>

typedef struct {
	int halt;

	struct Clock {
		int m;
		int t;
	} Clock;

	// Register
	struct Reg {
		// 8-bit register
		uint8_t a, b, c, d, e, h, l, f;
		// 16-bit register
		uint16_t pc, sp;
		// Clock
		int m, t;
	} Reg;
} z80_t;

extern void *opcodes[];

void init_state(z80_t* state);

void reset(z80_t* state);			// Reset registers
	
// Testing
void print_state(z80_t* state);

#endif
