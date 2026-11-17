#ifndef TCHATCHANNEL_H
#define TCHATCHANNEL_H

// tChatChannel (name from RTTI). Only what the matched code uses is declared.
// HYPOTHESIS: the members are not known yet; the size is 0x2c, the stride of the game's
// std::for_each over a range of channels (0x005fbb74).

class tChatChannel
{
	char m_unknown[0x2c];
};

#endif
