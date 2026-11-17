//	tPoliticalTensionMatrix.h
//
//	A database object whose name and nested classes come from the exe's RTTI
//	(tPoliticalTensionMatrix, tPoliticalTensionVector vtable 0x00798214,
//	tPoliticalTension vtable 0x00798224).
//
#if !defined( tPoliticalTensionMatrix_h )
#define tPoliticalTensionMatrix_h

#include <vector>

#include "Common/BaseTypes.h"
#include "Common/DataStore/Streamable.h"
#include "Common/Database/DatabaseObject.h"

// HYPOTHESIS: the matrix's own members and virtuals are not recovered.
class tPoliticalTensionMatrix : public nDatabase::tDatabaseObject
{
public:
	class tPoliticalTensionVector : public nDataStore::tStreamable
	{
	public:
		// HYPOTHESIS: layout not recovered past the tStreamable base.
		class tPoliticalTension : public nDataStore::tStreamable
		{
		public:
			virtual	void StreamOut( nDataStore::tOutput& output ) const;
			virtual	void StreamIn( nDataStore::tInput& input );

			// HYPOTHESIS: 0x18 bytes in all, from the element stride of the vector copy at
			// 0x00427740; the three members' types are not recovered.
			int32u	field_0xc;
			int32u	field_0x10;
			int32u	field_0x14;
		};

		tPoliticalTensionVector( const tPoliticalTensionVector& source );
		virtual ~tPoliticalTensionVector();

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

		// HYPOTHESIS: layout read from the copy constructor at 0x004273eb: one scalar,
		// then a 16-byte member copied out of line (0x00427740), taken as a std::vector.
		int32u								field_0xc;
		std::vector< tPoliticalTension >	field_0x10;
	};
};

#endif
