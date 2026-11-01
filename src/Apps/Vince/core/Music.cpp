// UNIT: src/Apps/Vince/core/Music.cpp OBJ: core\XBox\OfficialRelease\Music.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const basic_string& _Right);
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); } // src proto-pdb:default.pdb npos
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const E* _Ptr);
    static const unsigned int npos = (unsigned int)(-1);

private:
    unsigned int _pad00[0x1C / 4];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class FileHandler; // NAME: proto-pdb:default.pdb
class SwampScript; // NAME: proto-pdb:default.pdb

class BeepFile { // NAME: proto-pdb:default.pdb
public:
    bool IsValid() const { return _fh != 0; }

    FileHandler* _fh;                  // 0x00
};

class SoundStreamer { // NAME: proto-pdb:default.pdb
public:
    void Play();
    void SetVolume(float);
    bool IsPaused() const { return _file.IsValid() && _paused; }

    char field_0[0xA];                 // 0x00
    bool _paused;                      // 0x0A
    char field_B[0xDC - 0xB];          // 0x0B
    BeepFile _file;                    // 0xDC
};

class MusicManager { // NAME: proto-pdb:default.pdb
public:
    // FUNCTION: USA 0x00113069
    // PROTOTYPE: PREVIEW 0x004FD8D9 identical
    // NAME: proto-pdb:default.pdb
    struct Overlay { // NAME: proto-pdb:default.pdb
        unsigned int GetFlags() const;

        SwampString name;              // 0x00
        bool loop;                     // 0x1C
        bool music;                    // 0x1D
        SwampScript* _scriptToNotify;  // 0x20
    };

    enum { // NAME: proto-pdb:default.pdb
        STATE_IDLE = 0,
        STATE_TRACK = 1,
        STATE_TRANSITION = 2,
        STATE_OVERLAY = 3,
        STATE_TRACK_IN = 4,
        STATE_OVERLAY_IN = 5,
        STATE_OVERLAY_OUT = 6,
        STATE_OVERLAY_TRANSITION = 7,
        STATE_FADE_OUT = 8
    };

    void Resume();
    void EndTrack();

    SoundStreamer* _stream1;           // 0x00
    SoundStreamer* _stream2;           // 0x04
    unsigned int _state;               // 0x08
    unsigned int _toState;             // 0x0C
    bool _bTrack;                      // 0x10
    bool _bOverlay;                    // 0x11
    bool _bTransition;                 // 0x12
    bool _bSuspend;                    // 0x13
    char field_14[0x20 - 0x14];        // 0x14
    SwampString _playSong;             // 0x20
    char field_3C[0x50 - 0x3C];        // 0x3C
    SwampString _frontSong;            // 0x50
    SwampString _backSong;             // 0x6C
    char field_88[0xC0 - 0x88];        // 0x88
};
DECOMP_SIZE_ASSERT(MusicManager, 0xC0);

// FUNCTION: USA 0x00112885
// PROTOTYPE: PREVIEW 0x004FD0DC identical
// NAME: proto-pdb:default.pdb
unsigned int MusicManager::Overlay::GetFlags() const
{
    unsigned int flags = 0;
    if (loop) {
        flags = 1;
    }
    if (music) {
        return flags | 0x100;
    }
    return flags | 0x200;
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 2 = the two Play calls)
// FUNCTION: USA 0x00112A0B
// PROTOTYPE: PREVIEW 0x004FD261 near-identical
// NAME: proto-pdb:default.pdb
void MusicManager::Resume()
{
    if (_bSuspend) {
        _bSuspend = false;
        if (_stream1->IsPaused()) {
            _stream1->Play();
        }
        if (_stream2->IsPaused()) {
            _stream2->Play();
        }
        if (_state == 1 && _toState == 1) {
            _stream1->SetVolume(0.0f);
            _stream2->SetVolume(0.0f);
            _state = 4;
        }
    }
}
#endif

inline void* operator new(unsigned int, void* p) { return p; }

// FUNCTION: USA 0x00112BC1
// PROTOTYPE: PREVIEW 0x004FD431 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit Overlay copy ctor in this unit (no matched
// function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_Overlay_copy(void* p, const MusicManager::Overlay& v)
{
    new (p) MusicManager::Overlay(v);
}

// FUNCTION: USA 0x00112FC0
// PROTOTYPE: PREVIEW 0x004FD830 identical
// NAME: proto-pdb:default.pdb
void MusicManager::EndTrack()
{
    switch (_state) {
    case STATE_OVERLAY:
    case STATE_OVERLAY_IN:
    case STATE_OVERLAY_OUT:
    case STATE_OVERLAY_TRANSITION:
        break;
    default:
        _toState = STATE_IDLE;
        break;
    }
    switch (_state) {
    case STATE_TRACK:
    case STATE_TRACK_IN:
        break;
    default:
        _playSong = "";
        break;
    }
    if (_state != STATE_TRANSITION) {
        _frontSong = _backSong = "";
    }
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit Overlay::operator= in this unit (no matched
// function odr-uses it); this assignment odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_Overlay_assign(MusicManager::Overlay* p, const MusicManager::Overlay& v)
{
    *p = v;
}
