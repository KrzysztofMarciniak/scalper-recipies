#ifndef SCALPER_RECIPE_H
#define SCALPER_RECIPE_H
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
// Error flags
#define SCALPER_SUCCESS 0
#define SCALPER_MISSING_DEPENDENCY 1
#define SCALPER_MISSING_BUILD_TOOL 2
#define SCALPER_BUILD_FAILED 4

typedef struct build {
  const char *prefix;
  const char *workdir;
  const char *cachedir;
  int verbose;
  int dry_run;
  int error_flags;
  const char **use_flags;  // NULL-terminated
} Build;
typedef struct recipe {
  const char *name;
  const char *version;
  const char *category;
  const char *source;
  const char *checksum;
  const char *description;
  const char *license;
  const char **depends;      // NULL-terminated
  const char **build_tools;  // NULL-terminated
  const char **use_flags;    // NULL-terminated
  void (*build)(Build *ctx);
} Recipe;
inline void die(const char *fmt, ...) {
  va_list args;
  fprintf(stderr, "scalper: ");
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  fprintf(stderr, "\n");
  exit(1);
}
inline int checkBuildTools(Build *ctx, const char **tools) {
  int all_ok = 1;
  for (int i = 0; tools[i] != NULL; i++) {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "command -v %s > /dev/null 2>&1", tools[i]);
    if (system(cmd) != 0) {
      if (ctx->verbose) {
        printf("Missing build tool: %s\n", tools[i]);
      }
      ctx->error_flags |= SCALPER_MISSING_BUILD_TOOL;
      all_ok = 0;
    }
  }
  return all_ok;
}
inline int checkDependencies(Build *ctx, const char **deps) {
  int all_ok = 1;
  for (int i = 0; deps[i] != NULL; i++) {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "command -v %s > /dev/null 2>&1", deps[i]);
    if (system(cmd) != 0) {
      if (ctx->verbose) {
        printf("Missing dependency: %s\n", deps[i]);
      }
      ctx->error_flags |= SCALPER_MISSING_DEPENDENCY;
      all_ok = 0;
    }
  }
  return all_ok;
}
#endif
