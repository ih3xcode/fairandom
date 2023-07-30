#include <fairandom/fairandom.h>
#include <fairandom/hash.h>
#include <fairandom/hex.h>
#include <fairandom/proof.h>
#include <stdio.h>
#include <string.h>

void fr_generate_proof(fr_generator_t *generator, struct fr_proof *proof) {
  char *proof_hash = malloc(FR_PROOF_HASH_LEN);
  fr_hash_proof(generator->seed, generator->seed_len, proof_hash);
  proof->rounds = generator->rounds;
  memcpy(proof->salt, generator->salt, FR_SALT_LEN);
  memcpy(proof->proof, proof_hash, FR_PROOF_HASH_LEN);
  free(proof_hash);
}

bool fr_verify_proof(char *seed, size_t seed_len, char *data, size_t data_len,
                     struct fr_proof proof) {
  fr_generator_t *generator = fr_generator_new(proof.rounds, proof.salt);
  fr_generator_seed(generator, FR_SEED_TYPE_STRING, seed, seed_len);

  struct fr_proof real_proof;
  fr_generate_proof(generator, &real_proof);

  if (memcmp(&real_proof, &proof, sizeof(struct fr_proof)) != 0) {
    fr_generator_free(generator);
    return false;
  }

  char *output = malloc(data_len);
  fr_generate(generator, output, data_len);

  int cmp_result = memcmp(output, data, data_len);
  fr_generator_free(generator);
  free(output);

  return cmp_result == 0;
}

void fr_proof_as_string(struct fr_proof *proof, char *output) {
  // Convert proof struct to bytes and then to hex
  char *proof_bytes = malloc(sizeof(struct fr_proof));
  memcpy(proof_bytes, &proof->rounds, sizeof(uint32_t));
  memcpy(proof_bytes + sizeof(uint32_t), proof->salt, sizeof(proof->salt));
  memcpy(proof_bytes + sizeof(uint32_t) + sizeof(proof->salt), proof->proof,
         sizeof(proof->proof));
  fr_bytes_to_hex(proof_bytes, sizeof(struct fr_proof), output);
  free(proof_bytes);
}

void fr_proof_from_string(struct fr_proof *proof, const char *input) {
  // Convert hex to bytes and then to proof struct
  char *proof_bytes = malloc(sizeof(struct fr_proof));
  fr_hex_to_bytes(input, FR_PROOF_STRING_LEN, proof_bytes);
  memcpy(&proof->rounds, proof_bytes, sizeof(uint32_t));
  memcpy(proof->salt, proof_bytes + sizeof(uint32_t), sizeof(proof->salt));
  memcpy(proof->proof, proof_bytes + sizeof(uint32_t) + sizeof(proof->salt),
         sizeof(proof->proof));
  free(proof_bytes);
}