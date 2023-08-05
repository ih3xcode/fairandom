#pragma once
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdbool.h>
#include <string.h>
#include <xcmdparser.h>

#define INT_BASE 10

extern struct g_arg_top {
  bool version;
  bool silent;
} g_arg_top;

extern cmdp_command_st cmdp_generate;
extern cmdp_command_st cmdp_verify;

#ifndef __STDC_LIB_EXT1__
// memcpy_s is not available on some platforms
// define memcpy_s as memcpy and ignore clang-tidy warning
#define memcpy_s(dest, destsz, src, count) memcpy(dest, src, count) // NOLINT
#endif
