#include "unsorted/tGameState.h"
#include "unsorted/tValueBase.h"
#include "unsorted/tMetaClientCreator.h"
#include "unsorted/tMetaClientMapPanelCreator.h"
#include "unsorted/tMetaClientMissionPanelCreator.h"
#include "unsorted/tMetaClientNewsPanelCreator.h"
#include "unsorted/tMetaClientSupplyDockPanelCreator.h"
#include "unsorted/tMetaClientShipPanelCreator.h"
#include "unsorted/tMedalsPanelCreator.h"
#include "unsorted/tPlayerRelayCCreator.h"
#include "unsorted/tMessengerRelayCreator.h"
#include "unsorted/tPlayerInfoPanelCreator.h"
#include "unsorted/tMetaClientChatPanelCreator.h"
#include "unsorted/tMetaClientPlayerListPanelCreator.h"
#include "unsorted/tMetaClientHelpListPanelCreator.h"
#include "unsorted/tObject_0092deb4.h"
#include <string>
#include <vector>
#include "Tiger/tstring.h"
// HYPOTHESIS: the 16-byte container at tPanelMgr+0x1f0, keyed by an int and holding one 32-bit
// value per key; only its index operator (0x0060a8f0), its clear (0x0060ab15) and its iterator's
// increment (0x0052e144) are known. The layout is that of a balanced tree: a head node at +0x04
// whose left link is the first node; each node holds its key at +0x0c and its value at +0x10.
struct fn_0060a8f0_container {
    struct node {
        node *left;
        node *parent;
        node *right;
        int first;
        void *second;
    };
    class iterator {
    public:
        iterator(node *n) : m_node(n) {}
        node &operator*() const { return *m_node; }
        iterator &fn_0052e144();
        iterator &operator++() { return fn_0052e144(); }
        bool operator!=(const iterator &other) const { return m_node != other.m_node; }
        node *m_node;
    };
    ~fn_0060a8f0_container();	// 0x0060a919
    void *&fn_0060a8f0(const int &key);
    void fn_0060ab15();
    // HYPOTHESIS: find(key); the same function (0x0053aa7a) serves the container at +0x204.
    iterator fn_0053aa7a(const int &key);
    iterator begin() { return iterator(m_head->left); }
    iterator end() { return iterator(m_head); }
    char m_0x00[0x04];
    node *m_head;
    char m_0x08[0x08];
};

// HYPOTHESIS: the 16-byte container at tPanelMgr+0x1d0 has the layout of a VC6 vector of 32-bit
// values (an allocator byte, then first, last and end-of-storage pointers); only its append
// (0x00571a69) and its destructor (0x00571a7a, shared with the member at +0x1e0) are known.
struct fn_00571a69_vector {
    ~fn_00571a69_vector();
    void fn_00571a69(void *const &value);
    // HYPOTHESIS: erase(first, last), returning the iterator after the erased range.
    void **fn_00425ed6(void **first, void **last);
    void **begin() { return m_first; }
    void **end() { return m_last; }
    void clear() { fn_00425ed6(begin(), end()); }
    unsigned int size() const { return m_first == 0 ? 0 : m_last - m_first; }
    char m_allocator;
    void **m_first;
    void **m_last;
    void **m_end;
};

// HYPOTHESIS: the vector at tPanelMgr+0x1e0 is laid out like the one at +0x1d0 (and shares its
// destructor, 0x00571a7a) but is erased through its own function (0x00425f12), so it is taken to
// be a vector of another element type. Its append is assumed to be 0x00571a69 as well.
struct fn_00425f12_vector {
    ~fn_00425f12_vector();	// HYPOTHESIS: 0x00571a7a, as for the one at +0x1d0
    void fn_00571a69(void *const &value);
    // HYPOTHESIS: erase(first, last), returning the iterator after the erased range.
    void **fn_00425f12(void **first, void **last);
    void **begin() { return m_first; }
    void **end() { return m_last; }
    void clear() { fn_00425f12(begin(), end()); }
    unsigned int size() const { return m_first == 0 ? 0 : m_last - m_first; }
    char m_allocator;
    void **m_first;
    void **m_last;
    void **m_end;
};

