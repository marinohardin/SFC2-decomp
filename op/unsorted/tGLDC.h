//	tGLDC.h: the drawing context class built on the stack by the frame-rate overlay at 0x0046aa9a.
#ifndef tGLDC_h
#define tGLDC_h 1

// HYPOTHESIS: only the members this overlay calls are declared; the object is 0x9c bytes
// (stack slots of the caller). The real class has a vtable and more virtuals.
class tGLDC
{
public:
	char m_pad[0x9c];
	tGLDC();
	~tGLDC();
	int vfunction2(int *param_1);
	void vfunction9(unsigned int *param_1, unsigned int *param_2);
	void vfunction10(unsigned int *param_1, unsigned int *param_2);
	char vfunction13(unsigned int param_1);
	unsigned int vfunction16(unsigned int param_1);
	void vfunction22(unsigned int param_1);
};

#endif // tGLDC_h
