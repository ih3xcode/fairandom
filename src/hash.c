#include <fairandom/fairandom.h>
#include <fairandom/hash.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <string.h>

static void _hash_round(const char *input, size_t input_len, char *output,
                        const char *salt, EVP_MD_CTX *mdctx) {
  char *input_with_salt = malloc(input_len + FR_SALT_LEN);
  memcpy(input_with_salt, input, input_len);
  memcpy(input_with_salt + input_len, salt, FR_SALT_LEN);

  EVP_DigestInit_ex(mdctx, FR_ROUND_HASH_ALGO, NULL);
  EVP_DigestUpdate(mdctx, input_with_salt, input_len + FR_SALT_LEN);
  EVP_DigestFinal_ex(mdctx, output, NULL);

  free(input_with_salt);
}

void _fr_hash_rounds(const char *input, size_t input_len, char *output,
                     const char *salt, uint32_t rounds) {
  char *hash = malloc(input_len);
  memcpy(hash, input, input_len);

  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  for (uint32_t i = 0; i < rounds; i++) {
    _hash_round(hash, FR_ROUND_HASH_LEN, hash, salt, mdctx);
    EVP_MD_CTX_reset(mdctx);
  }
  EVP_MD_CTX_free(mdctx);

  memcpy(output, hash, FR_ROUND_HASH_LEN);
  free(hash);
}

void _fr_hash_proof(const char *input, size_t input_len, char *output) {
  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  EVP_DigestInit_ex(mdctx, FR_PROOF_HASH_ALGO, NULL);
  EVP_DigestUpdate(mdctx, input, input_len);
  EVP_DigestFinal_ex(mdctx, output, NULL);
  EVP_MD_CTX_free(mdctx);
}
