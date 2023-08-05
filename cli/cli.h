#pragma once
#include <stdbool.h>
#include <xcmdparser.h>

extern struct g_arg_top {
  bool version;
  bool silent;
} g_arg_top;

extern cmdp_command_st cmdp_generate;
extern cmdp_command_st cmdp_verify;
