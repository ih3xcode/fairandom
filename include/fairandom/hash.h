#pragma once
#include <openssl/evp.h>
#include <stdint.h>
#include <stdlib.h>

#define FR_PROOF_HASH_ALGO EVP_sha3_256()
#define FR_PROOF_HASH_LEN 32

#define FR_ROUND_HASH_ALGO EVP_sha3_512()
#define FR_ROUND_HASH_LEN 64

void fr_hash_round(const char *input, size_t input_len, char *output,
                   const char *salt, EVP_MD_CTX *mdctx);
void fr_hash_rounds(const char *input, size_t input_len, char *output,
                    const char *salt, uint32_t rounds);

void fr_hash_proof(const char *input, size_t input_len, char *output);