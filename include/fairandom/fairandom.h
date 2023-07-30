#pragma once
#include <stdint.h>
#include <stdlib.h>

#define FR_SALT_LEN 16

enum fr_seed_type {
  FR_SEED_TYPE_RANDOM,
  FR_SEED_TYPE_STRING,
};

typedef struct {
  uint32_t rounds;
  char salt[FR_SALT_LEN];
  char *seed;
  size_t seed_len;
} fr_generator_t;

#define FR_DEFAULT_ROUNDS 100000
#define FR_DEFAULT_SALT                                                     \
  (char[]) {                                                                \
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, \
        0x0c, 0x0d, 0x0e, 0x0f                                              \
  }

fr_generator_t *fr_generator_new(uint32_t rounds, const char salt[FR_SALT_LEN]);
void fr_generator_free(fr_generator_t *generator);

void fr_generator_seed(fr_generator_t *generator, enum fr_seed_type type,
                       const char *seed, size_t seed_len);
void fr_generator_getseed(fr_generator_t *generator, char *output,
                          size_t output_len);

void fr_generate(fr_generator_t *generator, char *output, size_t output_len);

void fr_random(char *output, size_t output_len);