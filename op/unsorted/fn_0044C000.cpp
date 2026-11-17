// Functions with no unit or class evidence on page 0x0044c000; original file unknown.

#include <stdlib.h>
#include <string.h>

// HYPOTHESIS: the callees below have no recovered class; declared by address.
class fn_0044c185_trait;
class fn_0044c185_object;

struct fn_0044c185_vec3
{
	float x;
	float y;
	float z;

	fn_0044c185_vec3();
	fn_0044c185_vec3(float ax, float ay, float az);
	fn_0044c185_vec3 & operator=(const fn_0044c185_vec3 & other);
	void FUN_0069b1d0(const fn_0044c185_vec3 * other);
	void FUN_0069b3d0();
	fn_0044c185_vec3 FUN_0069b270(float k) const;
};

// HYPOTHESIS: inline constructors and assignment; the original copies the
// vector field by field, not as a block.
inline fn_0044c185_vec3::fn_0044c185_vec3()
{
}

inline fn_0044c185_vec3::fn_0044c185_vec3(float ax, float ay, float az)
{
	x = ax;
	y = ay;
	z = az;
}

inline fn_0044c185_vec3 & fn_0044c185_vec3::operator=(const fn_0044c185_vec3 & other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

// HYPOTHESIS: a point and a direction, filled together by FUN_0044c276.
struct fn_0044c185_frame
{
	fn_0044c185_vec3 pos;
	fn_0044c185_vec3 dir;
};

extern void * FUN_0040fe6c(const char * name);
extern float FUN_0069b0e0();

extern float g_00797280;
extern float g_0079de70;

class fn_0044c185_trait
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4(int mode, fn_0044c185_frame * frame, float * scale);
};

// HYPOTHESIS: a named point set; slot 10 looks a point up by label.
class fn_0044c276_points
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual char vfunction10(fn_0044c185_vec3 * point, const char * label);
};

class fn_0044c185_object
{
public:
	virtual void vfunction0();
	virtual void vfunction1();
	virtual void vfunction2();
	virtual void vfunction3();
	virtual void vfunction4();
	virtual void vfunction5();
	virtual void vfunction6();
	virtual void vfunction7();
	virtual void vfunction8();
	virtual void vfunction9();
	virtual void vfunction10();
	virtual void vfunction11();
	virtual void vfunction12();
	virtual float vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18(fn_0044c185_trait * trait);

	fn_0044c276_points * FUN_004b94dc();
};

char FUN_0044c276(fn_0044c185_vec3 * point, fn_0044c185_object * object, const char * name);

// FUNCTION: STARFLEETOP 0x0044c185
void FUN_0044c185(fn_0044c185_object * object)
{
	fn_0044c185_trait * trait = (fn_0044c185_trait *)FUN_0040fe6c("tETrait_PlasmaVent");
	if (trait != 0)
	{
		float scale = object->vfunction13() * g_00797280;
		fn_0044c185_frame frame;
		if (FUN_0044c276(&frame.pos, object, "HardPoint"))
		{
			frame.dir.FUN_0069b1d0(&frame.pos);
			frame.dir.FUN_0069b3d0();
			frame.dir = frame.dir.FUN_0069b270(g_0079de70);
		}
		else
		{
			frame.pos = fn_0044c185_vec3(FUN_0069b0e0() * scale, FUN_0069b0e0() * scale, FUN_0069b0e0() * scale);
			frame.dir = frame.pos;
			frame.dir.FUN_0069b3d0();
			frame.dir = frame.dir.FUN_0069b270(g_0079de70);
		}
		trait->vfunction4(1, &frame, &scale);
		object->vfunction18(trait);
	}
}

// FUNCTION: STARFLEETOP 0x0044c276
char FUN_0044c276(fn_0044c185_vec3 * point, fn_0044c185_object * object, const char * name)
{
	fn_0044c276_points * points = object->FUN_004b94dc();
	if (points == 0)
	{
		return -1;
	}
	char label[80];
	strcpy(label, name);
	char * digits = label + strlen(label);
	int start = rand() % 20;
	fn_0044c185_vec3 * next = point;
	int found = 0;
	int index = start;
	do
	{
		_itoa(index, digits, 10);
		if (points->vfunction10(next, label))
		{
			found++;
			next++;
			if (found >= 2)
			{
				return 1;
			}
		}
		index++;
		if (index >= 20)
		{
			index = 0;
		}
	}
	while (index != start);
	return 0;
}
