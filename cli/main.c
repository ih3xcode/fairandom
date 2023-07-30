#include <fairandom/fairandom.h>
#include <fairandom/hex.h>
#include <fairandom/proof.h>
#include <prof.h>
#include <stdio.h>

#define TEST_SEED                                                              \
  "616161616161616161616161616161616161616161616161616168656c6c6f20776f726c64" \
  "61616161616161616161616161616161616161616161616161616161616161616161616161" \
  "6161616161616161616161616161616168656c6c6f20776f726c64"                     \
  "61616161616161616161616161616161616161616161616161616161616161616161616161" \
  "6161616161616161616161616161616168656c6c6f20776f726c64"                     \
  "61616161616161616161616161616161616161616161616161616161616161616161616161" \
  "6161616161616161616161616161616168656c6c6f20776f726c64"                     \
  "616161616161616161616161616161616161616161616161616161"
#define TEST_SEED_LEN 512
#define RANDOM_OUTPUT_LEN 256

int main(void) {
  PROF_START();
  fr_generator_t *generator =
      fr_generator_new(FR_DEFAULT_ROUNDS, FR_DEFAULT_SALT);

  char seed[TEST_SEED_LEN / 2];
  fr_hex_to_bytes(TEST_SEED, TEST_SEED_LEN, seed);

  fr_generator_seed(generator, FR_SEED_TYPE_STRING, seed, RANDOM_OUTPUT_LEN);

  char output[RANDOM_OUTPUT_LEN];
  fr_generate(generator, output, RANDOM_OUTPUT_LEN);

  char proof_string[FR_PROOF_STRING_LEN];
  struct fr_proof proof;
  fr_generate_proof(generator, &proof);

  fr_proof_as_string(&proof, proof_string);
  // fr_proof_from_string(&proof, proof_string);
  // fr_proof_as_string(&proof, proof_string);

  char output_string[RANDOM_OUTPUT_LEN * 2];
  fr_bytes_to_hex(output, RANDOM_OUTPUT_LEN, output_string);
  printf("Seed: %s\n", TEST_SEED);
  printf("Output: %s\n", output_string);
  printf("Proof: %s\n", proof_string);

  fr_generator_free(generator);

  printf("\nVerify result: %d\n",
         fr_verify_proof(seed, TEST_SEED_LEN / 2, output, RANDOM_OUTPUT_LEN,
                         proof));

  PROF_STDERR();

  return 0;
}