/*
 *
 */

#include "cpu.h"

struct registers registers;

// Assign the desired initial value of each register
void init_registers()
{
	registers.AF = 0x01B0;
	registers.BC = 0x0013;
	registers.DE = 0x00D8;
	registers.HL = 0x014D;
	registers.SP = 0xFFFE;
	registers.PC = 0x100;
}