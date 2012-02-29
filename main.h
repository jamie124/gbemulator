// MAIN.h
#ifndef _main_h
#define _main_h

#include "ut/uthash.h"
#include "z80.h"
#include "memory.h"

extern z80_t* state;

extern gb_mem8_t	*bios;
extern gb_mem8_t	*rom;
extern gb_mem8_t	*wram;
extern gb_mem8_t	*eram;
extern gb_mem8_t	*zram;

typedef void (*OpCodeType)(z80_t*);

void run_loop();

z80_t *getState();

long load_file(char *filename);
void process_file(unsigned long fileLength);
void load_bios();

void clear_memory_8(gb_mem8_t*);
void clear_memory_16(gb_mem16_t*);

#endif
