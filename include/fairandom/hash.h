#pragma once
#include <fairandom/types.h>
#include <openssl/evp.h>
#include <stdint.h>
#include <stdlib.h>

#define FR_PROOF_HASH_ALGO EVP_sha3_256()
#define FR_PROOF_HASH_LEN 32

#define FR_ROUND_HASH_ALGO EVP_sha3_512()
#define FR_ROUND_HASH_LEN 64

void _fr_hash_rounds(const fr_bytes_t input, size_t input_len,
                     fr_bytes_t output, const fr_salt_t salt, uint32_t rounds);

void _fr_hash_proof(const fr_bytes_t input, size_t input_len,
                    fr_bytes_t output);
