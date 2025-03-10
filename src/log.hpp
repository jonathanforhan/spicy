#pragma once

#include <cstdio>

#define LOG(_Msg, ...) (void)fprintf(stdout, _Msg "\n", ##__VA_ARGS__)

#ifndef NDEBUG
#define DEBUG(_Msg, ...) (void)fprintf(stdout, _Msg "\n", ##__VA_ARGS__)
#else
#define DEBUG(...) (void)0
#endif

#define ERROR(_Msg, ...) (void)fprintf(stderr, "[ERROR] " _Msg "\n", ##__VA_ARGS__)
