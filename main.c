// MAIN.c
#include "main.h"
#include "dbg.h"
#include "mmu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ut/utarray.h"

gb_mem8_t	*bios 	= NULL;
gb_mem8_t	*rom	= NULL;
gb_mem8_t	*wram	= NULL;
gb_mem8_t	*eram	= NULL;
gb_mem8_t	*zram	= NULL;

int IN_BIOS = 1;

z80_t 		*state;

uint8_t		*fileBuffer;


int main(int argc, char *argv[])
{	
	// BIOS is unmapped with first instruction above 0x00FF

	printf("Game Boy Emulator\n");

	state = malloc(sizeof(z80_t));

	reset(state);		// Make sure it's clean to start with

	load_bios();

	unsigned long fileLength = load_file("test.gb");

	if (fileLength > 0) {
		process_file(fileLength);
	}

	printf("Number of records in ROM %u\n", HASH_COUNT(rom));

	// Start executing instructions
	run_loop();

	//add_rc(state);
	//print_state(state);
	
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
		clear_memory_8(rom);	
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

// Passes the state pointer to the opcode pointer.
void process_opcode(OpCodeType opcode)
{
	opcode(state);
}

// Main run loop
void run_loop()
{
	int fclk = state->Clock.t + 256;

	printf("\nOpCodes:\n");
	do {
		uint8_t temp = read_byte(state->Reg.pc++);

//		printf(" %u\n", temp);
	
//		if (state->Reg.pc - 1 == 36){
//			printf("33\n");
//		}
		
		if (opcodes[temp] != NULL) {
			printf("OPCODE %u\n", temp);
		//	&opcodes[temp](state);
			process_opcode(opcodes[temp]);
		} else {
			printf("%u\n", temp);
		}
		

		state->Reg.pc &= 65535;
		state->Clock.m += state->Reg.m;
		state->Clock.t += 1;//state->Reg.t;

		//step();
	} while (state->Clock.t < fclk);

	printf("\n");
}

z80_t *getState()
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
	fileBuffer = (uint8_t*)malloc(fileLength);
	
	
	if (!fileBuffer) {
		fprintf(stderr, "Memory error.");
		fclose(file);
		return -1;
	}

	// Read contents into buffer
	fread(fileBuffer, 1, fileLength, file);
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
	for(i = 0;i < 100;i++){
	//	printf("%u ", fileBuffer[i]);
		add_memory_rom(i, fileBuffer[i]);
	}
	
	printf("\nLoaded into ROM\n");

}

// Load the BIOS data into memory
void load_bios()
{
	FILE *file;
	unsigned long fileLength;
	
	// Open file
	file = fopen("bios.dat", "rb");
	if (!file) {
		fprintf(stderr, "Unable to open bios.dat");
		return -1;
	}

	// Get file length
	fseek(file, 0, SEEK_END);
	fileLength = ftell(file);
	fseek(file, 0, SEEK_SET);
		
	// Allocate memory
	uint8_t *biosBuffer = (uint8_t*)malloc(fileLength);
	
	
	if (!biosBuffer) {
		fprintf(stderr, "Memory error.");
		fclose(file);
		return -1;
	}

	// Read contents into buffer
	fread(biosBuffer, 1, fileLength, file);
	fclose(file);
	
	printf("File is %lu bytes long.\n", fileLength);
	
	// Store in BIOS area of memory
	long i;
	for(i = 0;i < fileLength;i++){
		//printf("%d: %u, ", i, biosBuffer[i]);
		add_memory_bios(i, biosBuffer[i]);
	}

	free(biosBuffer);
	
	printf("\nBios was loaded.\n");
}

// Clear all items from a 8-Bit memory block
void clear_memory_8(gb_mem8_t* memory)
{
	gb_mem8_t* currMemory, *temp;

	HASH_ITER(hh, memory, currMemory, temp) {
		HASH_DEL(memory, currMemory);
		free(currMemory);
	}
}

// Clear all items from a 16-Bit memory block
void clear_memory_16(gb_mem16_t* memory)
{
	gb_mem16_t* currMemory, *temp;

	HASH_ITER(hh, memory, currMemory, temp) {
		HASH_DEL(memory, currMemory);
		free(currMemory);
	}
}
