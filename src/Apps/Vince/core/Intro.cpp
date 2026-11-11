// UNIT: src/Apps/Vince/core/Intro.cpp OBJ: core\XBox\OfficialRelease\Intro.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template<class _Elem> // src proto-pdb:default.pdb _Elem
struct char_traits {};

template<class _Ty>
class allocator {};

template<class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
class basic_string {
public:
    ~basic_string() { _Tidy(true); }

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};

template<class _Elem, class _Traits, class _Ax>
basic_string<_Elem, _Traits, _Ax> operator+(const _Elem* _Left, const basic_string<_Elem, _Traits, _Ax>& _Right);

template<class _Ty, class _Ax = allocator<_Ty> >
class list {
    struct _Node {
        _Node* _Next;
        _Node* _Prev;
        _Ty _Myval;
    };

public:
    class iterator {
    public:
        iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        _Ty& operator*() const { return _Ptr->_Myval; }

        _Node* _Ptr;
    };

    ~list() { _Tidy(); }
    iterator begin() { return iterator(_Myhead->_Next); }
    bool empty() const { return _Mysize == 0; }
    _Ty& front() { return *begin(); }
    iterator erase(iterator _Where);

protected:
    void _Tidy();

private:
    _Ax _Alval;                        // 0x0
    _Node* _Myhead;                    // 0x4
    unsigned int _Mysize;              // 0x8
};
}

class FileHandler; // NAME: proto-pdb:default.pdb
struct HWND__;     // NAME: proto-pdb:default.pdb

class BeepFile { // NAME: proto-pdb:default.pdb
public:
    bool IsValid() const { return _fh != 0; }

    FileHandler* _fh;                  // 0x00
    char field_4[0x20 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(BeepFile, 0x20);

void* operator new(unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb

class MoviePlayer { // NAME: proto-pdb:default.pdb
public:
    MoviePlayer(const std::basic_string<char>& name, unsigned int audio, unsigned int flags);
    ~MoviePlayer();
    void SetAudio(int);
    void Update();
    void Render() const;
    void Play();
    void Stop();
    bool IsValid() const { return _file.IsValid(); }
    bool IsActive() const;

    char field_0[0x8];                 // 0x00
    bool _active;                      // 0x08
    char field_9[0xB - 0x9];           // 0x09
    bool _clear;                       // 0x0B
    bool _cleanup;                     // 0x0C
    char field_D[0x40 - 0xD];          // 0x0D
    BeepFile _file;                    // 0x40
    char field_60[0x94 - 0x60];        // 0x60
};
DECOMP_SIZE_ASSERT(MoviePlayer, 0x94);

class SwampGamePad { // NAME: proto-pdb:default.pdb
public:
    bool IsAnyButton() const;
};

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedRealTime() const { return _actualElapsed; }
    const SwampGamePad* GetPad(int) const;

    float _elapsed;                    // 0x00
    float _actualElapsed;              // 0x04
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    SwampInput& GetInput() const { return *_input; }

    HWND__* _hwnd;                     // 0x00
    SwampInput* _input;                // 0x04
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class VinceIntro { // NAME: proto-pdb:default.pdb
public:
    ~VinceIntro();
    bool Update();
    void Render();

private:
    bool Next();

    bool _stopping;                    // 0x00
    bool _stopped;                     // 0x01
    char field_2[0x4 - 0x2];           // 0x02
    float _delay;                      // 0x04
    float _pauseTime;                  // 0x08
    std::list<std::basic_string<char> > _movies; // 0x0C
    MoviePlayer* _player;              // 0x18
};
DECOMP_SIZE_ASSERT(VinceIntro, 0x1C);

// FUNCTION: USA 0x00125C3E
// PROTOTYPE: PREVIEW 0x0050E755 identical
// NAME: proto-pdb:default.pdb
bool MoviePlayer::IsActive() const
{
    return _active || _clear || _cleanup;
}

// FUNCTION: USA 0x00125C53
// PROTOTYPE: PREVIEW 0x0050E76A identical
// NAME: proto-pdb:default.pdb
void VinceIntro::Render()
{
    if (_player)
        _player->Render();
}

// FUNCTION: USA 0x00125C60
// PROTOTYPE: PREVIEW 0x0050E777 near-identical
// NAME: proto-pdb:default.pdb
bool VinceIntro::Next()
{
    bool firstcall = true;
    if (_player) {
        firstcall = false;
        delete _player;
        _player = 0;
    }
    if (_movies.empty()) {
        if (!firstcall)
            _delay = 0.01f;
        return !firstcall && _pauseTime > 0.0f;
    }
    // !FAKE: c2 CSE; dummy local; the PDB records no local for the front node (held in edi)
    std::list<std::basic_string<char> >::iterator it = _movies.begin();
    std::basic_string<char> name = "d:/movies/" + *it;
    _movies.erase(it);
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
    _player = new("core\\Intro.cpp", 154) MoviePlayer(name, 0, 0x80);
    if (!_player->IsValid()) {
        firstcall = false;
    } else {
        _player->SetAudio(0);
        _delay = firstcall ? 0.0f : _pauseTime;
        if (_delay <= 0.0f)
            _player->Play();
        firstcall = true;
    }
    return firstcall;
}

// FUNCTION: USA 0x00125E7B
// PROTOTYPE: PREVIEW 0x0050E988 identical
// NAME: proto-pdb:default.pdb
VinceIntro::~VinceIntro()
{
    delete _player;
}

// FUNCTION: USA 0x00125E9E
// PROTOTYPE: PREVIEW 0x0050E9AB identical
// NAME: proto-pdb:default.pdb
bool VinceIntro::Update()
{
    if (_delay > 0.0f) {
        _delay -= g_swamp->GetInput().GetElapsedRealTime();
        if (_delay <= 0.0f) {
            _delay = 0.0f;
            if (!_player)
                return false;
            _player->Play();
        }
    }
    if (_player) {
        if (!_stopping && !_stopped && !_player->IsValid() && !_player->IsActive() && !Next())
            return false;
        if (_player)
            _player->Update();
    }
    if (g_swamp->GetInput().GetPad(0)->IsAnyButton()) {
        _stopping = true;
    } else if (_stopping) {
        _stopping = false;
        _stopped = true;
        if (_player)
            _player->Stop();
    } else if (_stopped) {
        _stopped = false;
    }
    return true;
}
