// Tiger/twfsys.cpp

#include <windows.h>
#include "Tiger/tstring.h"
#include "Tiger/tgfsys.h"

class twFileSystemPart {
public:
    virtual tVolIterator mGetVolIterator(tSession session);
    virtual tVolume mGetNamedVolume(tSession session, tString name);
    virtual tPath mGetAppPath(tSession session);
    virtual tPath mDuplicatePath(tSession session, tPath path);
    virtual tPath mFileOpenDialog(tSession session, tString &prompt, tString &filter, tDir dir);
};

tPath WinParsePath(tSession session, tString &path);
tDir WinParseDir(tSession session, tString &path);
tVolume WinParseVolume(tSession session, tString &path);

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

// FUNCTION: STARFLEETOP 0x0062ee00
tDir WinParseDir(tSession session, tString &path)
{
    tVolume volume = WinParseVolume(session, path);
    tDir dir = volume.mGetRoot();

    while (path.mStrChr('\\') != path.end())
    {
        tString name(path);
        name.mEndStrAt(name.mStrChr('\\'));
        tDir sub = dir.mGetNamedDir(name);
        if (!sub.mExist())
            break;
        dir = sub;
        tString rest(path);
        tString next(rest.mStrChr('\\') + 1);
        path = next;
    }
    return dir;
}

// HYPOTHESIS: the buffers the volume root is copied into for the system calls; their names are
// not recovered.
extern char g_00c3caec[MAX_PATH];
extern iWideChar g_00c3cbf0[MAX_PATH];

// HYPOTHESIS: only the parse and the volume queries are written. Where a query succeeds, the
// original builds a twVolume from the label and the root (its constructor expanded inline), makes
// its cookie through the tFSCookiePart's volume cookie manager and returns that; twVolume and
// tFSCookiePart are declared only in tgfsys.cpp and cannot be repeated here.
// FUNCTION: STARFLEETOP 0x0062efe0
tVolume WinParseVolume(tSession session, tString &path)
{
    tVolume volume;
    tString root;
    char name[MAX_PATH];
    WCHAR wideName[MAX_PATH];

    if (path[1] == ':')
    {
        tString drive(path);
        drive.mEndStrAt(drive.mStrChr('\\') + 1);
        root = drive;
        drive = tString(path.mStrChr('\\') + 1);
        path = drive;
    }
    else if (path[0] == '\\' && path[1] == '\\')
    {
        tString first(path);
        tString second(first.mStrChr('\\') + 1);
        tString server(second.mStrChr('\\') + 1);
        tString share(server.mStrChr('\\') + 1);
        tString rest(share.mStrChr('\\') + 1);
        first.mEndStrAt(first.mStrChr('\\') + 1);
        second.mEndStrAt(second.mStrChr('\\') + 1);
        server.mEndStrAt(server.mStrChr('\\') + 1);
        share.mEndStrAt(share.mStrChr('\\') + 1);
        first += second;
        first += server;
        first += share;
        root = first;
        path = rest;
    }
    else
        throw "Whoa!  Unknown volume descriptor just got thrown at me!";

    root.mCopyWideStr(g_00c3cbf0, MAX_PATH);
    if (GetVolumeInformationW(g_00c3cbf0, wideName, 254, NULL, NULL, NULL, NULL, 0))
    {
        tString label(wideName);
        tString volRoot(root);
    }
    else
    {
        if (GetLastError() != ERROR_CALL_NOT_IMPLEMENTED)
            throw "Windows hates us.  We cannot win.";
        root.mCopyCharStr(g_00c3caec, MAX_PATH);
        if (!GetVolumeInformationA(g_00c3caec, name, 254, NULL, NULL, NULL, NULL, 0))
            throw "Windows hates us.  We cannot win.";
        tString label(name);
        tString volRoot(root);
    }
    return volume;
}

// HYPOTHESIS: the original resolves the path's cookie through the tFSCookiePart's path cookie
// manager (the out-of-line find, the map's end and operator[], as tPathIterator::mAdd has them),
// copies the resolved twPath's name string at +0x7c and parses that into the returned path.
// tFSCookiePart and twPath are declared only in tgfsys.cpp and cannot be repeated here, so the
// name copied is not the resolved one.
// FUNCTION: STARFLEETOP 0x0062f920
tPath twFileSystemPart::mDuplicatePath(tSession session, tPath path)
{
    tString name;
    return WinParsePath(session, name);
}

// HYPOTHESIS: the dir.mValid() branch of the original resolves the directory's name through the
// tFSCookiePart's directory cookie manager (twDir +0x8c); tFSCookiePart and twDir are declared only
// in tgfsys.cpp and cannot be repeated here, so dirName stays empty.
// FUNCTION: STARFLEETOP 0x0062fab0
tPath twFileSystemPart::mFileOpenDialog(tSession session, tString &prompt, tString &filter, tDir dir)
{
    tPath path;
    tString fileName(prompt);
    tString dirName;

    if (dir.mValid())
    {
    }

    tString filterStr;
    if (filter.mStrLen())
    {
        filterStr = "*.";
        filterStr += filter;
        filterStr += (iBasicStringElement)0;
        filterStr += (iBasicStringElement)0;
    }

    WCHAR fileBuf[MAX_PATH];
    if (fileName.mStrLen() == 0)
        fileBuf[0] = 0;
    else
    {
        tString full(fileName);
        if (filter.mStrLen())
        {
            full += ".";
            full += filter;
        }
        full.mCopyWideStr(fileBuf, MAX_PATH);
    }
    return path;
}
