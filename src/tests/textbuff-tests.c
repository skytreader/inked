#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "../textbuff.h"

Test(textBuffer, normalUsage) {
    TextBuffer textBuff = (TextBuffer) new_TextBuffer;
    cr_assert(textBuff.strLen == 0);
    cr_assert(textBuff.strLen <= textBuff.bufferLen);
    TBAppend(&textBuff, 'a');
    cr_assert(eq(i32, textBuff.strLen, 1));
    cr_assert(textBuff.strLen <= textBuff.bufferLen);
}
