//	tListBox_News.h: tListBox_News and tListBox_News_Session, list boxes of news.
// Evidence:  class (RTTI, vftables 0x007a17dc and 0x007a18e8; original file unknown)

#ifndef tListBox_News_h
#define tListBox_News_h 1

#include "unsorted/tListBox_Base.h"

class tListBox_News : public tListBox_Base
{
public:
	// HYPOTHESIS: the slot at +0x104, the one tListBox_News adds to tListBox_Base's vftable;
	// 0x004bd534 calls it on the tListBox_News_Session it finds.
	virtual void vfunction66();
};

class tListBox_News_Session : public tListBox_News
{
public:
};

#endif	// tListBox_News_h
