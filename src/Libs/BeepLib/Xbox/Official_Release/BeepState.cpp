// UNIT: src/Libs/BeepLib/Xbox/Official_Release/BeepState.cpp OBJ: Xbox\Official_Release\BeepState.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class T> class allocator;
template <class T, class A = allocator<T> > class vector {
public:
    vector() : _Myfirst(0), _Mylast(0), _Myend(0) {}
    void push_back(const T& val);

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
}

class FrequencyCalculator { // NAME: proto-pdb:default.pdb
public:
    struct Sample { // NAME: proto-pdb:default.pdb
        char field_0[0x10];            // 0x00
    };

    FrequencyCalculator();
    Sample TakeSample();

    std::vector<Sample> _Samples;      // 0x00
};
DECOMP_SIZE_ASSERT(FrequencyCalculator, 0x10);

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    ~BeepTDF();

    class Record { // NAME: proto-pdb:default.pdb
    public:
        ~Record();
    };
};

class FileSystem { // NAME: proto-pdb:default.pdb
public:
    ~FileSystem();
};

class QuickRand { // NAME: proto-pdb:default.pdb
    char field_0[0x8];                 // 0x00
};

class BeepState { // NAME: proto-pdb:default.pdb
public:
    ~BeepState();
    unsigned __int64 GetTSC();

private:
    class DebugState { // NAME: proto-pdb:default.pdb
    public:
        ~DebugState();

        char field_0[0x14];            // 0x00
    };

    class TDFState { // NAME: proto-pdb:default.pdb
    public:
        BeepTDF* _DefaultBlock;        // 0x00
        BeepTDF::Record* _DefaultRecord; // 0x04
    };

    void CheckForMMX();
    void CheckForRDTSC();
    void CheckForSSE();

    DebugState _DebugState;            // 0x00
    TDFState _TDFState;                // 0x14
    FileSystem* _FileSystem;           // 0x1C
    char field_20[0x1C];               // 0x20
    bool _MMXAvailable;                // 0x3C
    bool _RDTSCAvailable;              // 0x3D
    bool _SSEAvailable;                // 0x3E
    QuickRand* _RNG;                   // 0x40
    char field_44[0xC14];              // 0x44
};
DECOMP_SIZE_ASSERT(BeepState, 0xC58);

// FUNCTION: USA 0x00035E44
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match; body is __asm rdtsc (verbatim, never a match; no rdtsc intrinsic in MSVC 7.1)
unsigned __int64 BeepState::GetTSC()
{
    unsigned __int64 tsc;
    __asm {
        rdtsc
        mov dword ptr [tsc], eax
        mov dword ptr [tsc + 4], edx
    }
    return tsc;
}
#endif

// FUNCTION: USA 0x00035E59
// PROTOTYPE: PREVIEW 0x004245C4 identical
// NAME: proto-pdb:default.pdb
void BeepState::CheckForMMX()
{
    _MMXAvailable = true;
}

// FUNCTION: USA 0x00035E5E
// PROTOTYPE: PREVIEW 0x004245C9 identical
// NAME: proto-pdb:default.pdb
void BeepState::CheckForRDTSC()
{
    _RDTSCAvailable = true;
}

// FUNCTION: USA 0x00035E63
// PROTOTYPE: PREVIEW 0x004245CE identical
// NAME: proto-pdb:default.pdb
void BeepState::CheckForSSE()
{
    _SSEAvailable = true;
}

// FUNCTION: USA 0x000365DD
// NAME: proto-pdb:default.pdb
BeepState::~BeepState()
{
    delete _TDFState._DefaultRecord;
    delete _TDFState._DefaultBlock;
    delete _RNG;
    delete _FileSystem;
}
// FUNCTION: USA 0x00036B75
// NAME: proto-pdb:default.pdb
FrequencyCalculator::FrequencyCalculator()
{
    for (int i = 0; i < 64; i++)
        _Samples.push_back(TakeSample());
}