// HYPOTHESIS: the container at tPanelMgr+0x204 is keyed by an int and holds one 32-bit value per
// key, like the one at +0x1f0 but with its own index operator (0x0060a951), clear (0x0060ac16)
// and iterator increment (0x0052e1ab); its layout is taken to be that of the one at +0x1f0.
struct fn_0060a951_container {
    typedef fn_0060a8f0_container::node node;
    class iterator {
    public:
        iterator(node *n) : m_node(n) {}
        node &operator*() const { return *m_node; }
        iterator &fn_0052e1ab();
        iterator &operator++() { return fn_0052e1ab(); }
        bool operator!=(const iterator &other) const { return m_node != other.m_node; }
        node *m_node;
    };
    ~fn_0060a951_container();	// 0x0060a97a
    void *&fn_0060a951(const int &key);
    void fn_0060ac16();
    // HYPOTHESIS: find(key), the same function (0x0053aa7a) as the container at +0x1f0 uses.
    iterator fn_0053aa7a(const int &key);
    iterator begin() { return iterator(m_head->left); }
    iterator end() { return iterator(m_head); }
    char m_0x00[0x04];
    node *m_head;
    char m_0x08[0x08];
};

#include "unsorted/fn_004d288f_object.h"

// HYPOTHESIS: the member at tPanelMgr+0x100; it is taken to end before the flag at +0x164 that
// fn_00608bd1 reads. Only its functions 0x004d290d (no argument), 0x004d288f (looks up an object
// by a pointer key) and 0x004d269b (two globals and a flag) are known.
struct fn_004d290d_object {
    ~fn_004d290d_object();	// 0x004d2636
    void fn_004d290d();
    fn_004d288f_object *fn_004d288f(const void *key);
    void fn_004d269b(void *a, void *b, bool flag);
    char m_0x00[0x64];
};

// HYPOTHESIS: a 20-byte element of the vector at tPanelMgr+0x280; only its function 0x0060b3b0
// (taking no argument) is known.
struct fn_0060b3b0_element {
    void fn_0060b3b0();
    char m_0x00[0x14];
};

// HYPOTHESIS: the vector at tPanelMgr+0x280, laid out like the one at +0x1d0 but of 20-byte
// elements; only its erase (0x0060abb1) is known.
struct fn_0060abb1_vector {
    ~fn_0060abb1_vector();	// 0x0060a9b2
    // HYPOTHESIS: erase(first, last), returning the iterator after the erased range.
    fn_0060b3b0_element *fn_0060abb1(fn_0060b3b0_element *first, fn_0060b3b0_element *last);
    fn_0060b3b0_element *begin() { return m_first; }
    fn_0060b3b0_element *end() { return m_last; }
    unsigned int size() const { return m_first == 0 ? 0 : m_last - m_first; }
    // HYPOTHESIS: calls 0x0060b3b0 on every element of a range.
    static void fn_inline_each(fn_0060b3b0_element *first, fn_0060b3b0_element *last)
    {
        for (; first != last; ++first)
            first->fn_0060b3b0();
    }
    // HYPOTHESIS: calls 0x0060b3b0 on every element, then erases them all.
    void clear()
    {
        fn_inline_each(begin(), end());
        fn_0060abb1(begin(), end());
    }
    char m_allocator;
    fn_0060b3b0_element *m_first;
    fn_0060b3b0_element *m_last;
    fn_0060b3b0_element *m_end;
};

// HYPOTHESIS: the object tPanelMgr::fn_0060a500 returns; only its test 0x005ed4ad is known.
class fn_005ed4ad_object {
public:
    bool fn_005ed4ad();
};

// HYPOTHESIS: a 32-byte message that fn_0046aa10 fills in: an id, one parameter, and six more
// fields of which three get initial values.
struct fn_0046aa10_message {
    fn_0046aa10_message() : id(0), param(-1), m_0x14(0), m_0x18(-1), m_0x1c(-1) {}
    int id;
    int param;
    int m_0x08;
    int m_0x0c;
    int m_0x10;
    int m_0x14;
    int m_0x18;
    int m_0x1c;
};

