#include "xcmdparser.h"

static cmdp_action_t cb_verify(cmdp_process_param_st *params);

cmdp_command_st cmdp_verify = {
    "verify",
    "Verify fairandom values",
    "usage: fairandom-cli verify ...\n\n",
    .fn_process = cb_verify,
};

static cmdp_action_t cb_verify(cmdp_process_param_st *params) {
  printf("cb_pull called.\n");
  return CMDP_ACT_OK;
}