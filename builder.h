#ifndef _BUILDER_BUILDER_H_
#define _BUILDER_BUILDER_H_

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#include <stdio.h>

typedef struct _Builder Builder;

Builder *builder_create();

bool builder_add_compiler(Builder *builder, char const *compiler);

bool builder_add_compiler_flag(Builder *builder, char const *compilerFlag);

bool builder_build(Builder *builder);

bool builder_add_file_path(Builder *builder, char *filePath);

//[INFO] : FOR debug purpose only
void display_file_path_list(Builder *builder);

void builder_display_compiler_command(Builder *builder);

#endif