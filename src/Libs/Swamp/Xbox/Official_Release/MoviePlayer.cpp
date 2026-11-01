// UNIT: src/Libs/Swamp/Xbox/Official_Release/MoviePlayer.cpp OBJ: Xbox\Official_Release\MoviePlayer.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
extern "C" void* __cdecl memset(void*, int, unsigned int); // NAME: proto-pdb:default.pdb
#pragma intrinsic(memset)

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T = char_traits<E>, class A = allocator<E> > class basic_string {
public:
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const E* _Ptr);

private:
    unsigned int _pad00[7];
};
}
typedef std::basic_string<char> SwampString;

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    void QueueMovieFinishedMessage(const SwampString&);
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampScript* GetScript() const;
};

class BeepFile { // NAME: proto-pdb:default.pdb
public:
    void Close();

    char field_0[0x20];                // 0x00
};
DECOMP_SIZE_ASSERT(BeepFile, 0x20);

class DiskQueue { // NAME: proto-pdb:default.pdb
public:
    void CancelRequest(unsigned int* resultLoc);

    char field_0[0x7C];                // 0x00
};

// Retail layout: DiskQueue shrank 0x84 -> 0x7C, so _dvdQueue sits at 0x7C (proto 0x84).
class DiskArbitrator { // NAME: proto-pdb:default.pdb
public:
    DiskQueue _hardDriveQueue;         // 0x00
    DiskQueue _dvdQueue;               // 0x7C
};

template <class T> void RELEASE(T*& p); // NAME: proto-pdb:default.pdb

struct XMVDecoder; // NAME: proto-pdb:default.pdb
struct D3DTexture; // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    D3DTexture* GetTexture() const;

    char field_24[0xC];                // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    T& operator()() { return *_Resource; }

    T* _Resource;                      // 0x00
};
DECOMP_SIZE_ASSERT(ResourceHandle<TextureResource>, 0x4);

class TextureManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<TextureResource> GetDefaultTexture() const { return _defaultTexture; }

    char field_0[0x58];                // 0x00
    ResourceHandle<TextureResource> _defaultTexture; // 0x58
    char field_5C[0x28];               // 0x5C
};
DECOMP_SIZE_ASSERT(TextureManager, 0x84);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    TextureManager& GetTextureManager() const { return *_textureManager; }

    char field_0[0x14];                // 0x00
    DiskArbitrator* _diskArbitrator;   // 0x14
    char field_18[0x38 - 0x18];        // 0x18
    TextureManager* _textureManager;   // 0x38
    char field_3C[0x2C];               // 0x3C
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

extern "C" void __stdcall D3DDevice_EnableOverlay(int); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall XMVDecoder_TerminateImmediately(XMVDecoder*); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall XMVDecoder_CloseDecoder(XMVDecoder*); // NAME: proto-pdb:default.pdb
struct IDirectSoundStream;
extern "C" long __stdcall XMVDecoder_EnableAudioStream(XMVDecoder*, unsigned long, unsigned long, void*, IDirectSoundStream**); // src proto-pdb:default.pdb XMVDecoder_EnableAudioStream

struct _XMVVIDEO_DESC { // NAME: proto-pdb:default.pdb
    unsigned long Width;               // 0x00
    unsigned long Height;              // 0x04
    unsigned long FramesPerSecond;     // 0x08
    unsigned long AudioStreamCount;    // 0x0C
};

class MoviePlayer { // NAME: proto-pdb:default.pdb
public:
    enum { // NAME: proto-pdb:default.pdb
        LOOPED = 0x1,
        PLAYNOW = 0x4,
        USE_SAFEAREA = 0x8,
        RENDER_TEXTURE = 0x10,
        NOTIFY = 0x20,
        PRELOAD = 0x40,
        DEFAULT_FLAGS = 0x4
    };
    enum { // NAME: proto-pdb:default.pdb
        MAX_PACKETS = 0x2
    };

    void Play();
    void SetAudio(int index);
    void Stop();
    D3DTexture* GetRenderTexture() const;

private:
    static long __stdcall NextPacket(unsigned long context, void** ppPacket, unsigned long* pOffsetToNextPacket);

public:

