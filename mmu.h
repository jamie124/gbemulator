// MMU.h
#ifndef _mmu_h
#define _mmu_h

#include <inttypes.h>
#include "memory.h"
#include "dbg.h"
#include "z80.h"

extern int		IN_BIOS;
extern uint16_t		ROM_OFFS;
extern uint16_t		RAM_OFFS;

extern int		CART_TYPE;
extern mbc_t		*mbc;



uint8_t read_byte(int addr);			// Read 8-bit byte
uint16_t read_word(int addr);			// Read 16-bit word

void write_byte(int addr, uint8_t val);		// Write 8-bit byte
void write_word(int addr, uint16_t val);	// Write 16-bit word


#endif
