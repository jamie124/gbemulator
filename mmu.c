// MMU.c
#include "mmu.h"
#include "main.h"

uint8_t read_byte(int addr)
{
	printf("Reading byte from %d\n", addr);

	switch(addr & 0xF000)
	{
		// BIOS (256b)/ROM0
		case 0x0000:
			if (IN_BIOS) {
				if (addr < 0x0100)
					return read_memory_8(bios, addr);
				else if (state->Reg.pc == 0x0100) {
					IN_BIOS = 0;
					printf("Leaving BIOS.\n");
					//return 0;
				}
			}

			//printf("Reading from rom\n");
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
			return read_memory_8(rom, ROM_OFFS + (addr & 0x3FFF));

		// Graphics: VRAM (8K)
		case 0x8000:
		case 0x9000:
			return read_memory_8(vram, addr & 0x1FFF);
		//	return -1;
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
	printf("Reading word from %d\n", addr);


	return read_byte(addr) + (read_byte(addr+1) << 8);
}

void write_byte(int addr, uint8_t val)
{
	printf("Writing %u to addr %d\n", val, addr);

	switch(addr & 0xF000) {
		
		// ROM bank 0
		case 0x0000:
		case 0x1000:
			switch(CART_TYPE) {
				case 1:
					mbc->ramOn = ((val & 0xF) == 0xA) ? 1 : 0;
					break;
					
			}
			break;

		// MBC1: ROM bank switch
		case 0x2000:
		case 0x3000:
			switch(CART_TYPE) {
				case 1:
					mbc->romBank &= 0x60;
					val &= 0x1F;
					
					if (!val)
						val = 1;

					mbc->romBank |= val;
					ROM_OFFS = mbc->romBank * 0x4000;
					break;
					
			}
			break;

		// ROM bank 1
		// MBC1: RAM bank switch
		case 0x4000:
		case 0x5000:
			switch(CART_TYPE) {
				case 1:
					if (mbc->mode) {
						mbc->ramBank = val & 3;
						RAM_OFFS = mbc->ramBank * 0x2000;
					} else {
						mbc->romBank &= 0x1F;
						mbc->romBank |= ((val & 3) << 5);
						ROM_OFFS = mbc->romBank * 0x4000;
					}
			}
			break;
	
		case 0x6000:
		case 0x7000:
			switch(CART_TYPE) {
				case 1:
					mbc->mode = val & 1;
					break;
			}
			break;

		// VRAM
		case 0x8000:
		case 0x9000:
			add_memory_vram(addr & 0x1FFF, val);
			// Update tile here
			break;
		
		// External RAM
		case 0xA000:
		case 0xB000:
			add_memory_eram(RAM_OFFS + (addr &0x1FFF), val);
			break;

		// Work RAM and echo
		case 0xC000:
		case 0xD000:
		case 0xE000:
			add_memory_wram(addr & 0x1FFF, val);

		// Everything else
		case 0xF000:
			switch(addr & 0x0F00) {
				// Echo RAM
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
					add_memory_wram(addr & 0x1FFF, val);
					break;

				// 0AM
				case 0xE00:
					if ((addr & 0xFF) < 0xA0)
						// oam
					// update oam
					break;
	
				// Zeropage
				case 0xF00:
					if (addr == 0xFFFF) {
						//IE = val;
					} else if (addr > 0xFF7F) {
						add_memory_zram(addr & 0x7F, val);
					} else {
						switch(addr & 0xF0) {
							case 0x00:
								switch(addr & 0xF) {
									case 0:
										// wb(val)
										break;	
									case 4:
									case 5:
									case 6:
									case 7:
										// timer.wb
										break;
									case 15:
										// IF = val;
										break;
								}
							case 0x10:
							case 0x20:
							case 0x30:
								break;

							case 0x40:
							case 0x50:
							case 0x60:
							case 0x70:
								// gpu.wb
								break;
						}
						
					}
					break;
			} 
	}
}

void write_word(int addr, uint16_t val)
{
	printf("Writing word to %d\n", addr);

	write_byte(addr, val & 255);
	write_byte(addr + 1, val >> 8);
}

