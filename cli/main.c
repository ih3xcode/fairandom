#include <xcmdparser.h>

#include "cli.h"

struct g_arg_top g_arg_top;

static cmdp_action_t cb_top_level(cmdp_process_param_st *params);

static cmdp_command_st cmdp_top = {
    .doc =
        "Usage: fairandom-cli [--version] [--help]\n"
        "                     [--silent] <command> [<args>]\n"
        "\n",
    .options =
        (cmdp_option_st[]){
            {'v', "version", "Show version", CMDP_TYPE_BOOL,
             &g_arg_top.version},
            {'s', "silent", "Silent mode", CMDP_TYPE_BOOL, &g_arg_top.silent},
            {0},
        },
    .sub_commands =
        (cmdp_command_st *[]){
            &cmdp_generate,
            &cmdp_verify,
            NULL,
        },
    .fn_process = cb_top_level,
};

int main(int argc, char **argv) {
  return cmdp_run(argc - 1, argv + 1, &cmdp_top, NULL);
}

static cmdp_action_t cb_top_level(cmdp_process_param_st *params) {
  if (g_arg_top.version) {
    printf("fairandom-cli version 0.1.0\n");
    return CMDP_ACT_OK;
  } else if (params->next == NULL) {
    return CMDP_ACT_SHOW_HELP;
  }
  return CMDP_ACT_CONTINUE;
}
