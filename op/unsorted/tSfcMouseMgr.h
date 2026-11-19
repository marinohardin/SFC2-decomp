#ifndef TSFCMOUSEMGR_H
#define TSFCMOUSEMGR_H

// tSfcMouseMgr: the game's mouse manager base class.
// HYPOTHESIS: the class has a virtual destructor; only its vtable pointer is known.
class tSfcMouseMgr {
public:
    virtual ~tSfcMouseMgr();
};

#endif
