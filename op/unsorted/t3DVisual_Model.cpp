#include "unsorted/t3DVisual.h"
#include "Tiger/tvectormath.h"
#include "Tiger/tbasicfile.h"
#include "dmp3d/common/stream_basicfile.h"
#include <ctype.h>
#include <string.h>
#include <vector>

// HYPOTHESIS: each visual is linked into the list headed at 0x008f5974. Member names
// are not known.
// HYPOTHESIS: m04 is t3DVisual's; the original stores it before the only vtable store,
// which the inlined base constructor explains once its own vtable store is dropped.

// HYPOTHESIS: the object at 0x008cc620 releases a handle through its second slot.
class tMgr_008cc620
{
public:
	virtual void vfunction1();
	virtual void vfunction2(int handle);
};

// HYPOTHESIS: one 0x24-byte entry of the array at +0x158.
struct tEntry_0042f918
{
	// HYPOTHESIS: an empty constructor; 0x004301e9's new[] keeps the count of its elided loop.
	tEntry_0042f918() {}

	unsigned char mFlags;
	char m01[0x13];
	int m14;
	int m18;
	int m1c;
	int m20;
};

// HYPOTHESIS: one 0x10-byte entry of the array at +0x17c: a point and the offset of its
// name in the string block at +0x154.
struct tEntry_0042fa82
{
	tPoint3 mPoint;
	int mName;
};

// HYPOTHESIS: the object 0x0042fd3f is given; slots 55 and 58 return matrices.
class tViewer_0042fd3f
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
	virtual void vfunction13();
	virtual void vfunction14();
	virtual void vfunction15();
	virtual void vfunction16();
	virtual void vfunction17();
	virtual void vfunction18();
	virtual void vfunction19();
	virtual void vfunction20();
	virtual void vfunction21();
	virtual void vfunction22();
	virtual void vfunction23();
	virtual void vfunction24();
	virtual void vfunction25();
	virtual void vfunction26();
	virtual void vfunction27();
	virtual void vfunction28();
	virtual void vfunction29();
	virtual void vfunction30();
	virtual void vfunction31();
	virtual void vfunction32();
	virtual void vfunction33();
	virtual void vfunction34();
	virtual void vfunction35();
	virtual void vfunction36();
	virtual void vfunction37();
	virtual void vfunction38();
	virtual void vfunction39();
	virtual void vfunction40();
	virtual void vfunction41();
	virtual void vfunction42();
	virtual void vfunction43();
	virtual void vfunction44();
	virtual void vfunction45();
	virtual void vfunction46();
	virtual void vfunction47();
	virtual void vfunction48();
	virtual void vfunction49();
	virtual void vfunction50();
	virtual void vfunction51();
	virtual void vfunction52();
	virtual void vfunction53();
	virtual void vfunction54();
	virtual tMatrix *vfunction55();
	virtual void vfunction56();
	virtual void vfunction57();
	virtual tMatrix *vfunction58();
};

// HYPOTHESIS: the polymorphic objects owned at +0x160.
class tPart_0042f918
{
public:
	virtual ~tPart_0042f918();
	virtual void vfunction1();
	// HYPOTHESIS: slot 2 takes the object, the entry array at +0x158 and the two values
	// 0x0042fd3f passes on.
	virtual void vfunction2(tViewer_0042fd3f *param_1, tEntry_0042f918 *param_2, float param_3, int param_4);
	// HYPOTHESIS: slot 3 takes the object, the entry array at +0x158 and the three values
	// 0x0042fddb passes on.
	virtual void vfunction3(tViewer_0042fd3f *param_1, tEntry_0042f918 *param_2, float param_3, float param_4, int param_5);
	// HYPOTHESIS: slot 4 takes the object, the entry array at +0x158 and the two values
	// 0x0042fe7e passes on.
	virtual void vfunction4(tViewer_0042fd3f *param_1, tEntry_0042f918 *param_2, int param_3, int param_4);
	// HYPOTHESIS: slot 5 takes the scale 0x0042fbd4 passes on.
	virtual void vfunction5(float scale);
	// HYPOTHESIS: slot 6 returns a value 0x0042fc61 passes on.
	virtual int vfunction6();
	// HYPOTHESIS: slot 7 takes two values and returns a bool, as 0x0042fc73 passes it on.
	virtual bool vfunction7(int param_1, int param_2);
	virtual void vfunction8();
	virtual void vfunction9();
	// HYPOTHESIS: slot 10 takes four values and returns a bool, as 0x0042febe passes it on.
	virtual bool vfunction10(int param_1, int param_2, int param_3, int param_4);
	// HYPOTHESIS: slot 11 takes the one value 0x00430476 passes on.
	virtual void vfunction11(int param_1);
	// HYPOTHESIS: slot 12 returns a value 0x004304ac passes on.
	virtual int vfunction12();
};

