//	MetaMapDataRequest.h
//
//	The map data request: the hexes of the meta map as the client receives them.
//	Only what the rebuilt functions need is declared yet.
//
#if !defined( MetaMapDataRequest_h )
#define MetaMapDataRequest_h

#pragma once

#include "Common/DataStore/Streamable.h"

class tMetaMapHex;

// HYPOTHESIS: kMaxPointValue and kMax8BitNum are named by the assertion strings; their values
// are the constants the original compares with.
const int32 kMaxPointValue = 100;
const int32 kMax8BitNum = 255;

namespace nStoredProcedureArguments
{

	class tMetaMapDataRequest
	{
	public:
		class tResponse
		{
		public:
			class tClientHex : public nDataStore::tStreamable
			{
			public:
				tClientHex();
				tClientHex( tMetaMapHex& hex, int32 maxEconomyValue, int32 maxVictoryValue );

				virtual	void StreamOut( nDataStore::tOutput& output ) const;
				virtual	void StreamIn( nDataStore::tInput& input );

				void fn_0054c0d5();

				int8u	m_0c;
				int8u	m_0d;
				int32	m_10;
				bool	m_14;
				bool	m_15;
				char	m_16[0x18 - 0x16];
				float	m_18;
				float	m_1c;
				float	m_20;
				float	m_24;
				int8u	m_28;
				int8u	m_29;
				int8u	m_2a;
			};
		};
	};

};

#endif
