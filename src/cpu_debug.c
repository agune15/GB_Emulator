#include <stdio.h>
#include <stdbool.h>
#include "cpu_debug.h"
#include "registers.h"
#include "memory.h"

bool is_debugging_CPU = true;
bool debug_next_instruction = false;
int current_opcode;
registers_t regs_before_op;

static void store_current_opcode(void);
static void store_regs_before_opcode(void);

// Print last ran opcode and registers state before-after opcode
void print_cpu_info(void)
{
    printf("Opcode: %X\n"
           "Registers BEFORE:              Registers AFTER:\n"
           "   AF: %X-%X                      AF: %X-%X\n"
           "   BC: %X-%X                      BC: %X-%X\n"
           "   DE: %X-%X                      DE: %X-%X\n"
           "   HL: %X-%X                      HL: %X-%X\n"
           "   SP: %X                         SP: %X\n"
           "   PC: %X                         PC: %X\n\n", current_opcode, regs_before_op.A,
           regs_before_op.F, registers.A, registers.F, regs_before_op.B, regs_before_op.C, registers.B,
           registers.C, regs_before_op.D, regs_before_op.E, registers.D, registers.E, regs_before_op.H,
           regs_before_op.L, registers.H, registers.L, regs_before_op.SP, registers.SP, regs_before_op.PC,
           registers.PC);
}

// Store current CPU state
void store_cpu_state_before_opcode(void)
{
    store_current_opcode();
    store_regs_before_opcode();
}

// Store current instruction opcode
static void store_current_opcode(void)
{
    current_opcode = read_byte(registers.PC);
    if (current_opcode == 0xCB) {
        current_opcode <<= 8;
        current_opcode += read_byte(registers.PC + 1);
    }
}

// Store current registers state
static void store_regs_before_opcode(void) { regs_before_op = registers;}
