/*
 * main.c tests, PROBABLY NOT NEEDED
 */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

/* extern "C" {
	#include "../src/main.c"
}*/

TEST_CASE("Trial", "[Trial Tests]")
{
	CHECK(1 == 2);
	CHECK(2 == 2);
	CHECK(1 == 2);
	CHECK(4 == 2);

	// CHECK(get_an_int() == 3);
}

