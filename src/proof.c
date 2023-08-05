#include <fairandom/fairandom.h>
#include <fairandom/hash.h>
#include <fairandom/hex.h>
#include <fairandom/proof.h>
#include <stdio.h>
#include <string.h>

void fr_generate_proof(FrGenerator *generator, struct FrProof *proof) {
  fr_bytes_t proof_hash = malloc(FR_PROOF_HASH_LEN);
  _fr_hash_proof(generator->seed, generator->seed_len, proof_hash);
  proof->rounds = generator->rounds;
  memcpy(proof->salt, generator->salt, FR_SALT_LEN);
  memcpy(proof->proof, proof_hash, FR_PROOF_HASH_LEN);
  free(proof_hash);
}

bool fr_verify_proof(fr_bytes_t seed, size_t seed_len, fr_bytes_t data,
                     size_t data_len, struct FrProof proof) {
  FrGenerator *generator = fr_generator_new(proof.rounds, proof.salt);
  fr_generator_seed(generator, FR_SEED_TYPE_STRING, seed, seed_len);

  struct FrProof real_proof;
  fr_generate_proof(generator, &real_proof);

  if (memcmp(&real_proof, &proof, sizeof(struct FrProof)) != 0) {
    fr_generator_free(generator);
    return false;
  }

  fr_bytes_t output = malloc(data_len);
  fr_generator_generate(generator, output, data_len);

  int cmp_result = memcmp(output, data, data_len);
  fr_generator_free(generator);
  free(output);

  return cmp_result == 0;
}

void fr_proof_as_string(struct FrProof *proof, fr_bytes_t output) {
  // Convert proof struct to bytes and then to hex
  fr_bytes_t proof_bytes = malloc(sizeof(struct FrProof));
  memcpy(proof_bytes, &proof->rounds, sizeof(uint32_t));
  memcpy(proof_bytes + sizeof(uint32_t), proof->salt, sizeof(proof->salt));
  memcpy(proof_bytes + sizeof(uint32_t) + sizeof(proof->salt), proof->proof,
         sizeof(proof->proof));
  Fr_BytesToHex(proof_bytes, sizeof(struct FrProof), output);
  free(proof_bytes);
}

void fr_proof_from_string(struct FrProof *proof, const fr_bytes_t input) {
  // Convert hex to bytes and then to proof struct
  fr_bytes_t proof_bytes = malloc(sizeof(struct FrProof));
  Fr_HexToBytes(input, sizeof(struct FrProof), proof_bytes);
  memcpy(&proof->rounds, proof_bytes, sizeof(uint32_t));
  memcpy(proof->salt, proof_bytes + sizeof(uint32_t), sizeof(proof->salt));
  memcpy(proof->proof, proof_bytes + sizeof(uint32_t) + sizeof(proof->salt),
         sizeof(proof->proof));
  free(proof_bytes);
}
