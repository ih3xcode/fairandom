#include <fairandom/fairandom.h>
#include <fairandom/proof.h>
#include <stdio.h>
#include <string.h>

#define TEST_VECTORS_COUNT 4
#define TEST_VECTOR_LEN 64

#define testvec_array(name)                                                    \
  unsigned char name[TEST_VECTORS_COUNT][TEST_VECTOR_LEN]

testvec_array(test_vectors) = {
    {0x69, 0xF7, 0x4D, 0x2A, 0x7E, 0x1A, 0x36, 0x41, 0xA5, 0xA4, 0xFC,
     0x3E, 0x82, 0x5B, 0x6E, 0x52, 0x68, 0xED, 0x54, 0x66, 0x15, 0x28,
     0xCA, 0xBC, 0xEA, 0x11, 0x1A, 0xA3, 0x1D, 0x7E, 0x26, 0x5A, 0x41,
     0xD1, 0xB2, 0xA1, 0x1C, 0x73, 0x51, 0xC2, 0x1F, 0xFB, 0x58, 0xBD,
     0x91, 0x00, 0x70, 0x0D, 0x9F, 0x8C, 0x63, 0x92, 0x23, 0xAC, 0xD4,
     0x4C, 0xEB, 0x6F, 0xEF, 0x15, 0x1C, 0x92, 0xCB, 0x61},
    {0x92, 0x6A, 0x55, 0x6C, 0xA9, 0x6C, 0xE1, 0x59, 0x6D, 0xF9, 0x09,
     0xA1, 0x33, 0xA7, 0xD5, 0x98, 0x58, 0xF1, 0x4F, 0xA2, 0x40, 0x8D,
     0xB4, 0x66, 0xC0, 0x47, 0x81, 0x51, 0x4A, 0xA0, 0xEE, 0x5C, 0x4D,
     0x24, 0x90, 0xB6, 0x29, 0xC6, 0x8A, 0xDC, 0xDC, 0x98, 0x4B, 0x51,
     0x10, 0x09, 0x1C, 0xFB, 0x7E, 0x75, 0x32, 0x9F, 0xD8, 0x2A, 0x5B,
     0x4D, 0xAD, 0x2E, 0xCB, 0xD1, 0x11, 0x0F, 0xA7, 0xDA},
    {0x60, 0x9B, 0x90, 0xF9, 0x46, 0xF6, 0xCD, 0x6D, 0xF0, 0x1E, 0x5B,
     0x26, 0x67, 0x6B, 0x1C, 0x49, 0x6A, 0x82, 0x0D, 0x66, 0xEB, 0x6C,
     0x1A, 0xD6, 0x67, 0x86, 0x80, 0x8F, 0x0C, 0x1E, 0x35, 0x64, 0x96,
     0x4B, 0x46, 0xC8, 0x3F, 0xD6, 0x33, 0x03, 0x0A, 0x68, 0xB8, 0xFC,
     0x34, 0xB0, 0xB2, 0x25, 0x0F, 0xE1, 0xFA, 0x64, 0xB5, 0xF5, 0x25,
     0xD8, 0xC3, 0x37, 0x9D, 0x2F, 0xE2, 0x34, 0x06, 0x49},
    {0x48, 0x98, 0x51, 0x77, 0x3F, 0x8A, 0x6A, 0x51, 0xCD, 0xDC, 0x4B,
     0x39, 0x66, 0x9A, 0x82, 0x23, 0x63, 0xF6, 0xA2, 0x7A, 0x74, 0x04,
     0x72, 0xE5, 0x59, 0x86, 0xA9, 0xB4, 0x1B, 0xFA, 0xFB, 0x58, 0x93,
     0x27, 0x4C, 0x5B, 0xE9, 0x3E, 0x71, 0x5F, 0x83, 0x0E, 0x59, 0x31,
     0x66, 0x35, 0xE3, 0xF0, 0x03, 0xD3, 0x9A, 0x4E, 0xBF, 0xC3, 0x20,
     0x10, 0x6B, 0xE5, 0x39, 0x2D, 0x79, 0xE7, 0xA4, 0x9F}};

char test_vector_proofs[TEST_VECTORS_COUNT][FR_PROOF_STRING_LEN] = {
    "a0860100000102030405060708090a0b0c0d0e0f605d661ebd5cb1e75ead7542f6f96c43ed"
    "9c9303f38ae91067593e4050625f81",
    "a0860100000102030405060708090a0b0c0d0e0f18ba2f20e5bffa9aa83be2a3364096283a"
    "45a3e253f0b9e8481b3116523fd39b",
    "a0860100000102030405060708090a0b0c0d0e0f30f8d432d78a30d21f5458436c1f29bc78"
    "e3ac09d82bf6e75e7ed5029e7d8de9",
    "a0860100000102030405060708090a0b0c0d0e0fccb79b0b0aed0d283a351b693c6d51c4fa"
    "d540bf3d02e6bf1f671a2ca739b390"};

int main(void) {
  for (int i = 0; i < TEST_VECTORS_COUNT; i++) {
    FrGenerator *generator =
        fr_generator_new(FR_DEFAULT_ROUNDS, FR_DEFAULT_SALT);
    fr_generator_seed(generator, FR_SEED_TYPE_STRING,
                      (fr_bytes_t)test_vectors[i], TEST_VECTOR_LEN);

    struct FrProof proof;
    fr_generate_proof(generator, &proof);

    unsigned char proof_string[FR_PROOF_STRING_LEN];
    fr_proof_as_string(&proof, (fr_bytes_t)proof_string);

    fr_generator_free(generator);

    if (strcmp((char *)proof_string, test_vector_proofs[i]) != 0) {
      printf("Test %d failed\n", i);
      return 1;
    }
  }
  return 0;
}
