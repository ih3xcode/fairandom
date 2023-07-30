#include <fairandom/fairandom.h>
#include <fairandom/proof.h>

#define TEST_DATA_LEN 1024

int main(void) {
  fr_generator_t *generator =
      fr_generator_new(FR_DEFAULT_ROUNDS, FR_DEFAULT_SALT);
  fr_generator_seed(generator, FR_SEED_TYPE_RANDOM, NULL, 1024);

  char data[TEST_DATA_LEN];
  fr_generate(generator, data, TEST_DATA_LEN);

  struct fr_proof proof;
  fr_generate_proof(generator, &proof);

  char seed[TEST_DATA_LEN];
  fr_generator_getseed(generator, seed, TEST_DATA_LEN);

  fr_generator_free(generator);

  return !fr_verify_proof(seed, TEST_DATA_LEN, data, TEST_DATA_LEN, proof);
}