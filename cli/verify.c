#include <errno.h>
#include <fairandom/fairandom.h>
#include <fairandom/hex.h>
#include <fairandom/proof.h>
#include <string.h>
#include <xcmdparser.h>

#include "cli.h"

static struct {
  bool hex;
} arg_verify;

static cmdp_action_t cb_verify(cmdp_process_param_st *params);

cmdp_command_st cmdp_verify = {
    "verify",
    "Verify fairandom values. This command is assuming that you have "
    "{input_file} along with {input_file}.proof and {input_file}.proof.seed"
    " in the same directory.",
    "usage: fairandom-cli [<options>] verify [--hex] <input_file>\n"
    "\n\n",
    .options =
        (cmdp_option_st[]){
            {'x', "hex", "Input in hex format", CMDP_TYPE_BOOL,
             &arg_verify.hex},
            {0},
        },
    .fn_process = cb_verify,
};

static cmdp_action_t cb_verify(cmdp_process_param_st *params) {
  if (params->argc == 0) {
    fprintf(stderr, "fatal: You must specify an input file.\n\n");
    return CMDP_ACT_ERROR | CMDP_ACT_SHOW_HELP;
  }

  char *input_file = NULL;

  if (params->argc == 1) {
    input_file = params->argv[0];
  } else {
    fprintf(stderr, "fatal: too many arguments\n\n");
    return CMDP_ACT_ERROR | CMDP_ACT_SHOW_HELP;
  }

  FILE *fp = fopen(input_file, "rb");
  if (fp == NULL) {
    fprintf(stderr, "fopen(): %s\n", (fr_bytes_t)strerror(errno));
    return CMDP_ACT_ERROR;
  }

  fseek(fp, 0, SEEK_END);
  size_t data_file_size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  fr_bytes_t data = malloc(data_file_size);
  if (data == NULL) {
    fprintf(stderr, "malloc(): %s\n", (fr_bytes_t)strerror(errno));
    return CMDP_ACT_ERROR;
  }

  if (fread(data, 1, data_file_size, fp) != data_file_size) {
    fprintf(stderr, "fread(): %s\n", (fr_bytes_t)strerror(errno));
    return CMDP_ACT_ERROR;
  }
  fclose(fp);

  FILE *proof_fp = fopen(strcat(input_file, ".proof"), "rb");
  if (proof_fp == NULL) {
    fprintf(stderr, "fopen(): %s\n", (fr_bytes_t)strerror(errno));
    return CMDP_ACT_ERROR;
  }

  fseek(proof_fp, 0, SEEK_END);
  size_t proof_file_size = ftell(proof_fp);
  fseek(proof_fp, 0, SEEK_SET);

  if (proof_file_size !=
      FR_PROOF_STRING_LEN - 1) {  // Proof string in file does
                                  // not include null-terminator
    fprintf(stderr, "fatal: proof file must be %lu bytes in length\n",
            FR_PROOF_STRING_LEN);
    return CMDP_ACT_ERROR;
  }

  fr_bytes_t proof_data = malloc(proof_file_size);
  if (proof_data == NULL) {
    fprintf(stderr, "malloc(): %s\n", (fr_bytes_t)strerror(errno));
    return CMDP_ACT_ERROR;
  }

  if (fread(proof_data, 1, proof_file_size, proof_fp) != proof_file_size) {
    fprintf(stderr, "fread(): %s\n", (fr_bytes_t)strerror(errno));
    return CMDP_ACT_ERROR;
  }

  fclose(proof_fp);

  FILE *seed_fp = fopen(strcat(input_file, ".seed"), "rb");
  if (seed_fp == NULL) {
    fprintf(stderr, "fopen(): %s\n", (fr_bytes_t)strerror(errno));
    return CMDP_ACT_ERROR;
  }

  fseek(seed_fp, 0, SEEK_END);
  size_t seed_file_size = ftell(seed_fp);
  fseek(seed_fp, 0, SEEK_SET);

  fr_bytes_t seed_data = malloc(seed_file_size);
  if (seed_data == NULL) {
    fprintf(stderr, "malloc(): %s\n", (fr_bytes_t)strerror(errno));
    return CMDP_ACT_ERROR;
  }

  if (fread(seed_data, 1, seed_file_size, seed_fp) != seed_file_size) {
    fprintf(stderr, "fread(): %s\n", (fr_bytes_t)strerror(errno));
    return CMDP_ACT_ERROR;
  }

  fclose(seed_fp);

  if (arg_verify.hex) {
    fr_bytes_t data_raw = malloc(data_file_size / 2);
    Fr_HexToBytes(data, data_file_size, data_raw);
    data_file_size /= 2;
    free(data);
    data = data_raw;

    fr_bytes_t seed_raw = malloc(seed_file_size / 2);
    Fr_HexToBytes(seed_data, seed_file_size, seed_raw);
    seed_file_size /= 2;
    free(seed_data);
    seed_data = seed_raw;
  }

  if (seed_file_size < data_file_size) {
    fprintf(stderr, "fatal: seed file must be at least %lu bytes in length\n",
            data_file_size);
    return CMDP_ACT_ERROR;
  }

  struct FrProof proof;
  fr_proof_from_string(&proof, proof_data);
  free(proof_data);

  bool result =
      fr_verify_proof(seed_data, seed_file_size, data, data_file_size, proof);

  free(data);
  free(seed_data);

  if (result) {
    if (!g_arg_top.silent) printf("OK\n");
    return CMDP_ACT_OK;
  } else {
    if (!g_arg_top.silent) printf("FAIL\n");
    return CMDP_ACT_ERROR;
  }
}
