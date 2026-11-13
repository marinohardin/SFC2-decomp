// Tiger/tFormatStr.cpp

#include <string.h>

#include "Tiger/tstring.h"
#include "Tiger/tcglue.h"
#include "Tiger/tBag.h"
#include "Tiger/tFormatStr.h"
#include "Tiger/tstringtag.h"

// HYPOTHESIS: one entry of the token list, 0x30 bytes: a token kind (8 for a %tag%, 9 for
// literal text) and the token's text; the word at +4 is not used here
class tFormatToken {
public:
    tFormatToken();

    int mType;
    int m04;
    tString mText;
};

class tFormatStr;
class tFormatTag;

// HYPOTHESIS: one entry of a tag's token list, 0x38 bytes: a token kind, the text's length and
// the text
class tTagToken {
public:
    int mType;
    char mLength;
    char mText[0x33];
};

// HYPOTHESIS: layout from the inlined constructor in mAssembleFormattedString; field names are ours
class tTagInfo {
    friend class tFormatStr;
    friend class tFormatTag;

public:
    tTagInfo()
    {
        m37[0] = 0;
        m6C = 0;
        m70 = 0;
        m71 = 0xff;
        m72 = 'R';
    }
    virtual ~tTagInfo();

    // HYPOTHESIS: vtable slot 1, after the destructor, as tFormatTag::mGetString calls it
    virtual tString mGetString(tFormatTag* tag) = 0;

protected:
    signed char mParseTagTokenList(char* text, tTagToken* tokens);
    void mClassifyTagTokens(signed char count, tTagToken* tokens);
    void mSetTagName(signed char count, tTagToken* tokens);

    char m04[0x33];   // HYPOTHESIS: the tag's text between the % signs
    char m37[0x35];   // HYPOTHESIS: the tag's name, the label it is found under
    int m6C;
    char m70;
    unsigned char m71;
    char m72;
};

// HYPOTHESIS: 0xac bytes; vtable order from the tag classes' virtual functions
class tFormatTag : public tTagInfo {
    friend class tFormatStr;
    friend class tIntTag;

public:
    tFormatTag(char* text)
    {
        clib::strcpy(m04, text);
        m74 = NULL;
        m78[0] = 0;
    }

    virtual iTruth mValidate();
    virtual tString mGetString(tFormatTag* tag);
    virtual void mAddCopy(tFormatStr* str);

    tString mGetString();

protected:
    virtual void mSetGroupSpec(signed char count, tTagToken* tokens);

    tTagInfo* m74;  // HYPOTHESIS: the tag stored in the bag under this tag's name
    char m78[0x34]; // HYPOTHESIS: the group spec, as tIntTag::m74
};

// HYPOTHESIS: 0xac bytes; the integer stored at +0xa8, the byte at +0x74 begins a text buffer
class tIntTag : public tTagInfo {
    friend class tFormatStr;

public:
    tIntTag(char* text, long value)
    {
        clib::strcpy(m04, text);
        mA8 = value;
        m74[0] = 0;
    }

    // vtable order from the original's tIntTag vtable
    virtual tString mGetString(tFormatTag* tag);
    virtual iTruth mValidate();
    virtual void mAddCopy(tFormatStr* str);

protected:
    virtual void mSetGroupSpec(signed char count, tTagToken* tokens);

    char m74[0x34];
    long mA8;
};

// HYPOTHESIS: 0x9c bytes; the string value at +0x74. The class name is from the exe's
// tCharTag methods, which sit next to tIntTag's; the vtable order follows tIntTag's
class tCharTag : public tTagInfo {
    friend class tFormatStr;

public:
    tCharTag(char* text, tString& value)
    {
        clib::strcpy(m04, text);
        m74 = value;
    }

    virtual tString mGetString(tFormatTag* tag);
    virtual iTruth mValidate();
    virtual void mAddCopy(tFormatStr* str);

protected:
    virtual void mSetGroupSpec(signed char count, tTagToken* tokens);

    tString m74;
};

// HYPOTHESIS: the exception type thrown by the format classes; values seen are 0 and 2
enum eFormatException {
};

