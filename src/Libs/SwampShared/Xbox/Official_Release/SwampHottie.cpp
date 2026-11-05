// UNIT: src/Libs/SwampShared/Xbox/Official_Release/SwampHottie.cpp OBJ: Xbox\Official_Release\SwampHottie.obj OPT: /O1 /Oy /Oi
#include "../../../../include/decomp.h"
#include <string.h>
#pragma warning(disable: 4530)
#include <algorithm>

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class E, class T, class A> class basic_string;
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

void* operator new(unsigned int size, const char* file, int line);

class BeepFile { // NAME: proto-pdb:default.pdb
public:
    BeepFile();
    void* Open(unsigned int);
    bool Open(const void*, unsigned int);

    char field_0[0x20];                // 0x00
};
DECOMP_SIZE_ASSERT(BeepFile, 0x20);

struct HottieFile; // NAME: proto-pdb:default.pdb

extern "C" int uncompress(unsigned char* dest, unsigned long* destLen, const unsigned char* source, unsigned long sourceLen); // NAME: proto-pdb:default.pdb

struct HottieBundle { // NAME: proto-pdb:default.pdb
    const HottieFile* GetFileInfo(const SwampString&) const;
    unsigned int GetFileInfoSlot(const HottieFile* hf) const;
    const void* GetFileDataPtr(unsigned int slot) const;
    BeepFile* OpenBundledFile(const SwampString& filename) const;

    unsigned int id;                   // 0x00
    unsigned int version;              // 0x04
    unsigned int sizeInfo;             // 0x08
    unsigned int sizeHeaders;          // 0x0C
    unsigned int sizeTotal;            // 0x10
    unsigned int offsetStrings;        // 0x14
    unsigned int numFiles;             // 0x18
};
DECOMP_SIZE_ASSERT(HottieBundle, 0x1C);

struct HottieFile { // NAME: proto-pdb:default.pdb
    bool UnpackData(void* dest, const HottieBundle* hot) const;

    unsigned int locName;              // 0x00
    unsigned int sizeHeader;           // 0x04
    unsigned int offsetHeader;         // 0x08
    unsigned int sizeDataUncompressed; // 0x0C
    unsigned int sizeDataCompressed;   // 0x10
    unsigned int offsetData;           // 0x14
};
DECOMP_SIZE_ASSERT(HottieFile, 0x18);

inline unsigned int HottieBundle::GetFileInfoSlot(const HottieFile* hf) const
{
    return ((const char*)hf - (const char*)(this + 1)) / sizeof(HottieFile);
}

inline const void* HottieBundle::GetFileDataPtr(unsigned int slot) const
{
    return (const char*)this + ((const HottieFile*)(this + 1))[slot].offsetData;
}

bool operator<(const HottieFile& hf0, const HottieFile& hf1); // NAME: proto-pdb:default.pdb (retail 0x000DF1C5)

// FUNCTION: USA 0x000DF25E
// PROTOTYPE: PREVIEW 0x004CA635 identical
// NAME: proto-pdb:default.pdb
template HottieFile const * std::_Lower_bound<HottieFile const *,HottieFile,int>(HottieFile const *,HottieFile const *,HottieFile const &,int *);

// FUNCTION: USA 0x000DF215
// PROTOTYPE: PREVIEW 0x004CA5EC identical
// NAME: proto-pdb:default.pdb
bool HottieFile::UnpackData(void* dest, const HottieBundle* hot) const
{
    // !FAKE: c2 register allocation; neutral stand-in local; retail computes the data pointer into esi before the branch (match_one 63.9 -> 100.0)
    const unsigned char* p =(const unsigned char*)hot + offsetData;
    if (sizeDataCompressed) {
        unsigned long destLen = sizeDataUncompressed;
        return uncompress((unsigned char*)dest, &destLen, p, sizeDataCompressed) == 0;
    }
    memcpy(dest, p, sizeDataUncompressed);
    return true;
}

// FUNCTION: USA 0x000DF337
// PROTOTYPE: PREVIEW 0x004CA70E identical
// NAME: proto-pdb:default.pdb
BeepFile* HottieBundle::OpenBundledFile(const SwampString& filename) const
{
    // !FAKE: c1xx front end; neutral local; the PDB records no local for the file info
    const HottieFile* hf = GetFileInfo(filename);
    if (!hf)
        return 0;
    // !FAKE: c1xx front end; neutral local; the PDB records no local for the new file
    BeepFile* p = new("SwampHottie.cpp", 59) BeepFile;
    if (hf->sizeDataCompressed) {
        // !FAKE: c2 block layout; temporary; retail calls Open before pushing this, sharing the size push with the else arm (match_one 90.2 -> 100.0)
        void* v0 = p->Open(hf->sizeDataUncompressed);
        hf->UnpackData(v0, this);
    } else
        p->Open(GetFileDataPtr(GetFileInfoSlot(hf)), hf->sizeDataUncompressed);
    return p;
}
