#pragma once

#include <fairandom/fairandom.h>
#include <stdlib.h>

#define HEX_LEN(x) (x * (size_t)2)
#define HEX_LEN_WITH_NULL(x) (HEX_LEN(x) + 1)

void Fr_BytesToHex(const fr_bytes_t input, size_t data_len, fr_bytes_t output);
void Fr_HexToBytes(const fr_bytes_t input, size_t data_len, fr_bytes_t output);
