#ifndef COMMON_COMMANDLINEARGS_H
#define COMMON_COMMANDLINEARGS_H

#include <iosfwd>

#include "Common/AutoPtr.h"

class tCommandLineArgs
{
	public:
		static tCommandLineArgs *Instance(void);
		void Setup(const std::string &commandLine);

	private:
		typedef std::auto_ptr<tCommandLineArgs> iAutoPtr;
		friend iAutoPtr;

		tCommandLineArgs();
		virtual ~tCommandLineArgs();

		// HYPOTHESIS: the static's name is not known; the exe holds it at 0x00c7f8b8.
		static iAutoPtr g_00c7f8b8;

		// HYPOTHESIS: the object is 0x34 bytes (Instance allocates that much); the members
		// are not recovered yet.
		char m04[0x30];
};

#endif
