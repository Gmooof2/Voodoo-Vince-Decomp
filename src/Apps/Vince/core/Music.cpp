// UNIT: src/Apps/Vince/core/Music.cpp OBJ: core\XBox\OfficialRelease\Music.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator {};
template <class _InIt, class _OutIt> _OutIt copy(_InIt _First, _InIt _Last, _OutIt _Dest); // retail 0x001133C6
template <class _Ty, class _Alloc> void _Destroy_range(_Ty* _First, _Ty* _Last, _Alloc& _Al); // retail 0x001133E0
struct _Nonscalar_ptr_iterator_tag {};
template <class _InIt, class _OutIt> inline _OutIt _Copy_opt(_InIt _First, _InIt _Last, _OutIt _Dest, _Nonscalar_ptr_iterator_tag)
{
    for (; _First != _Last; ++_Dest, ++_First)
        *_Dest = *_First;
    return (_Dest);
}
template <class E, class T, class A> class basic_string {
public:
    enum { _BUF_SIZE = 16 / sizeof(E) < 1 ? 1 : 16 / sizeof(E) };
    // !FAKE: inliner; hand form of the VC7.1 ctor; retail inlines _Tidy(false) but calls _Tidy(true) by name (C0245)
    basic_string() { _Myres = _BUF_SIZE - 1; _Eos(0); }
    basic_string(const basic_string& _Right);
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true, 0); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); } // src proto-pdb:default.pdb npos
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& operator+=(const basic_string& _Right) { return append(_Right); }
    basic_string& append(const basic_string& _Right) { return append(_Right, 0, npos); }
    basic_string& append(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const E* _Ptr);
    const E& operator[](unsigned int _Off) const;
    const E* c_str() const { return _Myptr(); }
    unsigned int find(const E* _Ptr, unsigned int _Off, unsigned int _Count) const;
    unsigned int find(E _Ch, unsigned int _Off = 0) const { return find((const E*)&_Ch, _Off, 1); }
    int compare(const basic_string& _Right) const;
    static const unsigned int npos = (unsigned int)(-1);
    unsigned int size() const { return _Mysize; }

protected:
    void _Eos(unsigned int _Newsize);
    void _Tidy(bool _Built, unsigned int _Newsize);
    const E* _Myptr() const { return _BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf; }

