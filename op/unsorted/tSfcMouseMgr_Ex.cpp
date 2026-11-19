// tSfcMouseMgr_Ex: the game's mouse manager (RTTI name).
// Original source file unknown; functions grouped by class (op/UNITS.tsv).

#include "unsorted/tSfcMouseMgr.h"

class tSfcMouseMgr_Ex : public tSfcMouseMgr {
public:
    virtual ~tSfcMouseMgr_Ex();
    virtual void vfunction2();
};

// FUNCTION: STARFLEETOP 0x0040be17
tSfcMouseMgr_Ex::~tSfcMouseMgr_Ex()
{
}

// HYPOTHESIS: the code at 0x0040be9b (its exception-frame setup and body) is rebuilt as
// fn_0040bea5_mgr::fn_0040bea5, which claims 0x0040be9b. This empty definition is not the original; it stays
// until that code moves here, because a caller or a vtable needs it.
void tSfcMouseMgr_Ex::vfunction2()
{
}

// The stream fn_00627140 hands out. HYPOTHESIS: slot 1 opens a named file and
// reports success, slot 4 closes it; fn_00520990 releases the stream.
class fn_00627140_stream {
public:
    virtual void fn_slot0();
    virtual bool fn_slot1(char *name);
    virtual void fn_slot2();
    virtual void fn_slot3();
    virtual void fn_slot4();
    virtual void fn_slot5();
    // HYPOTHESIS: reads up to count bytes into buffer, returns the number read.
    virtual int fn_slot6(void *buffer, int count);
    void fn_00520990();
};

fn_00627140_stream *fn_00627140();
// Reads the next byte of the stream, -1 at the end.
int fn_0040c016(fn_00627140_stream *stream);

// HYPOTHESIS: one entry of the cursor list, 0x10 bytes, linked through +0.
class fn_0040b688_cursor {
public:
    fn_0040b688_cursor(char *name);
    ~fn_0040b688_cursor(); // HYPOTHESIS: 0x0040b6d8
    void fn_0040b6e7(fn_00627140_stream *stream, char *line);
    fn_0040b688_cursor *m_next; // +0
    char *m_4;                  // +4 HYPOTHESIS: the cursor's name
    long m_8;                   // +8
    struct fn_0040c183_image *m_c; // +0xc HYPOTHESIS: the cursor's image
};

// HYPOTHESIS: the image a cursor entry holds; fields as fn_0040c183 reads them.
struct fn_0040c183_image {
    long m_0;
    unsigned short m_4;
    unsigned short m_6;
    unsigned short m_8;
    unsigned short m_a;
    long m_c;
    long m_10;
    long m_14;
    long m_18;
    long m_1c;
};

// HYPOTHESIS: compares two strings, 0 when equal.
int fn_0040f9ea(char *a, char *b);

// HYPOTHESIS: an object held at +0xc of the mouse manager; slot 1 takes five
// arguments that the manager passes through unchanged.
class fn_0040c063_inner {
public:
    virtual void fn_slot0();
    virtual void fn_slot1(long a, long b, long c, long d, long e);
    virtual void fn_slot2();
    virtual void fn_slot3();
    virtual void fn_slot4(long a);
    virtual void fn_slot5();
    virtual void fn_slot6();
    virtual void fn_slot7(long a, long b, long c, long d, long e, long f,
                          long g, long h, long i);
    virtual void fn_slot8();
    virtual void fn_slot9(long a);
    virtual void fn_slot10(long a);
};

class fn_0040bea5_mgr {
public:
    virtual void fn_slot0();
    virtual void fn_slot1();
    virtual void fn_slot2();
    virtual void fn_slot3();
    virtual void fn_slot4();
    virtual void fn_slot5();
    virtual void fn_slot6();
    virtual void fn_slot7();
    virtual void fn_slot8();
    // HYPOTHESIS: selects the cursor named in cursors.txt.
    virtual void fn_slot9(char *name);
    bool fn_0040bea5();
    void fn_0040c037();
    void fn_0040c063(long a, long b, long c, long d, long e);
    void fn_0040c08b();
    void fn_0040c094();
    void fn_0040c09d(long a);
    void fn_0040c0ad();
    void fn_0040c0b6();
    void fn_0040c0bf(int race);
    void fn_0040c183(char *name);
    void fn_0040c1e7(long a);
    void fn_0040c1f7(long a);
    fn_0040b688_cursor *m_cursors; // +4
    fn_0040b688_cursor *m_8;       // +8 HYPOTHESIS: the current cursor
    fn_0040c063_inner m_c;         // +0xc
};

