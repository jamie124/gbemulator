#ifndef _memory_h
#define _memory_h

#include "ut/uthash.h"
#include <inttypes.h>

// 8-bit Memory location
struct GB_Memory_8 {
	int addr;
	uint8_t value;
	UT_hash_handle hh;	// Make hashable
};

// 16-bit Memory location
struct GB_Memory_16 {
	int addr;
	uint16_t value;
	UT_hash_handle hh;	// make hashable
};

void add_memory_bios(int addr, uint8_t value);
void add_memory_rom(int addr, uint8_t value);
void add_memory_wram(int addr, uint8_t value);
void add_memory_eram(int addr, uint8_t value);
void add_memory_zram(int addr, uint8_t value);

// Read a value from an 8-bit area of memory
uint8_t read_memory_8(struct GB_Memory_8* hash, int addr);
// Read a value from an 16-bit area of memory
uint16_t read_memory_16(struct GB_Memory_16* hash, int addr);


#endif