// HYPOTHESIS: a bag item holding a tag
class tTagItemBag : public tBaseItemBag {
public:
    tTagInfo* fData;

    // HYPOTHESIS: bag type 6 marks a tag item
    tTagItemBag(tTagInfo* data)
    {
        fBagType = 6;
        fData = data;
        mUpdateMasterMemInfo(sizeof(tTagItemBag));
    }
    virtual ~tTagItemBag();
};

// HYPOTHESIS: a bag of tags; adding a tag wraps it in a tTagItemBag
class tTagBag : public tItemBag {
public:
    tTagBag() {}
    virtual ~tTagBag();

    long mAddItem(tTagInfo* data, char* label)
    {
        tTagItemBag* item = new tTagItemBag(data);
        if (item)
            return mAddToList(item, label);
        return 0;
    }
};

// FUNCTION: STARFLEETOP 0x0062c800
tFormatStr::~tFormatStr()
{
    delete m04;
    delete m0C;
    delete m08;
}

// FUNCTION: STARFLEETOP 0x0062c840
signed char tFormatStr::mGenerateFormatTokenList(tFormatToken *param_1)
{
    signed char count = 0;
    iTruth inTag = 0;
    tString text;
    tString::iterator it;

    for (it = m0C->begin(); it != m0C->end(); it++) {
        tString ch(*it);
        if (ch == "%" || (it + 1) == m0C->end()) {
            if (count < 200) {
                if (inTag || (it + 1) == m0C->end())
                    text += ch;
                if (inTag) {
                    inTag = 0;
                    param_1[count].mType = 8;
                } else {
                    param_1[count].mType = 9;
                    inTag = 1;
                }
                if (text == "%%") {
                    text = "%";
                    param_1[count].mType = 9;
                }
                param_1[count].mText = text;
                count++;
                text = "";
            }
            if (inTag)
                text += ch;
        } else {
            text += ch;
        }
    }
    return count;
}

// FUNCTION: STARFLEETOP 0x0062ca70
void tFormatStr::mAssembleFormattedString(signed char param_1, tFormatToken *param_2)
{
    tString result;
    // HYPOTHESIS: a tag whose name is not in the bag keeps the previous tag's data
    // For the first tag that read is of an uninitialised pointer, as in the original; a port
    // build that trips over it needs a PORT: line here.
    tTagInfo* data;

    for (int i = 0; i < param_1; i++) {
        if (param_2[i].mType == 9) {
            result += param_2[i].mText;
        } else {
            tString name("");
            tString::iterator it;
            for (it = param_2[i].mText.begin() + 1; it < param_2[i].mText.end() - 1; it++)
                name += *it;

            tFormatTag* tag = new tFormatTag(name.mCharStr(NULL));
            tTagToken tokens[10];
            signed char numTokens = tag->mParseTagTokenList(tag->m04, tokens);
            tag->mClassifyTagTokens(numTokens, tokens);

            int kind = 0;
            if (numTokens > 1) {
                if (tokens[0].mType == 1) {
                    if (tokens[1].mType == 4)
                        kind = 3;
                } else if (tokens[0].mType == 2) {
                    kind = 1;
                } else if (tokens[0].mType == 3) {
                    kind = 2;
                }
            }
            tag->m6C = kind;
            tag->mSetTagName(numTokens, tokens);
            tag->mSetGroupSpec(numTokens, tokens);

            // MATCH-ONLY: the local fixes the register order of the lookup's this and label
            tItemBag* bag = m04;
            tTagItemBag* item = (tTagItemBag*)bag->mFindItemWithLabel(tag->m37);
            if (item)
                data = item->fData;
            tag->m74 = data;

            result += tag->mGetString();
            delete tag;
        }
    }

    if (m08) {
        delete m08;
        m08 = NULL;
    }
    m08 = new tString(result);
}

// FUNCTION: STARFLEETOP 0x0062ce90
tFormatToken::tFormatToken()
{
}

// FUNCTION: STARFLEETOP 0x0062cd70
void tFormatStr::mGenFormattedStr(tString &param_1, tString param_2)
{
    if (m0C)
        *m0C = param_2;
    else
        m0C = new tString(param_2);

    tFormatToken tokens[200];
    signed char numTokens = mGenerateFormatTokenList(tokens);
    mAssembleFormattedString(numTokens, tokens);
    param_1 = *m08;
}

