#ifndef TTRANSPORTERHOLDITEM_H
#define TTRANSPORTERHOLDITEM_H

class fn_0050d6a0_Stream;

// HYPOTHESIS: 20 bytes (a vtable and four dwords); the member layout is not known yet. The
// assignment copies only the last two dwords. The copy constructor is the compiler's.
struct tTransporterHoldItem
{
	virtual ~tTransporterHoldItem();
	int m_04;
	int m_08;
	int m_0c;
	int m_10;
	tTransporterHoldItem& operator=( const tTransporterHoldItem& other );
	// HYPOTHESIS: virtual slot 3 in the exe; declared non-virtual until slots 1 and 2 are known.
	void fn_004282f8( fn_0050d6a0_Stream* stream );
};

// FUNCTION: STARFLEETOP 0x00447f3f
inline tTransporterHoldItem& tTransporterHoldItem::operator=( const tTransporterHoldItem& other )
{
	m_0c = other.m_0c;
	m_10 = other.m_10;
	return *this;
}

#endif
