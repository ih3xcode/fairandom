#include <errno.h>
#include <fairandom/fairandom.h>
#include <fairandom/hex.h>
#include <fairandom/proof.h>
#include <fairandom/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcmdparser.h>

#include "cli.h"

static struct {
  bool random_seed;
  int rounds;
  char *output;
  bool hex;
  char *salt;
} arg_generate;

static cmdp_action_t cb_generate(cmdp_process_param_st *params);

cmdp_command_st cmdp_generate = {
    "generate",
    "Generate new provable randomness",
    "usage: fairandom-cli [<options>] generate [--random-seed] [--salt "
    "<salt_hex>] [--rounds <rounds]\n"
    "                                          [--output "
    "<output_file>] [--hex] "
    "<length> [input_file]\n"
    "                                          \n",
    (cmdp_option_st[]){
        {'r', "random-seed", "Use random seed", CMDP_TYPE_BOOL,
         &arg_generate.random_seed},
        {'R', "rounds", "Number of rounds", CMDP_TYPE_INT4,
         &arg_generate.rounds, "<rounds>"},
        {'o', "output", "Output file", CMDP_TYPE_STRING_PTR,
         &arg_generate.output, "<output_file>"},
        {'x', "hex", "Output in hex format", CMDP_TYPE_BOOL, &arg_generate.hex},
        {'s', "salt", "16 bytes of salt in hex format", CMDP_TYPE_STRING_PTR,
         &arg_generate.salt, "<salt_hex>"},
        {0},
    },
    .fn_process = cb_generate,
};

