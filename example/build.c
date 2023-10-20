#include "../builder.h"

int main(int argc, char const *argv[])
{
    Builder *builder = builder_create();

    if (builder == NULL)
    {
        printf("cannot create the builder\n");
        return 1;
    }

    builder_add_compiler(builder, "gcc");
    builder_add_compiler_flag(builder, "-c");

    builder_add_file_path(builder, "build.c");

    builder_build(builder);
    return 0;
}
