#include "catch.hpp"

extern "C" {
	#include "../src/input.c"
}

//TODO: input.h testcases

TEST_CASE("Button already pressed", "[input]") {
	for(int button = 0, bit_mask = 0x01; button <= START; button++, bit_mask *= 2) {
		buttons_state = ~bit_mask;
		CHECK(is_button_pressed((button_t)button) == true);
		buttons_state = bit_mask;
		CHECK(is_button_pressed((button_t)button) == false);
	}
}

TEST_CASE("Set button flag", "[input]") {
	for(int button = 0; button <= START; button++) {
		set_button_flag((button_t)button);
		CHECK(is_button_pressed((button_t)button) == false);
	}
}

TEST_CASE("Clear button flag", "[input]") {
	for(int button = 0; button <= START; button++) {
		clear_button_flag((button_t)button);
		CHECK(is_button_pressed((button_t)button) == true);
	}
}

TEST_CASE("Standard buttons selected", "[input]") {
	write_byte(0xFF00, ~0x20);
	CHECK(is_standard_selected() == true);
	write_byte(0xFF00, 0x20);
	CHECK(is_standard_selected() == false);
}

TEST_CASE("Direction buttons selected", "[input]") {
	write_byte(0xFF00, ~0x10);
	CHECK(is_direction_selected() == true);
	write_byte(0xFF00, 0x10);
	CHECK(is_direction_selected() == false);
}




