#ifndef COMMON_COMMANDLINEARGS_H
#define COMMON_COMMANDLINEARGS_H

#include <iosfwd>

class tCommandLineArgs
{
	public:
		static tCommandLineArgs *Instance(void);
		void Setup(const std::string &commandLine);
};

#endif
