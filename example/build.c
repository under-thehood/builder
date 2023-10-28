#include "../builder.h"

bool is_suffix_with(char *strToCheck, char *suffix)
{
    size_t suffixLen = strlen(suffix);

    size_t strToCheckLen = strlen(strToCheck);

    if (strToCheckLen <= suffixLen)
        return false;

    return strcmp(strToCheck + (strToCheckLen - suffixLen), suffix) == 0;
}

bool filter_c_file(char *file)
{
    // bool status = true;

    // size_t fileSize = strlen(file);

    // if (fileSize <= fileExtensionLen)
    //     return false;

    // return strcmp(file + (fileSize - fileExtensionLen), fileExtension) == 0;
    return is_suffix_with(file, ".c");
}

int main(int argc, char const *argv[])
{
    Builder *builder = builder_create();

    if (builder == NULL)
    {
        printf("cannot create the builder\n");
        return 1;
    }

    // builder_add_compiler(builder, "gcc");
    // builder_add_compiler_flag(builder, "-c");

    // builder_add_file_path(builder, "build.c");

    // builder_build(builder);

    FilePaths *files = find_files_with_filter("./", filter_c_file);
    builder_add_file_path(builder, files);

    builder_add_file(builder, "example/build.c");

    if (files == NULL)
        return 1;

    // display_file_path_list();

    // display_file_path_list(files);

    return 0;
}
