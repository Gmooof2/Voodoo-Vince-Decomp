// UNIT: src/Libs/Swamp/Xbox/Official_Release/SoundNode.cpp OBJ: Xbox\Official_Release\SoundNode.obj OPT: /O1 /Oy
// !FAKE: c1xx front end; file-scope warning pragma; VC7.1 <string> without /EH warns C4530 and the build refuses it
#pragma warning(disable: 4530)
#include <string>
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class T> struct less;
template <class K, class Pr, class A, bool Mfl> class _Tset_traits {
public:
    typedef K key_type;
};
template <class Tr> class _Tree {
public:
    unsigned int erase(const typename Tr::key_type& k);

private:
    char _Alval;                       // 0x00
    void* _Myhead;                     // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class Pr = less<K>, class A = allocator<K> > class set
    : public _Tree<_Tset_traits<K, Pr, A, false> > {
};
}

struct AABox;      // NAME: proto-pdb:default.pdb
struct Vector3 {   // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);
struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);
struct IDirectSoundBuffer { // NAME: proto-pdb:default.pdb
    long __stdcall SetPosition(float x, float y, float z, unsigned long apply);
};

class GatorNode { // NAME: proto-pdb:default.pdb
public:
    void GetBoneOrientation(int index, Quaternion& rot, Vector3& pos) const;
};

extern "C" long __stdcall IDirectSoundBuffer_SetMinDistance(IDirectSoundBuffer*, float, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall IDirectSoundBuffer_SetMaxDistance(IDirectSoundBuffer*, float, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" long __stdcall IDirectSoundBuffer_Pause(IDirectSoundBuffer*, unsigned long); // src proto-pdb:default.pdb IDirectSoundBuffer_Pause
extern "C" long __stdcall IDirectSoundBuffer_GetStatus(IDirectSoundBuffer*, unsigned long*); // src proto-pdb:default.pdb IDirectSoundBuffer_GetStatus
extern "C" long __stdcall IDirectSoundBuffer_SetVolume(IDirectSoundBuffer*, long); // src proto-pdb:default.pdb IDirectSoundBuffer_SetVolume

// Retail layout: the volume getters inline with retail bodies (proto has out-of-line getters with a _mute
// test); retail reads 0x70/0x78/0x80/0x84 (proto PDB names differ by version: field_<off>).
class SoundManager { // NAME: proto-pdb:default.pdb
public:
    float GetSoundVolume() const { return field_78 * _soundVolume; }
    float GetVoiceVolume() const { return field_80 * _soundVolume; }
    float GetNoMuteVolume() const { return field_70; } // 0x70
    float GetOverlayVolume() const { return field_84 * _soundVolume; }

    char field_0[0x64];                // 0x00
    float _soundVolume;                // 0x64
    char field_68[0x8];                // 0x68
    float field_70;                    // 0x70
    char field_74[0x4];                // 0x74
    float field_78;                    // 0x78
    char field_7C[0x4];                // 0x7C
    float field_80;                    // 0x80
    float field_84;                    // 0x84
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x34];                // 0x00
    SoundManager* _soundManager;       // 0x34
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    void QueueSoundFinishedMessage(const std::string&);
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        Invalid = 0x1
    };

    virtual ~SwampSceneNode();                         // vtable+0x00
    virtual const AABox& GetBounds() const;            // vtable+0x04
    virtual Vector3 GetWorldPosition() const;          // vtable+0x08
    virtual Quaternion GetWorldRotation() const;       // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);     // vtable+0x10
    virtual void Invalidate();                         // vtable+0x14

    SwampScript* GetScript() const;
    const Vector3& SetLocation(const Vector3& location);
    const Quaternion& SetRotation(const Quaternion& rotation);
    bool IsValid() const { return !(_RenderingOptions & Invalid); }

    int _Type;                         // 0x04
    std::string _Name;                 // 0x08
    SwampSceneNode* _Parent;           // 0x24
    char field_28[0x60];               // 0x28
    unsigned int _RenderingOptions;    // 0x88
    char field_8C[0x18];               // 0x8C
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class AnimationControl; // NAME: proto-pdb:default.pdb

class SoundNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum { // NAME: proto-pdb:default.pdb
        LOOPED = 0x1,
        ONESHOT = 0x2,
        PLAYNOW = 0x4,
        USE3D = 0x8,
        NOTIFYPARENT = 0x20,
        VOICE = 0x100,
        OVERLAY = 0x200,
        NOMUTE = 0x400,
        NOAUTOPAUSE = 0x1000,
        DEFAULT_FLAGS = 0x6
    };

    virtual void Update();
    virtual void Invalidate();

    void Stop();
    void Pause();
    void SetVolume(float vol);
    bool IsPaused() const;
    void SetMinDistance(float dist);
    void SetMaxDistance(float dist);
    bool IsPlaying() const;
    void UpdatePosition();

protected:
    void DoNotifyParent();

public:
    bool _active;                      // 0xA4
    unsigned int _flags;               // 0xA8
    int _useParentBoneIndex;           // 0xAC
    float _volume;                    // 0xB0
    char field_B4[0x4];                // 0xB4
    float _minDist;                    // 0xB8
    float _maxDist;                    // 0xBC
    char field_C0[0x8];                // 0xC0
    IDirectSoundBuffer* _obj;          // 0xC8
    AnimationControl* _owner;          // 0xCC
};
DECOMP_SIZE_ASSERT(SoundNode, 0xD0);