// HYPOTHESIS: value objects at +0x4c (0x2c bytes each), +0x174 and +0x234 (with a tString at
// +0x18); the member at +0x1b4 (destroyed by 0x0060a8b8) and the object at +0x22c (deleted
// through its virtual destructor) are otherwise unknown.
struct c_tPanelMgr_value2c : public tValueBase { char m_18[0x14]; };
struct c_tPanelMgr_value40 : public tValueBase { tString m_18; };
struct c_tPanelMgr_1b4 { ~c_tPanelMgr_1b4(); char m_00[0x18]; };
struct c_tPanelMgr_22c { virtual ~c_tPanelMgr_22c(); };

// HYPOTHESIS: only members the code touches or the destructor destroys are laid out.
class tPanelMgr : public tGameState {
public:
    virtual ~tPanelMgr();
    // HYPOTHESIS: virtual slots 1 to 18 are not known; fn_00608bd1 calls slots 19 and 20.
    virtual void vslot_0x04();
    virtual void vslot_0x08();
    virtual void vslot_0x0c();
    virtual void vslot_0x10();
    virtual void vslot_0x14();
    virtual void vslot_0x18();
    virtual void vslot_0x1c();
    virtual void vslot_0x20();
    virtual void vslot_0x24();
    virtual void vslot_0x28();
    virtual void vslot_0x2c();
    virtual void vslot_0x30();
    virtual void vslot_0x34();
    virtual void vslot_0x38();
    virtual void vslot_0x3c();
    virtual void vslot_0x40();
    virtual void vslot_0x44();
    virtual void vslot_0x48();
    virtual void vslot_0x4c();
    virtual bool vslot_0x50();
    tPanelMgr *scalar_deleting_destructor(unsigned char flags);
    void fn_006082e2();
    void fn_006082f9();
    void fn_00608493();
    void fn_006084e4();
    void fn_0060857a();
    void fn_006085bf();
    void fn_006086e9();
    void fn_00608766();

    bool vfunction4();
    void fn_0060a6fd();
    void fn_0060a796();
    void fn_006094a3();
    void fn_0060918d();
    void vfunction3();
    void fn_00609e57(int arg);
    void fn_006099f1();
    void fn_006094c7();
    void fn_0060a690();
    void fn_006091ef(int arg);
    void fn_00609cfc(fn_0046aa10_message msg);
    void fn_00609bb2();
    void fn_0060a730();
    void fn_00609bce();
    fn_005ed4ad_object *fn_0060a500();
    char *vfunction13();
    // HYPOTHESIS: 0x00424ee1 destroys 0x28-byte elements by a virtual destructor: strings.
    std::vector<tString> m_0x00c[4];
    c_tPanelMgr_value2c m_0x04c[4];
    // HYPOTHESIS: a flag, cleared by vfunction4.
    bool m_0x0fc;
    char m_0x0fd[0x03];
    fn_004d290d_object m_0x100;
    bool m_0x164;
    char m_0x165[0x0b];
    bool m_0x170;
    char m_0x171[0x03];
    c_tPanelMgr_value40 m_0x174;
    c_tPanelMgr_1b4 m_0x1b4;
    int m_0x1cc;
    fn_00571a69_vector m_0x1d0;
    fn_00425f12_vector m_0x1e0;
    fn_0060a8f0_container m_0x1f0;
    // HYPOTHESIS: the member at +0x200 is read through an inline accessor; the original loads
    // it into a register and tests it, which a direct comparison of the member does not give.
    int fn_inline_0x200() const { return m_0x200; }
    int m_0x200;
    fn_0060a951_container m_0x204;
    char m_0x214[0x04];
    int m_0x218;
    int m_0x21c;
    int m_0x220;
    bool m_0x224;
    bool m_0x225;
    char m_0x226[0x06];
    c_tPanelMgr_22c *m_0x22c;
    char m_0x230[0x04];
    c_tPanelMgr_value40 m_0x234;
    char m_0x274[0x0c];
    fn_0060abb1_vector m_0x280;
    bool m_0x290;
};

// FUNCTION: STARFLEETOP 0x00607faa
// MATCH-ONLY: the scalar deleting destructor is compiler-generated and has no source
// line, so the harness cannot claim it directly; it is written out as a member that
// calls the destructor, then deletes when bit 0 of the flag is set.
tPanelMgr *tPanelMgr::scalar_deleting_destructor(unsigned char flags)
{
    this->tPanelMgr::~tPanelMgr();
    if (flags & 1)
        ::operator delete(this);
    return this;
}

