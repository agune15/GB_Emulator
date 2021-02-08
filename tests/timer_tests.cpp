#include "catch.hpp"

extern "C" {
	#include "../src/timer.c"
	#include "memory.h"
}

TEST_CASE("Clock state - enabled", "[timer]") {
	unsigned char value = GENERATE(take(10, random(0x00, 0xFF)));
	value |= (1 << 2);
	write_byte(TAC_ADDRESS, value);
	CHECK(is_clock_enabled());
}

TEST_CASE("Clock state - disabled", "[timer]") {
	unsigned char value = GENERATE(take(10, random(0x00, 0xFF)));
	value &= ~(1 << 2);
	write_byte(TAC_ADDRESS, value);
	CHECK(!is_clock_enabled());
}

TEST_CASE("Reset timer counter", "[timer]") {
	unsigned char clock_rate = GENERATE(0, 1, 2, 3);
	write_byte(TAC_ADDRESS, clock_rate);
	reset_timer_counter();

	switch(clock_rate) {
		case 0:
			CHECK(timer_counter == 1024);
			break;
		case 1:
			CHECK(timer_counter == 16);
			break;
		case 2:
			CHECK(timer_counter == 64);
			break;
		case 3:
			CHECK(timer_counter == 256);
			break;
		default:
			break;
	}
}

TEST_CASE("Update timer: 4096 Hz", "[timer]") {
	unsigned char modulo = GENERATE(take(10, random(0x00, 0xFE)));
	write_byte(TMA_ADDRESS, modulo);
	write_byte(TIMA_ADDRESS, read_byte(TMA_ADDRESS));
	write_byte(TAC_ADDRESS, 0x04);	// Clock enabled + 00 clock rate
	reset_timer_counter();
	update_timer(1024);
	CHECK(read_byte(TIMA_ADDRESS) == read_byte(TMA_ADDRESS) + 1);
}

TEST_CASE("Update timer: 4096 Hz, overflow", "[timer]") {
	unsigned char modulo = GENERATE(take(10, random(0x00, 0xFE)));
	write_byte(TMA_ADDRESS, modulo);
	write_byte(TIMA_ADDRESS, 0xFF);
	write_byte(TAC_ADDRESS, 0x04);	// Clock enabled + 00 clock rate
	reset_timer_counter();
	update_timer(1024);
	CHECK(read_byte(TIMA_ADDRESS) == read_byte(TMA_ADDRESS));
	//TODO: Check requested interrupt
}

TEST_CASE("Update timer: 262144 Hz", "[timer]") {
	unsigned char modulo = GENERATE(take(10, random(0x00, 0xFE)));
	write_byte(TMA_ADDRESS, modulo);
	write_byte(TIMA_ADDRESS, read_byte(TMA_ADDRESS));
	write_byte(TAC_ADDRESS, 0x05);	// Clock enabled + 01 clock rate
	reset_timer_counter();
	update_timer(16);
	CHECK(read_byte(TIMA_ADDRESS) == read_byte(TMA_ADDRESS) + 1);
}

TEST_CASE("Update timer: 262144 Hz, overflow", "[timer]") {
	unsigned char modulo = GENERATE(take(10, random(0x00, 0xFE)));
	write_byte(TMA_ADDRESS, modulo);
	write_byte(TIMA_ADDRESS, 0xFF);
	write_byte(TAC_ADDRESS, 0x05);	// Clock enabled + 01 clock rate
	reset_timer_counter();
	update_timer(16);
	CHECK(read_byte(TIMA_ADDRESS) == read_byte(TMA_ADDRESS));
	//TODO: Check requested interrupt
}

TEST_CASE("Update timer: 65536 Hz", "[timer]") {
	unsigned char modulo = GENERATE(take(10, random(0x00, 0xFE)));
	write_byte(TMA_ADDRESS, modulo);
	write_byte(TIMA_ADDRESS, read_byte(TMA_ADDRESS));
	write_byte(TAC_ADDRESS, 0x06);	// Clock enabled + 10 clock rate
	reset_timer_counter();
	update_timer(64);
	CHECK(read_byte(TIMA_ADDRESS) == read_byte(TMA_ADDRESS) + 1);
}

TEST_CASE("Update timer: 65536 Hz, overflow", "[timer]") {
	unsigned char modulo = GENERATE(take(10, random(0x00, 0xFE)));
	write_byte(TMA_ADDRESS, modulo);
	write_byte(TIMA_ADDRESS, 0xFF);
	write_byte(TAC_ADDRESS, 0x06);	// Clock enabled + 10 clock rate
	reset_timer_counter();
	update_timer(64);
	CHECK(read_byte(TIMA_ADDRESS) == read_byte(TMA_ADDRESS));
	//TODO: Check requested interrupt
}

TEST_CASE("Update timer: 16384 Hz", "[timer]") {
	unsigned char modulo = GENERATE(take(10, random(0x00, 0xFE)));
	write_byte(TMA_ADDRESS, modulo);
	write_byte(TIMA_ADDRESS, read_byte(TMA_ADDRESS));
	write_byte(TAC_ADDRESS, 0x07);	// Clock enabled + 11 clock rate
	reset_timer_counter();
	update_timer(256);
	CHECK(read_byte(TIMA_ADDRESS) == read_byte(TMA_ADDRESS) + 1);
}

TEST_CASE("Update timer: 16384 Hz, overflow", "[timer]") {
	unsigned char modulo = GENERATE(take(10, random(0x00, 0xFE)));
	write_byte(TMA_ADDRESS, modulo);
	write_byte(TIMA_ADDRESS, 0xFF);
	write_byte(TAC_ADDRESS, 0x07);	// Clock enabled + 11 clock rate
	reset_timer_counter();
	update_timer(256);
	CHECK(read_byte(TIMA_ADDRESS) == read_byte(TMA_ADDRESS));
	//TODO: Check requested interrupt
}