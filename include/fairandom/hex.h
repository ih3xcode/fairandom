#pragma once

#include <stdlib.h>

#define HEX_LEN(x) (x * 2)
#define HEX_LEN_WITH_NULL(x) (HEX_LEN(x) + 1)

void fr_bytes_to_hex(unsigned const char *input, size_t data_len, char *output);
void fr_hex_to_bytes(const char *input, size_t data_len, char *output);