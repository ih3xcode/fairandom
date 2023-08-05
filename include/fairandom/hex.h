#pragma once

#include <stdlib.h>

#define HEX_LEN(x) (x * 2)
#define HEX_LEN_WITH_NULL(x) (HEX_LEN(x) + 1)

void Fr_BytesToHex(unsigned const char *input, size_t data_len, char *output);
void Fr_HexToBytes(const char *input, size_t data_len, char *output);
