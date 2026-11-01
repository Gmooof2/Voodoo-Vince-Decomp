// UNIT: src/Libs/Swamp/Xbox/Official_Release/SoundStreamer.cpp OBJ: Xbox\Official_Release\SoundStreamer.obj OPT: /O1 /Oy
// !FAKE: c1xx front end; file-scope warning pragma; VC7.1 <string> without /EH warns C4530 and the build refuses it
#pragma warning(disable: 4530)
#include <string>
#include "../../../../include/decomp.h"
#pragma intrinsic(memset)

struct IDirectSoundStream { // NAME: proto-pdb:default.pdb
    virtual unsigned long __stdcall AddRef() = 0;                  // vtable+0x00
    virtual unsigned long __stdcall Release() = 0;                 // vtable+0x04
    virtual long __stdcall GetInfo(struct _XMEDIAINFO*) = 0;       // vtable+0x08 // src proto-pdb:default.pdb _XMEDIAINFO
    virtual long __stdcall GetStatus(unsigned long*) = 0;          // vtable+0x0C
};

template <class T>
void RELEASE(T*& obj) // NAME: proto-pdb:default.pdb
{
    if (obj) {
        obj->Release();
        obj = 0;
    }
}

// FUNCTION: USA 0x000D8BBC
// PROTOTYPE: PREVIEW 0x004C4143 identical
// NAME: proto-pdb:default.pdb
template void RELEASE<IDirectSoundStream>(IDirectSoundStream*&);

extern "C" void __stdcall DirectSoundDoWork(); // NAME: proto-pdb:default.pdb

class ResourceCache { // NAME: proto-pdb:default.pdb
public:
    void FreeHunk(void*);
};

class BundleWrapper { // NAME: proto-pdb:default.pdb
public:
    static ResourceCache& GetMemoryPool();
};

struct FileHandler; // NAME: proto-pdb:default.pdb

class BeepFile { // NAME: proto-pdb:default.pdb
public:
    ~BeepFile();
    bool OpenForAsync(const std::string&);
    void Close();

    FileHandler* _fh;                  // 0x00
    char field_4[0x20 - 0x4];          // 0x04
};

#pragma pack(push, 1)
struct tWAVEFORMATEX { // NAME: proto-pdb:default.pdb
    unsigned short wFormatTag;         // 0x00
    unsigned short nChannels;          // 0x02
    unsigned long nSamplesPerSec;      // 0x04
    unsigned long nAvgBytesPerSec;     // 0x08
    unsigned short nBlockAlign;        // 0x0C
    unsigned short wBitsPerSample;     // 0x0E
    unsigned short cbSize;             // 0x10
};
#pragma pack(pop)
DECOMP_SIZE_ASSERT(tWAVEFORMATEX, 0x12);

extern "C" long __stdcall IDirectSoundStream_Pause(IDirectSoundStream*, unsigned long); // src proto-pdb:default.pdb IDirectSoundStream_Pause
extern "C" long __stdcall IDirectSoundStream_FlushEx(IDirectSoundStream*, __int64, unsigned long); // NAME: proto-pdb:default.pdb

class SoundStreamer;

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    void QueueSoundStreamerFinishedMessage(const std::string&);
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() const;
};

class SoundManager { // NAME: proto-pdb:default.pdb
private:
    void Stopped(SoundStreamer*);
    friend class SoundStreamer;
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x34];                // 0x00
    SoundManager* _soundManager;       // 0x34
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

namespace BeepLib { // NAME: proto-pdb:default.pdb
double GetTime();
std::string StripPath(const std::string&);
std::string StripExtension(const std::string&);
}

class SoundStreamer { // NAME: proto-pdb:default.pdb
public:
    ~SoundStreamer();
    bool Set(const std::string&, unsigned int);
    void Play();
    void Pause();
    void Stop();

    static int _streamerCount; // NAME: proto-pdb:default.pdb