// FUNCTION: STARFLEETOP 0x0062cee0
void tFormatStr::mTag(char *param_1, long param_2)
{
    if (param_1) {
        tIntTag* tag = new tIntTag(param_1, param_2);
        tTagToken tokens[10];
        signed char numTokens = tag->mParseTagTokenList(tag->m04, tokens);
        tag->mClassifyTagTokens(numTokens, tokens);

        int kind = 0;
        if (numTokens > 1) {
            if (tokens[0].mType == 1) {
                if (tokens[1].mType == 4)
                    kind = 3;
            } else if (tokens[0].mType == 2) {
                kind = 1;
            } else if (tokens[0].mType == 3) {
                kind = 2;
            }
        }
        tag->m6C = kind;
        tag->mSetTagName(numTokens, tokens);
        tag->mSetGroupSpec(numTokens, tokens);

        if (tag->mValidate()) {
            if (m04 == NULL)
                m04 = new tTagBag;
            m04->mAddItem(tag, tag->m37);
            return;
        }
        // HYPOTHESIS: 2 reports a tag that fails validation
        delete tag;
        throw (eFormatException)2;
    }
    // HYPOTHESIS: 0 reports a missing tag text
    throw (eFormatException)0;
}

// FUNCTION: STARFLEETOP 0x0062d110
void tFormatStr::mTag(char *param_1, tString &param_2)
{
    if (param_1) {
        tCharTag* tag = new tCharTag(param_1, param_2);
        tTagToken tokens[10];
        signed char numTokens = tag->mParseTagTokenList(tag->m04, tokens);
        tag->mClassifyTagTokens(numTokens, tokens);

        int kind = 0;
        if (numTokens > 1) {
            if (tokens[0].mType == 1) {
                if (tokens[1].mType == 4)
                    kind = 3;
            } else if (tokens[0].mType == 2) {
                kind = 1;
            } else if (tokens[0].mType == 3) {
                kind = 2;
            }
        }
        tag->m6C = kind;
        tag->mSetTagName(numTokens, tokens);
        tag->mSetGroupSpec(numTokens, tokens);

        if (tag->mValidate()) {
            if (m04 == NULL)
                m04 = new tTagBag;
            m04->mAddItem(tag, tag->m37);
            return;
        }
        // HYPOTHESIS: 2 reports a tag that fails validation
        delete tag;
        throw (eFormatException)2;
    }
    // HYPOTHESIS: 0 reports a missing tag text
    throw (eFormatException)0;
}

// FUNCTION: STARFLEETOP 0x0062d350
void tFormatStr::mTag(char *param_1, char *param_2)
{
    tString value(param_2);
    mTag(param_1, value);
}

// FUNCTION: STARFLEETOP 0x0062d3c0
signed char tTagInfo::mParseTagTokenList(char* text, tTagToken* tokens)
{
    signed char count = 0;
    int len = 0;
    char buf[0x34];
    // MATCH-ONLY: the loop test reads the first character into a local, as the original's code does
    char c = text[0];
    int i;

    for (i = 0; c != 0; c = text[i + 1], i++) {
        if (text[i] == ':' || text[i + 1] == 0) {
            if (text[i + 1] == 0 && len < 50)
                buf[len++] = text[i];
            if (count < 10) {
                if (len > 0)
                    memcpy(tokens[count].mText, buf, len);
                tokens[count].mText[len] = 0;
                tokens[count].mLength = len;
                len = 0;
                count++;
            }
        } else if (len < 50) {
            buf[len++] = text[i];
        }
    }
    // HYPOTHESIS: 0 reports a tag text without tokens, as in mTag
    if (count == 0)
        throw (eFormatException)0;
    return count;
}

// HYPOTHESIS: the three identical digit checks below are one inlined helper; the name is ours
static inline iTruth fn_IsAllDigits(char* text, int len)
{
    iTruth isNum = 1;
    for (int j = 0; j < len; j++) {
        char c = text[j];
        if (c < '0' || c > '9')
            isNum = 0;
    }
    return isNum;
}

