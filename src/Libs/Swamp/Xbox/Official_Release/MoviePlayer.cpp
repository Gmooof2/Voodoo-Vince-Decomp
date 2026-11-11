// UNIT: src/Libs/Swamp/Xbox/Official_Release/MoviePlayer.cpp OBJ: Xbox\Official_Release\MoviePlayer.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
extern "C" void* __cdecl memset(void*, int, unsigned int); // NAME: proto-pdb:default.pdb
#pragma intrinsic(memset)

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T = char_traits<E>, class A = allocator<E> > class basic_string {
public:
    basic_string();
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const E* _Ptr);
    const E* c_str() const { return _Myptr(); }
    const E* _Myptr() const { return _BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf; }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    enum { _BUF_SIZE = 16 / sizeof(E) < 1 ? 1 : 16 / sizeof(E) };
    char _Alval;                       // 0x00
    union {
        E _Buf[_BUF_SIZE];
        E* _Ptr;
    } _Bx;                             // 0x04
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
}
typedef std::basic_string<char> SwampString;

namespace BeepLib { // NAME: proto-pdb:default.pdb
SwampString StripPath(const SwampString& path);
SwampString StripExtension(const SwampString& path);
}

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
    BeepFile();
    ~BeepFile();
    bool OpenForAsync(const SwampString& filename);
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
struct D3DSurface; // NAME: proto-pdb:default.pdb

struct tagRECT { // NAME: proto-pdb:default.pdb
    long left;                         // 0x00
    long top;                          // 0x04
    long right;                        // 0x08
    long bottom;                       // 0x0C
};
DECOMP_SIZE_ASSERT(tagRECT, 0x10);

class SwampRenderer { // NAME: proto-pdb:default.pdb
public:
    int GetScreenWidth() const { return _screenWidth; }
    int GetScreenHeight() const { return _screenHeight; }
    int GetScreenSafeLeft() const { return _screenSafeWidthMargin; }
    int GetScreenSafeTop() const { return _screenSafeHeightMargin; }
    int GetScreenSafeRight() const { return _screenSafeWidth + _screenSafeWidthMargin; }
    int GetScreenSafeBottom() const { return _screenSafeHeight + _screenSafeHeightMargin; }

    char field_0[0x2C];                // 0x00
    int _screenWidth;                  // 0x2C
    int _screenHeight;                 // 0x30
    int _screenSafeWidth;              // 0x34
    int _screenSafeHeight;             // 0x38
    int _screenSafeWidthMargin;        // 0x3C
    int _screenSafeHeightMargin;       // 0x40
    char field_44[0x38C - 0x44];       // 0x44
};
DECOMP_SIZE_ASSERT(SwampRenderer, 0x38C);

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

