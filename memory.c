// MEMORY.c
#include "dbg.h"
#include "main.h"
#include "memory.h"
#include <stdio.h>

// Add a value to an 8-bit area of memory
void add_memory_bios(int addr, uint8_t value)
{
	gb_mem8_t *s;

	s = malloc(sizeof(gb_mem8_t));
	s->addr = addr;
	s->value = value;
	HASH_ADD_INT(bios, addr, s);
}

void add_memory_rom(int addr, uint8_t value)
{
	gb_mem8_t *s;

	s = malloc(sizeof(gb_mem8_t));
	s->addr = addr;
	s->value = value;
	HASH_ADD_INT(rom, addr, s);
}

void add_memory_wram(int addr, uint8_t value)
{
	gb_mem8_t *s;

	s = malloc(sizeof(gb_mem8_t));
	s->addr = addr;
	s->value = value;
	HASH_ADD_INT(wram, addr, s);
}

void add_memory_eram(int addr, uint8_t value)
{
	gb_mem8_t *s;

	s = malloc(sizeof(gb_mem8_t));
	s->addr = addr;
	s->value = value;
	HASH_ADD_INT(eram, addr, s);
}

void add_memory_zram(int addr, uint8_t value)
{
	gb_mem8_t *s;

	s = malloc(sizeof(gb_mem8_t));
	s->addr = addr;
	s->value = value;
	HASH_ADD_INT(zram, addr, s);
}

// Read a value from an 8-bit area of memory
uint8_t read_memory_8(gb_mem8_t* hash, int addr)
{
	gb_mem8_t *s;

	HASH_FIND_INT(hash, &addr, s);

	//check_mem(s);
	return s->value;
//error:
//	return 0;
}

// Read a value from an 16-bit area of memory
uint16_t read_memory_16(gb_mem16_t* hash, int addr)
{
	gb_mem16_t *s;
		
	HASH_FIND_INT(hash, &addr, s);
	return s->value;
}
