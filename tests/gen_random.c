#include <fairandom/fairandom.h>
#include <fairandom/hex.h>
#include <fairandom/proof.h>
#include <stdio.h>
#include <string.h>

#define TESTS_COUNT 4
#define TEST_VECTOR_LEN 64

int main(void) {
  char outputs[TESTS_COUNT][TEST_VECTOR_LEN];

  for (int i = 0; i < TESTS_COUNT; i++) {
    FrGenerator *generator =
        fr_generator_new(FR_DEFAULT_ROUNDS, FR_DEFAULT_SALT);
    fr_generator_seed(generator, FR_SEED_TYPE_RANDOM, NULL, TEST_VECTOR_LEN);

    fr_generator_generate(generator, outputs[i], TEST_VECTOR_LEN);

    fr_generator_free(generator);
  }

  for (int i = 0; i < TESTS_COUNT; i++) {
    for (int j = i + 1; j < TESTS_COUNT; j++) {
      if (memcmp(outputs[i], outputs[j], (long unsigned int)TEST_VECTOR_LEN) ==
          0) {
        printf("Test %d failed\n", i);
        return 1;
      }
    }
  }
  return 0;
}
