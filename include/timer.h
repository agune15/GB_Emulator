#pragma once

#define TIMA_ADDRESS 0xFF05	//Timer counter
#define TMA_ADDRESS  0xFF06	//Timer modulo (loaded when timer overflows)
#define TAC_ADDRESS  0xFF07	//Timer control

void update_timer(int cycles);