// FUNCTION: STARFLEETOP 0x00607fd5
tPanelMgr::~tPanelMgr()
{
    delete m_0x22c;
    m_0x22c = 0;
    fn_006082e2();
}

// HYPOTHESIS: an 8-byte table entry (two 32-bit fields of unknown meaning). Six other
// modules each report the byte size of their table and hand out its first entry; this
// function joins them into one table after four fixed entries, ending with a fifth fixed one.
struct fn_0060810e_entry {
    int a;
    int b;
};

int fn_005fecbf();
fn_0060810e_entry *fn_005feca4();
int fn_005fc79c();
fn_0060810e_entry *fn_005fc781();
int fn_005f5316();
fn_0060810e_entry *fn_005f52fb();
int fn_005f220a();
fn_0060810e_entry *fn_005f21ef();
int fn_00602764();
fn_0060810e_entry *fn_00602749();
int fn_006054e8();
fn_0060810e_entry *fn_006054cd();
extern fn_0060810e_entry g_0087b7d0[5];
extern fn_0060810e_entry *g_00c3a68c;
// FUNCTION: STARFLEETOP 0x0060810e
void fn_0060810e()
{
    int n1 = fn_005fecbf() / 8;
    int n2 = fn_005fc79c() / 8;
    int n3 = fn_005f5316() / 8;
    int n4 = fn_005f220a() / 8;
    int n5 = fn_00602764() / 8;
    int n6 = fn_006054e8() / 8;
    int i;
    int n;
    g_00c3a68c = new fn_0060810e_entry[n1 + n2 + n3 + n4 + n5 + n6 + 5];
    n = 0;
    for (i = 0; i < 4; i++)
        g_00c3a68c[n++] = g_0087b7d0[i];
    for (i = 0; i < n1; i++, n++)
        g_00c3a68c[n] = fn_005feca4()[i];
    for (i = 0; i < n2; i++, n++)
        g_00c3a68c[n] = fn_005fc781()[i];
    for (i = 0; i < n3; i++, n++)
        g_00c3a68c[n] = fn_005f52fb()[i];
    for (i = 0; i < n4; i++, n++)
        g_00c3a68c[n] = fn_005f21ef()[i];
    for (i = 0; i < n5; i++, n++)
        g_00c3a68c[n] = fn_00602749()[i];
    for (i = 0; i < n6; i++, n++)
        g_00c3a68c[n] = fn_006054cd()[i];
    g_00c3a68c[n] = g_0087b7d0[4];
}

// HYPOTHESIS: releases the table fn_0060810e builds.
// FUNCTION: STARFLEETOP 0x006082e2
void tPanelMgr::fn_006082e2()
{
    delete[] g_00c3a68c;
}

// HYPOTHESIS: this is the whole function that starts at 0x006082ef; the analysis records its
// exception-handling prolog (10 bytes) and its body (0x006082f9, 410 bytes) as two functions, so
// the body is scored against a build that also carries the prolog. Unless the member at +0x200
// is set, it makes one creator of each of six kinds and stores what each creator's inherited
// function (0x00607460) returns in the container at +0x1f0, under the keys 0 to 5.
// FUNCTION: STARFLEETOP 0x006082ef
void tPanelMgr::fn_006082f9()
{
    if (!fn_inline_0x200()) {
        tMetaClientMapPanelCreator mapCreator;
        m_0x1f0.fn_0060a8f0(0) = mapCreator.vfunction2();
        tMetaClientMissionPanelCreator missionCreator;
        m_0x1f0.fn_0060a8f0(1) = missionCreator.vfunction2();
        tMetaClientNewsPanelCreator newsCreator;
        m_0x1f0.fn_0060a8f0(2) = newsCreator.vfunction2();
        tMetaClientSupplyDockPanelCreator supplyDockCreator;
        m_0x1f0.fn_0060a8f0(3) = supplyDockCreator.vfunction2();
        tMetaClientShipPanelCreator shipCreator;
        m_0x1f0.fn_0060a8f0(4) = shipCreator.vfunction2();
        tMedalsPanelCreator medalsCreator;
        m_0x1f0.fn_0060a8f0(5) = medalsCreator.vfunction2();
    }
}

