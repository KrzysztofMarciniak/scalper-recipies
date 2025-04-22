#include <stdio.h>
#include <stdlib.h>

#include "scalper-recipe.h"
static const char *deps[]        = {"make", "gcc", NULL};
static const char *build_tools[] = {"curl", "tar", "make", NULL};
void buildHello(Build *ctx) {
  if (!checkBuildTools(ctx, build_tools)) {
    die("Missing required build tools.");
  }
  if (!checkDependencies(ctx, deps)) {
    die("Missing required runtime dependencies.");
  }
  const char *name    = "hello";
  const char *version = "2.10";
  const char *source  = "https://ftp.gnu.org/gnu/hello/hello-2.10.tar.gz";
  printf("Building %s %s...\n", name, version);
  char cmd[512];
  snprintf(cmd, sizeof(cmd), "curl -LO %s", source);
  if (system(cmd) != 0) die("Failed to fetch source.");
  snprintf(cmd, sizeof(cmd), "tar xzf hello-%s.tar.gz", version);
  if (system(cmd) != 0) die("Failed to extract source.");
  snprintf(cmd, sizeof(cmd), "cd hello-%s && ./configure --prefix=%s && make && make install",
           version, ctx->prefix);
  if (!ctx->dry_run && system(cmd) != 0) {
    ctx->error_flags |= SCALPER_BUILD_FAILED;
    die("Build/install process failed.");
  }
  printf("%s %s built and installed to %s\n", name, version, ctx->prefix);
}
Recipe helloRecipe = {
    .name        = "hello",
    .version     = "2.10",
    .category    = "core",
    .source      = "https://ftp.gnu.org/gnu/hello/hello-2.10.tar.gz",
    .checksum    = NULL,
    .description = "GNU Hello — classic greeting program",
    .license     = "GPL-3.0",
    .depends     = deps,
    .build_tools = build_tools,
    .use_flags   = NULL,
    .build       = buildHello,
};
