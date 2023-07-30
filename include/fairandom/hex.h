#pragma once

#include <stdlib.h>

void fr_bytes_to_hex(unsigned const char *input, size_t data_len, char *output);
void fr_hex_to_bytes(const char *input, size_t data_len, char *output);