// FUNCTION: STARFLEETOP 0x0040be9b
// MATCH-ONLY: the original function starts at 0x0040be9b with its EH prolog;
// the analysis scores the body from 0x0040bea5. HYPOTHESIS: this is a member of
// tSfcMouseMgr_Ex that loads the cursor table; written on a stand-in class of
// the same layout, named after the address.
bool fn_0040bea5_mgr::fn_0040bea5()
{
    fn_slot2();
    fn_00627140_stream *file = fn_00627140();
    if (!file)
        return false;
    bool result;
    char name[] = "assets/mouse/cursors.txt";
    if (!file->fn_slot1(name)) {
        result = false;
    } else {
        fn_00627140_stream *stream = fn_00627140();
        if (!stream) {
            result = false;
        } else {
            char line[260];
            line[0] = 0;
            fn_0040b688_cursor *cursor = 0;
            char c;
            do {
                c = 0;
                while (c != (char)0xff && c != '|')
                    c = fn_0040c016(file);
                if (c == (char)0xff)
                    break;
                int i = 0;
                for (;;) {
                    c = fn_0040c016(file);
                    if (c == '|')
                        break;
                    line[i++] = c;
                    if (c == (char)0xff)
                        break;
                }
                line[i] = 0;
                if (c == (char)0xff)
                    break;
                if (i != 0) {
                    cursor = new fn_0040b688_cursor(line);
                    cursor->m_next = m_cursors;
                    m_cursors = cursor;
                }
                i = 0;
                for (;;) {
                    c = fn_0040c016(file);
                    if (c == (char)0xff || c == '\r' || c == '\n')
                        break;
                    line[i++] = c;
                }
                line[i] = 0;
                if (line[0] && cursor)
                    cursor->fn_0040b6e7(stream, line);
            } while (c != (char)0xff);
            stream->fn_slot4();
            stream->fn_00520990();
            result = true;
        }
        file->fn_slot4();
    }
    file->fn_00520990();
    return result;
}

// FUNCTION: STARFLEETOP 0x0040c016
int fn_0040c016(fn_00627140_stream *stream)
{
    unsigned char c;
    if (stream->fn_slot6(&c, 1) != 1)
        return -1;
    return c;
}

// FUNCTION: STARFLEETOP 0x0040c037
// The analysis names this a scalar deleting destructor, but it takes no flag
// and returns nothing: it frees the cursor list that fn_0040bea5 builds.
// HYPOTHESIS: a member of tSfcMouseMgr_Ex, written on the same stand-in class.
void fn_0040bea5_mgr::fn_0040c037()
{
    fn_slot4();
    while (m_cursors) {
        fn_0040b688_cursor *cursor = m_cursors;
        m_cursors = cursor->m_next;
        delete cursor;
    }
    m_8 = 0;
}

// FUNCTION: STARFLEETOP 0x0040c063
// HYPOTHESIS: a member of tSfcMouseMgr_Ex, written on the same stand-in class;
// it runs the manager's slot 4 and passes its arguments on to the object at +0xc.
void fn_0040bea5_mgr::fn_0040c063(long a, long b, long c, long d, long e)
{
    fn_slot4();
    m_c.fn_slot1(a, b, c, d, e);
}

// FUNCTION: STARFLEETOP 0x0040c08b
// HYPOTHESIS: a member of tSfcMouseMgr_Ex, written on the same stand-in class;
// it forwards to slot 2 of the object at +0xc.
void fn_0040bea5_mgr::fn_0040c08b()
{
    m_c.fn_slot2();
}

