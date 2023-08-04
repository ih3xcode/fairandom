#include <errno.h>
#include <fairandom/fairandom.h>
#include <fairandom/hex.h>
#include <stdlib.h>
#include <string.h>
#include <xcmdparser.h>

#include "cli.h"

struct {
  bool random_seed;
  int rounds;
  char *salt;
} arg_generate;

static cmdp_action_t cb_generate(cmdp_process_param_st *params);

cmdp_command_st cmdp_generate = {
    "generate",
    "Generate new provable randomness",
    "usage: fairandom-cli [<options>] generate [--random-seed] [--salt "
    "<salt_hex>]\n"
    "                                          <length> [input_file]\n"
    "                                          \n",
    (cmdp_option_st[]){
        {'r', "random-seed", "Use random seed", CMDP_TYPE_BOOL,
         &arg_generate.random_seed},
        {'R', "rounds", "Number of rounds", CMDP_TYPE_INT4,
         &arg_generate.rounds, "<rounds>"},
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
    length = atoi(params->argv[0]);
    input_file = NULL;
  } else if (params->argc == 2) {
    length = atoi(params->argv[0]);
    input_file = params->argv[1];
  } else {
    fprintf(stderr, "fatal: too many arguments\n\n");
    return CMDP_ACT_ERROR | CMDP_ACT_SHOW_HELP;
  }

  if (arg_generate.random_seed == (input_file != NULL)) {
    fprintf(stderr,
            "fatal: you must specify either --random-seed or input_file\n\n");
    return CMDP_ACT_ERROR | CMDP_ACT_SHOW_HELP;
  }

  char *salt = malloc(FR_SALT_LEN);
  if (salt == NULL) {
    fprintf(stderr, "malloc(): %s\n", (char *)strerror(errno));
    return CMDP_ACT_ERROR;
  }

  if (arg_generate.salt != NULL) {
    if (strlen(arg_generate.salt) != HEX_LEN(FR_SALT_LEN)) {
      fprintf(stderr, "fatal: salt must be %d bytes in hex format\n\n",
              FR_SALT_LEN);
      return CMDP_ACT_ERROR | CMDP_ACT_SHOW_HELP;
    }
    fr_hex_to_bytes(arg_generate.salt, FR_SALT_LEN, salt);
  } else {
    memcpy(salt, FR_DEFAULT_SALT, FR_SALT_LEN);
  }

  if (!arg_top.silent) {
    fprintf(stderr, "Generating %d bytes of randomness using %s\n", length,
            arg_generate.random_seed ? "random seed" : "input file");
    char *hex_salt = malloc(HEX_LEN_WITH_NULL(FR_SALT_LEN));
    fr_bytes_to_hex(salt, FR_SALT_LEN, hex_salt);
    fprintf(stderr, "salt: %s\n", hex_salt);
    free(hex_salt);
    fprintf(stderr, "rounds: %d\n", arg_generate.rounds);
  }

  fr_generator_t *generator = fr_generator_new(arg_generate.rounds, salt);
  if (generator == NULL) {
    fprintf(stderr, "fr_generator_new(): %s\n", (char *)strerror(errno));
    return CMDP_ACT_ERROR;
  }

  if (arg_generate.random_seed) {
    fr_generator_seed(generator, FR_SEED_TYPE_RANDOM, NULL, length);
  } else {
    FILE *fp = fopen(input_file, "rb");
    if (fp == NULL) {
      fprintf(stderr, "fopen(): %s\n", (char *)strerror(errno));
      return CMDP_ACT_ERROR;
    }

    fseek(fp, 0, SEEK_END);
    size_t input_file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (input_file_size < length) {
      fprintf(stderr, "fatal: input file must be at least %d bytes in length\n",
              length);
      return CMDP_ACT_ERROR;
    }

    char *input = malloc(length);
    if (input == NULL) {
      fprintf(stderr, "malloc(): %s\n", (char *)strerror(errno));
      return CMDP_ACT_ERROR;
    }

    if (fread(input, 1, length, fp) != length) {
      fprintf(stderr, "fread(): %s\n", (char *)strerror(errno));
      return CMDP_ACT_ERROR;
    }

    fclose(fp);

    fr_generator_seed(generator, FR_SEED_TYPE_STRING, input, length);
    free(input);
  }

  char *output = malloc(length);
  if (output == NULL) {
    fprintf(stderr, "malloc(): %s\n", (char *)strerror(errno));
    return CMDP_ACT_ERROR;
  }

  fr_generate(generator, output, length);

  char *hex_output = malloc(HEX_LEN_WITH_NULL(length));
  fr_bytes_to_hex(output, length, hex_output);
  free(output);

  fprintf(stdout, "%s\n", hex_output);
  free(hex_output);
  return CMDP_ACT_OK;
}