// HYPOTHESIS: token kinds: 1 a $-prefixed name, 2 '#', 3 '$', 4 a name, 5 the first number,
// 6 the second number, 7 an alignment letter (L or R)
// FUNCTION: STARFLEETOP 0x0062d4a0
void tTagInfo::mClassifyTagTokens(signed char count, tTagToken* tokens)
{
    // MATCH-ONLY: the StringToNum calls index tokens[i]; written through tok alone, the
    // compiler shares one text register
    iTruth isNum;

    for (int i = 0; i < count; i++) {
        tTagToken* tok = &tokens[i];
        tok->mType = 0;
        if (tok->mLength > 0) {
            switch (i) {
            case 0:
                if (count == 1)
                    tok->mType = 4;
                else if (tok->mLength == 1 && tok->mText[0] == '#')
                    tok->mType = 2;
                else if (tok->mLength == 1 && tok->mText[0] == '$')
                    tok->mType = 3;
                else if (tok->mText[0] == '$')
                    tok->mType = 1;
                break;
            case 1:
                isNum = fn_IsAllDigits(tok->mText, tok->mLength);
                if (isNum) {
                    if (tokens[i - 1].mType == 0)
                        tokens[i - 1].mType = 4;
                    tok->mType = 5;
                    m70 = (char)StringToNum(tokens[i].mText);
                } else if (tok->mLength == 1 && (tok->mText[0] == 'L' || tok->mText[0] == 'R')) {
                    tok->mType = 7;
                    m72 = tok->mText[0];
                    tokens[i - 1].mType = 4;
                } else {
                    if (tokens[i - 1].mType == 0)
                        tokens[i - 1].mType = 1;
                    tok->mType = 4;
                }
                break;
            case 2:
                isNum = fn_IsAllDigits(tok->mText, tok->mLength);
                if (isNum) {
                    if (tokens[i - 1].mType == 5) {
                        tok->mType = 6;
                        m71 = (unsigned char)StringToNum(tokens[i].mText);
                    } else {
                        tok->mType = 5;
                        m70 = (char)StringToNum(tokens[i].mText);
                    }
                } else if (tokens[i - 1].mType == 5 && tok->mLength == 1
                           && (tok->mText[0] == 'L' || tok->mText[0] == 'R')) {
                    tok->mType = 7;
                    m72 = tok->mText[0];
                }
                break;
            case 3:
                isNum = fn_IsAllDigits(tok->mText, tok->mLength);
                if (isNum) {
                    tok->mType = 6;
                    m71 = (unsigned char)StringToNum(tokens[i].mText);
                } else if ((tokens[i - 1].mType == 5 || tokens[i - 1].mType == 6)
                           && tok->mLength == 1
                           && (tok->mText[0] == 'L' || tok->mText[0] == 'R')) {
                    tok->mType = 7;
                    m72 = tok->mText[0];
                }
                break;
            }
        }
    }
}

// FUNCTION: STARFLEETOP 0x0062d6b0
void tTagInfo::mSetTagName(signed char count, tTagToken* tokens)
{
    tTagToken* name = NULL;
    for (int i = 0; i < count; i++) {
        if (tokens[i].mType == 4)
            name = &tokens[i];
    }
    if (name) {
        int j;
        for (j = 0; j < name->mLength; j++)
            m37[j] = name->mText[j];
        m37[j] = 0;
        return;
    }
    m37[0] = 0;
    // HYPOTHESIS: 2 reports a tag without a name token
    throw (eFormatException)2;
}

// FUNCTION: STARFLEETOP 0x0062d720
iTruth tCharTag::mValidate()
{
    // HYPOTHESIS: kind 2 is the string kind ('$'); a tag without a kind defaults to it
    if (m6C == 0)
        m6C = 2;
    iTruth valid = 1;
    if (m6C != 2 || m37[0] == 0)
        valid = 0;
    return valid;
}

// FUNCTION: STARFLEETOP 0x0062d750
tString tCharTag::mGetString(tFormatTag* tag)
{
    tString str(m74);
    // HYPOTHESIS: m70 is the field width and m72 the alignment; a right-aligned value is padded
    // on the left
    if (m70) {
        while (str.mStrLen() < m70) {
            tString pad(" ");
            if (m72 == 'R')
                str = pad + str;
            else
                str = str + pad;
        }
    }
    return str;
}

