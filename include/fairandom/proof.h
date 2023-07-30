#pragma once

#include <fairandom/fairandom.h>
#include <openssl/evp.h>
#include <stdbool.h>

#define FR_PROOF_STRING_LEN sizeof(struct fr_proof) * 2 + 1

struct fr_proof {
  uint32_t rounds;
  char salt[FR_SALT_LEN];
  char proof[32];
};

void fr_generate_proof(fr_generator_t *generator, struct fr_proof *proof);
bool fr_verify_proof(char *seed, size_t seed_len, char *data, size_t data_len,
                     struct fr_proof proof);

void fr_proof_as_string(struct fr_proof *proof, char *output);
void fr_proof_from_string(struct fr_proof *proof, const char *input);
