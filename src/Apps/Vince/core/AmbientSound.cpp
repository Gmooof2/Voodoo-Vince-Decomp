// UNIT: src/Apps/Vince/core/AmbientSound.cpp OBJ: core\XBox\OfficialRelease\AmbientSound.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    const E* c_str() const { return _Myptr(); }
    unsigned int size() const { return _Mysize; }
    static const unsigned int npos = (unsigned int)(-1);
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
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

extern "C" int __cdecl sscanf(const char*, const char*, ...); // NAME: proto-pdb:default.pdb
void* operator new(unsigned int size, const char* file, int line);

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    const SwampString GetBlockName() const { return _blockname; }
    const SwampString GetStringRecord(const SwampString& id, const SwampString& def) const;
    int GetIntRecord(const SwampString& id, int def) const;
    double GetFloatRecord(const SwampString& id, double def) const;
    bool GetBoolRecord(const SwampString& id, bool def) const;

    SwampString _blockname;            // 0x00
    char field_1C[0x58 - 0x1C];        // 0x1C
};
DECOMP_SIZE_ASSERT(BeepTDF, 0x58);

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
    float LengthSquared() const { return x * x + y * y + z * z; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { _RefCount--; }

    SwampString _Name;                 // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class SoundResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }

    T* _Resource;                      // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
};

class SoundManager : public ResourceManager<SoundResource> { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x94];                // 0x00
    Vector3 _listenerPos;              // 0x94
};

class SwampSceneNode;  // NAME: proto-pdb:default.pdb

class SwampScene { // NAME: proto-pdb:default.pdb
public:
    void AddNode(SwampSceneNode* node, unsigned int);
};

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x4];                 // 0x08
    SwampScene* _scene;                // 0x0C
    char field_10[0x34 - 0x10];        // 0x10
    SoundManager* _soundManager;       // 0x34
};

namespace BeepLib { // NAME: proto-pdb:default.pdb
double RandomFloat(double);
}

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

struct AABox;      // NAME: proto-pdb:default.pdb
struct Quaternion; // NAME: proto-pdb:default.pdb
struct Matrix;     // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                                 // vtable+0x00
    virtual const AABox& GetBounds() const;                    // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                  // vtable+0x08
    virtual Quaternion GetWorldRotation() const;               // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);             // vtable+0x10
    virtual void Invalidate();                                 // vtable+0x14
protected:
    virtual void UpdateAnimation(unsigned int);                // vtable+0x18
    virtual void Update();                                     // vtable+0x1C
    virtual void Render(const Matrix&) const;                  // vtable+0x20
public:

    const Vector3& SetLocation(const Vector3& location);

    char field_4[0x4];                 // 0x04
    SwampString _Name;                 // 0x08
    char field_24[0x34 - 0x24];        // 0x24
    Vector3 _Location;                 // 0x34
};

class SoundNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum { // NAME: proto-pdb:default.pdb
        LOOPED = 0x1,
        USE3D = 0x8
    };

    SoundNode(const ResourceHandle<SoundResource>& sh, unsigned int flags, float, float);
    virtual ~SoundNode();
    virtual void Update();
    virtual void Invalidate();
    void Play();
    void Stop();
    bool IsPlaying() const;
    void SetVolume(float vol);
    void SetMinDistance(float dist);
    void SetMaxDistance(float dist);

    char field_40[0xA8 - 0x40];        // 0x40
    unsigned int _flags;               // 0xA8
    char field_AC[0xBC - 0xAC];        // 0xAC
    float _maxDist;                    // 0xBC
    char field_C0[0xD0 - 0xC0];        // 0xC0
};

class AmbientSound : public SoundNode { // NAME: proto-pdb:default.pdb
public:
    AmbientSound(const ResourceHandle<SoundResource>& sh, unsigned int flags);
    virtual void Update();
    static void CreateAmbientSound(const BeepTDF* tdf);

private:
    bool IsInRange() const;

    float _delay;                      // 0xD0
    float _minDelay;                   // 0xD4
    float _variance;                   // 0xD8
};
DECOMP_SIZE_ASSERT(AmbientSound, 0xDC);

