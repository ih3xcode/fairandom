#pragma once
#include <stdint.h>
#include <stdlib.h>

#define FR_SALT_LEN 16
typedef unsigned char *fr_bytes_t;
typedef unsigned char fr_salt_t[FR_SALT_LEN];

enum FrSeedType {
  FR_SEED_TYPE_RANDOM,
  FR_SEED_TYPE_STRING,
};

struct FrGenerator {
  uint32_t rounds;
  fr_salt_t salt;
  fr_bytes_t seed;
  size_t seed_len;
};

typedef struct FrGenerator FrGenerator;
