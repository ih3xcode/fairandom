#include <fairandom/fairandom.h>
#include <openssl/rand.h>

void fr_random(char *output, size_t output_len) {
  if (RAND_bytes(output, output_len) != 1) {
    abort();
  }
}