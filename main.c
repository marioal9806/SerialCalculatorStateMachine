#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

float acum[2] = {0.0, 0.0};
float multiplicador = 1.0;
float operation_result = 0.0;
unsigned char digit_flag = 0; // Flag that indicates second digit is read
char operand = 0;

enum error_codes {
	PROTOCOL_ERROR,
	NUMBER_OVERFLOW_ERROR,
	UNDEFINED_STATE_ERROR,
} last_error;

enum state_codes { 
	first_parenthesis, 
	left_digit, 
	right_digit,
	result,
	error
};

enum ret_codes { 
	ok, 
	fail, 
	repeat, 
	done_reading // Indicates you already read the second digit
};

enum ret_codes first_parenthesis_state(char chr);
enum ret_codes left_digit_state(char chr);
enum ret_codes right_digit_state(char chr);
enum ret_codes result_state(char chr);
enum ret_codes error_state(char chr);

/* array and enum below must be in sync! */
enum ret_codes (*state[])(char) = {
	first_parenthesis_state,
	left_digit_state,
	right_digit_state,
	result_state,
	error_state
};

struct transition {
	enum state_codes src_state;
	enum ret_codes   ret_code;
	enum state_codes dst_state;
};

struct transition state_transitions[] = {
	{first_parenthesis, ok, left_digit},
	{first_parenthesis, repeat, first_parenthesis},
	{left_digit, ok, right_digit},
	{left_digit, repeat, left_digit},
	{left_digit, fail, error},
	{right_digit, ok, left_digit},
	{right_digit, done_reading, result},
	{right_digit, repeat, right_digit},
	{right_digit, fail, error},
	{result, ok, first_parenthesis},
	{result, fail, error},
	{error, ok, first_parenthesis}
};

enum state_codes lookup_transitions(enum state_codes cur_state, enum ret_codes rc) {
	int i;
	for (i = 0; i < sizeof(state_transitions) / sizeof(struct transition); i++) {
		if (cur_state == state_transitions[i].src_state && rc == state_transitions[i].ret_code) {
			return state_transitions[i].dst_state;
		}
	}
	last_error = UNDEFINED_STATE_ERROR;
	return fail;
}

int first_parenthesis_state(char chr) {
	if (chr == '(') {
		printf("(");
		return ok;
	}
	else { // Ignore any input other than left parenthesis
		return repeat;
	}
}

int left_digit_state(char chr) {
	float prev_acum;
	if ((chr >= '0') && (chr <= '9')) { // Only accept digits
		prev_acum = acum[digit_flag];
		acum[digit_flag] *= 10;
		acum[digit_flag] += (chr - '0');

		if (prev_acum > acum[digit_flag]) { // Check for number overflow
			last_error = NUMBER_OVERFLOW_ERROR;
			return fail;
		}
		printf("%c", chr);
		return repeat;
	}
	else if (chr == '.') {
		printf("%c", chr);
		return ok;
	}
	else {
		last_error = PROTOCOL_ERROR;
		return fail;
	}
}

int right_digit_state(char chr) {
	float prev_acum;
	if ((chr >= '0') && (chr <= '9')) { // Only accept digits
		prev_acum = acum[digit_flag];
		multiplicador *= (float)0.1;
		acum[digit_flag] += ((chr - '0') * multiplicador);

		if (prev_acum > acum[digit_flag]) { // Check for number overflow
			last_error = NUMBER_OVERFLOW_ERROR;
			return fail;
		}
		printf("%c", chr);
		return repeat;
	}
	else if (chr == '+' || chr == '-' || chr == '*' || chr == '/') {
		if (!digit_flag) {
			printf("%c", chr);
			operand = chr;
			digit_flag = 1; // Activate the flag, since you're done reading the first number
			multiplicador = 1.0; // Reset the floating point multiplier
			return ok;
		}
		else {
			last_error = PROTOCOL_ERROR;
			return fail;
		}
	}
	else if (chr == ')') {
		if (!digit_flag) {
			last_error = PROTOCOL_ERROR;
			return fail;
		}
		else {
			printf("%c", chr);
			digit_flag = 0; // Reset the digit flag for next operation
			multiplicador = 1.0; // Reset the floating point multiplier
			return done_reading;
		}
	}
	else {
		last_error = PROTOCOL_ERROR;
		return fail;
	}
}

int result_state(char chr) {
	if (chr == '=') {
		switch (operand)
		{
		case '+':
			operation_result = acum[0] + acum[1];
			break;
		case '-':
			operation_result = acum[0] - acum[1];
			break;
		case '*':
			operation_result = acum[0] * acum[1];
			break;
		case '/':
			operation_result = acum[0] / acum[1];
			break;
		}
		printf("=%f\n", operation_result);
		// Reset aux variables for next operation
		acum[0] = 0.0;
		acum[1] = 0.0;
		multiplicador = 1.0;
		operation_result = 0.0;
		operand = 0;
		digit_flag = 0;
		return ok;
	}
	else { // Ignore any input other than left parenthesis
		last_error = PROTOCOL_ERROR;
		return fail;
	}
}

int error_state(char chr) {
	printf("\n");
	switch (last_error) {
	case PROTOCOL_ERROR:
		printf("Error: Utiliza el siguiente formato para introducir datos: ");
		printf("(<Numero><Operador><Numero>)=\n");
		break;
	case NUMBER_OVERFLOW_ERROR:
		printf("Error: Limite de precision alcanzado\n");
		break;
	case UNDEFINED_STATE_ERROR:
		printf("Error: Estado indefinido\n");
		break;
	}
	// Reset aux variables for next operation
	acum[0] = 0.0;
	acum[1] = 0.0;
	multiplicador = 1.0;
	operation_result = 0.0;
	operand = 0;
	digit_flag = 0;
	return ok;
}


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