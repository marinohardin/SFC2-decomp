//	MetaMapDataRequest.h
//
//	The map data request: the hexes of the meta map as the client receives them.
//	Only what the rebuilt functions need is declared yet.
//
#if !defined( MetaMapDataRequest_h )
#define MetaMapDataRequest_h

#pragma once

#include "Common/DataStore/Streamable.h"
#include "unsorted/nSwitch__tResponse.h"
#include <vector>

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
		class tResponse : public nSwitch::tResponse
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
				tClientHex& operator=( const tClientHex& other );

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

			tResponse( bool param_1 );
			tResponse( int32 param_1, int32 param_2, bool param_3 );
			virtual ~tResponse();

			virtual	void StreamOut( nDataStore::tOutput& output ) const;
			virtual	void StreamIn( nDataStore::tInput& input );

			void fn_0054c311( tMetaMapHex& hex, int32 maxEconomyValue, int32 maxVictoryValue );
			// HYPOTHESIS: the index of a hex in m_10 from its two coordinates.
			int32 fn_0054c4e3( long param_1, long param_2 );
			// HYPOTHESIS: the hex at two coordinates.
			tClientHex& fn_0054c471( long param_1, long param_2 );
			tClientHex* fn_0054c48b( int32* param_1, int32* param_2 );

			std::vector< tClientHex >	m_10;
			bool	m_20;
			int8u	m_21;
			int8u	m_22;
			int32	m_24;
			int32	m_28;
		};
	};

};

#endif