private:
    A _Alval;                          // 0x00
    union _Bxty {
        E _Buf[_BUF_SIZE];
        E* _Ptr;
    } _Bx;                             // 0x04
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class E, class T, class A> inline bool operator==(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right)
{
    return (_Left.compare(_Right) == 0);
}
template <class E, class T, class A> basic_string<E, T, A> operator+(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right); // retail 0x0002C2AF
template <class E, class T, class A> basic_string<E, T, A> operator+(const basic_string<E, T, A>& _Left, E _Right); // retail 0x000BE397
template <class T, class A = allocator<T> > class vector { // NAME: proto-pdb:default.pdb
public:
    // !FAKE: inliner; hand form of the VC7.1 body; vector() { _Buy(0); } folds to these three stores (C0191)
    vector() { _Myfirst = 0; _Mylast = 0; _Myend = 0; }
    ~vector() { _Tidy(); }
    class iterator {
    public:
        T& operator*() const { return (*_Myptr); }
        T* operator->() const { return (&**this); }
        iterator& operator++() { ++_Myptr; return (*this); }
        bool operator==(const iterator& _Right) const { return (_Myptr == _Right._Myptr); }
        bool operator!=(const iterator& _Right) const { return (!(*this == _Right)); }

        T* _Myptr;
    };
    // !FAKE: inliner; hand form of the VC7.1 begin/end/back; iterator ctors would change erase's matched return
    iterator begin() { iterator _It; _It._Myptr = _Myfirst; return (_It); }
    iterator end() { iterator _It; _It._Myptr = _Mylast; return (_It); }
    T& back() { return (*(_Mylast - 1)); }
    unsigned int size() const;
    bool empty() const { return (size() == 0); }
    void push_back(const T& _Val); // retail 0x00114F87
    void pop_back()
    {
        if (!empty()) {
            _Destroy(_Mylast - 1, _Mylast);
            --_Mylast;
        }
    }
    iterator erase(iterator _Where)
    {
        copy(_Where._Myptr + 1, _Mylast, _Where._Myptr);
        _Destroy(_Mylast - 1, _Mylast);
        --_Mylast;
        return (_Where);
    }

protected:
    void _Destroy(T* _First, T* _Last) { _Destroy_range(_First, _Last, this->_Alval); }
    void _Tidy();

private:
    A _Alval;                          // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

namespace BeepLib { // NAME: proto-pdb:default.pdb
SwampString StripExtension(const SwampString&);
SwampString StripPath(const SwampString&);
SwampString AddExtension(const SwampString&, const SwampString&);
}

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class FileHandler; // NAME: proto-pdb:default.pdb
class SwampScript; // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Vector4 { // NAME: proto-pdb:default.pdb
    Vector4() {}

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Quaternion { // NAME: proto-pdb:default.pdb
    void Apply(const Vector3&, Vector3&) const;
    // !FAKE: inliner; hand form of the inline by-value Apply overload; no before/after recorded (C0191)
    Vector3 Apply(const Vector3& v) const { Vector3 r; Apply(v, r); return r; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

Vector4 GetPlaneEquation(const Vector3&, const Vector3&); // NAME: proto-pdb:default.pdb

struct Matrix; // NAME: proto-pdb:default.pdb
class BeepTDF; // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                 // vtable+0x00
    virtual const struct AABox& GetBounds() const;             // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                  // vtable+0x08
    virtual Quaternion GetWorldRotation() const;               // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);             // vtable+0x10
    virtual void Invalidate();                                 // vtable+0x14
protected:
    virtual void UpdateAnimation(unsigned int);                // vtable+0x18
    virtual void Update();                                     // vtable+0x1C
    virtual void Render(const Matrix&) const;                  // vtable+0x20

public:
    char field_4[0xA4 - 0x4];          // 0x04
};

// Vince derives from Critter -> GatorNode -> ... -> SwampSceneNode (PDB); only the SwampSceneNode base is used here
class Vince : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampScript();                                                       // vtable+0x00
    virtual bool TestActivate() const;                                            // vtable+0x04
    virtual void Activate();                                                      // vtable+0x08
    virtual void Deactivate();                                                    // vtable+0x0C
    virtual void FrameTick();                                                     // vtable+0x10
    virtual void TimerEvent(unsigned int);                                        // vtable+0x14
    virtual void AnimationFinished();                                             // vtable+0x18
    virtual void SecondaryAnimationFinished();                                    // vtable+0x1C
    virtual void AnimationInterrupted();                                          // vtable+0x20
    virtual void SecondaryAnimationInterrupted();                                 // vtable+0x24
    virtual void AnimationTrigger(const SwampString&, float, float, unsigned int); // vtable+0x28
    virtual bool LosingControl(const SwampString&);                               // vtable+0x2C
    virtual void GainingControl(SwampScript*);                                    // vtable+0x30
    virtual void Message(SwampScript*, int, void*);                               // vtable+0x34
    virtual void EnteringRegion(SwampSceneNode*);                                 // vtable+0x38
    virtual void LeavingRegion(SwampSceneNode*);                                  // vtable+0x3C
    virtual void SoundFinished(const SwampString&);                               // vtable+0x40

    SwampString _name;                 // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x4C - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class BeepFile { // NAME: proto-pdb:default.pdb
public:
    bool IsValid() const { return _fh != 0; }

    FileHandler* _fh;                  // 0x00
};

class SoundStreamer { // NAME: proto-pdb:default.pdb
public:
    ~SoundStreamer();
    void Stop();
    void Play();
    void Pause();
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

    MusicManager();
    ~MusicManager();
    void Suspend();
    void Resume();
    void Reset();
    void EndTrack();
    void PlayTrack(const SwampString&);
    void PlayTransition(const SwampString&, const SwampString&, Vector4, float);
    void SetBasePath(const SwampString&);
    void PushOverlay(const SwampString&, bool, SwampScript*);
    void PushMusicOverlay(const SwampString&, bool, SwampScript*);
    void PopOverlay(const SwampString&);
private:
    void ComputeTransition();
    SwampString ShortName(const SwampString&) const;
    SwampString MakePath(const SwampString&) const;
public:

    SoundStreamer* _stream1;           // 0x00
    SoundStreamer* _stream2;           // 0x04
    unsigned int _state;               // 0x08
    unsigned int _toState;             // 0x0C
    bool _bTrack;                      // 0x10
    bool _bOverlay;                    // 0x11
    bool _bTransition;                 // 0x12
    bool _bSuspend;                    // 0x13
    bool _bInstant;                    // 0x14
    float _fVolTarget;                 // 0x18
    float _bVolTarget;                 // 0x1C
    SwampString _playSong;             // 0x20
    Vector4 _plane;                    // 0x3C
    float _falloff;                    // 0x4C
    SwampString _frontSong;            // 0x50
    SwampString _backSong;             // 0x6C
    std::vector<Overlay> _overlay;     // 0x88
    char field_98[0xA0 - 0x98];        // 0x98
    SwampString _basePath;             // 0xA0
    char field_BC[0xC0 - 0xBC];        // 0xBC
};
DECOMP_SIZE_ASSERT(MusicManager, 0xC0);

// FUNCTION: USA 0x001134D0
// PROTOTYPE: PREVIEW 0x004FDD38 identical
// NAME: proto-pdb:default.pdb
template void std::vector<MusicManager::Overlay,std::allocator<MusicManager::Overlay> >::pop_back();

// FUNCTION: USA 0x001134F3
// PROTOTYPE: PREVIEW 0x004FDD5B identical
// NAME: proto-pdb:default.pdb
template std::vector<MusicManager::Overlay,std::allocator<MusicManager::Overlay> >::iterator std::vector<MusicManager::Overlay,std::allocator<MusicManager::Overlay> >::erase(std::vector<MusicManager::Overlay,std::allocator<MusicManager::Overlay> >::iterator);

extern MusicManager* g_MusicManager; // NAME: proto-pdb:default.pdb

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    void SetVolumeSuppression(bool);
};