class AnimationControl { // NAME: proto-pdb:default.pdb
public:
    void SoundInvalidated(SoundNode* p) { _sounds.erase(p); }

    char field_0[0x40];                // 0x00
    double _lastUpdateTime;            // 0x40
    char field_48[0x58 - 0x48];        // 0x48
    std::set<SoundNode*> _sounds;      // 0x58
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

// FUNCTION: USA 0x000C5B46
// PROTOTYPE: PREVIEW 0x004B0E49 identical
// NAME: proto-pdb:default.pdb
void SoundNode::Pause()
{
    if (_obj && _active) {
        _active = false;
        IDirectSoundBuffer_Pause(_obj, 1);
    }
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 2)
// FUNCTION: USA 0x000C5B67
// PROTOTYPE: PREVIEW 0x004B0E6A near-identical
// NAME: proto-pdb:default.pdb
void SoundNode::SetVolume(float vol)
{
    if (_obj) {
        long db = -10000;
        float master;
        if (_flags & NOMUTE)
            master = g_swamp->_soundManager->GetNoMuteVolume();
        else if (_flags & VOICE)
            master = g_swamp->_soundManager->GetVoiceVolume();
        else if (_flags & OVERLAY)
            master = g_swamp->_soundManager->GetOverlayVolume();
        else
            master = g_swamp->_soundManager->GetSoundVolume();
        if (vol > 0.0f && master > 0.0f)
            db = (long)((1.0f - master * vol) * -3000.0f);
        IDirectSoundBuffer_SetVolume(_obj, db);
    }
    _volume = vol;
}
#endif

// FUNCTION: USA 0x000C5C6F
// PROTOTYPE: PREVIEW 0x004B0F71 identical
// NAME: proto-pdb:default.pdb
void SoundNode::SetMinDistance(float dist)
{
    if (_obj)
        IDirectSoundBuffer_SetMinDistance(_obj, dist, 1);
    _minDist = dist;
}

// FUNCTION: USA 0x000C5C9A
// PROTOTYPE: PREVIEW 0x004B0F9C identical
// NAME: proto-pdb:default.pdb
void SoundNode::SetMaxDistance(float dist)
{
    if (_obj)
        IDirectSoundBuffer_SetMaxDistance(_obj, dist, 1);
    _maxDist = dist;
}

// FUNCTION: USA 0x000C5CC5
// PROTOTYPE: PREVIEW 0x004B0FC7 identical
// NAME: proto-pdb:default.pdb
bool SoundNode::IsPlaying() const
{
    if (_obj) {
        unsigned long status = 0;
        IDirectSoundBuffer_GetStatus(_obj, &status);
        return (status & 3) != 0;
    }
    return false;
}

// FUNCTION: USA 0x000C5CF2
// PROTOTYPE: PREVIEW 0x004B0FF4 identical
// NAME: proto-pdb:default.pdb
bool SoundNode::IsPaused() const
{
    if (_obj) {
        unsigned long status = 0;
        IDirectSoundBuffer_GetStatus(_obj, &status);
        return (status & 2) != 0;
    }
    return false;
}

// FUNCTION: USA 0x000C5D1C
// PROTOTYPE: PREVIEW 0x004B101E identical
// NAME: proto-pdb:default.pdb
void SoundNode::UpdatePosition()
{
    if (_flags & USE3D) {
        if (_useParentBoneIndex != -1) {
            Quaternion rot;
            Vector3 pos;
            ((GatorNode*)_Parent)->GetBoneOrientation(_useParentBoneIndex, rot, pos);
            SetRotation(rot);
            SetLocation(pos);
        }
        if (_obj) {
            Vector3 pos = GetWorldPosition();
            _obj->SetPosition(pos.x, pos.y, pos.z, 1);
        }
    }
}

// FUNCTION: USA 0x000C5E2B
// PROTOTYPE: PREVIEW 0x004B112D identical
// NAME: proto-pdb:default.pdb
void SoundNode::DoNotifyParent()
{
    if (_flags & NOTIFYPARENT && _Parent) {
        SwampScript* p = _Parent->GetScript();
        if (p)
            p->QueueSoundFinishedMessage(_Name);
    }
}

// FUNCTION: USA 0x000C5EFD
// PROTOTYPE: PREVIEW 0x004B11FF identical
// NAME: proto-pdb:default.pdb
void SoundNode::Update()
{
    if (IsValid() && _active) {
        if (!IsPlaying()) {
            if (_flags & ONESHOT)
                Invalidate();
            else
                Stop();
        } else {
            UpdatePosition();
        }
    }
}

// FUNCTION: USA 0x000C623E
// NAME: proto-pdb:default.pdb
void SoundNode::Invalidate()
{
    Stop();
    if (_owner) {
        _owner->SoundInvalidated(this);
        _owner = 0;
    }
    SwampSceneNode::Invalidate();
}
