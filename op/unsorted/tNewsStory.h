#ifndef TNEWSSTORY_H
#define TNEWSSTORY_H

// tNewsStory and its nested tDetailTime (names from RTTI). Only the members the matched
// code uses are declared.
// HYPOTHESIS: tNewsStory's bases (tStreamable, tAllocatedClass per its vtables) and data
// members are left out until a function needs them.

class fn_0040651e_type;

class tNewsStory
{
public:
	class tDetailTime
	{
	public:
		virtual ~tDetailTime();

		// HYPOTHESIS: an equality and an ordering test on two detail times.
		char fn_0054695e( const tDetailTime& other ) const;
		bool fn_00546986( const tDetailTime& other ) const;

	private:
		long m04;
		long m08;
		long m0c;
		long m10;
	};

	char fn_00547448() const;
	char fn_0054744c() const;
	tDetailTime fn_0054745f() const;
	fn_0040651e_type* fn_00547450( int ascii ) const;
};

// HYPOTHESIS: the text type returned by tNewsStory::fn_00547450; 0x0040651e compares two.
class fn_0040651e_type
{
public:
	int fn_0040651e( fn_0040651e_type* other );
};

#endif