static cmdp_action_t cb_generate(cmdp_process_param_st *params) {
  if (params->argc == 0) {
    fprintf(stderr, "fatal: You must specify a length.\n\n");
    return CMDP_ACT_ERROR | CMDP_ACT_SHOW_HELP;
  }

  int length = 0;
  char *input_file = NULL;

  if (params->argc == 1) {
    length = (int)strtol(params->argv[0], NULL, INT_BASE);
    input_file = NULL;
  } else if (params->argc == 2) {
    length = (int)strtol(params->argv[0], NULL, INT_BASE);
    input_file = params->argv[1];
  } else {
    fprintf(stderr, "fatal: too many arguments\n\n");
    return CMDP_ACT_ERROR | CMDP_ACT_SHOW_HELP;
  }

  if (arg_generate.rounds == 0) {
    arg_generate.rounds = FR_DEFAULT_ROUNDS;
  }

  if (arg_generate.random_seed == (input_file != NULL)) {
    fprintf(stderr,
            "fatal: you must specify either --random-seed or input_file\n\n");
    return CMDP_ACT_ERROR | CMDP_ACT_SHOW_HELP;
  }

  fr_bytes_t salt = malloc(FR_SALT_LEN);
  if (salt == NULL) {
    fprintf(stderr, "malloc(): %s\n", (fr_bytes_t)strerror(errno));
    return CMDP_ACT_ERROR;
  }

  if (arg_generate.salt != NULL) {
    if (strlen(arg_generate.salt) != HEX_LEN(FR_SALT_LEN)) {
      fprintf(stderr, "fatal: salt must be %d bytes in hex format\n\n",
              FR_SALT_LEN);
      free(salt);
      return CMDP_ACT_ERROR | CMDP_ACT_SHOW_HELP;
    }
    Fr_HexToBytes((fr_bytes_t)arg_generate.salt, FR_SALT_LEN, salt);
  } else {
    memcpy_s(salt, FR_SALT_LEN, FR_DEFAULT_SALT, FR_SALT_LEN);
  }

  if (!g_arg_top.silent) {
    fprintf(stderr, "Generating %d bytes of randomness using %s\n", length,
            arg_generate.random_seed ? "random seed" : "input file");
    fr_bytes_t hex_salt = malloc(HEX_LEN_WITH_NULL(FR_SALT_LEN));
    Fr_BytesToHex(salt, FR_SALT_LEN, hex_salt);
    fprintf(stderr, "salt: %s\n", hex_salt);
    free(hex_salt);
    fprintf(stderr, "rounds: %d\n", arg_generate.rounds);
  }

  FrGenerator *generator = fr_generator_new(arg_generate.rounds, salt);
  if (generator == NULL) {
    fprintf(stderr, "fr_generator_new(): %s\n", (fr_bytes_t)strerror(errno));
    return CMDP_ACT_ERROR;
  }
  free(salt);

  size_t seed_len = FR_FULL_BLOCK_LEN(length);
  if (arg_generate.random_seed) {
    fr_generator_seed(generator, FR_SEED_TYPE_RANDOM, NULL, seed_len);
  } else {
    FILE *input_fp = fopen(input_file, "rb");
    if (input_fp == NULL) {
      fprintf(stderr, "fopen(): %s\n", (fr_bytes_t)strerror(errno));
      return CMDP_ACT_ERROR;
    }

    fseek(input_fp, 0, SEEK_END);
    size_t input_file_size = ftell(input_fp);
    fseek(input_fp, 0, SEEK_SET);

    if (input_file_size < seed_len) {
      fprintf(stderr,
              "fatal: input file must be at least %zu bytes in length\n",
              seed_len);
      fclose(input_fp);
      return CMDP_ACT_ERROR;
    }

    fr_bytes_t input = malloc(seed_len);
    if (input == NULL) {
      fprintf(stderr, "malloc(): %s\n", (fr_bytes_t)strerror(errno));
      fclose(input_fp);
      return CMDP_ACT_ERROR;
    }

    if (fread(input, 1, seed_len, input_fp) != seed_len) {
      fprintf(stderr, "fread(): %s\n", (fr_bytes_t)strerror(errno));
      fclose(input_fp);
      free(input);
      return CMDP_ACT_ERROR;
    }

    fclose(input_fp);

    fr_generator_seed(generator, FR_SEED_TYPE_STRING, input, seed_len);
    free(input);
  }

  fr_bytes_t randomness = malloc(length);
  if (randomness == NULL) {
    fprintf(stderr, "malloc(): %s\n", (fr_bytes_t)strerror(errno));
    return CMDP_ACT_ERROR;
  }

  Fr_Generate(generator, randomness, length);

  struct FrProof proof;
  fr_generate_proof(generator, &proof);

  fr_bytes_t str_proof = malloc(FR_PROOF_STRING_LEN);
  if (str_proof == NULL) {
    fprintf(stderr, "malloc(): %s\n", (fr_bytes_t)strerror(errno));
    return CMDP_ACT_ERROR;
  }

  fr_proof_as_string(&proof, str_proof);

  fr_bytes_t actual_seed = malloc(seed_len);
  if (actual_seed == NULL) {
    fprintf(stderr, "malloc(): %s\n", (fr_bytes_t)strerror(errno));
    return CMDP_ACT_ERROR;
  }

  fr_generator_getseed(generator, actual_seed);

  fr_generator_free(generator);

  if (arg_generate.output != NULL) {
    FILE *output_fp = fopen(arg_generate.output, "wb");
    if (output_fp == NULL) {
      fprintf(stderr, "fopen(): %s\n", (fr_bytes_t)strerror(errno));
      return CMDP_ACT_ERROR;
    }

    if (arg_generate.hex) {
      fr_bytes_t hex_output = malloc(HEX_LEN_WITH_NULL(length));
      if (hex_output == NULL) {
        fprintf(stderr, "malloc(): %s\n", (fr_bytes_t)strerror(errno));
        return CMDP_ACT_ERROR;
      }
      Fr_BytesToHex(randomness, length, hex_output);
      fprintf(output_fp, "%s", hex_output);
      free(hex_output);
    } else {
      fwrite(randomness, 1, length, output_fp);
    }

    if (!g_arg_top.silent) {
      fprintf(stderr, "Output written to %s\n", arg_generate.output);
    }

    fclose(output_fp);

    FILE *proof_fp = fopen(strcat(arg_generate.output, ".proof"), "wb");
    if (proof_fp == NULL) {
      fprintf(stderr, "fopen(): %s\n", (fr_bytes_t)strerror(errno));
      return CMDP_ACT_ERROR;
    }
    fprintf(proof_fp, "%s", str_proof);
    fclose(proof_fp);

    if (!g_arg_top.silent) {
      fprintf(stderr, "Proof written to %s\n", arg_generate.output);
    }

    FILE *seed_fp = fopen(strcat(arg_generate.output, ".seed"), "wb");
    if (seed_fp == NULL) {
      fprintf(stderr, "fopen(): %s\n", (fr_bytes_t)strerror(errno));
      return CMDP_ACT_ERROR;
    }

    if (arg_generate.hex) {
      fr_bytes_t hex_seed = malloc(HEX_LEN_WITH_NULL(seed_len));
      if (hex_seed == NULL) {
        fprintf(stderr, "malloc(): %s\n", (fr_bytes_t)strerror(errno));
        return CMDP_ACT_ERROR;
      }
      Fr_BytesToHex(actual_seed, seed_len, hex_seed);
      fprintf(seed_fp, "%s", hex_seed);
      free(hex_seed);
    } else {
      fwrite(actual_seed, 1, seed_len, seed_fp);
    }
    if (!g_arg_top.silent) {
      fprintf(stderr, "Seed written to %s\n", arg_generate.output);
    }

    fclose(seed_fp);
  } else {
    if (arg_generate.hex) {
      fr_bytes_t hex_output = malloc(HEX_LEN_WITH_NULL(length));
      if (hex_output == NULL) {
        fprintf(stderr, "malloc(): %s\n", (fr_bytes_t)strerror(errno));
        return CMDP_ACT_ERROR;
      }
      Fr_BytesToHex(randomness, length, hex_output);
      printf("%s\n", hex_output);
      free(hex_output);
    } else {
      fwrite(randomness, 1, length, stdout);
    }
    printf("%s\n", str_proof);
    if (arg_generate.hex) {
      fr_bytes_t hex_seed = malloc(HEX_LEN_WITH_NULL(seed_len));
      if (hex_seed == NULL) {
        fprintf(stderr, "malloc(): %s\n", (fr_bytes_t)strerror(errno));
        return CMDP_ACT_ERROR;
      }
      Fr_BytesToHex(actual_seed, seed_len, hex_seed);
      printf("%s\n", hex_seed);
      free(hex_seed);
    } else {
      fwrite(actual_seed, 1, seed_len, stdout);
    }
  }

  free(randomness);
  free(str_proof);
  free(actual_seed);

  return CMDP_ACT_OK;
}