// HYPOTHESIS: the object each value in the container at +0x1f0 points to; only its virtual slot
// 10 (taking one int, called with 1) is known.
class fn_00608493_panel {
public:
    virtual void vfunction1();
    virtual void vfunction2();
    virtual void vfunction3();
    virtual void vfunction4();
    virtual void vfunction5();
    virtual void vfunction6();
    virtual void vfunction7();
    virtual void vfunction8();
    virtual void vfunction9();
    virtual void vfunction10();
    virtual void vfunction11(int flag);
    virtual void vfunction12();
    virtual void vfunction13();
    virtual void vfunction14();
    virtual void vfunction15();
    virtual void vfunction16();
};

// HYPOTHESIS: for every value in the container at +0x1f0, call that object's virtual slot 10
// with 1, then empty the container (0x0060ab15).
// FUNCTION: STARFLEETOP 0x00608493
void tPanelMgr::fn_00608493()
{
    for (fn_0060a8f0_container::iterator it = m_0x1f0.begin(); it != m_0x1f0.end(); ++it) {
        fn_00608493_panel *panel = (fn_00608493_panel *)(*it).second;
        if (panel)
            panel->vfunction11(1);
    }
    m_0x1f0.fn_0060ab15();
}

// HYPOTHESIS: this is the body of the function that starts at 0x006084da (see above). If the
// container at +0x1d0 is empty, it makes a player relay creator and a messenger relay creator and
// appends what each one's inherited function (0x00607460) returns.
// FUNCTION: STARFLEETOP 0x006084da
void tPanelMgr::fn_006084e4()
{
    if (m_0x1d0.size() == 0) {
        tPlayerRelayCCreator playerCreator;
        m_0x1d0.fn_00571a69(playerCreator.vfunction2());
        tMessengerRelayCreator messengerCreator;
        m_0x1d0.fn_00571a69(messengerCreator.vfunction2());
    }
}

// HYPOTHESIS: for every value in the vector at +0x1d0, call that object's virtual slot 10 with 1
// (as fn_00608493 does for the container at +0x1f0), then erase the whole range (0x00425ed6).
// FUNCTION: STARFLEETOP 0x0060857a
void tPanelMgr::fn_0060857a()
{
    for (void **it = m_0x1d0.begin(); it != m_0x1d0.end(); ++it) {
        fn_00608493_panel *panel = (fn_00608493_panel *)*it;
        if (panel)
            panel->vfunction11(1);
    }
    m_0x1d0.clear();
}

// HYPOTHESIS: this is the body of the function that starts at 0x006085b5 (see above). If the
// vector at +0x1e0 is empty, it makes a player info creator and a chat panel creator and appends
// what each one's inherited function (0x00607460) returns; when the member at +0x1cc is 5 it
// stores a player list panel under key 0 of the container at +0x204, and it always stores a help
// list panel under key 1.
// FUNCTION: STARFLEETOP 0x006085b5
void tPanelMgr::fn_006085bf()
{
    if (m_0x1e0.size() == 0) {
        tPlayerInfoPanelCreator playerInfoCreator;
        m_0x1e0.fn_00571a69(playerInfoCreator.vfunction2());
        tMetaClientChatPanelCreator chatCreator;
        m_0x1e0.fn_00571a69(chatCreator.vfunction2());
        if (m_0x1cc == 5) {
            tMetaClientPlayerListPanelCreator playerListCreator;
            m_0x204.fn_0060a951(0) = playerListCreator.vfunction2();
        }
        tMetaClientHelpListPanelCreator helpListCreator;
        m_0x204.fn_0060a951(1) = helpListCreator.vfunction2();
    }
}

// HYPOTHESIS: the counterpart of fn_006085bf: for every value in the vector at +0x1e0 and in the
// container at +0x204, call that object's virtual slot 10 with 1, then empty both.
// FUNCTION: STARFLEETOP 0x006086e9
void tPanelMgr::fn_006086e9()
{
    for (void **it = m_0x1e0.begin(); it != m_0x1e0.end(); ++it) {
        fn_00608493_panel *panel = (fn_00608493_panel *)*it;
        if (panel)
            panel->vfunction11(1);
    }
    m_0x1e0.clear();
    for (fn_0060a951_container::iterator it2 = m_0x204.begin(); it2 != m_0x204.end(); ++it2) {
        fn_00608493_panel *panel = (fn_00608493_panel *)(*it2).second;
        if (panel)
            panel->vfunction11(1);
    }
    m_0x204.fn_0060ac16();
}

