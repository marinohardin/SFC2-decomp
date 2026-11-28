#include "Common/Database/DatabaseID.h"
#include "unsorted/nSwitch__tRequestWithResponse.h"

// The constructor at 0x00555dc8 installs this RTTI-backed vtable and constructs
// the two tID members at +0x18 and +0x28. Stream input normalizes the byte at +0x38.
namespace nStoredProcedureArguments { class tSetPreparedMissionsIDRequest : public nSwitch::tRequestWithResponse
	{
	public:
		bool fn_00555f85() const;

	private:
		nDatabase::tID m_18;
		nDatabase::tID m_28;
		bool m_38;
	};
}

// FUNCTION: STARFLEETOP 0x00555f85
bool nStoredProcedureArguments::tSetPreparedMissionsIDRequest::fn_00555f85() const
{
	return m_38;
}
