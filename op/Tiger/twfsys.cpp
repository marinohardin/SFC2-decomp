// Tiger/twfsys.cpp

#include <windows.h>
#include "Tiger/tstring.h"
#include "Tiger/tgfsys.h"

class twFileSystemPart {
public:
    virtual tVolIterator mGetVolIterator(tSession session);
    virtual tVolume mGetNamedVolume(tSession session, tString name);
    virtual tPath mGetAppPath(tSession session);
};

tPath WinParsePath(tSession session, tString &path);
tDir WinParseDir(tSession session, tString &path);

// Claimed and scored as partial: the original body (about 1.5 KB) walks drive letters 'A' to
// 'Z' with GetVolumeInformation-style calls and builds the iterator from tString and tVolume
// temporaries. None of that is rebuilt yet, so only the signature is real.
// FUNCTION: STARFLEETOP 0x0062e1c0
tVolIterator twFileSystemPart::mGetVolIterator(tSession session)
{
    tVolIterator it;
    return it;
}

// HYPOTHESIS: only the first test of the original's loop is written; the second (the name of the
// volume the part's cookie manager resolves) needs tFSCookiePart and twVolume, which are declared
// only in tgfsys.cpp and cannot be repeated here.
// FUNCTION: STARFLEETOP 0x0062e7c0
tVolume twFileSystemPart::mGetNamedVolume(tSession session, tString name)
{
    tVolume v;
    tVolIterator it = mGetVolIterator(session);

    for (v = it.mFirst(); v.mValid(); v = it.mNext(v))
    {
        if (name.mStrICmp(v.mGetName()) == 0)
            return v;
    }
    return tVolume();
}

// FUNCTION: STARFLEETOP 0x0062e9f0
tPath twFileSystemPart::mGetAppPath(tSession session)
{
    tPath path;
    tString cmdLine(GetCommandLineA());

    if (cmdLine[0] == '"')
    {
        tString quoted(cmdLine.mStrChr('"') + 1);
        quoted.mEndStrAt(quoted.mStrChr('"'));
        cmdLine = quoted;
    }
    else
    {
        if (cmdLine.mStrChr(' ') != cmdLine.end())
            cmdLine.mEndStrAt(cmdLine.mStrChr(' '));
        if (cmdLine.mStrChr('\t') != cmdLine.end())
            cmdLine.mEndStrAt(cmdLine.mStrChr('\t'));
        if (cmdLine.mStrChr('\n') != cmdLine.end())
            cmdLine.mEndStrAt(cmdLine.mStrChr('\n'));
    }

    if (cmdLine.mStrLen() > 2 && cmdLine[1] != ':' && cmdLine[0] != '\\' && cmdLine[1] != '\\')
    {
        char dir[512];
        GetCurrentDirectoryA(511, dir);
        tString fullPath(dir);
        if (fullPath.mStrLen() > 1 && fullPath[fullPath.mStrLen() - 1] != '\\')
            fullPath += (iBasicStringElement)'\\';
        fullPath += cmdLine;
        cmdLine = fullPath;
    }

    path = WinParsePath(session, cmdLine);
    return path;
}

// FUNCTION: STARFLEETOP 0x0062ed40
tPath WinParsePath(tSession session, tString &path)
{
    tDir dir = WinParseDir(session, path);
    tPath file = dir.mGetNamedFile(path);

    if (!file.mValid())
        throw "Yow! WinParsePath failed to find the file!";
    return file;
}
