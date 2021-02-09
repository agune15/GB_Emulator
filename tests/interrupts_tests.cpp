#include "catch.hpp"

extern "C" {
	#include "../src/interrupts.c"
	#include "memory.h"
	#include "cpu.h"
}

TEST_CASE("Request interrupt", "[interrupts]") {
	int interrupt_flag = GENERATE(0, 1, 2, 3, 4);
	request_interrupt((interrupt_t)interrupt_flag);
	int bit_state = (read_byte(IF_ADDRESS) >> interrupt_flag) & 1;
	CHECK(bit_state == 1);
}

TEST_CASE("Perform interrupt", "[interrupts]") {
	int interrupt_flag = GENERATE(0, 1, 2, 3, 4);
	interrupt_master_enable = true;
	request_interrupt((interrupt_t)interrupt_flag);
	perform_interrupt((interrupt_t)interrupt_flag);
	int bit_state = (read_byte(IF_ADDRESS) >> interrupt_flag) & 1;
	CHECK(bit_state == 0);
	CHECK(registers.PC == interrupt_addresses[interrupt_flag]);
	CHECK(interrupt_master_enable == false);
	CHECK(((read_byte(IF_ADDRESS) >> interrupt_flag) & 1) == 0);
}

TEST_CASE("Do interrupts", "[interrupts]") {
	int IE_reg = GENERATE(take(10, random(0x00, 0x1F)));
	write_byte(IE_ADDRESS, IE_reg);
	int IF_reg = GENERATE(take(10, random(0x00, 0x1F)));
	write_byte(IF_ADDRESS, IF_reg);
	interrupt_master_enable = true;
	check_interrupts_state();
	int interrupt_flag = GENERATE(0, 1, 2, 3, 4);
	int bit_state = (read_byte(IF_ADDRESS) >> interrupt_flag) & 1;
	if((IE_reg >> interrupt_flag) & 1)
		CHECK(bit_state == 0);
}