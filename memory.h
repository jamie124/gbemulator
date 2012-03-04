#ifndef _memory_h
#define _memory_h

#include "ut/uthash.h"
#include <inttypes.h>

typedef struct {
	int romBank;
	int ramBank;
	int ramOn;
	int mode;
} mbc_t;

// 8-bit Memory location
typedef struct {
	int addr;
	uint8_t value;
	UT_hash_handle hh;	// Make hashable
} gb_mem8_t;

// 16-bit Memory location
typedef struct {
	int addr;
	uint16_t value;
	UT_hash_handle hh;	// make hashable
} gb_mem16_t;

void add_memory_bios(int addr, uint8_t value);
void add_memory_rom(int addr, uint8_t value);
void add_memory_wram(int addr, uint8_t value);
void add_memory_eram(int addr, uint8_t value);
void add_memory_vram(int addr, uint8_t value);
void add_memory_zram(int addr, uint8_t value);

// Read a value from an 8-bit area of memory
uint8_t read_memory_8(gb_mem8_t* hash, int addr);
// Read a value from an 16-bit area of memory
uint16_t read_memory_16(gb_mem16_t* hash, int addr);


#endif