#include "unsorted/tObject_0090b730.h"
// HYPOTHESIS: calls slots 86 and 109 of the object at 0x0092deb4, then slot 15 for every index
// below its slot 9 count, from the last down to the first, then the function of the global object
// at 0x0090b730. It does not use this.
// FUNCTION: STARFLEETOP 0x00608766
void tPanelMgr::fn_00608766()
{
    gObject_0092deb4->vfunction86();
    gObject_0092deb4->vfunction109();
    int count = gObject_0092deb4->vfunction9();
    while (count > 0) {
        gObject_0092deb4->vfunction15(--count);
    }
    gObject_0090b730.fn_0060cac6();
}

// HYPOTHESIS: the object each value in the vector at +0x1e0 points to; only its non-virtual
// function 0x00607676 (taking no argument) is known.
class fn_00607676_panel {
public:
    void fn_00607676();
};

// HYPOTHESIS: two globals passed by value to two free functions; their types are not known.
extern void *g_0087b69c;
extern void *g_0087b684;
void fn_004d3306(void *arg);
void fn_004d32c4(void *arg);
void fn_004d3e88();

// HYPOTHESIS: clears the flag at +0xfc, calls three members of this, then 0x00607676 on every
// object in the vector at +0x1e0; when the member at +0x1cc is 4 it passes the global at
// 0x0087b69c to 0x004d3306; then calls the member at +0x100, another member of this, passes the
// global at 0x0087b684 to 0x004d32c4, calls 0x004d3e88 and returns true.
// FUNCTION: STARFLEETOP 0x00608b4e
bool tPanelMgr::vfunction4()
{
    m_0x0fc = false;
    fn_0060a6fd();
    fn_0060a796();
    fn_006094a3();
    for (void **it = m_0x1e0.begin(); it != m_0x1e0.end(); ++it)
        ((fn_00607676_panel *)*it)->fn_00607676();
    if (m_0x1cc == 4)
        fn_004d3306(g_0087b69c);
    m_0x100.fn_004d290d();
    fn_0060918d();
    fn_004d32c4(g_0087b684);
    fn_004d3e88();
    return true;
}

// HYPOTHESIS: free functions and globals fn_00608bd1 uses; their types are not known beyond how
// they are called. The data at 0x0087bac8 is read as a C string, and 0x008443c0 and 0x0087baa0
// are passed by address.
bool fn_0046aa10(fn_0046aa10_message *msg);
void fn_004d31ed(void *arg, int a, int b, int width, int height);
extern void *g_0087b6a0;
extern void *g_0087b688;
extern void *g_0087b68c;
extern void *g_0087b6bc;
extern const char g_0087bac8[];
extern const char g_008443c0[];
extern const char g_0087baa0[];
class fn_005ef71a_object {
public:
    void fn_005ef71a();
};
fn_005ef71a_object *fn_005edc3b();
class fn_00600bd5_object {
public:
    void fn_00600bd5();
};
fn_00600bd5_object *fn_00600ab8();
class fn_0060a617_object {
public:
    int fn_0060a617();
};
fn_0060a617_object *fn_00607d61();
// HYPOTHESIS: the global objects at 0x0091f930 and 0x0091a298 share a test (0x004bda12).
class fn_004bda12_object {
public:
    bool fn_004bda12();
};
extern fn_004bda12_object g_0091f930;
extern fn_004bda12_object g_0091a298;
void fn_004be3e3();
void fn_004be0d0();
void fn_004bd534();

