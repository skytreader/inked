#include <stdlib.h>
#include <string.h>

#include "textbuff.h"

void TBAppend(TextBuffer *tb, char c) {
    if (c == '\0') {
        return;
    }
    // First make sure we have enough space in the buffer.
    if (tb->strLen == tb->bufferLen) {
        if (tb->bufferLen == 0) {
            tb->bufferLen = INITIAL_BUFFER_SIZE;
        } else {
            tb->bufferLen *= 2;
        }
        char *extendedBuffer = realloc(tb->buffer, tb->bufferLen);

        if (extendedBuffer == NULL) {
            // FIXME Shouldn't we error somehow?
            return;
        }
        tb->buffer = extendedBuffer;
    }

    // append the character into the buffer and update internal trackers
    tb->buffer[tb->strLen] = c;
    tb->buffer[tb->strLen + 1] = '\0';
    tb->strLen++;
    if (c == '\n') {
        tb->newlineCount++;
    }
}

void TBDelete(TextBuffer *tb, unsigned int index) {
    if (tb->buffer[index] == '\n') {
        tb->newlineCount--;
    }
    memmove(&tb->buffer[index], &tb->buffer[index + 1], tb->strLen - index);
    tb->strLen--;
}

void TBFree(TextBuffer *tb) {
    free(tb->buffer);
}

unsigned int tbStrLen(TextBuffer *tb, bool shouldUpdate) {
    unsigned int count = 0;
    for (; count < tb->bufferLen; count++) {
        if (tb->buffer[count] == '\0') {
            break;
        }
    }

    if (shouldUpdate) {
        tb->strLen = count;
    }

    return count;
}
