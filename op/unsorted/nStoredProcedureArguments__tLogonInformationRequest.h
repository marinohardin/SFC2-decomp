//	nStoredProcedureArguments__tLogonInformationRequest.h
//
//	Name from the exe's RTTI. Constructor 0x0054bcf1, destructor 0x0054bd28. Size 0x1c from a
//	local in the map initialize procedure.
//
#if !defined( nStoredProcedureArguments__tLogonInformationRequest_h )
#define nStoredProcedureArguments__tLogonInformationRequest_h

#include "unsorted/nSwitch__tRequestWithResponse.h"

namespace nStoredProcedureArguments
{

	// HYPOTHESIS: a request with response like its neighbours; the map initialize procedure
	// builds it with 3, whose meaning is not recovered.
	class tLogonInformationRequest : public nSwitch::tRequestWithResponse
	{
	public:
		tLogonInformationRequest( int32 param_1 );
		virtual ~tLogonInformationRequest();

		virtual	void StreamOut( nDataStore::tOutput& output ) const;
		virtual	void StreamIn( nDataStore::tInput& input );

	protected:
		int32 field_18;
	};

};

#endif
