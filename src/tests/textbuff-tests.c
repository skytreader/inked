#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "../textbuff.h"

Test(textBuffer, normalUsage) {
    TextBuffer textBuff = (TextBuffer) new_TextBuffer;
    cr_assert(zero(textBuff.strLen));
    cr_assert(le(uint, textBuff.strLen, textBuff.bufferLen));
    TBAppend(&textBuff, 'a');
    cr_assert(eq(uint, textBuff.strLen, 1));
    cr_assert(le(uint, textBuff.strLen, textBuff.bufferLen));
}

Test(textBuffer, addNullChar) {
    TextBuffer textBuff = (TextBuffer) new_TextBuffer;
    cr_assert(zero(textBuff.strLen));
    TBAppend(&textBuff, '\0');
    cr_assert(eq(uint, textBuff.strLen, 0));
}

Test(textBuffer, deleteMiddle) {
    TextBuffer textBuff = (TextBuffer) new_TextBuffer;
    TBAppend(&textBuff, 'a');
    TBAppend(&textBuff, 'b');
    TBAppend(&textBuff, 'c');
    cr_assert(eq(uint, 3, textBuff.strLen));
    cr_assert(eq(chr, 'a', textBuff.buffer[0]));
    cr_assert(eq(chr, 'b', textBuff.buffer[1]));
    cr_assert(eq(chr, 'c', textBuff.buffer[2]));
    TBDelete(&textBuff, 1);
    cr_assert(eq(uint, 2, textBuff.strLen));
    cr_assert(eq(chr, 'a', textBuff.buffer[0]));
    cr_assert(eq(chr, 'c', textBuff.buffer[1]));
}

Test(textBuffer, deleteStart) {
    TextBuffer textBuff = (TextBuffer) new_TextBuffer;
    TBAppend(&textBuff, 'a');
    TBAppend(&textBuff, 'b');
    TBAppend(&textBuff, 'c');
    cr_assert(eq(uint, 3, textBuff.strLen));
    cr_assert(eq(chr, 'a', textBuff.buffer[0]));
    cr_assert(eq(chr, 'b', textBuff.buffer[1]));
    cr_assert(eq(chr, 'c', textBuff.buffer[2]));
    TBDelete(&textBuff, 0);
    cr_assert(eq(uint, 2, textBuff.strLen));
    cr_assert(eq(chr, 'b', textBuff.buffer[0]));
    cr_assert(eq(chr, 'c', textBuff.buffer[1]));
}

Test(textBuffer, deleteEnd) {
    TextBuffer textBuff = (TextBuffer) new_TextBuffer;
    TBAppend(&textBuff, 'a');
    TBAppend(&textBuff, 'b');
    TBAppend(&textBuff, 'c');
    cr_assert(eq(uint, 3, textBuff.strLen));
    cr_assert(eq(chr, 'a', textBuff.buffer[0]));
    cr_assert(eq(chr, 'b', textBuff.buffer[1]));
    cr_assert(eq(chr, 'c', textBuff.buffer[2]));
    TBDelete(&textBuff, 2);
    cr_assert(eq(uint, 2, textBuff.strLen));
    cr_assert(eq(chr, 'a', textBuff.buffer[0]));
    cr_assert(eq(chr, 'b', textBuff.buffer[1]));
}

Test(textBuffer, deleteBeyondBuffer) {
    TextBuffer textBuff = (TextBuffer) new_TextBuffer;
    TBAppend(&textBuff, 'a');
    TBAppend(&textBuff, 'b');
    TBAppend(&textBuff, 'c');
    cr_assert(eq(uint, 3, textBuff.strLen));
    cr_assert(ge(uint, 4, textBuff.bufferLen));
    cr_assert(eq(chr, 'a', textBuff.buffer[0]));
    cr_assert(eq(chr, 'b', textBuff.buffer[1]));
    cr_assert(eq(chr, 'c', textBuff.buffer[2]));
    TBDelete(&textBuff, 3);
    cr_assert(eq(uint, 3, textBuff.strLen));
    cr_assert(eq(chr, 'a', textBuff.buffer[0]));
    cr_assert(eq(chr, 'b', textBuff.buffer[1]));
    cr_assert(eq(chr, 'c', textBuff.buffer[2]));
}

Test(textBuffer, tbStrLen) {
    TextBuffer textBuff = (TextBuffer) new_TextBuffer;
    cr_assert(zero(tbStrLen(&textBuff, false)));
    TBAppend(&textBuff, '\0');
    cr_assert(zero(tbStrLen(&textBuff, false)));
    TBAppend(&textBuff, 'c');
    cr_assert(eq(uint, 1, tbStrLen(&textBuff, false)));
    TBAppend(&textBuff, 'd');
    cr_assert(eq(uint, 2, tbStrLen(&textBuff, false)));
}

Test(textBuffer, tbStrLen_edgeCase) {
    TextBuffer textBuff = (TextBuffer) new_TextBuffer;
    cr_assert(zero(tbStrLen(&textBuff, false)));
}

Test(textBuffer, tbStrLen_sideEffect) {
    TextBuffer textBuff = (TextBuffer) new_TextBuffer;
    cr_assert(zero(tbStrLen(&textBuff, false)));
    TBAppend(&textBuff, 'c');
    TBAppend(&textBuff, 'd');
    cr_assert(eq(uint, tbStrLen(&textBuff, false), textBuff.strLen));
    // Mess-up textBuff.strLen on purpose
    textBuff.strLen = 0;
    cr_assert(ne(uint, tbStrLen(&textBuff, false), textBuff.strLen));
    // Don't do the side-effect call in the same assert statement because we are
    // not here to test evaluation order.
    tbStrLen(&textBuff, true);
    cr_assert(eq(uint, tbStrLen(&textBuff, false), textBuff.strLen));
}