// FUNCTION: STARFLEETOP 0x0040c094
// HYPOTHESIS: a member of tSfcMouseMgr_Ex, written on the same stand-in class;
// it forwards to slot 3 of the object at +0xc.
void fn_0040bea5_mgr::fn_0040c094()
{
    m_c.fn_slot3();
}

// FUNCTION: STARFLEETOP 0x0040c09d
// HYPOTHESIS: a member of tSfcMouseMgr_Ex, written on the same stand-in class;
// it passes its argument on to slot 4 of the object at +0xc.
void fn_0040bea5_mgr::fn_0040c09d(long a)
{
    m_c.fn_slot4(a);
}

// FUNCTION: STARFLEETOP 0x0040c0ad
// HYPOTHESIS: a member of tSfcMouseMgr_Ex, written on the same stand-in class;
// it forwards to slot 5 of the object at +0xc.
void fn_0040bea5_mgr::fn_0040c0ad()
{
    m_c.fn_slot5();
}

// FUNCTION: STARFLEETOP 0x0040c0b6
// HYPOTHESIS: a member of tSfcMouseMgr_Ex, written on the same stand-in class;
// it forwards to slot 6 of the object at +0xc.
void fn_0040bea5_mgr::fn_0040c0b6()
{
    m_c.fn_slot6();
}

// HYPOTHESIS: the game's mouse manager instance, at 0x00898c24.
extern fn_0040bea5_mgr *g_00898c24;

// FUNCTION: STARFLEETOP 0x0040c0bf
// HYPOTHESIS: a member of tSfcMouseMgr_Ex, written on the same stand-in class;
// it picks the cursor of a race (names from assets/mouse/cursors.txt). The
// race numbering (0 Federation .. 7 Mirak, 8-15 Orion cartels) is inferred
// from the cursor names.
void fn_0040bea5_mgr::fn_0040c0bf(int race)
{
    switch (race) {
    case 0:
    case 16:
        g_00898c24->fn_slot9("starrow");
        break;
    case 1:
        g_00898c24->fn_slot9("dktagh");
        break;
    case 2:
        g_00898c24->fn_slot9("rom3");
        break;
    case 3:
        g_00898c24->fn_slot9("lyran");
        break;
    case 4:
        g_00898c24->fn_slot9("hydran");
        break;
    case 5:
        g_00898c24->fn_slot9("gorn");
        break;
    case 6:
        g_00898c24->fn_slot9("isc");
        break;
    case 7:
        g_00898c24->fn_slot9("mirak");
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        g_00898c24->fn_slot9("orion");
        break;
    }
}

// FUNCTION: STARFLEETOP 0x0040c183
// HYPOTHESIS: a member of tSfcMouseMgr_Ex, written on the same stand-in class;
// it finds the cursor of the given name, hands its image to slot 7 of the
// object at +0xc and makes it the current cursor.
void fn_0040bea5_mgr::fn_0040c183(char *name)
{
    fn_0040b688_cursor *cursor = m_cursors;
    while (cursor) {
        if (fn_0040f9ea(cursor->m_4, name) == 0)
            break;
        cursor = cursor->m_next;
    }
    if (cursor && cursor->m_c) {
        fn_0040c183_image *image = cursor->m_c;
        m_c.fn_slot7(image->m_10, image->m_8, image->m_a, image->m_c,
                     image->m_1c, image->m_4, image->m_6, image->m_18,
                     image->m_14);
        m_8 = cursor;
    }
}

// FUNCTION: STARFLEETOP 0x0040c1e7
// HYPOTHESIS: a member of tSfcMouseMgr_Ex, written on the same stand-in class;
// it passes its argument on to slot 9 of the object at +0xc.
void fn_0040bea5_mgr::fn_0040c1e7(long a)
{
    m_c.fn_slot9(a);
}

// FUNCTION: STARFLEETOP 0x0040c1f7
// HYPOTHESIS: a member of tSfcMouseMgr_Ex, written on the same stand-in class;
// it passes its argument on to slot 10 of the object at +0xc.
void fn_0040bea5_mgr::fn_0040c1f7(long a)
{
    m_c.fn_slot10(a);
}