// HYPOTHESIS: the analysis records this function's body (1190 bytes) as 0x00608bd1, apart from its
// exception-handling prolog at 0x00608bc7. Built whole, it starts with that prolog, so the body
// cannot be scored at 0x00608bd1: every instruction sits 10 bytes off. Both addresses are listed in
// analysis/results/boundary_errors.tsv; this whole function is decoded from the body and carries no annotation until the
// analysis records 0x00608bc7 and 0x00608bd1 as one function.
// It pumps messages through fn_0046aa10: while the flag at +0x170 is set it only waits for
// message 0x10032 (or, the first time, opens a box); otherwise it dispatches each message by id,
// then updates the current panels of the containers at +0x1f0, +0x1e0 and +0x204 through their
// virtual slot 15.
void tPanelMgr::vfunction3()
{
    fn_0046aa10_message msg;

    if (m_0x224 && vslot_0x50())
        return;

    if (m_0x170) {
        if (m_0x164) {
            while (fn_0046aa10(&msg)) {
                if (msg.id == 0x10032) {
                    m_0x164 = false;
                    fn_004d32c4(g_0087b6a0);
                    vslot_0x4c();
                }
            }
        } else {
            m_0x164 = true;
            fn_004d31ed(g_0087b6a0, 0, 0, 0x280, 0x1e0);
            std::string text(g_0087bac8);
            m_0x100.fn_004d288f(g_008443c0)->fn_004d2573(text);
        }
        return;
    }

    fn_005edc3b()->fn_005ef71a();
    while (fn_0046aa10(&msg)) {
        if ((msg.id == 2 || msg.id == 0x100) && m_0x280.size() != 0) {
            m_0x280.clear();
            continue;
        }
        if (m_0x225) {
            switch (msg.id) {
            case 0x10003:
                fn_004d32c4(g_0087b688);
                vslot_0x4c();
                return;
            case 0x10004:
                m_0x225 = false;
                fn_004d32c4(g_0087b688);
                fn_00609e57(m_0x21c);
                break;
            }
            continue;
        }
        switch (msg.id) {
        case 0x10006:
            fn_006099f1();
            break;
        case 0x10005:
            fn_006094c7();
            break;
        case 0x10001:
            fn_00600ab8()->fn_00600bd5();
            m_0x21c = fn_00607d61()->fn_0060a617();
            fn_00609e57(5);
            fn_0060a690();
            fn_004d31ed(g_0087b688, 0, 0, 0x280, 0x1e0);
            m_0x225 = true;
            break;
        case 0x10000:
            fn_0060a690();
            break;
        case 0x202:
            fn_006091ef(msg.param);
            // fall through
        default:
            fn_00609cfc(msg);
            break;
        case 0x10007:
            fn_00609bb2();
            break;
        case 0x10035:
            if (m_0x218 != 1) {
                fn_0060a6fd();
                fn_006094a3();
            }
            fn_004d31ed(g_0087b6a0, 0, 0, 0x280, 0x1e0);
            m_0x100.fn_004d288f(g_008443c0)->fn_004d2521(g_0087baa0);
            m_0x290 = true;
            break;
        case 0x10032:
            if (m_0x290) {
                m_0x290 = false;
                fn_004d32c4(g_0087b6a0);
                fn_00609e57(!fn_0060a500()->fn_005ed4ad());
            } else {
                fn_00600ab8()->fn_00600bd5();
            }
            break;
        case 0x1000c:
            fn_0060a730();
            break;
        case 0x10008:
            fn_00609bce();
            break;
        }
    }

    if (g_0091f930.fn_004bda12()) {
        fn_004be3e3();
        fn_004be0d0();
    }
    if (g_0091a298.fn_004bda12())
        fn_004bd534();
    if (fn_0060a500())
        m_0x100.fn_004d269b(g_0087b68c, g_0087b6bc, !fn_0060a500()->fn_005ed4ad());

    fn_0060a8f0_container::iterator it = m_0x1f0.fn_0053aa7a(m_0x218);
    if (it != m_0x1f0.end())
        ((fn_00608493_panel *)(*it).second)->vfunction16();
    for (void **p = m_0x1e0.begin(); p != m_0x1e0.end(); ++p)
        ((fn_00608493_panel *)*p)->vfunction16();
    fn_0060a951_container::iterator it2 = m_0x204.fn_0053aa7a(m_0x220);
    if (it2 != m_0x204.end())
        ((fn_00608493_panel *)m_0x204.fn_0060a951(m_0x220))->vfunction16();
}

// FUNCTION: STARFLEETOP 0x00609077
char *tPanelMgr::vfunction13()
{
    return "PanelMgr";
}
