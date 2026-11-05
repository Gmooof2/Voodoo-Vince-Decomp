// UNIT: src/Apps/Vince/core/xbutil.cpp OBJ: core\XBox\OfficialRelease\xbutil.obj OPT: /O1 /Oy /Oi
#include "../../../include/decomp.h"
#include <string.h>

extern "C" int __cdecl sprintf(char*, const char*, ...); // NAME: proto-pdb:default.pdb
extern "C" void* __stdcall CreateFileA(const char* lpFileName, unsigned long dwDesiredAccess, unsigned long dwShareMode, void* lpSecurityAttributes, unsigned long dwCreationDisposition, unsigned long dwFlagsAndAttributes, void* hTemplateFile); // NAME: proto-pdb:default.pdb
extern "C" int __stdcall CloseHandle(void* hObject); // NAME: proto-pdb:default.pdb

extern char g_strMediaPath[]; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0013851E
// PROTOTYPE: PREVIEW 0x00522771 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 99.91, bytes match; retail cut carries a DIR32 reloc DAT_80000000 on the GENERIC_READ immediate
long XBUtil_FindMediaFile(char* strPath, const char* strFilename)
{
    if (strFilename == 0 || strPath == 0)
        return 0x80070057;

    strcpy(strPath, strFilename);

    if (strFilename[1] != ':')
        sprintf(strPath, "%s%s", g_strMediaPath, strFilename);

    void* hFile = CreateFileA(strPath, 0x80000000, 1, 0, 3, 0, 0);
    if (hFile == (void*)-1)
    {
        char strBuffer[80];
        sprintf(strBuffer, "XBUtil_FindMediaFile(): Could not find file [%s]\n", strFilename);
        return 0x82000004;
    }

    CloseHandle(hFile);

    return 0;
}
#endif
