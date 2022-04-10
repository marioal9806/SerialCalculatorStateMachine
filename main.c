#include <stdio.h>
#include <stdlib.h>

#include "SerialCalculatorStateMachine/config.h"

#include "SerialCalculatorStateMachine/serial.h"
#include "SerialCalculatorStateMachine/state_machine.h"


#ifdef SERIAL
#define READ_FUNCTION(chr_p) serialRead((chr_p))
#else
#include <conio.h>
#define READ_FUNCTION(chr_p) readFromKeyboard((chr_p))

int readFromKeyboard(char* chr) {
	while (1) { // We must cycle here until we get a keyboard hit
		if (_kbhit()) { // Not a blocking function like ReadFile()
			*chr = _getch();
			return 0;
		}
	}
	return 1;
}
#endif // SERIAL


// Function pointer array
enum ret_codes(*state[])(char) = {
	first_parenthesis_state,
	left_digit_state,
	right_digit_state,
	result_state,
	error_state
};

int main() {
	enum state_codes cur_state = first_parenthesis;
	enum ret_codes rc = ok;
	enum ret_codes (*state_fun)(char);

	int ret = 0;
	ret = init_serial();
	if (ret) {
		return 0;
	}

	char chr = 0;
	while (chr != 8 && chr != 27) {		//ESC o Backspace terminan la ejecucion del programa
		if (rc != fail) {
			ret = READ_FUNCTION(&chr);
			if (ret) { // Check for successful read
				printf("Error: Unsuccessful read\n");
				break;
			}
		}

		if (chr != 8 && chr != 27) {
			state_fun = state[cur_state];
			rc = state_fun(chr);
			cur_state = lookup_transitions(cur_state, rc);
		}
	}

	printf("\nFin!!!\n");
	return 0;
}