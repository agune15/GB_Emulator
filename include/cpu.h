/*
 *
 */

#pragma once

#define FLAG_Z 7
#define FLAG_N 6
#define FLAG_H 5
#define FLAG_C 4

void init_registers();

struct registers {
	union {
		unsigned short AF;
		struct {
			unsigned char F;
			unsigned char A;
		};
	};
	union {
		unsigned short BC;
		struct {
			unsigned char C;
			unsigned char B;
		};
	};
	union {
		unsigned short DE;
		struct {
			unsigned char E;
			unsigned char D;
		};
	};
	union {
		unsigned short HL;
		struct {
			unsigned char L;
			unsigned char H;
		};
	};
	unsigned short SP;
	unsigned short PC;
};