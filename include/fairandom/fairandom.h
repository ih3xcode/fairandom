#pragma once
#include <fairandom/hash.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define FR_SALT_LEN 16

enum FrSeedType {
  FR_SEED_TYPE_RANDOM,
  FR_SEED_TYPE_STRING,
};

struct FrGenerator {
  uint32_t rounds;
  char salt[FR_SALT_LEN];
  char *seed;
  size_t seed_len;
};

typedef struct FrGenerator FrGenerator;

#define FR_DEFAULT_ROUNDS 100000
#define FR_DEFAULT_SALT                                                     \
  (char[]) {                                                                \
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, \
        0x0c, 0x0d, 0x0e, 0x0f                                              \
  }

#ifdef FR_USE_DOUBLE_ARITHMETIC
#define FR_LIGHT_CEIL(x, y) ceil((double)x / (double)y)
#else
#define FR_LIGHT_CEIL(x, y) ((x / y) + (x % y != 0))
#endif

#define FR_FULL_BLOCK_LEN(x) \
  FR_LIGHT_CEIL(x, FR_ROUND_HASH_LEN) * FR_ROUND_HASH_LEN

FrGenerator *fr_generator_new(uint32_t rounds, const char salt[FR_SALT_LEN]);
void fr_generator_free(FrGenerator *generator);

void fr_generator_seed(FrGenerator *generator, enum FrSeedType type,
                       const char *seed, size_t seed_len);
void fr_generator_getseed(FrGenerator *generator, char *output,
                          size_t output_len);

void fr_generator_generate(FrGenerator *generator, char *output,
                           size_t output_len);

#define Fr_Generate fr_generator_generate

void _fr_random(char *output, size_t output_len);