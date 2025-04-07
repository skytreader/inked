typedef struct tbuff {
    char *buffer;
    /*
     * Track how much memory we actually have allocated for this buffer.
     */
    int bufferLen;
    /*
     * Track the current length of the string in the buffer. Invariants:
     *
     * strLen <= bufferLen
     */
    int strLen;
    /*
     * Number of newlines in this buffer. Invariants:
     *
     * newlineCount <= len
     */
    int newlineCount;
    /*
     * For pagination. Number of lines to _not_ display from the start of the
     * buffer. Invariants:
     *
     * lineOffset <= newlineCount
     */
    int lineOffset;
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
void TBDelete(TextBuffer *tb, int index);
void TBFree(TextBuffer *tb);
