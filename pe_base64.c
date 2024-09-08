#include "pe_base64.h"

#include <stdio.h>

uint64_t pe_base64_encode_bytesize(uint64_t input_size) {
	return (input_size / 3 + (input_size % 3 != 0)) * 4;
}

uint64_t pe_base64_decode_bytesize(const char *input, uint64_t input_size) {
	return input_size / 4 * 3 - (input[input_size - 1] == '=') - (input[input_size - 2] == '=');
}

static const char base64_table_encode[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};

PE_Base64_Result pe_base64_encode(const void *input_v, uint64_t input_size, char *output_v, uint64_t output_size) {
	uint64_t success = PE_BASE64_SUCCESS;

	const uint8_t *input = (const uint8_t *)input_v;
	uint8_t *output = (uint8_t *)output_v;
	uint64_t loop_count = input_size / 3;

	for (uint64_t index = 0; index < loop_count; index++) {
		uint8_t current[3];
		current[0] = input[index * 3 + 0];
		current[1] = input[index * 3 + 1];
		current[2] = input[index * 3 + 2];
 
		uint8_t split_input[4];
		split_input[0] = (current[0] & 0b11111100) >> 2;
		split_input[1] = (((current[0] & 0b00000011) << 4) & 0b00110000) | (((current[1] & 0b11110000) >> 4) & 0b00001111);
		split_input[2] = (((current[1] & 0b00001111) << 2) & 0b00111100) | (((current[2] & 0b11000000) >> 6) & 0b00000011);
		split_input[3] = current[2] & 0b00111111;

		output[index * 4 + 0] = base64_table_encode[split_input[0] & 0b00111111];
		output[index * 4 + 1] = base64_table_encode[split_input[1] & 0b00111111];
		output[index * 4 + 2] = base64_table_encode[split_input[2] & 0b00111111];
		output[index * 4 + 3] = base64_table_encode[split_input[3] & 0b00111111];
	}

	uint64_t input_pad_length = input_size % 3;
	if (input_pad_length == 1) {
		output[loop_count * 4 + 0] = base64_table_encode[(input[loop_count * 3 + 0] & 0b11111100) >> 2];
		output[loop_count * 4 + 1] = base64_table_encode[(((input[loop_count * 3 + 0] & 0b00000011) << 4) & 0b00110000)];
		output[loop_count * 4 + 2] = '=';
		output[loop_count * 4 + 3] = '=';
	} else if (input_pad_length == 2) {
		output[loop_count * 4 + 0] = base64_table_encode[(input[loop_count * 3 + 0] & 0b11111100) >> 2];
		output[loop_count * 4 + 1] = base64_table_encode[(((input[loop_count * 3 + 0] & 0b00000011) << 4) & 0b00110000) | (((input[loop_count * 3 + 1] & 0b11110000) >> 4) & 0b00001111)];
		output[loop_count * 4 + 2] = base64_table_encode[(((input[loop_count * 3 + 1] & 0b00001111) << 2) & 0b00111100)];
		output[loop_count * 4 + 3] = '=';
	}

	return success;
}

static const char base64_table_decode[] = {
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, /* 0 - 15 */
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, /* 16 - 31 */
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 62, 128, 128, 128, 63, /* 32 - 47 */
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 128, 128, 128, 64, 128, 128, /* 48 - 63 */
    128,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, /* 64 - 79 */
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 128, 128, 128, 128, 128, /* 128 - 96 */
    128, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, /* 97 - 111 */
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 128, 128, 128, 128, 128 /* 112 - 127 */
};

PE_Base64_Result pe_base64_decode(const char *input_v, uint64_t input_size, void *output_v, uint64_t output_size) {
	uint64_t success = PE_BASE64_SUCCESS;

	const uint8_t *input = (const uint8_t *)input_v;
	uint8_t *output = (uint8_t *)output_v;
	uint64_t loop_count = input_size / 4;

	for (uint64_t index = 0; index < loop_count; index++) {
		uint8_t input_decode_0 = base64_table_decode[input[index * 4 + 0]];
		uint8_t input_decode_1 = base64_table_decode[input[index * 4 + 1]];
		uint8_t input_decode_2 = base64_table_decode[input[index * 4 + 2]];
		uint8_t input_decode_3 = base64_table_decode[input[index * 4 + 3]];

		uint8_t input_combined = (input_decode_0 | input_decode_1 | input_decode_2 | input_decode_3);
		if (input_combined & 0b01000000) {
			if (input_combined & 0b11000000) {
				output[index * 3 + 0] = ((input_decode_0 & 0b00111111) << 2) | ((input_decode_1 & 0b00110000) >> 4);
				if (input_decode_2 == 64) {
					output[index * 3 + 1] = ((input_decode_1 & 0b00001111) << 4);
				} else if (input_decode_3 == 64) {
					output[index * 3 + 1] = ((input_decode_1 & 0b00001111) << 4) | ((input_decode_2 & 0b00111100) >> 2);
					output[index * 3 + 2] = ((input_decode_2 & 0b00000011) << 6);
				}
				break;
			} else {
				return PE_BASE64_FAILURE;
			}
		}

		output[index * 3 + 0] = ((input_decode_0 & 0b00111111) << 2) | ((input_decode_1 & 0b00110000) >> 4);
		output[index * 3 + 1] = ((input_decode_1 & 0b00001111) << 4) | ((input_decode_2 & 0b00111100) >> 2);
		output[index * 3 + 2] = ((input_decode_2 & 0b00000011) << 6) | ((input_decode_3 & 0b00111111) >> 0);
	}

	return success;
}

uint64_t pe_base64_validate(const char *input, uint64_t input_size) {

	if (input_size % 4 != 0) {
		return 0;
	}

	uint64_t has_padding = 0;

	uint64_t index = 0;
	while (index < input_size) {
		char c = input[index];
		if (c == '=') {
			has_padding = 1;
			break;
		}

		char c_upper = c & 0b11011111;
		if (
			!(
				(c_upper >= 'A' && c_upper <= 'Z') ||
				(c >= '0' && c <= '9') ||
				c == '+' || c == '/'
			)
		) {
			return 0;
		}
		index++;
	}

	if (has_padding) {
		// check that there is at least two padding and the data ends in padding
		if (index % 4 != 0) {
			while (input[index] == '=') {
				index++;
			}
			if (index != input_size) {
				return 0;
			}
		} else {
			return 0;
		}
	}

	return 1;
}
