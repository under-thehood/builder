#include "builder.h"

#define INITIAL_COMMAND_SIZE 12
#define INITIAL_SUBBUILDERLIST_SIZE 3
#define INITIAL_FILEPATH_LIST_SIZE 5

typedef struct
{
    char *_command;
    size_t size;
    size_t capacity;
} StringStream;

typedef struct
{
    char **filepaths;
    size_t size;
    size_t capacity;
} FilePaths;

typedef struct _Builder
{
    StringStream command;
    FilePaths files;
} Builder;

Builder *builder_create()
{
    Builder *builder = (Builder *)malloc(sizeof(Builder));
    builder->command.size = 0;
    builder->files.size = 0;

    builder->command._command = (char *)malloc(INITIAL_COMMAND_SIZE * sizeof(char));
    if (builder->command._command == NULL)
        return NULL;
    builder->command.capacity = INITIAL_COMMAND_SIZE;

    builder->files.filepaths = (char **)malloc(sizeof(char *) * INITIAL_FILEPATH_LIST_SIZE);
    if (builder->files.filepaths == NULL)
        return NULL;

    builder->files.capacity = INITIAL_FILEPATH_LIST_SIZE;

    return builder;
}

static void *check_and_regrow(char *dest, size_t *destSize, size_t *destCapacity, size_t srcSize)
{
    if (*destCapacity <= *destSize + srcSize)
    {
        dest = (char *)realloc(dest, *destCapacity * 2);
        if (dest == NULL)
            return NULL;
        *destCapacity = *destCapacity * 2;
    }
    return dest;
}

// For internal purpose only
static void *add_data_helper(char *dest, size_t *destSize, size_t *destCapacity, const char *src, size_t srcSize)
{

    dest = check_and_regrow(dest, destSize, destCapacity, srcSize);

    if (dest == NULL)
        return NULL;

    memcpy(dest + *destSize, src, srcSize);

    *destSize = *destSize + srcSize + 1;

    dest[*destSize - 1] = ' ';

    dest[*destSize] = '\0';

    return dest;
}

bool builder_add_compiler(Builder *builder, const char *compiler)
{
    assert(builder->command.size == 0);
    if (add_data_helper(builder->command._command, &(builder->command.size), &(builder->command.capacity), compiler, strlen(compiler)) == NULL)
        return false;
    return true;
}

bool builder_add_compiler_flag(Builder *builder, char const *compilerFlag)
{
    if (add_data_helper(builder->command._command, &(builder->command.size), &(builder->command.capacity), compilerFlag, strlen(compilerFlag)) == NULL)
        return false;
    return true;
}

bool builder_add_file_path(Builder *builder, char *filePath)
{

    builder->files.filepaths = (char **)check_and_regrow(*builder->files.filepaths, &(builder->files.size), &(builder->files.capacity), sizeof(filePath));

    if (builder->files.filepaths == NULL)
        return false;

    builder->files.filepaths[builder->files.size] = filePath;
    builder->files.size++;
    return true;
}

void display_file_path_list(Builder *builder)
{
    for (size_t i = 0; i < builder->files.size; i++)
    {
        printf("%s\n", builder->files.filepaths[i]);
    }
}

bool builder_build(Builder *builder)
{
    size_t fileCount = builder->files.size;
    const size_t compilerCommandSize = builder->command.size;

    char **fileArray = builder->files.filepaths;

    for (size_t i = 0; i < fileCount; i++)
    {
        builder->command._command = add_data_helper(builder->command._command, &(builder->command.size), &(builder->command.capacity), fileArray[i], strlen(fileArray[i]));

        if (builder->command._command == NULL)
        {
            fprintf(stderr, "[Error]: cannot build the file %s", fileArray[i]);
            return false;
        }

        system(builder->command._command);

        builder->command.size = compilerCommandSize;
    }
}

void builder_display_compiler_command(Builder *builder)
{
    printf(builder->command._command);
}
