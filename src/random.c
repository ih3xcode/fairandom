#include <fairandom/fairandom.h>
#include <openssl/rand.h>

void _fr_random(fr_bytes_t output, size_t output_len) {
  if (RAND_bytes((unsigned char *)output, output_len) != 1) {
    abort();
  }
}
