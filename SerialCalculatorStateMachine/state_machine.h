#pragma once

// Defines the possible states
enum state_codes {
	first_parenthesis,
	left_digit,
	right_digit,
	result,
	error
};

// Defines the possible return codes
enum ret_codes {
	ok,
	fail,
	repeat,
	done_reading // Indicates you already read the second digit
};

struct transition {
	enum state_codes src_state;
	enum ret_codes   ret_code;
	enum state_codes dst_state;
};

enum ret_codes first_parenthesis_state(char chr);
enum ret_codes left_digit_state(char chr);
enum ret_codes right_digit_state(char chr);
enum ret_codes result_state(char chr);
enum ret_codes error_state(char chr);

enum state_codes lookup_transitions(enum state_codes cur_state, enum ret_codes rc);

// Defines the possible error codes
enum error_codes {
	PROTOCOL_ERROR,
	NUMBER_OVERFLOW_ERROR,
	UNDEFINED_STATE_ERROR,
} last_error;