// HYPOTHESIS: the node list at +0x184; its destructor is 0x0042f467.
class tNode_0042f467
{
public:
	// HYPOTHESIS: 0x0042f421 copies the first length characters of name.
	tNode_0042f467(const char *name, int length);
	~tNode_0042f467();
	// HYPOTHESIS: 0x0042f488 appends the entry's point to the array.
	void fn_0042f488(tEntry_0042fa82 *entry);

	tNode_0042f467 *mNext;
	// HYPOTHESIS: a named array of points, as read by 0x0042fb2b.
	char *mName;
	tPoint3 *mPoints;
	unsigned int mCount;
	// HYPOTHESIS: 0x00430367 allocates 0x14 bytes per node; the last field is not read here.
	int m10;
};

class t3DVisual_Model : public t3DVisual
{
public:
	t3DVisual_Model();
	virtual ~t3DVisual_Model();
	void vfunction7(tViewer_0042fd3f *param_1, float param_2, int param_3);
	void vfunction8(tViewer_0042fd3f *param_1, float param_2, float param_3, int param_4);
	void vfunction9(tViewer_0042fd3f *param_1, int param_2, int param_3);
	// HYPOTHESIS: slot 10 of the vtable; declared non-virtual until the earlier slots are known.
	float vfunction10();
	bool vfunction11(tPoint3 *out, const char *name);
	int vfunction12();
	bool vfunction13(tPoint3 *out, const char *name, unsigned int index);
	unsigned int vfunction14(const char *name);
	void vfunction15(float scale);
	float vfunction16();
	int vfunction17(int index);
	void vfunction18();
	int vfunction19();
	int vfunction20();
	int vfunction21();
	bool vfunction22(int param_1, int param_2);
	int vfunction23(float time);
	void fn_0042fea4(unsigned int param_1);
	void fn_0042feb6();
	bool fn_0042febe(int param_1, int param_2, int param_3, int param_4, tEntry_0042f918 **param_5);
	bool fn_0042fef9(const char *name);
	// HYPOTHESIS: chunk readers 0x0042fef9 calls with the chunk size and the stream.
	tPart_0042f918 *fn_0042f121(unsigned long end, tStream_BasicFile *stream, tEntry_0042f918 *entries, char *strings);
	void fn_004301bd(unsigned long size, tStream_BasicFile *stream);
	void fn_004301e9(unsigned long size, tStream_BasicFile *stream);
	void fn_004302e8(unsigned long size, tStream_BasicFile *stream);
	void fn_00430367();
	void vfunction28(int param_1);
	int vfunction29(int param_1);

	int m08;
	t3DVisual_Model *mNext;
	t3DVisual_Model *mPrev;
	// HYPOTHESIS: the name the model was loaded from (0x0042fef9 copies it here).
	char m14[0x118 - 0x14];
	float m118;
	int m11c;
	int m120;
	int m124;
	float m128;
	float m12c[5];
	float m140[5];
	char *m154;
	tEntry_0042f918 *m158;
	int m15c;
	tPart_0042f918 *m160[5];
	int m174;
	float m178;
	tEntry_0042fa82 *m17c;
	int m180;
	tNode_0042f467 *m184;
};

