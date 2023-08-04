#include <assert.h>
#include <fairandom/fairandom.h>
#include <fairandom/hash.h>
#include <math.h>
#include <string.h>

fr_generator_t *fr_generator_new(uint32_t rounds,
                                 const char salt[FR_SALT_LEN]) {
  fr_generator_t *generator = malloc(sizeof(fr_generator_t));
  if (generator == NULL) {
    return NULL;
  }
  generator->rounds = rounds;
  memcpy(generator->salt, salt, 16);
  return generator;
}

void fr_generator_free(fr_generator_t *generator) {
  free(generator->seed);
  free(generator);
}

void fr_generator_seed(fr_generator_t *generator, enum fr_seed_type type,
                       const char *seed, size_t seed_len) {
  if (type == FR_SEED_TYPE_RANDOM) {
    generator->seed = malloc(seed_len);
    generator->seed_len = seed_len;
    fr_random(generator->seed, seed_len);
  } else if (type == FR_SEED_TYPE_STRING) {
    generator->seed = malloc(seed_len);
    generator->seed_len = seed_len;
    memcpy(generator->seed, seed, seed_len);
  }
}

void fr_generator_getseed(fr_generator_t *generator, char *output,
                          size_t output_len) {
  assert(output_len >= generator->seed_len);
  memcpy(output, generator->seed, generator->seed_len);
}

void fr_generate(fr_generator_t *generator, char *output, size_t output_len) {
  assert(FR_FULL_BLOCK_LEN(output_len) <= generator->seed_len);
  uint32_t blocks_count = FR_LIGHT_CEIL(output_len, FR_ROUND_HASH_LEN);
  char *blocks[blocks_count];

  // Split generator->seed into blocks and hash each block
  for (uint32_t i = 0; i < blocks_count; i++) {
    char *block = malloc(FR_ROUND_HASH_LEN);
    memcpy(block, generator->seed + (i * FR_ROUND_HASH_LEN), FR_ROUND_HASH_LEN);
    fr_hash_rounds(block, FR_ROUND_HASH_LEN, block, generator->salt,
                   generator->rounds);
    blocks[i] = block;
  }

  // Concatenate the blocks
  char *concatenated = malloc(blocks_count * FR_ROUND_HASH_LEN);
  for (uint32_t i = 0; i < blocks_count; i++) {
    memcpy(concatenated + (i * FR_ROUND_HASH_LEN), blocks[i],
           FR_ROUND_HASH_LEN);
    free(blocks[i]);
  }

  // Stip the concatenated blocks to the desired length
  memcpy(output, concatenated, output_len);

  // Free the concatenated blocks
  free(concatenated);
}