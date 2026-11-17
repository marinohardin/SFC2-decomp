#ifndef TTRANSPORTERHOLDITEM_H
#define TTRANSPORTERHOLDITEM_H

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
};

// FUNCTION: STARFLEETOP 0x00447f3f
inline tTransporterHoldItem& tTransporterHoldItem::operator=( const tTransporterHoldItem& other )
{
	m_0c = other.m_0c;
	m_10 = other.m_10;
	return *this;
}

#endif
