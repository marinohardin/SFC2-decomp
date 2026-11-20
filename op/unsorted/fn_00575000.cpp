#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBTimeProc/AsyncDatabaseProcedure_GetCurrentTurn.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBTimeProc/AsyncDatabaseProcedure_LockCurrentTurn.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBTimeProc/AsyncDatabaseProcedure_UnlockCurrentTurn.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBShipProc/AsyncDatabaseProcedure_RepairShips.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBCharacterProc/AsyncDatabaseProcedure_LogOffCharacter.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBCharacterProc/AsyncDatabaseProcedure_GetLoggedOnCharacterIDs.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBCharacterProc/AsyncDatabaseProcedure_GetFleetData.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBCharacterProc/AsyncDatabaseProcedure_GetCharacterPosition.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBCharacterProc/AsyncDatabaseProcedure_GetCharacterShipIDs.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBCharacterProc/AsyncDatabaseProcedure_GetCharacterPrestige.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBCharacterProc/AsyncDatabaseProcedure_GetCharacterOpenBidShipIDs.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBCharacterProc/AsyncDatabaseProcedure_GetCharacterRank.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBCharacterProc/AsyncDatabaseProcedure_GetCharacterMedals.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBCharacterProc/AsyncDatabaseProcedure_SetPreparedMissionsID.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBCharacterProc/AsyncDatabaseProcedure_ClearPreparedMissionsID.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBMissionProc/AsyncDatabaseProcedure_CheckExistencePreparedMission.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBMissionProc/AsyncDatabaseProcedure_UpdatePreparedMission.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBMissionProc/AsyncDatabaseProcedure_KillPreparedMission.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBShipProc/AsyncDatabaseProcedure_TransactionUpdateStores.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBMissionProc/AsyncDatabaseProcedure_AddPreparedMission.h"
#include "Meta/MetaDatabase/AsyncDatabaseProcedures/AsyncDBMissionProc/AsyncDatabaseProcedure_GetPreparedMission.h"
// HYPOTHESIS: the 0x48-byte object built here, and the owner holding the value at +0xd0, are unnamed
class fn_0057430c_object
{
public:
	fn_0057430c_object(void* arg);
	char m_pad[0x48];
};

class fn_00575066_object;

class fn_00575173_object;
class fn_005751b6_object;
class fn_005751f9_object;
class fn_0057523c_object;