extern t3DVisual_Model *g_008f5974;

// FUNCTION: STARFLEETOP 0x0042f858
t3DVisual_Model::t3DVisual_Model()
{
	m08 = 1;
	mNext = g_008f5974;
	mPrev = 0;
	g_008f5974 = this;
	if (mNext != 0)
		mNext->mPrev = this;
	m118 = 0.0f;
	m120 = -1;
	m128 = 0.0f;
	m14[0] = 0;
	m11c = 0;
	m124 = 0;
	for (int i = 0; i < 5; i++)
	{
		m12c[i] = 0.0f;
		m140[i] = 0.0f;
		m160[i] = 0;
	}
	m154 = 0;
	m158 = 0;
	m178 = 1.0f;
	m15c = 0;
	m174 = 0;
	m17c = 0;
	m180 = 0;
	m184 = 0;
}

extern tMgr_008cc620 *g_008cc620;

// FUNCTION: STARFLEETOP 0x0042f8fc
// FUNCTION: STARFLEETOP 0x0042f918
t3DVisual_Model::~t3DVisual_Model()
{
	if (m154 != 0)
	{
		for (int i = 0; i < m15c; i++)
		{
			tEntry_0042f918 *entry = &m158[i];
			if (entry->m14 != 0)
				g_008cc620->vfunction2(entry->m14);
			if ((entry->mFlags & 0x20) && entry->m1c != 0)
				g_008cc620->vfunction2(entry->m1c);
		}
	}
	for (int j = 0; j < 5; j++)
	{
		if (m160[j] != 0)
		{
			delete m160[j];
			m160[j] = 0;
		}
	}
	if (m154 != 0)
	{
		delete m154;
		m154 = 0;
	}
	if (m158 != 0)
	{
		delete m158;
		m158 = 0;
		m15c = 0;
	}
	if (m17c != 0)
	{
		delete m17c;
		m17c = 0;
		m180 = 0;
	}
	while (m184 != 0)
	{
		tNode_0042f467 *next = m184->mNext;
		delete m184;
		m184 = next;
	}
	if (mPrev != 0)
		mPrev->mNext = mNext;
	else
		g_008f5974 = mNext;
	if (mNext != 0)
		mNext->mPrev = mPrev;
}

// FUNCTION: STARFLEETOP 0x0042fa75
float t3DVisual_Model::vfunction10()
{
	return m178 * m118;
}

