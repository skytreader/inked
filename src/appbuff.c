#include <stdlib.h>
#include <string.h>

#include "appbuff.h"

void ABAppend(AppendBuffer *ab, const char *s, int len) {
    char *new = realloc(ab->buffer, ab->len + len);

    if (new == NULL) {
        return;
    }

    memcpy(&new[ab->len], s, len);
    ab->buffer = new;
    ab->len += len;
}

void ABFree(AppendBuffer *ab) {
    free(ab->buffer);
}
