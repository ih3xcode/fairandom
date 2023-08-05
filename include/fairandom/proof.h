#pragma once

#include <fairandom/fairandom.h>
#include <openssl/evp.h>
#include <stdbool.h>

#define FR_PROOF_STRING_LEN sizeof(struct FrProof) * 2 + 1

struct FrProof {
  uint32_t rounds;
  char salt[FR_SALT_LEN];
  char proof[32];
};

void fr_generate_proof(FrGenerator *generator, struct FrProof *proof);
bool fr_verify_proof(char *seed, size_t seed_len, char *data, size_t data_len,
                     struct FrProof proof);

void fr_proof_as_string(struct FrProof *proof, char *output);
void fr_proof_from_string(struct FrProof *proof, const char *input);
