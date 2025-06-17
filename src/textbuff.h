#include <stdbool.h>

typedef struct tbuff {
    char *buffer;
    /*
     * Track how much memory we actually have allocated for this buffer.
     */
    unsigned int bufferLen;
    /*
     * Track the current length of the string in the buffer. Invariants:
     *
     * 0 <= strLen <= bufferLen
     */
    unsigned int strLen;
    /*
     * Number of newlines in this buffer. Invariants:
     *
     * 0 <= newlineCount <= len
     */
    unsigned int newlineCount;
    /*
     * For pagination. Number of lines to _not_ display from the start of the
     * buffer. Invariants:
     *
     * 0 <= lineOffset <= newlineCount
     */
    unsigned int lineOffset;
} TextBuffer;

/*
 * The buffer will not actually use this size unless it is triggered at least
 * once.
 */
#define INITIAL_BUFFER_SIZE 2
#define new_TextBuffer {NULL, 0, 0, 0, 0};

/*
 * Adds the given character to the buffer. This will double the size of the
 * buffer for everytime we need more space.
 */
void TBAppend(TextBuffer *tb, char c);
/*
 * Deletes the character in the buffer at the given index.
 */
void TBDelete(TextBuffer *tb, unsigned int index);
void TBFree(TextBuffer *tb);
/*
 * Counts the length of the C-string in the text buffer. Flag shouldUpdate will
 * update the internal strLen field if set to true.
 *
 * This is an O(n) operation. Use with caution.
 */
unsigned int tbStrLen(TextBuffer *tb, bool shouldUpdate);
