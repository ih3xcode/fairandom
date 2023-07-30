#include <fairandom/fairandom.h>
#include <fairandom/hex.h>
#include <fairandom/proof.h>
#include <stdio.h>

#define TEST_SEED                                                              \
  "1f96ef7ace29f054847e294c39169c001bc3723b1ade50502bbad397572f49b5371e902e09" \
  "64f87860eada5255b128d56b9b78813cd5b9fa8b9619f5433ff7c0"
#define TEST_SEED_LEN 128

int main(void) {
  fr_generator_t *generator = fr_generator_new(2, FR_DEFAULT_SALT);

  char seed[64];
  fr_hex_to_bytes(TEST_SEED, TEST_SEED_LEN, seed);

  fr_generator_seed(generator, FR_SEED_TYPE_STRING, seed, 64);

  char output[64];
  fr_generate(generator, output, 64);

  char proof_string[FR_PROOF_STRING_LEN];
  struct fr_proof proof;
  fr_generate_proof(generator, &proof);

  fr_proof_as_string(&proof, proof_string);

  char output_string[129];
  fr_bytes_to_hex(output, 64, output_string);
  printf("Seed: %s\n", TEST_SEED);
  printf("Output: %s\n", output_string);
  printf("Proof: %s\n", proof_string);

  fr_generator_free(generator);
  return 0;
}