// FUNCTION: STARFLEETOP 0x0042fa82
bool t3DVisual_Model::vfunction11(tPoint3 *out, const char *name)
{
	if (out == 0)
		return false;
	out->x = 0.0f;
	out->y = 0.0f;
	out->z = 0.0f;
	if (name == 0 || m154 == 0)
		return false;
	for (int i = 0; i < m180; i++)
	{
		tEntry_0042fa82 *entry = &m17c[i];
		if (stricmp(m154 + entry->mName, name) == 0)
		{
			*out = entry->mPoint * m178;
			return true;
		}
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x0042fb24
int t3DVisual_Model::vfunction12()
{
	return m180;
}

// FUNCTION: STARFLEETOP 0x0042fb2b
bool t3DVisual_Model::vfunction13(tPoint3 *out, const char *name, unsigned int index)
{
	if (out == 0 || name == 0 || *name == 0)
		return false;
	out->x = 0.0f;
	out->y = 0.0f;
	out->z = 0.0f;
	for (tNode_0042f467 *node = m184; node != 0; node = node->mNext)
	{
		if (stricmp(node->mName, name) == 0)
		{
			if (index >= node->mCount)
				return false;
			*out = node->mPoints[index];
			return true;
		}
	}
	return false;
}

// FUNCTION: STARFLEETOP 0x0042fb9c
unsigned int t3DVisual_Model::vfunction14(const char *name)
{
	if (name != 0 && *name != 0)
	{
		for (tNode_0042f467 *node = m184; node != 0; node = node->mNext)
		{
			if (stricmp(node->mName, name) == 0)
				return node->mCount;
		}
	}
	return 0;
}

// FUNCTION: STARFLEETOP 0x0042fbd4
void t3DVisual_Model::vfunction15(float scale)
{
	m178 = scale;
	for (int i = 0; i < m11c; i++)
	{
		if (m160[i] != 0)
			m160[i]->vfunction5(scale);
	}
}

// FUNCTION: STARFLEETOP 0x0042fc18
float t3DVisual_Model::vfunction16()
{
	return m178;
}

// FUNCTION: STARFLEETOP 0x0042fc1f
int t3DVisual_Model::vfunction17(int index)
{
	if (index < 0 || index >= m11c)
		return m124;
	m120 = index;
	return index;
}

// FUNCTION: STARFLEETOP 0x0042fc40
void t3DVisual_Model::vfunction18()
{
	m120 = -1;
}

// FUNCTION: STARFLEETOP 0x0042fc48
int t3DVisual_Model::vfunction19()
{
	return m11c;
}

// FUNCTION: STARFLEETOP 0x0042fc4f
int t3DVisual_Model::vfunction20()
{
	int result = m120;
	if (result == -1)
		result = m124;
	return result;
}

// FUNCTION: STARFLEETOP 0x0042fc61
int t3DVisual_Model::vfunction21()
{
	if (m160[0] != 0)
		return m160[0]->vfunction6();
	return 0;
}

// FUNCTION: STARFLEETOP 0x0042fc73
bool t3DVisual_Model::vfunction22(int param_1, int param_2)
{
	if (m160[0] != 0)
		return m160[0]->vfunction7(param_1, param_2);
	return false;
}

// FUNCTION: STARFLEETOP 0x0042fc91
int t3DVisual_Model::vfunction23(float time)
{
	if (m11c <= 1)
		return 0;
	if (m120 != -1 && m120 < m11c)
		return m120;
	int result = m124;
	if (time > m128)
	{
		for (int i = result + 1; i < m11c; i++)
		{
			if (time > m12c[i])
				result = i;
			m128 = time;
		}
	}
	else if (time < m128)
	{
		for (int i = result - 1; i >= 0; i--)
		{
			if (time < m140[i])
				result = i;
			m128 = time;
		}
	}
	return result;
}

// FUNCTION: STARFLEETOP 0x0042fd3f
void t3DVisual_Model::vfunction7(tViewer_0042fd3f *param_1, float param_2, int param_3)
{
	int index = 0;
	if (m120 != -1)
		index = m120;
	else if (m11c > 1)
	{
		tMatrix *matrix = param_1->vfunction58();
		tPoint3 point(matrix->mGetItem(0, 3), matrix->mGetItem(1, 3), matrix->mGetItem(2, 3));
		param_1->vfunction55()->mTransform(point);
		// HYPOTHESIS: 0x0069b250 is the point's dot product with itself (the squared length).
		index = vfunction23(point * point);
		m124 = index;
	}
	if (m160[index] != 0)
		m160[index]->vfunction2(param_1, m158, param_2, param_3);
}

// FUNCTION: STARFLEETOP 0x0042fddb
void t3DVisual_Model::vfunction8(tViewer_0042fd3f *param_1, float param_2, float param_3, int param_4)
{
	int index = 0;
	if (m120 != -1)
		index = m120;
	else if (m11c > 1)
	{
		tMatrix *matrix = param_1->vfunction58();
		tPoint3 point(matrix->mGetItem(0, 3), matrix->mGetItem(1, 3), matrix->mGetItem(2, 3));
		param_1->vfunction55()->mTransform(point);
		index = vfunction23(point * point);
		m124 = index;
	}
	if (m160[index] != 0)
		m160[index]->vfunction3(param_1, m158, param_2, param_3, param_4);
}

// FUNCTION: STARFLEETOP 0x0042fe7e
void t3DVisual_Model::vfunction9(tViewer_0042fd3f *param_1, int param_2, int param_3)
{
	if (m160[0] != 0)
		m160[0]->vfunction4(param_1, m158, param_2, param_3);
}

// FUNCTION: STARFLEETOP 0x0042fea4
void t3DVisual_Model::fn_0042fea4(unsigned int param_1)
{
	m174 = param_1 | 0x80000000;
}

// FUNCTION: STARFLEETOP 0x0042feb6
void t3DVisual_Model::fn_0042feb6()
{
	m174 = 0;
}

// FUNCTION: STARFLEETOP 0x0042febe
bool t3DVisual_Model::fn_0042febe(int param_1, int param_2, int param_3, int param_4, tEntry_0042f918 **param_5)
{
	if (m160[0] != 0 && m160[0]->vfunction10(param_1, param_2, param_3, param_4))
	{
		*param_5 = m158;
		return true;
	}
	return false;
}

// HYPOTHESIS: a chunk header, the tag first and the size of the data after it.
struct tChunk_0042dd9b
{
	unsigned long mTag;
	unsigned long mSize;
};

void fn_0042dd9b(tStream_BasicFile *stream, tChunk_0042dd9b *chunk);

// FUNCTION: STARFLEETOP 0x0042fef9
bool t3DVisual_Model::fn_0042fef9(const char *name)
{
	tBasicFile *file = tBasicFile::mNewBasicFile();
	try
	{
		if (file == 0)
			throw "[Internal error] Could not create file object\n";
		if (!file->mOpenForRead(name))
			throw "Unable to open input file\n";

		tStream_BasicFile stream(file, true);
		tChunk_0042dd9b header;
		fn_0042dd9b(&stream, &header);
		if (header.mTag != 0x534c444d)
			throw "Model file seems corrupt\n";
		unsigned long end = (stream.fn_006a2da0() + header.mSize + 3) & ~3;

		unsigned long version;
		stream.fn_006a2f80(&version, 4);
		stream.fn_006a2f80(&m118, 4);
		stream.fn_006a2f80(&m11c, 4);
		if (m11c <= 0 || m11c >= 5)
			throw "Model has invalid number of LODs\n";
		if ((version & 0xffffff00) < 0x200)
			throw "Model has a format less than version 2.\n";

		float ranges[5];
		// MATCH-ONLY: the block lets base share its stack slot with the chunk read below, as
		// in the original.
		{
			float base;
			stream.fn_006a2f80(&base, 4);
			for (int i = 0; i < 5; i++)
				stream.fn_006a2f80(&ranges[i], 4);
			if (m11c > 1)
			{
				for (int j = 1; j < m11c; j++)
				{
					float outer = base + ranges[j];
					m12c[j] = outer * outer;
					float inner = ranges[j] - base;
					m140[j - 1] = inner * inner;
				}
			}
		}

		int part = 0;
		while (stream.fn_006a2da0() < end)
		{
			tChunk_0042dd9b chunk;
			fn_0042dd9b(&stream, &chunk);
			unsigned long start = stream.fn_006a2da0();
			switch (chunk.mTag)
			{
			case 0x53544e50:
				fn_004302e8(chunk.mSize, &stream);
				break;
			case 0x5354414d:
				fn_004301e9(chunk.mSize, &stream);
				break;
			case 0x53525453:
				fn_004301bd(chunk.mSize, &stream);
				break;
			case 0x30444f4d:
				if (part < m11c)
				{
					m160[part] = fn_0042f121(chunk.mSize + start, &stream, m158, m154);
					part++;
				}
				break;
			}
			stream.fn_006a2db0((chunk.mSize + start + 3) & ~3);
		}

		file->mClose();
		file = file->mDispose();
		strcpy(m14, name);
		fn_00430367();
		return true;
	}
	// HYPOTHESIS: the handlers sit past the analysed end (0x0042fef9 + 0x27b); their
	// 56 bytes up to the shared epilogue fit these two. The named variable matches the
	// original's frame size.
	catch (const char *error)
	{
		if (file != 0)
		{
			file->mClose();
			file->mDispose();
		}
		return false;
	}
	catch (fn_006a2ec0_Error error)
	{
		if (file != 0)
		{
			file->mClose();
			file->mDispose();
		}
		return false;
	}
}

// FUNCTION: STARFLEETOP 0x004301bd
void t3DVisual_Model::fn_004301bd(unsigned long size, tStream_BasicFile *stream)
{
	if (size != 0)
	{
		m154 = new char[size];
		stream->mGet(m154, size);
	}
}

// HYPOTHESIS: reads one entry of the array at +0x158 from the stream.
void fn_0042de34(tStream_BasicFile *stream, tEntry_0042f918 *entry);

// FUNCTION: STARFLEETOP 0x004301e9
void t3DVisual_Model::fn_004301e9(unsigned long size, tStream_BasicFile *stream)
{
	std::vector<tEntry_0042f918> entries;
	unsigned long start = stream->fn_006a2da0();
	while (stream->fn_006a2da0() - start < size)
	{
		tEntry_0042f918 entry;
		fn_0042de34(stream, &entry);
		entries.push_back(entry);
	}
	m15c = entries.size();
	if (m15c != 0)
		m158 = new tEntry_0042f918[m15c];
	else
		m158 = 0;
	if (m158 != 0)
	{
		for (unsigned int i = 0; i < entries.size(); i++)
			m158[i] = entries[i];
	}
}

// HYPOTHESIS: stream extraction operators. The point reader 0x0042ddc1 returns the stream,
// and the int reader is inlined: the original takes the int's address before calling the
// point reader, as an inline operator's argument would be.
tStream_BasicFile &operator>>(tStream_BasicFile &stream, tPoint3 &point);

// MATCH-ONLY: inlined in the original, so it has no address of its own.
inline tStream_BasicFile &operator>>(tStream_BasicFile &stream, int &value)
{
	stream.fn_006a2f80(&value, 4);
	return stream;
}

// FUNCTION: STARFLEETOP 0x004302e8
void t3DVisual_Model::fn_004302e8(unsigned long size, tStream_BasicFile *stream)
{
	m180 = size / sizeof(tEntry_0042fa82);
	if (m180 != 0)
	{
		m17c = new tEntry_0042fa82[m180];
		if (m17c != 0)
		{
			for (int i = 0; i < m180; i++)
			{
				*stream >> m17c[i].mPoint >> m17c[i].mName;
			}
		}
	}
}

// FUNCTION: STARFLEETOP 0x00430367
void t3DVisual_Model::fn_00430367()
{
	if (m154 != 0 && m180 != 0)
	{
		for (int i = 0; i < m180; i++)
		{
			char *name = m154 + m17c[i].mName;
			if (*name != 0)
			{
				int length = 0;
				char c = *name;
				while (c != 0)
				{
					if (isdigit(c))
						break;
					length++;
					c = name[length];
				}
				tNode_0042f467 *node;
				for (node = m184; node != 0; node = node->mNext)
				{
					if (strnicmp(node->mName, name, length) == 0)
						break;
				}
				if (node == 0)
				{
					node = new tNode_0042f467(name, length);
					if (node == 0)
						continue;
					node->mNext = m184;
					m184 = node;
				}
				node->fn_0042f488(&m17c[i]);
			}
		}
	}
}

// FUNCTION: STARFLEETOP 0x00430476
void t3DVisual_Model::vfunction28(int param_1)
{
	for (int i = 0; i < m11c; i++)
	{
		if (m160[i] != 0)
			m160[i]->vfunction11(param_1);
	}
}

// FUNCTION: STARFLEETOP 0x004304ac
int t3DVisual_Model::vfunction29(int param_1)
{
	if (param_1 < m11c && m160[param_1] != 0)
		return m160[param_1]->vfunction12();
	if (m160[0] != 0)
		return m160[0]->vfunction12();
	return 0;
}

// HYPOTHESIS: 0x0042f51a takes the one value 0x004304e4 passes on.
void fn_0042f51a(int param_1);

// FUNCTION: STARFLEETOP 0x004304e4
void fn_004304e4(int param_1)
{
	fn_0042f51a(param_1);
}
