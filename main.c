#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "pe_base64.h"

int main() {

	printf("%llu\n", (uint64_t)2 / (uint64_t)3);
	{
		printf("ENCODE TEST\n");

		const char *encode_test_inputs[] = {
			"",
			"f",
			"fo",
			"foo",
			"foob",
			"fooba",
			"foobar",
		};
		const char *encode_test_outputs[] = {
			"",
			"Zg==",
			"Zm8=",
			"Zm9v",
			"Zm9vYg==",
			"Zm9vYmE=",
			"Zm9vYmFy",
		};
		const uint64_t decode_test_count = 7;

		for (uint64_t i = 0; i < decode_test_count; i++) {
			const char *input = encode_test_inputs[i];
			uint64_t input_size = strlen(input);

			uint64_t output_size = pe_base64_encode_bytesize(input_size);
			char *output = malloc(output_size + 1);

			pe_base64_encode(input, strlen(input), output, output_size);
			output[output_size] = '\0';

			if (strcmp(output, encode_test_outputs[i]) == 0) {
				printf("[OK] Input '%s':%llu => Output '%s':%llu\n", input, input_size, output, output_size);
			} else {
				printf("[FAIL] Input '%s':%llu => Output '%s':%llu instead of Output '%s'\n", input, input_size, output, output_size, encode_test_outputs[i]);
			}

			free(output);
		}
		printf("\n");
	}

	{
		printf("VALIDATE TEST\n");

		const char *validate_test_inputs[] = {
			"",
			"Zg==",
			"Zm8=",
			"Zm9v",
			"Zm9vYg==",
			"Zm9vYmE=",
			"Zm9vYmFy",
			"Zg=",
			";Zg-",
			";Zg+",
		};
		const uint64_t validate_test_outputs[] = {
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			0,
			0,
			0,
		};
		const uint64_t validate_test_count = 10;

		for (uint64_t i = 0; i < validate_test_count; i++) {
			const char *input = validate_test_inputs[i];
			uint64_t input_size = strlen(input);
			
			uint64_t is_valid = pe_base64_validate(input, input_size);
			if (is_valid == validate_test_outputs[i]) {
				if (is_valid) {
					printf("[OK] Input '%s' => Valid\n", input);
				} else {
					printf("[OK] Input '%s' => Not Valid\n", input);
				}
			} else {
				if (is_valid) {
					printf("[FAIL] Input '%s' => Valid instead of Not Valid\n", input);
				} else {
					printf("[FAIL] Input '%s' => Not Valid instead of Valid\n", input);
				}
			}
		}
		printf("\n");
	}

	{
		printf("DECODE TEST\n");

		const char *decode_test_inputs[] = {
			"",
			"Zg==",
			"Zm8=",
			"Zm9v",
			"Zm9vYg==",
			"Zm9vYmE=",
			"Zm9vYmFy",
		};
		const char *decode_test_outputs[] = {
			"",
			"f",
			"fo",
			"foo",
			"foob",
			"fooba",
			"foobar",
		};
		const uint64_t decode_test_count = 7;

		for (uint64_t i = 0; i < decode_test_count; i++) {
			const char *input = decode_test_inputs[i];
			uint64_t input_size = strlen(input);
			
			uint64_t output_size = pe_base64_decode_bytesize(input, input_size);
			char *output = malloc(output_size + 1);

			pe_base64_decode(input, strlen(input), output, output_size);
			output[output_size] = '\0';

			if (strcmp(output, decode_test_outputs[i]) == 0) {
				printf("[OK] Input '%s':%llu => Output '%s':%llu\n", input, input_size, output, output_size);
			} else {
				printf("[FAIL] Input '%s':%llu => Output '%s':%llu instead of Output '%s'\n", input, input_size, output, output_size, decode_test_outputs[i]);
			}
		}
		printf("\n");
	}
}