    XMVDecoder* _decoder;              // 0x00
    void* _buffer;                     // 0x04
    bool _active;                      // 0x08
    bool _playing;                     // 0x09
    bool _paused;                      // 0x0A
    bool _clear;                       // 0x0B
    bool _cleanup;                     // 0x0C
    bool _swap;                        // 0x0D
    unsigned int _validFrame;          // 0x10
    unsigned int _flags;               // 0x14
    unsigned int _nextPacket;          // 0x18
    unsigned int _packetSize;          // 0x1C
    unsigned int _maxPacketSize;       // 0x20
    unsigned int _errors;              // 0x24
    int _audIndex;                     // 0x28
    SwampSceneNode* _node;             // 0x2C
    unsigned int _loadState[2];        // 0x30
    unsigned int _loadSize[2];         // 0x38
    BeepFile _file;                    // 0x40
    SwampString _name;                 // 0x60
    D3DTexture* _bufferFront;          // 0x7C
    D3DTexture* _bufferBack;           // 0x80
    _XMVVIDEO_DESC _desc;              // 0x84
};
DECOMP_SIZE_ASSERT(MoviePlayer, 0x94);

// FUNCTION: USA 0x000D9CF8
// PROTOTYPE: PREVIEW 0x004C5242 identical
// NAME: proto-pdb:default.pdb
long __stdcall MoviePlayer::NextPacket(unsigned long context, void** ppPacket, unsigned long* pOffsetToNextPacket)
{
    MoviePlayer* p = (MoviePlayer*)context;
    if (p) {
        unsigned int i = p->_nextPacket;
        switch (p->_loadState[i]) { // 0, 1, 0x17: names unknown
        default:
            p->_loadState[i] = 0x17;
            *ppPacket = (char*)p->_buffer + p->_maxPacketSize * i;
            *pOffsetToNextPacket = p->_loadSize[i];
            if (++i >= MAX_PACKETS)
                i = 0;
            p->_nextPacket = i;
            return 0;
        case 1:
            *ppPacket = 0;
            *pOffsetToNextPacket = 0;
            return 0;
        case 0:
        case 0x17:
            *ppPacket = 0;
            *pOffsetToNextPacket = 0;
            break;
        }
    }
    return 0x80004005;
}

// FUNCTION: USA 0x000D9D68
// PROTOTYPE: PREVIEW 0x004C52B2 identical
// NAME: proto-pdb:default.pdb
void MoviePlayer::Play()
{
    _paused = false;
    if (!_playing) {
        if (_decoder && !(_flags & RENDER_TEXTURE))
            D3DDevice_EnableOverlay(1);
        _active = true;
        _playing = true;
    }
}

// FUNCTION: USA 0x000D9D90
// PROTOTYPE: PREVIEW 0x004C52DA identical
// NAME: proto-pdb:default.pdb
void MoviePlayer::SetAudio(int index)
{
    _audIndex = index;
    if (_decoder && index != -1 && (unsigned long)index < _desc.AudioStreamCount)
        XMVDecoder_EnableAudioStream(_decoder, index, 0, 0, 0);
}

// FUNCTION: USA 0x000DA005
// PROTOTYPE: PREVIEW 0x004C5552 identical
// NAME: proto-pdb:default.pdb
D3DTexture* MoviePlayer::GetRenderTexture() const
{
    if (_decoder && (_flags & RENDER_TEXTURE) && _bufferFront)
        return _bufferFront;
    return g_swamp->GetTextureManager().GetDefaultTexture()().GetTexture();
}

#ifdef NON_MATCHING
// best: match_one objdiff 96.22, bytes mismatch (masked 1)
// FUNCTION: USA 0x000DA108
// PROTOTYPE: PREVIEW 0x004C5630 near-identical
// NAME: proto-pdb:default.pdb
void MoviePlayer::Stop()
{
    // !FAKE: c2 register allocation; local; retail hoists the queue into edi before the loop (match_one 85.67 -> 96.22);
    // the proto PDB lists no such local
    DiskQueue& queue = g_swamp->_diskArbitrator->_dvdQueue;
    for (int i = 0; i < 2; i++) {
        if (_loadState[i] == 1) {
            queue.CancelRequest(&_loadState[i]);
        }
        _loadState[i] = 0x17;
    }
    if (_decoder) {
        if (!(_flags & RENDER_TEXTURE)) {
            _clear = true;
        }
        XMVDecoder_TerminateImmediately(_decoder);
        if ((_flags & NOTIFY) && _node && _node->GetScript()) {
            _node->GetScript()->QueueMovieFinishedMessage(_name);
        }
    }
    _active = false;
    _playing = false;
    _paused = false;
    _swap = false;
    _validFrame = 0;
    _errors = 0;
    _nextPacket = 0;
    _name = "";
    if (_decoder) {
        XMVDecoder_CloseDecoder(_decoder);
        _decoder = 0;
    }
    if (!_clear) {
        memset(&_desc, 0, sizeof(_desc));
        RELEASE(_bufferFront);
        RELEASE(_bufferBack);
    }
    _file.Close();
}
#endif