    IDirectSoundStream* _stream;       // 0x00
    void* _buffer;                     // 0x04
    bool _active;                      // 0x08
    bool _playing;                     // 0x09
    bool _paused;                      // 0x0A
    bool _setpos;                      // 0x0B
    unsigned int _flags;               // 0x0C
    float _volume;                     // 0x10
    unsigned int _fileLoc;             // 0x14
    unsigned int _dataLoc;             // 0x18
    unsigned int _dataLen;             // 0x1C
    unsigned int _errors;              // 0x20
    unsigned int _reqSize;             // 0x24
    unsigned int _reqDiff;             // 0x28
    unsigned int _nextPacket;          // 0x2C
    unsigned int _reqPos;              // 0x30
    SwampSceneNode* _node;             // 0x34
    char field_38[0xDC - 0x38];        // 0x38
    BeepFile _file;                    // 0xDC
    std::string _name;                 // 0xFC
    tWAVEFORMATEX _format;             // 0x118
    float _pauseTime;                  // 0x12C
    SoundStreamer* _syncTarget;        // 0x130
    unsigned int _syncPacket;          // 0x134
private:
    void CancelRequests();
};

// FUNCTION: USA 0x000D8BFB
// PROTOTYPE: PREVIEW 0x004C41F8 near-identical
// NAME: proto-pdb:default.pdb
void SoundStreamer::Pause()
{
    if (_playing && !_paused) {
        _paused = true;
        _active = false;
        _pauseTime = BeepLib::GetTime();
        if (_stream) {
            IDirectSoundStream_Pause(_stream, 1);
        }
    }
}

#ifdef NON_MATCHING
// best: match_one objdiff 95.44, bytes mismatch (masked 3)
// FUNCTION: USA 0x000D91A7
// PROTOTYPE: PREVIEW 0x004C4782 near-identical
// NAME: proto-pdb:default.pdb
void SoundStreamer::Stop()
{
    if (_stream) {
        IDirectSoundStream_FlushEx(_stream, 0, 1);
        if ((_flags & 0x20) && _node && _node->GetScript()) {
            _node->GetScript()->QueueSoundStreamerFinishedMessage(_name);
        }
    }
    g_swamp->_soundManager->Stopped(this);
    if (_syncTarget) {
        if (_syncTarget->_file._fh) {
            _syncTarget->Play();
        }
        _syncPacket = 0xFFFFFFFF;
        _syncTarget = 0;
    }
    _active = false;
    _playing = false;
    _paused = false;
    _setpos = false;
    _fileLoc = 0;
    _dataLoc = 0;
    _dataLen = 0;
    _errors = 0;
    _nextPacket = 0;
    _reqPos = 0;
    memset(&_format, 0, sizeof(_format));
    _name = "";
    CancelRequests();
    _file.Close();
}
#endif

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1)
// FUNCTION: USA 0x000D94AF
// PROTOTYPE: PREVIEW 0x004C4A67 near-identical
// NAME: proto-pdb:default.pdb
SoundStreamer::~SoundStreamer()
{
    unsigned long status;

    Stop();
    if (_stream) {
        do {
            DirectSoundDoWork();
            _stream->GetStatus(&status);
        } while (status & 0x10000);
        RELEASE(_stream);
    }
    if (_buffer) {
        BundleWrapper::GetMemoryPool().FreeHunk(_buffer);
        _buffer = 0;
    }
    --_streamerCount;
}
#endif

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1): the Play call 0x000D9104 is unresolved (no symbols.txt row)
// FUNCTION: USA 0x000D9519
// PROTOTYPE: PREVIEW 0x004C4ACE near-identical
// NAME: proto-pdb:default.pdb
bool SoundStreamer::Set(const std::string& fname, unsigned int flags)
{
    unsigned long status;

    if (!_buffer) {
        return false;
    }
    Stop();
    if (_stream) {
        do {
            DirectSoundDoWork();
            _stream->GetStatus(&status);
        } while (status & 0x10000);
        RELEASE(_stream);
    }
    if (!_file.OpenForAsync(fname)) {
        return false;
    }
    _flags = flags;
    _name = BeepLib::StripExtension(BeepLib::StripPath(fname));
    if (flags & 4) {
        Play();
    } else if (flags & 0x40) {
        _active = true;
    }
    return true;
}
#endif