// FUNCTION: USA 0x00138047
// PROTOTYPE: PREVIEW 0x0052227E identical
// NAME: proto-pdb:default.pdb
AmbientSound::AmbientSound(const ResourceHandle<SoundResource>& sh, unsigned int flags)
    : SoundNode(sh, flags, 0.0f, 0.0f), _delay(0.0f), _minDelay(0.0f), _variance(0.0f)
{
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes mismatch (masked 0; 3 bytes = the fmul operand address)
// FUNCTION: USA 0x00138088
// PROTOTYPE: PREVIEW 0x005222DB near-identical
// NAME: proto-pdb:default.pdb
bool AmbientSound::IsInRange() const
{
    const Vector3& listener = g_swamp->_soundManager->_listenerPos;
    Vector3 diff(_Location.x - listener.x, _Location.y - listener.y, _Location.z - listener.z);
    float range = _maxDist * 1.1f;
    return diff.LengthSquared() <= range * range;
}
#endif

// FUNCTION: USA 0x001380DE
// PROTOTYPE: PREVIEW 0x00522331 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 98.48 (100.0 bytes match with IsInRange compiled in the unit: cl drops the ecx reload after a callee it compiled); wall: IsInRange guarded (its 1.1f at 0x1F9EC0 has no pin)
void AmbientSound::Update()
{
    if (!(_flags & LOOPED)) {
        _delay -= g_swamp->_input->GetElapsedTime();
        if (_delay <= 0.0f) {
            _delay = (float)BeepLib::RandomFloat(_variance) + _minDelay;
            if (!(_flags & USE3D) || IsInRange())
                Play();
        }
    } else if (_delay > 0.0f) {
        _delay -= g_swamp->_input->GetElapsedTime();
        if (_delay <= 0.0f) {
            _delay = 0.0f;
            if (!(_flags & USE3D) || IsInRange())
                Play();
        }
    } else if (_flags & USE3D) {
        if (IsInRange()) {
            if (!IsPlaying())
                Play();
        } else if (IsPlaying()) {
            Stop();
        }
    }
    SoundNode::Update();
}
#endif

// FUNCTION: USA 0x001381B8
// PROTOTYPE: PREVIEW 0x0052240B identical
// NAME: proto-pdb:default.pdb
void AmbientSound::CreateAmbientSound(const BeepTDF* tdf)
{
    ResourceHandle<SoundResource> sh = g_swamp->_soundManager->Load(tdf->GetBlockName());
    unsigned int flags = 0;
    float delay = (float)tdf->GetFloatRecord("sounddelay", 0.0);
    float variance = (float)tdf->GetFloatRecord("soundvariance", 0.0);
    if (delay <= 0.0f && variance <= 0.0f)
        flags = LOOPED;
    bool spatialize = tdf->GetBoolRecord("sound3d", false);
    if (spatialize)
        flags |= USE3D;
    // !FAKE: c2 register allocation; dummy local; the PDB records no local for the new node (held in esi)
    AmbientSound* p = new("core\\AmbientSound.cpp", 96) AmbientSound(sh, flags);
    SwampString instanceName = tdf->GetStringRecord("instancename", "");
    if (instanceName.size() == 0)
        instanceName = tdf->GetBlockName();
    p->_Name = instanceName;
    Vector3 loc;
    sscanf(tdf->GetStringRecord("location", "").c_str(), "%f %f %f", &loc.x, &loc.y, &loc.z);
    p->SetLocation(loc);
    if (!(flags & LOOPED))
        p->_delay = (float)BeepLib::RandomFloat(variance) + delay;
    p->_minDelay = delay;
    p->_variance = variance;
    if (flags & USE3D) {
        float range = (float)tdf->GetFloatRecord("soundrange", 10000.0);
        p->SetMinDistance(range * 0.00125f);
        p->SetMaxDistance(range * 0.01f);
    }
    int volume = tdf->GetIntRecord("soundvolume", 100);
    p->SetVolume(volume * 0.01f);
    float initDelay = (float)tdf->GetFloatRecord("soundinitialdelay", 0.0);
    if (initDelay > 0.0f)
        p->_delay += initDelay;
    else if ((flags & (LOOPED | USE3D)) == LOOPED)
        p->Play();
    g_swamp->_scene->AddNode(p, 0);
}
