#ifndef _PE_BASE64_H_
#define _PE_BASE64_H_

#include <stdint.h>

typedef uint64_t PE_Base64_Result;
#define PE_BASE64_SUCCESS 1
#define PE_BASE64_FAILURE 0

uint64_t pe_base64_encode_bytesize(uint64_t input_size);
uint64_t pe_base64_decode_bytesize(const char *input, uint64_t input_size);

PE_Base64_Result pe_base64_encode(const void *input, uint64_t input_size, char *output, uint64_t output_size);
PE_Base64_Result pe_base64_decode(const char *input, uint64_t input_size, void *output, uint64_t output_size);

uint64_t pe_base64_validate(const char *input, uint64_t input_size);

#endif
