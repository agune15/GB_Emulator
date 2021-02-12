#include "catch.hpp"

extern "C" {
	#include "../src/input.c"
}

#define JOYPAD_ADDRESS  0xFF00

//region input.h

TEST_CASE("Button down", "[input]") {
	buttons_state = 0xFF;
	for(int button = 0; button < START; button++) {
		joypad_button_down((button_t)button);
		CHECK(is_button_pressed((button_t)button) == true);
	}
}

TEST_CASE("Button up", "[input]") {
	buttons_state = 0x00;
	for(int button = 0; button < START; button++) {
		joypad_button_up((button_t)button);
		CHECK(is_button_pressed((button_t)button) == false);
	}
}

TEST_CASE("Get joypad state", "[input]") {
	buttons_state = GENERATE(take(10, random(0x00, 0xFF)));
	unsigned char joypad_state = ~GENERATE(take(4, random(0x20, 0x2F)));
	write_byte(JOYPAD_ADDRESS, joypad_state);
	unsigned char state = (joypad_state & 0xF0) | (buttons_state >> 4);
	SECTION("Standard selected") {
		CHECK(read_byte(JOYPAD_ADDRESS) == state);
	}

	joypad_state = ~GENERATE(take(4, random(0x10, 0x1F)));
	write_byte(JOYPAD_ADDRESS, joypad_state);
	state = (joypad_state & 0xF0) | (buttons_state & 0x0F);
	SECTION("Direction selected") {
		CHECK(read_byte(JOYPAD_ADDRESS) == state);
	}
}

//endregion

//region input.c

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

//endregion



