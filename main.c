#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "SerialCalculatorStateMachine/state_machine.h"

// Function pointer array
enum ret_codes(*state[])(char) = {
	first_parenthesis_state,
	left_digit_state,
	right_digit_state,
	result_state,
	error_state
};

void main() {
	enum state_codes cur_state = first_parenthesis;
	enum ret_codes rc;
	enum ret_codes (*state_fun)(char);

	char chr = 0;
	while (chr != 8 && chr != 27) {		//ESC o Backspace terminan la ejecucion del programa
		if (_kbhit()) {		//Buscando entrada del teclado
			chr=_getch();	//Tomando entrada del teclado
			if (chr != 8 && chr != 27) {
				state_fun = state[cur_state];
				rc = state_fun(chr);
				cur_state = lookup_transitions(cur_state, rc);
			}
		}
	}
	printf("\nFin!!!\n");
}