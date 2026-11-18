#ifndef TOBJECT_0090B730_H
#define TOBJECT_0090B730_H

// HYPOTHESIS: the global object at 0x0090b730; only the non-virtual functions its callers
// use are known. Its class is the one 0x0060c7dc constructs, written out with its layout in
// fn_0060C000.cpp; the signatures here are that unit's.
class tObject_0090b730 {
public:
    long fn_005c7fb3();
    void fn_0060ca42(int value);
    int fn_0060ca4f();
    int fn_0060ca9e();
    void fn_0060cac6();
};

extern tObject_0090b730 gObject_0090b730;

#endif
