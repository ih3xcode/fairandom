#include <fairandom/hex.h>
#include <stdio.h>

void fr_bytes_to_hex(unsigned const char *input, size_t data_len,
                     char *output) {
  for (size_t i = 0; i < data_len; i++) {
    sprintf(output + i * 2, "%02x", input[i]);
  }
  output[data_len * 2] = '\0';
}

void fr_hex_to_bytes(const char *input, size_t data_len, char *output) {
  for (size_t i = 0; i < data_len * 2; i++) {
    sscanf(input + i * 2, "%02x", (unsigned int *)(output + i));
  }
}