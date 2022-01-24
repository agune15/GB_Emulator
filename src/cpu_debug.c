#include <stdio.h>
#include <stdbool.h>
#include "cpu_debug.h"
#include "registers.h"
#include "memory.h"

bool is_debugging_CPU = false;
bool debug_next_instruction = false;
int current_opcode;
int operand1, operand2;
int instruction_count = 0;
registers_t regs_before_op;

static void store_current_opcode(void);
static void store_operands(unsigned short pc_reg_value);
static void store_regs_before_opcode(void);

// Print last ran opcode and registers state before-after opcode
void print_cpu_info(void)
{
    return;

    /*if (instruction_count < 638600 || instruction_count > 638604)
        return;*/

    /*if (registers.PC > 0x100)
        return;*/

    printf("Opcode: %X   Instruction count: %d\n"
           "Operand 1: %02X   Operand 2: %02X\n"
           "Registers BEFORE:              Registers AFTER:\n"
           "   AF: %02X-%02X                      AF: %02X-%02X\n"
           "   BC: %02X-%02X                      BC: %02X-%02X\n"
           "   DE: %02X-%02X                      DE: %02X-%02X\n"
           "   HL: %02X-%02X                      HL: %02X-%02X\n"
           "   SP: %04X                         SP: %04X\n"
           "   PC: %04X                         PC: %04X\n\n", current_opcode, instruction_count, operand1, operand2,
           regs_before_op.A, regs_before_op.F, registers.A, registers.F, regs_before_op.B, regs_before_op.C, registers.B,
           registers.C, regs_before_op.D, regs_before_op.E, registers.D, registers.E, regs_before_op.H, regs_before_op.L,
           registers.H, registers.L, regs_before_op.SP, registers.SP, regs_before_op.PC, registers.PC);
}

// Store current CPU state
void store_cpu_state_before_opcode(void)
{
    store_current_opcode();
    store_regs_before_opcode();
    instruction_count++;
}

// Store current instruction opcode
static void store_current_opcode(void)
{
    int pc_reg_value = registers.PC;

    current_opcode = read_byte(pc_reg_value++);
    if (current_opcode == 0xCB) {
        current_opcode <<= 8;
        current_opcode += read_byte(pc_reg_value++);
    }
    store_operands(pc_reg_value);
}

// Store current registers state
static void store_regs_before_opcode(void) { regs_before_op = registers;}

static void store_operands(unsigned short pc_reg_value)
{
    operand1 = read_byte(pc_reg_value);
    operand2 = read_byte(pc_reg_value+1);
}
