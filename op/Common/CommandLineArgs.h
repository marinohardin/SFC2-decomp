#ifndef COMMON_COMMANDLINEARGS_H
#define COMMON_COMMANDLINEARGS_H

#include <string>

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

		// HYPOTHESIS: names; the constructor builds three strings and fills the second
		// from the process command line.
		std::string m04;
		std::string m14;
		std::string m24;
};

#endif
