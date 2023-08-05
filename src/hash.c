#include <fairandom/fairandom.h>
#include <fairandom/hash.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <string.h>

static void _hash_round(const fr_bytes_t input, size_t input_len,
                        fr_bytes_t output, const fr_salt_t salt,
                        EVP_MD_CTX *mdctx) {
  char *input_with_salt = malloc(input_len + FR_SALT_LEN);
  memcpy(input_with_salt, input, input_len);
  memcpy(input_with_salt + input_len, salt, FR_SALT_LEN);

  EVP_DigestInit_ex(mdctx, FR_ROUND_HASH_ALGO, NULL);
  EVP_DigestUpdate(mdctx, input_with_salt, input_len + FR_SALT_LEN);
  EVP_DigestFinal_ex(mdctx, (unsigned char *)output, NULL);

  free(input_with_salt);
}

void _fr_hash_rounds(const fr_bytes_t input, size_t input_len,
                     fr_bytes_t output, const fr_salt_t salt, uint32_t rounds) {
  fr_bytes_t hash = malloc(input_len);
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

void _fr_hash_proof(const fr_bytes_t input, size_t input_len,
                    fr_bytes_t output) {
  EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
  EVP_DigestInit_ex(mdctx, FR_PROOF_HASH_ALGO, NULL);
  EVP_DigestUpdate(mdctx, input, input_len);
  EVP_DigestFinal_ex(mdctx, (unsigned char *)output, NULL);
  EVP_MD_CTX_free(mdctx);
}
