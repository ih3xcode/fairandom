#include <fairandom/hex.h>
#include <stdio.h>

void Fr_BytesToHex(const fr_bytes_t input, size_t data_len, fr_bytes_t output) {
  int pos = 0;
  for (size_t i = 0; i < data_len; i++) {
    pos += sprintf((char *)output + pos, "%02x", input[i]);
  }
}

void Fr_HexToBytes(const fr_bytes_t input, size_t data_len, fr_bytes_t output) {
  // data_len is the length of the resulting bytes, not the hex string
  for (size_t i = 0; i < data_len; i++) {
    sscanf((const char *)input + i * 2, "%2hhx", output + i);
  }
}
