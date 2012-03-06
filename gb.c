#include "main.h"
#include "gb.h"

void setup_boot(z80_t* state)
{
	state->Reg.a = 1;
	state->Reg.f = 176;
	state->Reg.d = 0;
	state->Reg.e = 216;
	state->Reg.h = 1;
	state->Reg.l = 77;
	state->Reg.pc = 256;
	state->Reg.sp = 65534;

   add_memory_wram(0xFF05, 0x00);
   add_memory_wram(0xFF06, 0x00);
   add_memory_wram(0xFF07, 0x00);
   add_memory_wram(0xFF10, 0x80);
   add_memory_wram(0xFF11, 0xBF);
   add_memory_wram(0xFF12, 0xF3);
   add_memory_wram(0xFF14, 0xBF);
   add_memory_wram(0xFF16, 0x3F);
   add_memory_wram(0xFF17, 0x00);
   add_memory_wram(0xFF19, 0xBF);
   add_memory_wram(0xFF1A, 0x7F);
   add_memory_wram(0xFF1B, 0xFF);
   add_memory_wram(0xFF1C, 0x9F);
   add_memory_wram(0xFF1E, 0xBF);
   add_memory_wram(0xFF20, 0xFF);
   add_memory_wram(0xFF21, 0x00);
   add_memory_wram(0xFF22, 0x00);
   add_memory_wram(0xFF23, 0xBF);
   add_memory_wram(0xFF24, 0x77);
   add_memory_wram(0xFF25, 0xF3);
   add_memory_wram(0xFF26, 0xF1);
   add_memory_wram(0xFF40, 0x91);
   add_memory_wram(0xFF42, 0x00);
   add_memory_wram(0xFF43, 0x00);
   add_memory_wram(0xFF45, 0x00);
   add_memory_wram(0xFF47, 0xFC);
   add_memory_wram(0xFF48, 0xFF);
   add_memory_wram(0xFF49, 0xFF);
   add_memory_wram(0xFF4A, 0x00);
   add_memory_wram(0xFF4B, 0x00);
   add_memory_wram(0xFFFF, 0x00);
}
