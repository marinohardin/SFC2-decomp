#ifndef TMETAMAPCONTROL_H
#define TMETAMAPCONTROL_H

// HYPOTHESIS: 0x1ec bytes, from the operator new in the factory at 0x005e2f60;
// the members are not recovered yet.
class tMetaMapControl
{
public:
	tMetaMapControl();

	char m_data[0x1ec];
};

#endif
