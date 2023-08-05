#include <fairandom/hex.h>
#include <stdio.h>

void Fr_BytesToHex(unsigned const char *input, size_t data_len, char *output) {
  int pos = 0;
  for (size_t i = 0; i < data_len; i++) {
    pos += sprintf(output + pos, "%02x", input[i]);
  }
}

void Fr_HexToBytes(const char *input, size_t data_len, char *output) {
  // data_len is the length of the resulting bytes, not the hex string
  for (size_t i = 0; i < data_len - 1; i++) {
    sscanf(input + i * 2, "%2hhx", output + i);
  }
}