class GeometryNode; // NAME: proto-pdb:default.pdb

class SwampDebugGeometry { // NAME: proto-pdb:default.pdb
public:
    GeometryNode* AddBox(const Vector3&, float, float, float, const Vector3&, float);
    GeometryNode* AddPlaneSection(const Vector3&, const Quaternion&, const Vector3&, const Vector3&, float);
    GeometryNode* AddRotatedBox(const Vector3&, const Quaternion&, float, float, float, const Vector3&, float);
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x10];                // 0x00
    SwampDebugGeometry* _debugGeometry; // 0x10
    char field_14[0x34 - 0x14];        // 0x14
    SoundManager* _soundManager;       // 0x34
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

enum RegionShape { // NAME: proto-pdb:default.pdb
    REGION_AABB = 0,
    REGION_PLANE = 1,
    REGION_SPHERE = 2,
    REGION_OBB = 3,
    REGION_VERT_CYLINDER = 4,
    REGION_OTHER = 5
};

class RegionTrigger : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    RegionTrigger();
    virtual ~RegionTrigger();
    virtual void Load(const BeepTDF&);                         // vtable+0x24
    virtual void Save(SwampString&);                           // vtable+0x28
    virtual const AABox& GetBounds() const;
    virtual void Show() const;                                 // vtable+0x2C

    RegionShape _shape;                // 0xA4
    char field_A8[0xDC - 0xA8];        // 0xA8
    Vector3 _saveLoc;                  // 0xDC
    Quaternion _saveRot;               // 0xE8
    Vector3 _saveScale;                // 0xF8
    char field_104[0x124 - 0x104];     // 0x104
};
DECOMP_SIZE_ASSERT(RegionTrigger, 0x124);

class MusicRegion : public RegionTrigger { // NAME: proto-pdb:default.pdb
public:
    MusicRegion();
    virtual ~MusicRegion();
    virtual void Load(const BeepTDF&);
    virtual void Save(SwampString&);
    virtual void Show() const;

    SwampString _frontSong;            // 0x124
    SwampString _backSong;             // 0x140
    float _falloff;                    // 0x15C
private:
    static SwampSceneNode* creationFunc();
};
DECOMP_SIZE_ASSERT(MusicRegion, 0x160);

class MusicRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void EnteringRegion(SwampSceneNode*);
    virtual void LeavingRegion(SwampSceneNode*);
};
DECOMP_SIZE_ASSERT(MusicRegionMain, 0x4C);

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

// FUNCTION: USA 0x00112902
// PROTOTYPE: PREVIEW 0x004FD155 identical
// NAME: proto-pdb:default.pdb
void MusicManager::Suspend()
{
    if (!_bSuspend) {
        _bSuspend = true;
        _stream1->Pause();
        _stream2->Pause();
    }
}

