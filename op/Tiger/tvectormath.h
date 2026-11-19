//	tvectormath.h: the point, matrix and angle types, as far as the matched units need them.
// Evidence:  the SDK header of the same name. Kept apart from unsorted/tEntity.h so a unit that
// needs only tPoint3 does not pull in the entity headers (and <vector>'s static initializers).

#ifndef	tvectormath_h
#define	tvectormath_h	1

#include "Tiger/tbasic.h"

// The SDK's tvectormath.h declares its point types inside pack(1), so a tPoint3 member has
// byte alignment; tEntity's point at +0x7a depends on it.
#pragma pack(push)
#pragma pack(1)

typedef short	tAngle;

struct tPoint3
{
public:
	float	x, y, z;

public:
	inline tPoint3(const float aX, const float aY, const float aZ)
	{
		x = aX; y = aY; z = aZ;
	}
	inline tPoint3(const tPoint3 &p)
	{
		x = p.x; y = p.y; z = p.z;
	}
	inline tPoint3()
	{
		// do nothing, leave uninitialized
	}

	inline float &operator[](const int i)              { return (&x)[i];	}
	inline void mSet(const float aX, const float aY, const float aZ)
	{
		x = aX; y = aY; z = aZ;
	}

	tPoint3 operator+(const tPoint3 &a) const;
	tPoint3 &operator+=(const tPoint3 &a);
	tPoint3 operator-(const tPoint3 &a) const;
	tPoint3 &operator-=(const tPoint3 &a);
	tPoint3 operator%(const tPoint3 &a) const;
	float operator*(const tPoint3 &a) const;
	tPoint3 operator*(const float d) const;

	inline tPoint3 &operator=(const tPoint3 &a)
	{
		x = a.x; y = a.y; z = a.z;
		return(*this);
	}

	float mLength(void) const;
	iTruth	mNormalize(void);
	tPoint3 &operator*=(const float d);
	tPoint3 &operator/=(const float d);
};

struct tPoint2
{
	union	{ float x; float u; };
	union	{ float y; float v; };
};

struct tPoint4
{
	float x, y, z, w;
};

// As the SDK declares it.
const double kPi = 3.14159265358979323846;

#pragma pack(pop)

typedef float	tMtx4x4[4][4];

// Only the members the entity and HUD units call are declared.
class tMatrix
{
protected:
	union
	{
		float		fM[4][4];
		tMtx4x4		fMtx;
	};
	unsigned char fIdentity;
	unsigned char f3x4;

public:
	enum eInitMode
	{
		kUninitialized  = 0,
		kIdentity3x4    = 1,
		kIdentity4x4    = 2
	};

	inline tMatrix(eInitMode initMode = tMatrix::kIdentity3x4)
	{
		if (initMode != tMatrix::kUninitialized)
		{
			mIdentity();
			f3x4 = initMode == kIdentity3x4;
		}
	}

	// MATCH-ONLY: the original defines this inline here, in the class. Its out-of-line copy
	// (STARFLEETOP 0x004b2ae6) is defined in unsorted/tMyHud3D.cpp, its only caller so far,
	// because the harness claims a function only from a .cpp file.
	inline tMatrix(
		const float xx, const float xy, const float xz, const float xw,
		const float yx, const float yy, const float yz, const float yw,
		const float zx, const float zy, const float zz, const float zw,
		const float wx, const float wy, const float wz, const float ww);

	inline tMtx4x4 &mGetMtx4x4(void)
	{
		fIdentity = kFalse;
		return(fMtx);
	}

	void mIdentity(void);
	void mRotZRadians(const float radians);
	void mTransform(tPoint3 &out, const tPoint3 &in) const;
	inline float mGetItem(const unsigned kRow, const unsigned kCol) const	{	return(fM[kRow][kCol]);	}
	void mRotX(const tAngle angle);
	void mRotY(const tAngle angle);
	void mRotZ(const tAngle angle);
	void mTranslate(const float x, const float y, const float z);
	void mMult(tMatrix &a, const tMatrix &b) const;
	void mForceTranslate(const float x, const float y, const float z);
	void mPostTranslate(const float x, const float y, const float z);
	void mTransform(tPoint3 &p) const;
	void mTransform(tPoint4 &out, const tPoint3 &in) const;
	void mTransform3x3(tPoint3 &p) const;
};



enum
{
	kAngle1		= 32,			// one angle (32 seconds per degree)
	kAngle90	= kAngle1 * 64,	// 64*32 seconds per 90 degrees
	kAngle180	= kAngle90 * 2,
	kAngle270	= kAngle90 * 3,
	kAngle360	= kAngle90 * 4,
	kAngleMask	= kAngle360 - 1
};

inline float Sine(const tAngle angle)
{
	extern float gSinCosTable[];
	return( gSinCosTable[ angle & kAngleMask ]);
}

inline float Cosine(const tAngle angle)
{
	extern float gSinCosTable[];
	return( gSinCosTable[ (angle & kAngleMask) + kAngle90 ] );
}

#endif	// tvectormath_h
