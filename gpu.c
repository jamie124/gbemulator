// GPU.c
#include "gpu.h"
#include "main.h"

int mode = 0;
int modeClock = 0;
int line = 0;

void step()
{
	modeClock += state->Reg.t;

	switch(mode) {
		// 0AM read mode, scanline active
		case 2:
			if (modeClock >= 80){
				// Enter scanline mode 3
				modeClock = 0;
				mode = 3;
			}
			break;
		// VRAM read mode, scanline active
		// Tread end of mode 3 as end of scanline
		case 3:
			if (modeClock >= 172) {
				modeClock = 0;
				mode = 0;

				// Write scanline to framebuffer
				//render_scan();		
			}
			break;

		// Hblank
		// After the last hblank, push screen data
		case 0:
			if (modeClock >= 204) {
				modeClock = 0;
				line++;

				if (line == 143) {
					// Enter vblank
					mode = 1;
					
					// Write image data
				} else {
					mode = 2;
				}
			}
			break;

		// Vblank (10 lines)
		case 1:
			if (modeClock >= 456) {
				modeClock = 0;
				line++;
	
				if (line > 153) {
					// Restart scanning mode
					mode = 2;
					line = 0;
				}
			}
			break;
		
	}
}
