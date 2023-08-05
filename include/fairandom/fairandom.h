#pragma once
#include <fairandom/hash.h>
#include <fairandom/types.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define FR_DEFAULT_ROUNDS 100000
#define FR_DEFAULT_SALT                                                     \
  (fr_salt_t) {                                                             \
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

FrGenerator *fr_generator_new(uint32_t rounds, fr_salt_t salt);
void fr_generator_free(FrGenerator *generator);

void fr_generator_seed(FrGenerator *generator, enum FrSeedType type,
                       fr_bytes_t seed, size_t seed_len);
void fr_generator_getseed(FrGenerator *generator, fr_bytes_t output);

void fr_generator_generate(FrGenerator *generator, fr_bytes_t output,
                           size_t output_len);

#define Fr_Generate fr_generator_generate

void _fr_random(fr_bytes_t output, size_t output_len);
