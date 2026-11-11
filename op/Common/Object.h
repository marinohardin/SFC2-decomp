//	Object.h
//
//	A common base for classes that check their own integrity.
//	Declarations follow the SDK's header.
//
#if !defined( Object_h )
#define Object_h

#pragma once

class tObject
{
public:
	tObject();
	virtual ~tObject();

	bool Integrity() const;

protected:
	bool StartIntegrityCheck() const;
	virtual bool CheckIntegrity() const;
	void EndIntegrityCheck() const;

	mutable bool fCheckingIntegrity;
};

class tIntegrityCheck
{
public:
	tIntegrityCheck( const tObject* object );
	virtual ~tIntegrityCheck();

protected:
	const tObject* fObject;
};

#endif
