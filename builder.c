#include "./builder.h"

#define INITIAL_COMMAND_SIZE 12
#define INITIAL_SUBBUILDERLIST_SIZE 3
#define INITIAL_FILEPATH_LIST_SIZE 5

typedef struct
{
    char *_command;
    size_t size;
    size_t capacity;
} StringStream;

typedef struct _FilePaths
{
    char **filepaths;
    size_t size;
    size_t capacity;
} FilePaths;

typedef struct _Builder
{
    StringStream command;
    FilePaths *files;
} Builder;

static void *check_and_regrow(char *dest, size_t *destSize, size_t *destCapacity, size_t srcSize)
{
    if (*destCapacity <= *destSize + srcSize)
    {

        if (*destCapacity * 2 > *destSize + srcSize)
            dest = (char *)realloc(dest, *destCapacity * 2);
        else
            dest = (char *)realloc(dest, *destCapacity + srcSize + 2);

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

bool string_stream_init(StringStream *stream)
{
    stream->size = 0;

    stream->_command = (char *)malloc(INITIAL_COMMAND_SIZE * sizeof(char));
    if (stream->_command == NULL)
        return false;
    stream->capacity = INITIAL_COMMAND_SIZE;
    return true;
}

bool file_paths_init(FilePaths *files)
{
    files->size = 0;

    files->filepaths = (char **)malloc(sizeof(char *) * INITIAL_FILEPATH_LIST_SIZE);
    if (files->filepaths == NULL)
        return false;

    files->capacity = INITIAL_FILEPATH_LIST_SIZE;
    return true;
}

FilePaths *file_paths_create()
{
    FilePaths *files = (FilePaths *)malloc(1 * sizeof(FilePaths));
    if (files == NULL)
        return NULL;

    file_paths_init(files);
    return files;
}

bool string_stream_add_str(StringStream *stream, char const *str)
{
    if (add_data_helper(stream->_command, &(stream->size), &(stream->capacity), str, strlen(str)) == NULL)
        return false;
    return true;
}

bool string_stream_reserve(StringStream *stream, size_t size)
{
    assert(stream->capacity == 0 && "Stream should be initialized with zero value");

    stream->_command = (char *)malloc(size * sizeof(char));
    if (stream->_command == NULL)
        return false;
    stream->capacity = size;

    return true;
}

char *string_new(char *str)
{
    size_t len = strlen(str);

    StringStream stream = {0};

    string_stream_reserve(&stream, len + 2);

    string_stream_add_str(&stream, str);

    return stream._command;
}

bool file_paths_add(FilePaths *filePaths, char *filePath)
{
    filePaths->filepaths = (char **)check_and_regrow(filePaths->filepaths, &(filePaths->size), &(filePaths->capacity), sizeof(filePath));

    if (filePaths->filepaths == NULL)
        return false;

    filePaths->filepaths[filePaths->size] = filePath;
    filePaths->size++;
    return true;
}

bool always_return_true(char *filePath) { return true; }

FilePaths *find_files_with_filter(const char *dirname, FileFilterFunction filterFunction)
{
    FilePaths *files = file_paths_create();

    if (files == NULL)
        return NULL;

    /* Open directory stream */
    DIR *dir = opendir(dirname);
    if (!dir)
    {
        /* Could not open directory */
        fprintf(stderr,
                "Cannot open %s (%s)\n", dirname, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* append filtered file within the directory */
    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL)
    {
        if (ent->d_type == DT_REG && filterFunction(ent->d_name))
        {
            file_paths_add(files, string_new(ent->d_name));
        }
    }
    closedir(dir);

    return files;
}

FilePaths *find_all_file_in_dir(const char *dirname)
{
    return find_files_with_filter(dirname, always_return_true);
}

Builder *builder_create()
{
    Builder *builder = (Builder *)malloc(sizeof(Builder));

    if (string_stream_init(&(builder->command)) == false)
        return NULL;

    builder->files = file_paths_create();

    if (builder->files == NULL)
        return NULL;

    return builder;
}

bool builder_add_compiler(Builder *builder, const char *compiler)
{
    assert(builder->command.size == 0);
    return string_stream_add_str(&(builder->command), compiler);
}

bool builder_add_compiler_flag(Builder *builder, char const *compilerFlag)
{
    return string_stream_add_str(&(builder->command), compilerFlag);
}

bool builder_add_file(Builder *builder, char *filePath)
{
    return file_paths_add(builder->files, filePath);
}

void builder_add_file_path(Builder *builder, FilePaths *files)
{
    for (size_t i = 0; i < files->size; i++)
    {
        file_paths_add(builder->files, files->filepaths[i]);
    }
}

void display_file_path_list(FilePaths *files)
{
    for (size_t i = 0; i < files->size; i++)
    {
        printf("%s\n", files->filepaths[i]);
    }
}

bool builder_build(Builder *builder)
{
    size_t fileCount = builder->files->size;
    const size_t compilerCommandSize = builder->command.size;

    char **fileArray = builder->files->filepaths;

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