class SwampGamePad { // NAME: proto-pdb:default.pdb
public:
    bool WasButtonPressed(int button) const;
};

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    void Update();
    const SwampGamePad* GetPad(int index) const;
};

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    void Update();
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }
    SwampRenderer& GetRenderer() const { return *_renderer; }
    SoundManager& GetSoundManager() const { return *_soundManager; }
    TextureManager& GetTextureManager() const { return *_textureManager; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    SwampRenderer* _renderer;          // 0x08
    char field_C[0x14 - 0xC];          // 0x0C
    DiskArbitrator* _diskArbitrator;   // 0x14
    char field_18[0x34 - 0x18];        // 0x18
    SoundManager* _soundManager;       // 0x34
    TextureManager* _textureManager;   // 0x38
    char field_3C[0x2C];               // 0x3C
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

extern "C" void __stdcall D3DDevice_EnableOverlay(int); // NAME: proto-pdb:default.pdb
extern "C" int __stdcall D3DDevice_GetOverlayUpdateStatus(); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_BlockUntilVerticalBlank(); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall D3DDevice_UpdateOverlay(D3DSurface*, const tagRECT*, const tagRECT*, int, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" D3DSurface* __stdcall D3DTexture_GetSurfaceLevel2(D3DTexture*, unsigned int); // NAME: proto-pdb:default.pdb
extern "C" unsigned long __stdcall D3DResource_Release(D3DSurface*); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall XPhysicalFree(void*); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall XMVDecoder_TerminateImmediately(XMVDecoder*); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall XMVDecoder_CloseDecoder(XMVDecoder*); // NAME: proto-pdb:default.pdb
struct IDirectSoundStream;
extern "C" long __stdcall XMVDecoder_EnableAudioStream(XMVDecoder*, unsigned long, unsigned long, void*, IDirectSoundStream**); // src proto-pdb:default.pdb XMVDecoder_EnableAudioStream
extern "C" void* __stdcall XPhysicalAlloc(unsigned long, unsigned long, unsigned long, unsigned long); // NAME: proto-pdb:default.pdb

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

    MoviePlayer(const SwampString& fname, unsigned int flags, unsigned int maxp);
    ~MoviePlayer();
    bool Set(const SwampString& fname, unsigned int flags);
    void Play();
    void SetAudio(int index);
    void Stop();
    void Render() const;
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
    mutable bool _cleanup;             // 0x0C
    mutable bool _swap;                // 0x0D
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
    mutable D3DTexture* _bufferFront;  // 0x7C
    mutable D3DTexture* _bufferBack;   // 0x80
    mutable _XMVVIDEO_DESC _desc;      // 0x84
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

// FUNCTION: USA 0x000D9EFD
// PROTOTYPE: PREVIEW 0x004C544A identical
// NAME: proto-pdb:default.pdb
void MoviePlayer::Render() const
{
    if (_swap) {
        _swap = false;
        SwampRenderer& renderer = g_swamp->GetRenderer();
        if (!D3DDevice_GetOverlayUpdateStatus())
            D3DDevice_BlockUntilVerticalBlank();
        // !FAKE: c1xx front end; temporary; the proto PDB lists no swap local, retail stores _bufferBack first
        D3DTexture* tmp = _bufferBack;
        _bufferBack = _bufferFront;
        _bufferFront = tmp;
        D3DSurface* surf = D3DTexture_GetSurfaceLevel2(_bufferFront, 0);
        tagRECT srcRect;
        srcRect.left = 0;
        srcRect.top = 0;
        srcRect.right = _desc.Width;
        srcRect.bottom = _desc.Height;
        tagRECT dstRect;
        if (_flags & USE_SAFEAREA) {
            dstRect.top = renderer.GetScreenSafeTop();
            dstRect.left = renderer.GetScreenSafeLeft();
            dstRect.right = renderer.GetScreenSafeRight();
            dstRect.bottom = renderer.GetScreenSafeBottom();
        } else {
            dstRect.left = 0;
            dstRect.top = 0;
            dstRect.right = renderer.GetScreenWidth();
            dstRect.bottom = renderer.GetScreenHeight();
        }
        while (!D3DDevice_GetOverlayUpdateStatus())
            ;
        D3DDevice_UpdateOverlay(surf, &srcRect, &dstRect, 0, 0);
        D3DResource_Release(surf);
        if (_cleanup) {
            _cleanup = false;
            D3DDevice_BlockUntilVerticalBlank();
            while (!D3DDevice_GetOverlayUpdateStatus())
                ;
            D3DDevice_EnableOverlay(0);
            memset(&_desc, 0, sizeof(_desc));
            RELEASE(_bufferFront);
            RELEASE(_bufferBack);
        }
    }
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

extern "C" long __stdcall XMVDecoder_CreateDecoderForFile(unsigned long, const char*, XMVDecoder**); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall XMVDecoder_Play(XMVDecoder*, unsigned long, tagRECT*); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall XMVDecoder_TerminatePlayback(XMVDecoder*); // NAME: proto-pdb:default.pdb
extern "C" void* __stdcall CreateThread(void*, unsigned long, unsigned long (__stdcall*)(void*), void*, unsigned long, unsigned long*); // NAME: proto-pdb:default.pdb
extern "C" int __stdcall SetThreadPriority(void*, int); // NAME: proto-pdb:default.pdb
extern "C" int __stdcall CloseHandle(void*); // NAME: proto-pdb:default.pdb
extern "C" unsigned long __stdcall GetTickCount(); // NAME: proto-pdb:default.pdb
extern "C" void __stdcall Sleep(unsigned long); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000DA03A
// PROTOTYPE: PREVIEW 0x004C5587 near-identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best (local Run 0x000D9E55): match_one objdiff 99.91, bytes provisional (masked 2): GetTickCount 0x0014A4D9 only
// ghidra-named (C0192 xdk-callee-unnamed); PlayMovie 0x000DA03A has no symbols.txt name and calls unnamed 0x000B9E3C
bool PlayMovie(const SwampString& movie)
{
    struct KeyExitThread { // NAME: proto-pdb:default.pdb
        volatile bool pressed;         // 0x00
        volatile bool pleaseFinish;    // 0x01
        XMVDecoder* pDecoder;          // 0x04
        volatile unsigned long startTime; // 0x08
        void* hThread;                 // 0x0C

// FUNCTION: USA 0x000D9E55
// PROTOTYPE: PREVIEW 0x004C53A2 identical
// NAME: proto-pdb:default.pdb
        static unsigned long __stdcall Run(void* p)
        {
            // !FAKE: c2 scheduling; pointer-to-volatile cast; retail stores pressed before reading pDecoder and clears
            // hThread with mov after pop edi (match_one 92.45 -> 99.91; proto PDB lists hThread/pDecoder non-volatile)
            volatile KeyExitThread* thread = (volatile KeyExitThread*)p;
            SetThreadPriority(thread->hThread, 15);
            while (GetTickCount() - thread->startTime < 1000) {
                g_swamp->GetInput().Update();
                Sleep(10);
            }
            while (!thread->pleaseFinish) {
                g_swamp->GetInput().Update();
                const SwampGamePad* pad = g_swamp->GetInput().GetPad(0);
                if (pad->WasButtonPressed(4) || pad->WasButtonPressed(8)) {
                    thread->pressed = true;
                    XMVDecoder_TerminatePlayback(thread->pDecoder);
                    break;
                }
                Sleep(10);
            }
            CloseHandle(thread->hThread);
            thread->hThread = 0;
            return 0;
        }
    };

    XMVDecoder* pDecoder = 0;
    if (XMVDecoder_CreateDecoderForFile(0, movie.c_str(), &pDecoder) < 0 || !pDecoder)
        return false;
    // retail: g_swamp->GetSoundManager() member call 0x000B9E3C with (1, 1); callee unnamed (no symbols.txt start;
    // proto calls SoundManager::SetMute(bool) here)
    g_swamp->GetSoundManager().Update();
    XMVDecoder_EnableAudioStream(pDecoder, 0, 0, 0, 0);
    KeyExitThread thread;
    thread.pressed = false;
    thread.pleaseFinish = false;
    thread.pDecoder = pDecoder;
    thread.startTime = GetTickCount();
    thread.hThread = CreateThread(0, 0, KeyExitThread::Run, &thread, 0, 0);
    XMVDecoder_Play(pDecoder, 0, 0);
    thread.pleaseFinish = true;
    while (thread.hThread)
        Sleep(10);
    XMVDecoder_CloseDecoder(pDecoder);
    // retail: g_swamp->GetSoundManager() member call 0x000B9E3C with (0, 0); callee unnamed
    return !thread.pressed;
}
#endif

// callees/literals pinned 5.4 T25.c1
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
        if (_flags & NOTIFY) {
            // !FAKE: c2 register allocation; local; retail keeps _node in edi across both GetScript calls;
            // the proto PDB lists no such local
            SwampSceneNode* node = _node;
            if (node && node->GetScript()) {
                node->GetScript()->QueueMovieFinishedMessage(_name);
            }
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

// FUNCTION: USA 0x000DA1DE
// PROTOTYPE: PREVIEW 0x004C5709 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1): _XPhysicalFree@4 0x0001785A has only the ghidra name XPhysicalFree in symbols.txt
MoviePlayer::~MoviePlayer()
{
    Stop();
    RELEASE(_bufferFront);
    RELEASE(_bufferBack);
    if (_clear || _cleanup) {
        while (!D3DDevice_GetOverlayUpdateStatus())
            ;
        D3DDevice_EnableOverlay(0);
    }
    if (_buffer) {
        XPhysicalFree(_buffer);
        _buffer = 0;
    }
}
#endif

// FUNCTION: USA 0x000DA23E
// PROTOTYPE: PREVIEW 0x004C5769 identical
// NAME: proto-pdb:default.pdb
bool MoviePlayer::Set(const SwampString& fname, unsigned int flags)
{
    Stop();
    if (!_file.OpenForAsync(fname))
        return false;
    _flags = flags;
    _name = BeepLib::StripExtension(BeepLib::StripPath(fname));
    if (!_buffer) {
        _buffer = XPhysicalAlloc(_maxPacketSize * MAX_PACKETS, (unsigned long)-1, 0, 4);
        if (!_buffer)
            return false;
    }
    if (flags & PLAYNOW)
        Play();
    else if (flags & PRELOAD)
        _active = true;
    return true;
}

// FUNCTION: USA 0x000DA66C
// PROTOTYPE: PREVIEW 0x004C5B9D identical
// NAME: proto-pdb:default.pdb
MoviePlayer::MoviePlayer(const SwampString& fname, unsigned int flags, unsigned int maxp)
    : _decoder(0), _buffer(0), _active(false), _playing(false), _paused(false), _clear(false), _cleanup(false),
      _swap(false), _validFrame(0), _flags(0), _nextPacket(0), _maxPacketSize(maxp * 1024), _errors(0),
      _audIndex(-1), _node(0), _bufferFront(0), _bufferBack(0)
{
    memset(&_desc, 0, sizeof(_desc));
    for (int i = 0; i < MAX_PACKETS; i++) {
        _loadState[i] = 0x17;
        _loadSize[i] = 0;
    }
    Set(fname, flags);
}