class fn_00575023_owner
{
public:
	void fn_005750aa(nAsyncDatabaseProcedure::tGetCurrentTurn** out);
	void fn_005750ed(nAsyncDatabaseProcedure::tLockCurrentTurn** out);
	void fn_00575130(nAsyncDatabaseProcedure::tUnlockCurrentTurn** out);
	void fn_00575023(fn_0057430c_object** out);
	void fn_00575066(fn_00575066_object** out);
	void fn_00575173(fn_00575173_object** out);
	void fn_005751b6(fn_005751b6_object** out);
	void fn_005751f9(fn_005751f9_object** out);
	void fn_0057523c(fn_0057523c_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// HYPOTHESIS: the 0x4c-byte object built by fn_00575066 is unnamed; its constructor takes the value at +0xd0 and the owner
class fn_00575066_object
{
public:
	fn_00575066_object(void* arg, fn_00575023_owner* owner);
	char m_pad[0x4c];
};

// FUNCTION: STARFLEETOP 0x00575023
void fn_00575023_owner::fn_00575023(fn_0057430c_object** out)
{
	*out = new fn_0057430c_object(m_d0);
}

// FUNCTION: STARFLEETOP 0x00575066
void fn_00575023_owner::fn_00575066(fn_00575066_object** out)
{
	*out = new fn_00575066_object(m_d0, this);
}

// FUNCTION: STARFLEETOP 0x005750aa
void fn_00575023_owner::fn_005750aa(nAsyncDatabaseProcedure::tGetCurrentTurn** out)
{
	*out = new nAsyncDatabaseProcedure::tGetCurrentTurn( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}

// FUNCTION: STARFLEETOP 0x005750ed
void fn_00575023_owner::fn_005750ed(nAsyncDatabaseProcedure::tLockCurrentTurn** out)
{
	*out = new nAsyncDatabaseProcedure::tLockCurrentTurn( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}








// FUNCTION: STARFLEETOP 0x00575130
void fn_00575023_owner::fn_00575130(nAsyncDatabaseProcedure::tUnlockCurrentTurn** out)
{
	*out = new nAsyncDatabaseProcedure::tUnlockCurrentTurn( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}

// HYPOTHESIS: the 0x48-byte object built by fn_00575173 is unnamed
class fn_00575173_object
{
public:
	fn_00575173_object(void* arg);
	char m_pad[0x48];
};

// FUNCTION: STARFLEETOP 0x00575173
void fn_00575023_owner::fn_00575173(fn_00575173_object** out)
{
	*out = new fn_00575173_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_005751b6 is unnamed
class fn_005751b6_object
{
public:
	fn_005751b6_object(void* arg);
	char m_pad[0x48];
};

// FUNCTION: STARFLEETOP 0x005751b6
void fn_00575023_owner::fn_005751b6(fn_005751b6_object** out)
{
	*out = new fn_005751b6_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_005751f9 is unnamed
class fn_005751f9_object
{
public:
	fn_005751f9_object(void* arg);
	char m_pad[0x48];
};

// FUNCTION: STARFLEETOP 0x005751f9
void fn_00575023_owner::fn_005751f9(fn_005751f9_object** out)
{
	*out = new fn_005751f9_object(m_d0);
}

// HYPOTHESIS: the 0x98-byte object built by fn_0057523c is unnamed
class fn_0057523c_object
{
public:
	fn_0057523c_object(void* arg);
	char m_pad[0x98];
};

// FUNCTION: STARFLEETOP 0x0057523c
void fn_00575023_owner::fn_0057523c(fn_0057523c_object** out)
{
	*out = new fn_0057523c_object(m_d0);
}








class fn_00575282_owner
{
public:
	void fn_00575282(nAsyncDatabaseProcedure::tRepairShips** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575282
void fn_00575282_owner::fn_00575282(nAsyncDatabaseProcedure::tRepairShips** out)
{
	*out = new nAsyncDatabaseProcedure::tRepairShips( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}








class fn_005752c5_owner
{
public:
	void fn_005752c5(nAsyncDatabaseProcedure::tTransactionUpdateStores** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005752c5
void fn_005752c5_owner::fn_005752c5(nAsyncDatabaseProcedure::tTransactionUpdateStores** out)
{
	*out = new nAsyncDatabaseProcedure::tTransactionUpdateStores( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}

// HYPOTHESIS: the 0xa0-byte object built by fn_00575308 is unnamed
class fn_00575308_object
{
public:
	fn_00575308_object(void* arg);
	char m_pad[0xa0];
};

class fn_00575308_owner
{
public:
	void fn_00575308(fn_00575308_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575308
void fn_00575308_owner::fn_00575308(fn_00575308_object** out)
{
	*out = new fn_00575308_object(m_d0);
}

// HYPOTHESIS: the 0x234-byte object built by fn_0057534e is unnamed; its destructor is tBmpSurf's (0x0072eea0) or a related one
class fn_0057534e_object
{
public:
	fn_0057534e_object(void* arg);
	char m_pad[0x234];
};

class fn_0057534e_owner
{
public:
	void fn_0057534e(fn_0057534e_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x0057534e
void fn_0057534e_owner::fn_0057534e(fn_0057534e_object** out)
{
	*out = new fn_0057534e_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_00575394 is unnamed
class fn_00575394_object
{
public:
	fn_00575394_object(void* arg);
	char m_pad[0x48];
};

class fn_00575394_owner
{
public:
	void fn_00575394(fn_00575394_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575394
void fn_00575394_owner::fn_00575394(fn_00575394_object** out)
{
	*out = new fn_00575394_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_005753d7 is unnamed; it has a destructor, so the new-expression carries an EH frame
class fn_005753d7_object
{
public:
	fn_005753d7_object(void* arg);
	~fn_005753d7_object();
	char m_pad[0x48];
};

class fn_005753d7_owner
{
public:
	void fn_005753d7(fn_005753d7_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005753d7
void fn_005753d7_owner::fn_005753d7(fn_005753d7_object** out)
{
	*out = new fn_005753d7_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_0057541a is unnamed; its constructor is FUN_005642ca
class fn_0057541a_object
{
public:
	fn_0057541a_object(void* arg);
	~fn_0057541a_object();
	char m_pad[0x48];
};

class fn_0057541a_owner
{
public:
	void fn_0057541a(fn_0057541a_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x0057541a
void fn_0057541a_owner::fn_0057541a(fn_0057541a_object** out)
{
	*out = new fn_0057541a_object(m_d0);
}









class fn_0057545d_owner
{
public:
	void fn_0057545d(nAsyncDatabaseProcedure::tLogOffCharacter** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x0057545d
void fn_0057545d_owner::fn_0057545d(nAsyncDatabaseProcedure::tLogOffCharacter** out)
{
	*out = new nAsyncDatabaseProcedure::tLogOffCharacter( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}

// HYPOTHESIS: the object built by fn_005754a0 is unnamed
class fn_005754a0_object
{
public:
	fn_005754a0_object(void* arg);
	char m_pad[0x48];
};

class fn_005754a0_owner
{
public:
	void fn_005754a0(fn_005754a0_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005754a0
void fn_005754a0_owner::fn_005754a0(fn_005754a0_object** out)
{
	*out = new fn_005754a0_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_005754e3 is unnamed; its constructor is FUN_0056210d
class fn_005754e3_object
{
public:
	fn_005754e3_object(void* arg);
	~fn_005754e3_object();
	char m_pad[0x48];
};

class fn_005754e3_owner
{
public:
	void fn_005754e3(fn_005754e3_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005754e3
void fn_005754e3_owner::fn_005754e3(fn_005754e3_object** out)
{
	*out = new fn_005754e3_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_00575526 is unnamed; its constructor is FUN_00561106
class fn_00575526_object
{
public:
	fn_00575526_object(void* arg);
	char m_pad[0x48];
};

class fn_00575526_owner
{
public:
	void fn_00575526(fn_00575526_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575526
void fn_00575526_owner::fn_00575526(fn_00575526_object** out)
{
	*out = new fn_00575526_object(m_d0);
}

// HYPOTHESIS: the object built by fn_00575569 is unnamed; it has a destructor, so the new-expression carries an EH frame
class fn_00575569_object
{
public:
	fn_00575569_object(void* arg);
	~fn_00575569_object();
	char m_pad[0x48];
};

class fn_00575569_owner
{
public:
	void fn_00575569(fn_00575569_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575569
void fn_00575569_owner::fn_00575569(fn_00575569_object** out)
{
	*out = new fn_00575569_object(m_d0);
}










class fn_005755ac_owner
{
public:
	void fn_005755ac(nAsyncDatabaseProcedure::tGetLoggedOnCharacterIDs** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005755ac
void fn_005755ac_owner::fn_005755ac(nAsyncDatabaseProcedure::tGetLoggedOnCharacterIDs** out)
{
	*out = new nAsyncDatabaseProcedure::tGetLoggedOnCharacterIDs( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}

// HYPOTHESIS: the object built by fn_005755ef is unnamed; same shape as nAsyncDatabaseProcedure::tGetLoggedOnCharacterIDs
class fn_005755ef_object
{
public:
	fn_005755ef_object(void* arg);
	~fn_005755ef_object();
	char m_pad[0x48];
};

class fn_005755ef_owner
{
public:
	void fn_005755ef(fn_005755ef_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005755ef
void fn_005755ef_owner::fn_005755ef(fn_005755ef_object** out)
{
	*out = new fn_005755ef_object(m_d0);
}

// HYPOTHESIS: the object built by fn_00575632 is unnamed; constructor FUN_00562429, destructor tBmpSurf's
class fn_00575632_object
{
public:
	fn_00575632_object(void* arg);
	~fn_00575632_object();
	char m_pad[0x48];
};

class fn_00575632_owner
{
public:
	void fn_00575632(fn_00575632_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575632
void fn_00575632_owner::fn_00575632(fn_00575632_object** out)
{
	*out = new fn_00575632_object(m_d0);
}

// HYPOTHESIS: the object built by fn_00575675 is unnamed; same shape as fn_00575632_object
class fn_00575675_object
{
public:
	fn_00575675_object(void* arg);
	~fn_00575675_object();
	char m_pad[0x48];
};

class fn_00575675_owner
{
public:
	void fn_00575675(fn_00575675_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575675
void fn_00575675_owner::fn_00575675(fn_00575675_object** out)
{
	*out = new fn_00575675_object(m_d0);
}

// HYPOTHESIS: the object built by fn_005756b8 is unnamed; same shape as fn_00575675_object, 0x58 bytes
class fn_005756b8_object
{
public:
	fn_005756b8_object(void* arg);
	~fn_005756b8_object();
	char m_pad[0x58];
};

class fn_005756b8_owner
{
public:
	void fn_005756b8(fn_005756b8_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005756b8
void fn_005756b8_owner::fn_005756b8(fn_005756b8_object** out)
{
	*out = new fn_005756b8_object(m_d0);
}

// HYPOTHESIS: the object built by fn_005756fb is unnamed; same shape as fn_00575675_object
class fn_005756fb_object
{
public:
	fn_005756fb_object(void* arg);
	~fn_005756fb_object();
	char m_pad[0x48];
};

class fn_005756fb_owner
{
public:
	void fn_005756fb(fn_005756fb_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005756fb
void fn_005756fb_owner::fn_005756fb(fn_005756fb_object** out)
{
	*out = new fn_005756fb_object(m_d0);
}

// HYPOTHESIS: the object built by fn_0057573e is unnamed; 0x48 bytes like fn_005756fb_object
class fn_0057573e_object
{
public:
	fn_0057573e_object(void* arg);
	~fn_0057573e_object();
	char m_pad[0x48];
};

class fn_0057573e_owner
{
public:
	void fn_0057573e(fn_0057573e_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x0057573e
void fn_0057573e_owner::fn_0057573e(fn_0057573e_object** out)
{
	*out = new fn_0057573e_object(m_d0);
}

// HYPOTHESIS: the object built by fn_00575781 is unnamed; 0x48 bytes like fn_0057573e_object
class fn_00575781_object
{
public:
	fn_00575781_object(void* arg);
	~fn_00575781_object();
	char m_pad[0x48];
};

class fn_00575781_owner
{
public:
	void fn_00575781(fn_00575781_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575781
void fn_00575781_owner::fn_00575781(fn_00575781_object** out)
{
	*out = new fn_00575781_object(m_d0);
}









class fn_005757c4_owner
{
public:
	void fn_005757c4(nAsyncDatabaseProcedure::tGetFleetData** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005757c4
void fn_005757c4_owner::fn_005757c4(nAsyncDatabaseProcedure::tGetFleetData** out)
{
	*out = new nAsyncDatabaseProcedure::tGetFleetData( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}









class fn_00575807_owner
{
public:
	void fn_00575807(nAsyncDatabaseProcedure::tGetCharacterPosition** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575807
void fn_00575807_owner::fn_00575807(nAsyncDatabaseProcedure::tGetCharacterPosition** out)
{
	*out = new nAsyncDatabaseProcedure::tGetCharacterPosition( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}









class fn_0057584a_owner
{
public:
	void fn_0057584a(nAsyncDatabaseProcedure::tGetCharacterShipIDs** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x0057584a
void fn_0057584a_owner::fn_0057584a(nAsyncDatabaseProcedure::tGetCharacterShipIDs** out)
{
	*out = new nAsyncDatabaseProcedure::tGetCharacterShipIDs( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}









class fn_0057588d_owner
{
public:
	void fn_0057588d(nAsyncDatabaseProcedure::tGetCharacterPrestige** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x0057588d
void fn_0057588d_owner::fn_0057588d(nAsyncDatabaseProcedure::tGetCharacterPrestige** out)
{
	*out = new nAsyncDatabaseProcedure::tGetCharacterPrestige( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}









class fn_005758d0_owner
{
public:
	void fn_005758d0(nAsyncDatabaseProcedure::tGetCharacterOpenBidShipIDs** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005758d0
void fn_005758d0_owner::fn_005758d0(nAsyncDatabaseProcedure::tGetCharacterOpenBidShipIDs** out)
{
	*out = new nAsyncDatabaseProcedure::tGetCharacterOpenBidShipIDs( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}









class fn_00575913_owner
{
public:
	void fn_00575913(nAsyncDatabaseProcedure::tGetCharacterRank** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575913
void fn_00575913_owner::fn_00575913(nAsyncDatabaseProcedure::tGetCharacterRank** out)
{
	*out = new nAsyncDatabaseProcedure::tGetCharacterRank( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}









class fn_00575956_owner
{
public:
	void fn_00575956(nAsyncDatabaseProcedure::tGetCharacterMedals** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575956
void fn_00575956_owner::fn_00575956(nAsyncDatabaseProcedure::tGetCharacterMedals** out)
{
	*out = new nAsyncDatabaseProcedure::tGetCharacterMedals( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}









class fn_00575999_owner
{
public:
	void fn_00575999(nAsyncDatabaseProcedure::tSetPreparedMissionsID** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575999
void fn_00575999_owner::fn_00575999(nAsyncDatabaseProcedure::tSetPreparedMissionsID** out)
{
	*out = new nAsyncDatabaseProcedure::tSetPreparedMissionsID( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}









class fn_005759dc_owner
{
public:
	void fn_005759dc(nAsyncDatabaseProcedure::tClearPreparedMissionsID** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x005759dc
void fn_005759dc_owner::fn_005759dc(nAsyncDatabaseProcedure::tClearPreparedMissionsID** out)
{
	*out = new nAsyncDatabaseProcedure::tClearPreparedMissionsID( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}

// HYPOTHESIS: the 0x48-byte object built by fn_00575a1f is unnamed; it has a destructor, so the new-expression carries an EH frame
class fn_00575a1f_object
{
public:
	fn_00575a1f_object(void* arg);
	~fn_00575a1f_object();
	char m_pad[0x48];
};

class fn_00575a1f_owner
{
public:
	void fn_00575a1f(fn_00575a1f_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575a1f
void fn_00575a1f_owner::fn_00575a1f(fn_00575a1f_object** out)
{
	*out = new fn_00575a1f_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_00575a62 is unnamed; same shape as fn_00575a1f_object
class fn_00575a62_object
{
public:
	fn_00575a62_object(void* arg);
	~fn_00575a62_object();
	char m_pad[0x48];
};

class fn_00575a62_owner
{
public:
	void fn_00575a62(fn_00575a62_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575a62
void fn_00575a62_owner::fn_00575a62(fn_00575a62_object** out)
{
	*out = new fn_00575a62_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_00575aa5 is unnamed; same shape as fn_00575a62_object
class fn_00575aa5_object
{
public:
	fn_00575aa5_object(void* arg);
	~fn_00575aa5_object();
	char m_pad[0x48];
};

class fn_00575aa5_owner
{
public:
	void fn_00575aa5(fn_00575aa5_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575aa5
void fn_00575aa5_owner::fn_00575aa5(fn_00575aa5_object** out)
{
	*out = new fn_00575aa5_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_00575ae8 is unnamed; same shape as fn_00575aa5_object
class fn_00575ae8_object
{
public:
	fn_00575ae8_object(void* arg);
	~fn_00575ae8_object();
	char m_pad[0x48];
};

class fn_00575ae8_owner
{
public:
	void fn_00575ae8(fn_00575ae8_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575ae8
void fn_00575ae8_owner::fn_00575ae8(fn_00575ae8_object** out)
{
	*out = new fn_00575ae8_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_00575b2b is unnamed; same shape as fn_00575ae8_object
class fn_00575b2b_object
{
public:
	fn_00575b2b_object(void* arg);
	~fn_00575b2b_object();
	char m_pad[0x48];
};

class fn_00575b2b_owner
{
public:
	void fn_00575b2b(fn_00575b2b_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575b2b
void fn_00575b2b_owner::fn_00575b2b(fn_00575b2b_object** out)
{
	*out = new fn_00575b2b_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_00575b6e is unnamed; same shape as fn_00575b2b_object
class fn_00575b6e_object
{
public:
	fn_00575b6e_object(void* arg);
	~fn_00575b6e_object();
	char m_pad[0x48];
};

class fn_00575b6e_owner
{
public:
	void fn_00575b6e(fn_00575b6e_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575b6e
void fn_00575b6e_owner::fn_00575b6e(fn_00575b6e_object** out)
{
	*out = new fn_00575b6e_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_00575bb1 is unnamed; same shape as fn_00575b6e_object
class fn_00575bb1_object
{
public:
	fn_00575bb1_object(void* arg);
	~fn_00575bb1_object();
	char m_pad[0x48];
};

class fn_00575bb1_owner
{
public:
	void fn_00575bb1(fn_00575bb1_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575bb1
void fn_00575bb1_owner::fn_00575bb1(fn_00575bb1_object** out)
{
	*out = new fn_00575bb1_object(m_d0);
}

// HYPOTHESIS: the object built by fn_00575bf4 is unnamed; same shape as fn_00575bb1_object
class fn_00575bf4_object
{
public:
	fn_00575bf4_object(void* arg);
	~fn_00575bf4_object();
	char m_pad[0x48];
};

class fn_00575bf4_owner
{
public:
	void fn_00575bf4(fn_00575bf4_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575bf4
void fn_00575bf4_owner::fn_00575bf4(fn_00575bf4_object** out)
{
	*out = new fn_00575bf4_object(m_d0);
}

// HYPOTHESIS: the object built by fn_00575c37 is unnamed; same shape as fn_00575bf4_object
class fn_00575c37_object
{
public:
	fn_00575c37_object(void* arg);
	~fn_00575c37_object();
	char m_pad[0x48];
};

class fn_00575c37_owner
{
public:
	void fn_00575c37(fn_00575c37_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575c37
void fn_00575c37_owner::fn_00575c37(fn_00575c37_object** out)
{
	*out = new fn_00575c37_object(m_d0);
}

// HYPOTHESIS: the 0x1f4-byte object built by fn_00575c7a is unnamed
class fn_00575c7a_object
{
public:
	fn_00575c7a_object(void* arg);
	char m_pad[0x1f4];
};

class fn_00575c7a_owner
{
public:
	void fn_00575c7a(fn_00575c7a_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575c7a
void fn_00575c7a_owner::fn_00575c7a(fn_00575c7a_object** out)
{
	*out = new fn_00575c7a_object(m_d0);
}

// HYPOTHESIS: the 0x48-byte object built by fn_00575cc0 is unnamed; constructor FUN_00565ce5
class fn_00575cc0_object
{
public:
	fn_00575cc0_object(void* arg);
	char m_pad[0x48];
};

class fn_00575cc0_owner
{
public:
	void fn_00575cc0(fn_00575cc0_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575cc0
void fn_00575cc0_owner::fn_00575cc0(fn_00575cc0_object** out)
{
	*out = new fn_00575cc0_object(m_d0);
}

// HYPOTHESIS: the object built by fn_00575d03 is unnamed; constructor FUN_00565b49
class fn_00575d03_object
{
public:
	fn_00575d03_object(void* arg);
	char m_pad[0x48];
};

class fn_00575d03_owner
{
public:
	void fn_00575d03(fn_00575d03_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575d03
void fn_00575d03_owner::fn_00575d03(fn_00575d03_object** out)
{
	*out = new fn_00575d03_object(m_d0);
}









class fn_00575d46_owner
{
public:
	void fn_00575d46(nAsyncDatabaseProcedure::tCheckExistencePreparedMission** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575d46
void fn_00575d46_owner::fn_00575d46(nAsyncDatabaseProcedure::tCheckExistencePreparedMission** out)
{
	*out = new nAsyncDatabaseProcedure::tCheckExistencePreparedMission( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}









class fn_00575d89_owner
{
public:
	void fn_00575d89(nAsyncDatabaseProcedure::tAddPreparedMission** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575d89
void fn_00575d89_owner::fn_00575d89(nAsyncDatabaseProcedure::tAddPreparedMission** out)
{
	*out = new nAsyncDatabaseProcedure::tAddPreparedMission( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}









class fn_00575dcc_owner
{
public:
	void fn_00575dcc(nAsyncDatabaseProcedure::tUpdatePreparedMission** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575dcc
void fn_00575dcc_owner::fn_00575dcc(nAsyncDatabaseProcedure::tUpdatePreparedMission** out)
{
	*out = new nAsyncDatabaseProcedure::tUpdatePreparedMission( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}









class fn_00575e0f_owner
{
public:
	void fn_00575e0f(nAsyncDatabaseProcedure::tGetPreparedMission** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575e0f
void fn_00575e0f_owner::fn_00575e0f(nAsyncDatabaseProcedure::tGetPreparedMission** out)
{
	*out = new nAsyncDatabaseProcedure::tGetPreparedMission( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}









class fn_00575e52_owner
{
public:
	void fn_00575e52(nAsyncDatabaseProcedure::tKillPreparedMission** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575e52
void fn_00575e52_owner::fn_00575e52(nAsyncDatabaseProcedure::tKillPreparedMission** out)
{
	*out = new nAsyncDatabaseProcedure::tKillPreparedMission( (nAsyncDatabaseProcedure::tAsyncDatabase*)m_d0 );
}

// HYPOTHESIS: the object built by fn_00575e95 is unnamed; same shape as nAsyncDatabaseProcedure::tKillPreparedMission
class fn_00575e95_object
{
public:
	fn_00575e95_object(void* arg);
	~fn_00575e95_object();
	char m_pad[0x48];
};

class fn_00575e95_owner
{
public:
	void fn_00575e95(fn_00575e95_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575e95
void fn_00575e95_owner::fn_00575e95(fn_00575e95_object** out)
{
	*out = new fn_00575e95_object(m_d0);
}

// HYPOTHESIS: the object built by fn_00575ed8 is unnamed; same shape as fn_00575e95_object
class fn_00575ed8_object
{
public:
	fn_00575ed8_object(void* arg);
	~fn_00575ed8_object();
	char m_pad[0x60];
};

class fn_00575ed8_owner
{
public:
	void fn_00575ed8(fn_00575ed8_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575ed8
void fn_00575ed8_owner::fn_00575ed8(fn_00575ed8_object** out)
{
	*out = new fn_00575ed8_object(m_d0);
}

// HYPOTHESIS: the object built by fn_00575f1b is unnamed; same shape as fn_00575ed8_object, 0x48 bytes
class fn_00575f1b_object
{
public:
	fn_00575f1b_object(void* arg);
	~fn_00575f1b_object();
	char m_pad[0x48];
};

class fn_00575f1b_owner
{
public:
	void fn_00575f1b(fn_00575f1b_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575f1b
void fn_00575f1b_owner::fn_00575f1b(fn_00575f1b_object** out)
{
	*out = new fn_00575f1b_object(m_d0);
}

// HYPOTHESIS: the object built by fn_00575f5e is unnamed; same shape as fn_00575f1b_object, 0x48 bytes
class fn_00575f5e_object
{
public:
	fn_00575f5e_object(void* arg);
	~fn_00575f5e_object();
	char m_pad[0x48];
};

class fn_00575f5e_owner
{
public:
	void fn_00575f5e(fn_00575f5e_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575f5e
void fn_00575f5e_owner::fn_00575f5e(fn_00575f5e_object** out)
{
	*out = new fn_00575f5e_object(m_d0);
}

// HYPOTHESIS: the object built by fn_00575fa1 is unnamed; same shape as fn_00575632_object
class fn_00575fa1_object
{
public:
	fn_00575fa1_object(void* arg);
	~fn_00575fa1_object();
	char m_pad[0x48];
};

class fn_00575fa1_owner
{
public:
	void fn_00575fa1(fn_00575fa1_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575fa1
void fn_00575fa1_owner::fn_00575fa1(fn_00575fa1_object** out)
{
	*out = new fn_00575fa1_object(m_d0);
}

// HYPOTHESIS: the object built by fn_00575fe4 is unnamed; same shape as fn_00575fa1_object, 0x48 bytes
class fn_00575fe4_object
{
public:
	fn_00575fe4_object(void* arg);
	~fn_00575fe4_object();
	char m_pad[0x48];
};

class fn_00575fe4_owner
{
public:
	void fn_00575fe4(fn_00575fe4_object** out);
	char m_pad[0xd0];
	void* m_d0;
};

// FUNCTION: STARFLEETOP 0x00575fe4
void fn_00575fe4_owner::fn_00575fe4(fn_00575fe4_object** out)
{
	*out = new fn_00575fe4_object(m_d0);
}
