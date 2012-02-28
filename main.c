// MAIN.c
#include "main.h"
#include "dbg.h"
#include "mmu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ut/utarray.h"

struct GB_Memory_8	*bios 	= NULL;
struct GB_Memory_16	*rom	= NULL;
struct GB_Memory_8	*wram	= NULL;
struct GB_Memory_8	*eram	= NULL;
struct GB_Memory_8	*zram	= NULL;

int IN_BIOS = 1;

struct Z80 		*state;

uint16_t		*fileBuffer;

int main(int argc, char *argv[])
{	
	// BIOS is unmapped with first instruction above 0x00FF

	printf("Game Boy Emulator\n");

	state = malloc(sizeof(struct Z80));

	reset(state);		// Make sure it's clean to start with

	unsigned long fileLength = load_file("test.gb");

	if (fileLength > 0) {
		process_file(fileLength/2);
	}

	printf("Number of records in ROM %u\n", HASH_COUNT(rom));
	
	free(state);

	if (fileBuffer) {
		printf("Freeing filebuffer\n");
		free(fileBuffer);
	}

	if (bios != NULL) {
 		printf("Freeing bios\n");	
		clear_memory_8(bios);
	}
	if (rom != NULL){
		printf("Freeing rom\n");
		clear_memory_16(rom);	
	}
	
	if (wram != NULL){
		printf("Freeing wram\n");
		clear_memory_8(wram);	
	}
	
	if (eram != NULL){
		printf("Freeing eram\n");
		clear_memory_8(eram);
	}
	
	if (zram != NULL){
		printf("Freeing zram\n");
		clear_memory_8(zram);
	}

	return 0;
}

struct Z80 *getState()
{
	return state;
}

long load_file(char *filename)
{
	FILE *file;
	unsigned long fileLength;
	
	// Open file
	file = fopen(filename, "rb");
	if (!file) {
		fprintf(stderr, "Unable to load file %s", filename);
		return -1;
	}

	// Get file length
	fseek(file, 0, SEEK_END);
	fileLength = ftell(file);
	fseek(file, 0, SEEK_SET);
		
	// Allocate memory
	fileBuffer = (uint16_t*)malloc(fileLength);
	
	
	if (!fileBuffer) {
		fprintf(stderr, "Memory error.");
		fclose(file);
		return -1;
	}

	// Read contents into buffer
	fread(fileBuffer, 2, fileLength, file);
	fclose(file);
	
	printf("File is %lu bytes long.\n", fileLength);
	

	printf("File %s was loaded.\n", filename);

	return fileLength;
}

void process_file(unsigned long fileLength)
{
	if (fileBuffer == NULL) {
		printf("File buffer is NULL\n");
		return;
	}

	long i;
	for(i = 0;i < (fileLength - 1);i++){
//		printf("%x ", fileBuffer[i]);
		add_memory_rom(i, fileBuffer[i]);
	}
	
	printf("Loaded into ROM\n");

}

// Clear all items from a 8-Bit memory block
void clear_memory_8(struct GB_Memory_8* memory)
{
	struct GB_Memory_8* currMemory, *temp;

	HASH_ITER(hh, memory, currMemory, temp) {
		HASH_DEL(memory, currMemory);
		free(currMemory);
	}
}

// Clear all items from a 16-Bit memory block
void clear_memory_16(struct GB_Memory_16* memory)
{
	struct GB_Memory_16* currMemory, *temp;

	HASH_ITER(hh, memory, currMemory, temp) {
		HASH_DEL(memory, currMemory);
		free(currMemory);
	}
}
