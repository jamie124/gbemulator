// MMU.c
#include "mmu.h"
#include "main.h"

uint8_t read_byte(int addr)
{
	printf("%d: ", addr);

	switch(addr & 0xF000)
	{
		// BIOS (256b)/ROM0
		case 0x0000:
			if (IN_BIOS) {
				if (addr < 0x0100)
					return read_memory_8(bios, addr);
				else if (state->Reg.pc == 0x0100) {
					IN_BIOS = 0;
					//return 0;
				}
			}

			printf("Reading from rom\n");
			return read_memory_8(rom, addr);

		// ROM0
		case 0x1000:
		case 0x2000:
		case 0x3000:
			return read_memory_8(rom, addr);
		
		// ROM1 (unbanked) (16K)
		case 0x4000:
		case 0x5000:
		case 0x6000:
		case 0x7000:
			return read_memory_8(rom, addr);

		// Graphics: VRAM (8K)
		case 0x8000:
		case 0x9000:
		//	return read_memory_8(vram, addr & 0x1FFF);
			return -1;
		// External RAM (8K)
		case 0xA000:
		case 0xB000:
			return read_memory_8(eram, addr & 0x1FFF);

		// Working RAM (8K)
		case 0xC000:
		case 0xD000:
			return read_memory_8(wram, addr & 0x1FFF);

		// Working RAM shadow
		case 0xE000:
			return read_memory_8(wram, addr & 0x1FFF);

		// Working RAM shadow, I/O, Zero-page 
		case 0xF000:
			switch(addr &0x0F00){
				// Working RAM shadow
				case 0x000:
				case 0x100:
				case 0x200:
				case 0x300:
				case 0x400:
				case 0x500:
				case 0x600:
				case 0x700:
				case 0x800:
				case 0x900:
				case 0xA00:
				case 0xB00:
				case 0xC00:
				case 0xD00:
					return read_memory_8(wram, addr & 0x1FFF);

				// Graphics
				// 0AM is 160 bytes, remaining are 0
				case 0xE00:
					if (addr < 0xFEA0){
					//	return read_memory_8(oam, addr & 0xFF);
						return -1;
					} else { 
						return 0;
					}
				// Zero-page	
				case 0xF00:
					if (addr >= 0xFF80) {
						return read_memory_8(zram, addr & 0x7F);
					} else {
						return 0;
					}
			}  
	}

	return 0;
}

uint16_t read_word(int addr)
{
	return read_byte(addr) + (read_byte(addr+1) << 8);
}

void write_byte(int addr, uint8_t val)
{
	printf("Writing %u to addr %d\n", val, addr);
}

void write_word(int addr, uint16_t val)
{

}

