typedef struct abuf {
    char *buffer;
    int len;
} AppendBuffer;

#define new_AppendBuffer {NULL, 0};

void ABAppend(AppendBuffer *ab, const char *s, int len);
void ABFree(AppendBuffer *ab);
