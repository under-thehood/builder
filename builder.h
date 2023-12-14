#ifndef _BUILDER_BUILDER_H_
#define _BUILDER_BUILDER_H_

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

#include <stdio.h>
#include "./dirent.h"

typedef struct _Builder Builder;
typedef struct _FilePaths FilePaths;

typedef bool FileFilterFunction(char *filePath);

Builder *builder_create();

bool builder_add_compiler(Builder *builder, char const *compiler);

bool builder_add_compiler_flag(Builder *builder, char const *compilerFlag);

bool builder_build(Builder *builder);

void builder_add_file_path(Builder *builder, FilePaths *filePaths);

bool builder_add_file(Builder *builder, char *fileName);

FilePaths *find_all_file_in_dir(const char *dirname);

FilePaths *find_files_with_filter(const char *dirname, FileFilterFunction filterFunction);

//[INFO] : FOR debug purpose only
void display_file_path_list(FilePaths *files);
void builder_display_compiler_command(Builder *builder);

#endif