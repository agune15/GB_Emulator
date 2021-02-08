#include "catch.hpp"

extern "C" {
	#include "../src/display.c"
	#include "memory.h"
}

TEST_CASE("Update LCD disabled", "[display]") {
	update_globals();
	update_LCD_disabled();
	int current_mode = (read_byte(STAT_ADDRESS) & 0x03);
	CHECK(cycles_counter == SCANLINE_CYCLES);
	CHECK(IO[LY_R_ADDRESS] == 0);
	CHECK(current_mode == 1);
}

TEST_CASE("Update LCD mode - 0", "[display]") {
	update_globals();
	*pcurrent_line = GENERATE(take(10, random(0, VISIBLE_SCANLINES - 1)));
	cycles_counter = GENERATE(take(10, random(0, SCANLINE_CYCLES -
						     LCD_MODE_2_CYCLES -
						     LCD_MODE_3_CYCLES - 1)));
	update_LCD_mode();
	int mode = status & 0x03;
	CHECK(mode == 0);
}

TEST_CASE("Update LCD mode - 1", "[display]") {
	update_globals();
	*pcurrent_line = GENERATE(take(10, random(VISIBLE_SCANLINES, TOTAL_SCANLINES)));
	update_LCD_mode();
	int mode = status & 0x03;
	CHECK(mode == 1);
}

TEST_CASE("Update LCD mode - 2", "[display]") {
	update_globals();
	*pcurrent_line = GENERATE(take(10, random(0, VISIBLE_SCANLINES - 1)));
	cycles_counter = GENERATE(take(10, random(SCANLINE_CYCLES - LCD_MODE_2_CYCLES,
					   	  SCANLINE_CYCLES)));
	update_LCD_mode();
	int mode = status & 0x03;
	CHECK(mode == 2);
}

TEST_CASE("Update LCD mode - 3", "[display]") {
	update_globals();
	*pcurrent_line = GENERATE(take(10, random(0, VISIBLE_SCANLINES - 1)));
	cycles_counter = GENERATE(take(10, random(SCANLINE_CYCLES -
						  LCD_MODE_2_CYCLES -
						  LCD_MODE_3_CYCLES,
						  SCANLINE_CYCLES -
						  LCD_MODE_2_CYCLES - 1)));
	update_LCD_mode();
	int mode = status & 0x03;
	CHECK(mode == 3);
}

TEST_CASE("Update coincidence flag - Set", "[display]") {
	update_globals();
	*pcurrent_line = GENERATE(take(10, random(0, TOTAL_SCANLINES)));
	write_byte(LYC_ADDRESS, *pcurrent_line);
	update_coincidence_flag();
	bool flag_state = (status >> 2) & 1;
	CHECK(flag_state == true);
}

TEST_CASE("Update coincidence flag - Clear", "[display]") {
	update_globals();
	*pcurrent_line = GENERATE(take(10, random(0, TOTAL_SCANLINES)));
	write_byte(LYC_ADDRESS, *pcurrent_line+1);
	update_coincidence_flag();
	bool flag_state = (status >> 2) & 1;
	CHECK(flag_state == false);
}

TEST_CASE("Update scanline", "[display]") {
	update_globals();
	*pcurrent_line = GENERATE(take(10, random(0, TOTAL_SCANLINES - 1)));
	int last_line = *pcurrent_line;
	update_scanline(SCANLINE_CYCLES);
	CHECK(cycles_counter == SCANLINE_CYCLES);
	CHECK(*pcurrent_line == last_line+1);
}

TEST_CASE("Update scanline - overflow", "[display]") {
	update_globals();
	*pcurrent_line = TOTAL_SCANLINES;
	update_scanline(SCANLINE_CYCLES);
	CHECK(cycles_counter == SCANLINE_CYCLES);
	CHECK(*pcurrent_line == 0);
}