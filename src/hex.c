#include <fairandom/hex.h>
#include <stdio.h>

void Fr_BytesToHex(const fr_bytes_t input, size_t data_len, fr_bytes_t output) {
  int pos = 0;
  for (size_t i = 0; i < data_len; i++) {
    pos += sprintf((char *)output + pos, "%02x", input[i]);
  }
}

void Fr_HexToBytes(const fr_bytes_t input, size_t data_len, fr_bytes_t output) {
  for (size_t i = 0, j = 0; i < (data_len / 2); i++, j += 2)
    output[i] = (input[j] % 32 + 9) % 25 * 16 + (input[j + 1] % 32 + 9) % 25;
}
