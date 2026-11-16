// tMemoryPart.h: Tiger's memory part, as far as the sound units use it (name from the exe's
// RTTI; its type descriptor is 0x0083c608 and the cache of its binding 0x008e0b54).

#ifndef TMEMORYPART_H
#define TMEMORYPART_H

#include "Tiger/tpart.h"

// HYPOTHESIS: Source/entity/entity_physical.cpp declares this class with its binding's claim
// (0x004133d3), which cannot move here yet; this restatement stands in for the Tiger units until
// it does, so it stays on one line. Its slots follow that copy: slot 10 (offset 0x28) frees a
// block, from tAudioPriorityManagerPart::mClose (0x00662540) passing it its array's tables.
class tMemoryPart : public tPart { public: tMemoryPart() {} GenAbstractBinding(tMemoryPart); virtual void vfunction6(); virtual void *mAllocate(int, int, const char *); virtual void *vfunction8(void *, int); virtual void vfunction9(); virtual void vfunction10(void *); };

#endif // TMEMORYPART_H
