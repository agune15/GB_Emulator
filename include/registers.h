#pragma once

extern struct registers {
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
} registers;
