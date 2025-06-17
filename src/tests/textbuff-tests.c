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