// FUNCTION: STARFLEETOP 0x0062d840
void tCharTag::mAddCopy(tFormatStr* str)
{
    str->mTag(m04, m74);
}

// FUNCTION: STARFLEETOP 0x0062d860
iTruth tIntTag::mValidate()
{
    // HYPOTHESIS: kind 2 (the string kind) is turned into kind 3 for an integer value
    if (m6C == 2)
        m6C = 3;
    if (m37[0])
        return 1;
    return 0;
}

// FUNCTION: STARFLEETOP 0x0062d880
void tIntTag::mSetGroupSpec(signed char count, tTagToken* tokens)
{
    // HYPOTHESIS: a first token of kind 1 (a $-prefixed name) names the group; the text is kept
    // without its '$'
    if (tokens[0].mType == 1 && tokens) {
        int start = tokens[0].mText[0] == '$';
        int i;
        for (i = start; i < tokens[0].mLength; i++)
            m74[i - start] = tokens[0].mText[i];
        m74[i - start] = 0;
    }
}

// FUNCTION: STARFLEETOP 0x0062d8d0
tString tIntTag::mGetString(tFormatTag* tag)
{
    tString str;
    if (tag->m78[0] == 0) {
        NumToString(str, mA8);
    } else {
        // HYPOTHESIS: the format tag's group spec must name this tag's group, up to a '-'
        if (m74[0]) {
            char* a = tag->m78;
            char* b = m74;
            while (*a && *a != '-') {
                if (*a++ != *b++)
                    throw (eFormatException)6;
            }
            if (*b && *b != '-')
                throw (eFormatException)6;
        }
        tStringGroupTag group(tag->m78);
        str = group.mGetString(mA8);
    }
    // HYPOTHESIS: m70 is the field width and m72 the alignment, as in tCharTag::mGetString
    if (m70) {
        while (str.mStrLen() < m70) {
            tString pad(" ");
            if (m72 == 'R')
                str = pad + str;
            else
                str = str + pad;
        }
    }
    return str;
}

// FUNCTION: STARFLEETOP 0x0062da60
void tIntTag::mAddCopy(tFormatStr* str)
{
    str->mTag(m04, mA8);
}

// FUNCTION: STARFLEETOP 0x0062da80
void tFormatTag::mSetGroupSpec(signed char count, tTagToken* tokens)
{
    // HYPOTHESIS: as tIntTag::mSetGroupSpec, the group spec is kept without its '$'
    if (tokens[0].mType == 1 && tokens) {
        int start = tokens[0].mText[0] == '$';
        int i;
        for (i = start; i < tokens[0].mLength; i++)
            m78[i - start] = tokens[0].mText[i];
        m78[i - start] = 0;
    }
}

// FUNCTION: STARFLEETOP 0x0062dad0
tString tFormatTag::mGetString()
{
    // HYPOTHESIS: 5 reports a tag whose name is not in the bag, 6 a group spec that does not
    // fit the stored tag's kind, 7 a string tag used where a number is wanted
    if (m74 == NULL)
        throw (eFormatException)5;
    if (m78[0]) {
        if (m74->m6C == 1 || m74->m6C == 2)
            throw (eFormatException)6;
    } else {
        if (m74->m6C == 3)
            throw (eFormatException)6;
        if (m6C == 2 && (m74->m6C == 1 || m74->m6C == 0))
            throw (eFormatException)7;
    }
    tString str = m74->mGetString(this);
    // HYPOTHESIS: m70 is the field width and m72 the alignment, as in tCharTag::mGetString
    if (m70) {
        while (str.mStrLen() < m70) {
            tString pad(" ");
            if (m72 == 'R')
                str = pad + str;
            else
                str = str + pad;
        }
    }
    return str;
}

// FUNCTION: STARFLEETOP 0x0062dc40
// FUNCTION: STARFLEETOP 0x0062dc60
tTagItemBag::~tTagItemBag()
{
    delete fData;
    mUpdateMasterMemInfo(-(long)sizeof(tTagItemBag));
}
