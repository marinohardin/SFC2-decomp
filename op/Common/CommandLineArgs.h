#ifndef COMMON_COMMANDLINEARGS_H
#define COMMON_COMMANDLINEARGS_H

#include <string>

#include "Common/AutoPtr.h"

#include <vector>

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

		// HYPOTHESIS: names; the constructor builds a list and two strings and fills the
		// first string from the process command line. The destructor frees the list's
		// 16-byte elements as strings.
		std::vector<std::string> m04;
		std::string m14;
		std::string m24;
};

#endif
