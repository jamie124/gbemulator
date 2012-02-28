// MAIN.h
#ifndef _main_h
#define _main_h

#include "ut/uthash.h"
#include "z80.h"
#include "memory.h"

extern struct Z80* state;

extern struct GB_Memory_8	*bios;
extern struct GB_Memory_8	*rom;
extern struct GB_Memory_8	*wram;
extern struct GB_Memory_8	*eram;
extern struct GB_Memory_8	*zram;

void run_loop();

struct Z80 *getState();

long load_file(char *filename);
void process_file(unsigned long fileLength);

void clear_memory_8(struct GB_Memory_8*);
void clear_memory_16(struct GB_Memory_16*);

#endif
