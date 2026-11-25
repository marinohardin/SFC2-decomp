struct tS {
	char a;
	char b;
	char c;
	char d;
};

// HYPOTHESIS: constructor of tAIUnitData (no header yet); members and tS are guessed from the stores
class tX {
public:
	tS f0;
	int f4;
	int f8;
	char fc;
	int f10;
	int f14;
	int f18;
	int f1c;
	int f20;
	int f24;
	char f28;
	int f2c;
	int f30;
	int f34;
	void fn_00520ecf(int a, int b, tS c, int d);
};

// FUNCTION: STARFLEETOP 0x00520ecf
void tX::fn_00520ecf(int a, int b, tS c, int d)
{
	f0 = c;
	f4 = a;
	f8 = b;
	fc = c.d;
	f10 = 0;
	f14 = 0;
	f18 = 0;
	f24 = d;
	f28 = c.d;
	f2c = 0;
	f30 = 0;
	f34 = 0;
	f1c = 0;
	f20 = 0;
}