// FUNCTION: USA 0x00112921
// PROTOTYPE: PREVIEW 0x004FD177 identical
// NAME: proto-pdb:default.pdb
void MusicRegion::Show() const
{
    Vector3 clr(0.0f, 1.0f, 1.0f);
    switch (_shape) {
    case REGION_AABB:
        g_swamp->_debugGeometry->AddBox(_saveLoc, _saveScale.x, _falloff * 0.02f, _saveScale.z, clr, 0.0f);
        break;
    case REGION_PLANE:
        g_swamp->_debugGeometry->AddPlaneSection(_saveLoc, _saveRot, _saveScale, clr, 0.0f);
        break;
    case REGION_OBB:
        g_swamp->_debugGeometry->AddRotatedBox(_saveLoc, _saveRot, _saveScale.x, _falloff * 0.02f, _saveScale.z, clr, 0.0f);
        break;
    }
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

// FUNCTION: USA 0x00112A73
// PROTOTYPE: PREVIEW 0x004FD2C3 identical
// NAME: proto-pdb:default.pdb
void MusicManager::ComputeTransition()
{
    // !FAKE: c2 register allocation; neutral locals v0, v1; the PDB records no locals here beyond this
    Vector3 v0 = pManager->_vinceCache->GetWorldPosition();
    // !FAKE: c2 x87 evaluation order; reordered terms; source x,y,z emits z,y,x, retail is y,z,x (with < and the
    // repeated divisor: match_one 94.40 -> 100)
    float v1 = _plane.x * v0.x + _plane.z * v0.z + _plane.y * v0.y + _plane.w;
    if (v1 >= _falloff) {
        _fVolTarget = 1.0f;
        _bVolTarget = 0.0f;
    } else if (v1 < -_falloff) {
        _fVolTarget = 0.0f;
        _bVolTarget = 1.0f;
    } else {
        _fVolTarget = (v1 + _falloff) / (_falloff * 2.0f);
        _bVolTarget = -((v1 - _falloff) / (_falloff * 2.0f));
    }
}

// FUNCTION: USA 0x00112AFB
// PROTOTYPE: PREVIEW 0x004FD34B identical
// NAME: proto-pdb:default.pdb
SwampString MusicManager::ShortName(const SwampString& name) const
{
    return BeepLib::StripPath(BeepLib::StripExtension(name));
}

// FUNCTION: USA 0x00112B2C
// PROTOTYPE: PREVIEW 0x004FD37C identical
// NAME: proto-pdb:default.pdb
MusicRegion::MusicRegion()
{
}

// FUNCTION: USA 0x00112B54
// PROTOTYPE: PREVIEW 0x004FD3A4 identical
// NAME: proto-pdb:default.pdb
MusicRegion::~MusicRegion()
{
}

void* operator new(unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x00112B83
// PROTOTYPE: PREVIEW 0x004FD3F3 identical
// NAME: proto-pdb:default.pdb
SwampSceneNode* MusicRegion::creationFunc()
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\core\\Music.h", 14) MusicRegion;
}

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

// FUNCTION: USA 0x00112BE8
// PROTOTYPE: PREVIEW 0x004FD458 identical
// NAME: proto-pdb:default.pdb
SwampString MusicManager::MakePath(const SwampString& name) const
{
    SwampString path;
    if (name.find(':') == SwampString::npos) {
        path = _basePath + name;
    } else {
        path = name;
    }
    // !FAKE: c2 register allocation; named local v0 for the extension temp; retail reloads its address after the
    // ctor instead of using the ctor's eax (C0245 named block local); the block lets it share the operator+ temp slot
    {
        SwampString v0(".wav");
        return BeepLib::AddExtension(path, v0);
    }
}

// FUNCTION: USA 0x00112E96
// PROTOTYPE: PREVIEW 0x004FD706 identical
// NAME: proto-pdb:default.pdb
void MusicRegion::Save(SwampString& s)
{
    RegionTrigger::Save(s);
    if (_frontSong.size()) {
        s += format("\t\tsongfront = %s;\n", _frontSong.c_str());
    }
    if (_backSong.size()) {
        s += format("\t\tsongback = %s;\n", _backSong.c_str());
    }
    s += format("\t\tsongfalloff = %f;\n", _falloff);
}

// FUNCTION: USA 0x00112F6D
// PROTOTYPE: PREVIEW 0x004FD7DD identical
// NAME: proto-pdb:default.pdb
void MusicManager::PlayTrack(const SwampString& name)
{
    switch (_state) {
    case STATE_OVERLAY:
    case STATE_OVERLAY_IN:
    case STATE_OVERLAY_OUT:
    case STATE_OVERLAY_TRANSITION:
        break;
    default:
        _toState = STATE_TRACK;
        break;
    }
    if (_state != STATE_TRANSITION) {
        _frontSong = _backSong = "";
    }
    _bTrack = true;
    _playSong = name;
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

// FUNCTION: USA 0x0011300F
// PROTOTYPE: PREVIEW 0x004FD87F identical
// NAME: proto-pdb:default.pdb
void MusicManager::PlayTransition(const SwampString& front, const SwampString& back, Vector4 plane, float falloff)
{
    switch (_state) {
    case STATE_OVERLAY:
    case STATE_OVERLAY_IN:
    case STATE_OVERLAY_OUT:
    case STATE_OVERLAY_TRANSITION:
        break;
    default:
        _toState = STATE_TRANSITION;
        break;
    }
    _bTransition = true;
    _plane = plane;
    _falloff = falloff;
    _frontSong = front;
    _backSong = back;
}

// FUNCTION: USA 0x00113094
// PROTOTYPE: PREVIEW 0x004FD904 identical
// NAME: proto-pdb:default.pdb
void MusicManager::SetBasePath(const SwampString& basePath)
{
    // !FAKE: c2 register allocation; neutral local v0 for size(); retail keeps size() - 1 in edi across both
    // operator[] calls (local of size() - 1: esi/edi swapped, match_one 90.33 -> 100)
    unsigned int v0 = basePath.size();
    if (basePath[v0 - 1] == '/' || basePath[v0 - 1] == '\\') {
        _basePath = basePath;
    } else {
        _basePath = basePath + '/';
    }
}

// FUNCTION: USA 0x0011320B
// PROTOTYPE: PREVIEW 0x004FDA60 identical
// NAME: proto-pdb:default.pdb
void MusicRegionMain::EnteringRegion(SwampSceneNode* node)
{
    if (node == pManager->_vinceCache) {
        // !FAKE: c2 register allocation; neutral local; retail loads _node once into ebx (match_one reloads without it)
        MusicRegion* p = (MusicRegion*)_node;
        if (p->_frontSong.size() && p->_backSong.size()) {
            // !FAKE: inliner; hand form of the inline by-value Quaternion::Apply plus its copy; no before/after recorded (C0191)
            Vector3 tmp;
            p->_saveRot.Apply(Vector3(0.0f, 1.0f, 0.0f), tmp);
            Vector3 v1(tmp);
            Vector4 plane = GetPlaneEquation(v1, p->_saveLoc);
            g_MusicManager->PlayTransition(p->_frontSong, p->_backSong, plane, p->_falloff * 0.01f);
        } else if (p->_frontSong.size()) {
            g_MusicManager->PlayTrack(p->_frontSong);
        } else {
            g_MusicManager->EndTrack();
        }
    }
}

// FUNCTION: USA 0x001132DF
// PROTOTYPE: PREVIEW 0x004FDB34 identical
// NAME: proto-pdb:default.pdb
void MusicRegionMain::LeavingRegion(SwampSceneNode* node)
{
    if (node == pManager->_vinceCache) {
        // !FAKE: c2 register allocation; neutral local; retail loads _node once into ebx (match_one reloads without it)
        MusicRegion* v0 = (MusicRegion*)_node;
        if (v0->_frontSong.size() && v0->_backSong.size()) {
            // !FAKE: inliner; comma expression; hand form of the inline by-value Quaternion::Apply, copy and input
            // temps live in one full-expression so GetWorldPosition's temp reuses the copy slot (match_one 88.74 -> 100)
            Vector3 tmp;
            Vector4 plane = GetPlaneEquation((v0->_saveRot.Apply(Vector3(0.0f, 1.0f, 0.0f), tmp), Vector3(tmp)), v0->_saveLoc);
            // !FAKE: c2 register allocation; pointer local; retail reads the returned Vector3 through eax (C0191)
            const Vector3* p = &node->GetWorldPosition();
            // !FAKE: c2 x87 evaluation order; reordered terms; source x,y,z gives z,y,x, retail is y,z,x (listing order then equal)
            g_MusicManager->PlayTrack(plane.x * p->x + plane.z * p->z + plane.y * p->y + plane.w >= 0.0f ? v0->_frontSong : v0->_backSong);
        } else if (v0->_backSong.size()) {
            g_MusicManager->PlayTrack(v0->_backSong);
        } else {
            g_MusicManager->EndTrack();
        }
    }
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit Overlay::operator= in this unit (no matched
// function odr-uses it); this assignment odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_Overlay_assign(MusicManager::Overlay* p, const MusicManager::Overlay& v)
{
    *p = v;
}

// FUNCTION: USA 0x0011376F
// PROTOTYPE: PREVIEW 0x004FDFD7 identical
// NAME: proto-pdb:default.pdb
MusicManager::MusicManager()
    : _stream1(0), _stream2(0), _state(STATE_IDLE), _toState(STATE_IDLE),
      _bTrack(false), _bOverlay(false), _bTransition(false), _bSuspend(false), _bInstant(false),
      _fVolTarget(0.0f), _bVolTarget(0.0f)
{
    g_MusicManager = this;
}

// FUNCTION: USA 0x001137D6
// PROTOTYPE: PREVIEW 0x004FE03E identical
// NAME: proto-pdb:default.pdb
MusicManager::~MusicManager()
{
    g_MusicManager = 0;
    delete _stream1;
    delete _stream2;
    _stream1 = 0;
    _stream2 = 0;
}

// FUNCTION: USA 0x0011384E
// PROTOTYPE: PREVIEW 0x004FE0B6 identical
// NAME: proto-pdb:default.pdb
void MusicManager::Reset()
{
    _stream1->Stop();
    _stream2->Stop();
    _state = _toState = STATE_IDLE;
    _bTrack = _bOverlay = _bTransition = _bSuspend = _bInstant = false;
    _playSong = _frontSong = _backSong = "";
    while (_overlay.size())
        _overlay.pop_back();
    g_swamp->_soundManager->SetVolumeSuppression(false);
}

// FUNCTION: USA 0x001138D0
// PROTOTYPE: PREVIEW 0x004FE138 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 46.5 (47.1 with the else-if state chain), bytes mismatch +0x39 (5.2 T12.c22): the inverted state
// if/else gives retail's state block (setne, no xor); left: retail chains the pop branch into the notify tail and lays the
// name search after the idle/track jmp; cl keeps source order in 20 forms (bool/size tests, loop shapes, goto, early return)
void MusicManager::PopOverlay(const SwampString& name)
{
    if (_overlay.size()) {
        Overlay o;
        if (!name.size()) {
            o = _overlay.back();
            _overlay.pop_back();
        } else {
            // !FAKE: c1xx front end; neutral local v0; the PDB records no iterator local
            std::vector<Overlay>::iterator v0;
            for (v0 = _overlay.begin();; ++v0) {
                if (v0 == _overlay.end()) {
                    return;
                }
                if (v0->name == name) {
                    break;
                }
            }
            o = *v0;
            _overlay.erase(v0);
        }
        if (o._scriptToNotify) {
            o._scriptToNotify->SoundFinished(o.name);
        }
        if (!_overlay.size()) {
            if (!_frontSong.size() && !_backSong.size()) {
                _toState = _playSong.size() ? STATE_TRACK : STATE_IDLE;
            } else {
                _toState = STATE_TRANSITION;
            }
        } else {
            _toState = STATE_OVERLAY;
            _bOverlay = true;
        }
    }
}
#endif

// FUNCTION: USA 0x00113123
// PROTOTYPE: PREVIEW 0x004FD993 identical
// NAME: proto-pdb:default.pdb
template MusicManager::Overlay * std::_Copy_opt<MusicManager::Overlay *,MusicManager::Overlay *>(MusicManager::Overlay *,MusicManager::Overlay *,MusicManager::Overlay *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x00114FC7
// PROTOTYPE: PREVIEW 0x004FF7F5 identical
// NAME: proto-pdb:default.pdb
void MusicManager::PushOverlay(const SwampString& name, bool loop, SwampScript* scriptToNotify)
{
    _toState = STATE_OVERLAY;
    _bOverlay = true;
    Overlay o;
    o.name = name;
    o.loop = loop;
    o._scriptToNotify = scriptToNotify;
    o.music = false;
    _overlay.push_back(o);
}

// FUNCTION: USA 0x0011502B
// PROTOTYPE: PREVIEW 0x004FF859 identical
// NAME: proto-pdb:default.pdb
void MusicManager::PushMusicOverlay(const SwampString& name, bool loop, SwampScript* scriptToNotify)
{
    _toState = STATE_OVERLAY;
    _bOverlay = true;
    Overlay o;
    o.name = name;
    o.loop = loop;
    o._scriptToNotify = scriptToNotify;
    o.music = true;
    _overlay.push_back(o);
}
