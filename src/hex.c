#include <fairandom/hex.h>
#include <stdio.h>

void fr_bytes_to_hex(unsigned const char *input, size_t input_len,
                     char *output) {
  for (size_t i = 0; i < input_len; i++) {
    sprintf(output + i * 2, "%02x", input[i]);
  }
  sprintf(output + input_len * 2, "\0");
}

void fr_hex_to_bytes(const char *input, size_t input_len, char *output) {
  for (size_t i = 0; i < input_len; i++) {
    sscanf(input + i * 2, "%02x", (unsigned int *)(output + i));
  }
  sprintf(output + input_len, "\0");
}