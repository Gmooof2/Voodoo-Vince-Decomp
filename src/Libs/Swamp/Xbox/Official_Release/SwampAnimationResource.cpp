// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampAnimationResource.cpp OBJ: Xbox\Official_Release\SwampAnimationResource.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

void BeepFree(void* p); // NAME: proto-pdb:default.pdb

struct GatorAnimationFile;  // NAME: proto-pdb:default.pdb
struct GatorLightInfo;      // NAME: proto-pdb:default.pdb
class LightNode;            // NAME: proto-pdb:default.pdb

struct GatorAnimAuxInfo { // NAME: proto-pdb:default.pdb
    const GatorLightInfo* GetLightInfo(const GatorAnimationFile* file) const
    {
        return offsetLight ? (const GatorLightInfo*)((const char*)file + offsetLight) : 0;
    }

    char field_0[0x18];                // 0x00
    unsigned int offsetLight;          // 0x18
    unsigned int reserved;             // 0x1C
};
DECOMP_SIZE_ASSERT(GatorAnimAuxInfo, 0x20);

struct GatorTriggerCurve { // NAME: proto-pdb:default.pdb
    float time;                        // 0x00
    float value;                       // 0x04
};
DECOMP_SIZE_ASSERT(GatorTriggerCurve, 0x8);

struct GatorTriggerChannel { // NAME: proto-pdb:default.pdb
    const GatorTriggerCurve& GetCurve(unsigned int i) const { return ((const GatorTriggerCurve*)(this + 1))[i]; }

    unsigned int numCurves;            // 0x00
};
DECOMP_SIZE_ASSERT(GatorTriggerChannel, 0x4);

struct GatorAnimTrigger { // NAME: proto-pdb:default.pdb
    const GatorTriggerChannel& GetChannel(const GatorAnimationFile* file) const
    {
        return *(const GatorTriggerChannel*)((const char*)file + offset);
    }

    unsigned int offset;               // 0x00
    char field_4[0xC];                 // 0x04
};
DECOMP_SIZE_ASSERT(GatorAnimTrigger, 0x10);

struct GatorAnimationFile { // NAME: proto-pdb:default.pdb
    const GatorAnimTrigger* GetTriggers() const
    {
        return offsetTriggers ? (const GatorAnimTrigger*)((const char*)this + offsetTriggers) : 0;
    }
    const GatorAnimAuxInfo* GetAuxInfo() const
    {
        return offsetAuxInfo ? (const GatorAnimAuxInfo*)((const char*)this + offsetAuxInfo) : 0;
    }

    unsigned int id;                   // 0x00
    unsigned int version;              // 0x04
    float duration;                    // 0x08
    unsigned int numTracks;            // 0x0C
    char field_10[0x20 - 0x10];        // 0x10
    unsigned int offsetTriggers;       // 0x20
    unsigned int offsetAuxInfo;        // 0x24
    char field_28[0x30 - 0x28];        // 0x28
};
DECOMP_SIZE_ASSERT(GatorAnimationFile, 0x30);

class LightManager { // NAME: proto-pdb:default.pdb
public:
    static LightNode* CreateGatorLight(const GatorAnimationFile* anim, const GatorLightInfo* info);
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    SwampString _Name;                 // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    ~AnimationResource();
    unsigned int NumTracks() const;
    float CalculateTrigger(int index, float t) const;
    LightNode* CreateLight() const;

    const GatorAnimationFile* _anim;   // 0x24
    void* _myAnim;                     // 0x28
    float _length;                     // 0x2C
    int _triggers;                     // 0x30
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

#ifdef NON_MATCHING
// best: match_one objdiff 90.106384 (3 runs; early-return form 88.94, switch form 64.53)
// FUNCTION: USA 0x000D285D
// PROTOTYPE: PREVIEW 0x004BDB18 identical
// NAME: proto-pdb:default.pdb
float AnimationResource::CalculateTrigger(int index, float t) const
{
    if (_anim) {
        const GatorTriggerChannel& v1 = _anim->GetTriggers()[index].GetChannel(_anim);
        if (v1.numCurves) {
            if (v1.numCurves == 1)
                return v1.GetCurve(0).value;
            float v2 = v1.GetCurve(0).value;
            for (unsigned int i = 0; i < v1.numCurves; i++) {
                if (t >= v1.GetCurve(i).time)
                    v2 = v1.GetCurve(i).value;
                else if (t < v1.GetCurve(i).time)
                    break;
            }
            return v2;
        }
    }
    return 0.0f;
}
#endif

// FUNCTION: USA 0x000D28CF
// NAME: proto-pdb:default.pdb
LightNode* AnimationResource::CreateLight() const
{
    if (!_anim)
        return 0;
    const GatorAnimAuxInfo* v1 = _anim->GetAuxInfo();
    if (!v1)
        return 0;
    const GatorLightInfo* v2 = v1->GetLightInfo(_anim);
    if (!v2)
        return 0;
    return LightManager::CreateGatorLight(_anim, v2);
}

// FUNCTION: USA 0x000D2905
// PROTOTYPE: PREVIEW 0x004BDBC0 identical
// NAME: proto-pdb:default.pdb
unsigned int AnimationResource::NumTracks() const
{
    return _anim ? _anim->numTracks : 0;
}

// FUNCTION: USA 0x000D2A3C
// PROTOTYPE: PREVIEW 0x004BDCF7 identical
// NAME: proto-pdb:default.pdb
AnimationResource::~AnimationResource()
{
    BeepFree(_myAnim);
}

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceManager();
    void SetExtension(const SwampString& ext) { _Extension = ext; }

    char field_0[0x28];                // 0x00
    SwampString _Extension;            // 0x28
    char field_44[0x58 - 0x44];        // 0x44
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
private:
    AnimationManager();

    void* _soundTriggerCallback;       // 0x58 // src proto-pdb:default.pdb SoundTriggerCallback
};
DECOMP_SIZE_ASSERT(AnimationManager, 0x5C);

// FUNCTION: USA 0x000D2B53
// PROTOTYPE: PREVIEW 0x004BDE0E identical
// NAME: proto-pdb:default.pdb
AnimationManager::AnimationManager()
{
    _soundTriggerCallback = 0;
    SetExtension(".anim");
}
