// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampAnimationResource.cpp OBJ: Xbox\Official_Release\SwampAnimationResource.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string();
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const E* _Ptr);
    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x1C];
};

template <class T, class A = allocator<T> > class vector {
public:
    vector() : _Myfirst(0), _Mylast(0), _Myend(0) {}

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
template <class T> struct less {};
template <class T1, class T2> struct pair {
    T1 first;                          // 0x00
    T2 second;
};
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits {
public:
    typedef pair<const K, T> value_type;
};
template <class Tr> class _Tree {
    struct _Node;

protected:
    _Tree() { _Init(); }
    void _Init();

private:
    char _Alval;                       // 0x00
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > > class multimap
    : public _Tree<_Tmap_traits<K, T, Pr, A, true> > {
public:
    multimap() {}
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

void BeepFree(void* p); // NAME: proto-pdb:default.pdb

namespace BeepLib { // NAME: proto-pdb:default.pdb
SwampString StripPath(const SwampString&);
}

struct HottieRawData { // NAME: proto-pdb:default.pdb
    void* header;                      // 0x00
    void* data;                        // 0x04
    unsigned int size;                 // 0x08
};
DECOMP_SIZE_ASSERT(HottieRawData, 0xC);

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
    unsigned int name;                 // 0x04
    unsigned int bone;                 // 0x08
    unsigned int flags;                // 0x0C
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
    const char* GetString(unsigned int i) const
    {
        return (const char*)this + offsetStrings + ((const unsigned int*)((const char*)this + offsetStringTable))[i];
    }

    unsigned int id;                   // 0x00
    unsigned int version;              // 0x04
    float duration;                    // 0x08
    unsigned int numTracks;            // 0x0C
    unsigned int numTriggers;          // 0x10
    char field_14[0x20 - 0x14];        // 0x14
    unsigned int offsetTriggers;       // 0x20
    unsigned int offsetAuxInfo;        // 0x24
    unsigned int offsetStringTable;    // 0x28
    unsigned int offsetStrings;        // 0x2C
};
DECOMP_SIZE_ASSERT(GatorAnimationFile, 0x30);

class LightManager { // NAME: proto-pdb:default.pdb
public:
    static LightNode* CreateGatorLight(const GatorAnimationFile* anim, const GatorLightInfo* info);
};

class ResourceBase { // NAME: proto-pdb:default.pdb
protected:
    ResourceBase(const SwampString&);

public:
    SwampString _Name;                 // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    AnimationResource(const SwampString& name);
    ~AnimationResource();
    unsigned int NumTracks() const;
    void GetTriggerName(int index, SwampString& name, unsigned int& boneindex, unsigned int& flags) const;
    float CalculateTrigger(int index, float t) const;
    LightNode* CreateLight() const;

    const GatorAnimationFile* _anim;   // 0x24
    void* _myAnim;                     // 0x28
    float _length;                     // 0x2C
    int _triggers;                     // 0x30
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

// FUNCTION: USA 0x000D285D
// PROTOTYPE: PREVIEW 0x004BDB18 identical
// NAME: proto-pdb:default.pdb
float AnimationResource::CalculateTrigger(int index, float t) const
{
    // if/else with the return-0 arm first: cl keeps the shared return-0 block right after the _anim test, as retail
    if (!_anim) {
        return 0.0f;
    } else {
        const GatorTriggerChannel& v1 = _anim->GetTriggers()[index].GetChannel(_anim);
        if (!v1.numCurves)
            return 0.0f;
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

class BundleWrapper;

template <class T> class Ptr { // NAME: proto-pdb:default.pdb
public:
    T* _Object;                        // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    typedef std::multimap<SwampString, T*> NameMap;
    struct ResourceBundle { // NAME: proto-pdb:default.pdb
        Ptr<BundleWrapper> _bundle;    // 0x00
        bool _dontDiscard;             // 0x04
    };

    ResourceManager();
    HottieRawData GetDataInBundle(const SwampString&);
    void SetExtension(const SwampString& ext) { _Extension = ext; }

    NameMap _Objects;                              // 0x00
    SwampString _basePath;                         // 0x0C
    SwampString _Extension;                        // 0x28
    std::vector<ResourceBundle> _bundles;          // 0x44
    bool _missingResourceMessages;                 // 0x54
};

// C0231: the verbatim body (SwampResourceManager.cpp); the call's fold VA 0x000C1028 is that unit's matched row,
// so our byte-equal copy rides its hole as a fold duplicate
template <class T> ResourceManager<T>::ResourceManager() : _missingResourceMessages(true)
{
}

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
public:
    typedef void (*SoundTriggerCallback)(const SwampString&, float);

    void SetSoundTriggerCallback(SoundTriggerCallback func);
    void SoundTriggerHit(const SwampString& soundName, float length) const;

private:
    AnimationManager();

    SoundTriggerCallback _soundTriggerCallback; // 0x58 // src proto-pdb:default.pdb SoundTriggerCallback
};
DECOMP_SIZE_ASSERT(AnimationManager, 0x5C);

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x18];                        // 0x00
    AnimationManager* _animationManager;       // 0x18
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000D2913
// PROTOTYPE: PREVIEW 0x004BDBCE identical
// NAME: proto-pdb:default.pdb
void AnimationManager::SetSoundTriggerCallback(SoundTriggerCallback func)
{
    _soundTriggerCallback = func;
}

// FUNCTION: USA 0x000D291D
// PROTOTYPE: PREVIEW 0x004BDBD8 identical
// NAME: proto-pdb:default.pdb
void AnimationManager::SoundTriggerHit(const SwampString& soundName, float length) const
{
    if (_soundTriggerCallback)
        _soundTriggerCallback(soundName, length);
}

// FUNCTION: USA 0x000D2A55
// PROTOTYPE: PREVIEW 0x004BDD10 identical
// NAME: proto-pdb:default.pdb
AnimationResource::AnimationResource(const SwampString& name)
    : ResourceBase(name), _anim(0), _myAnim(0), _length(0.0f), _triggers(0)
{
    // !FAKE: c1xx front end; block scope; retail destroys shortname before the header check (swampfireflyresource.cpp form)
    {
        SwampString shortname = BeepLib::StripPath(name);
        HottieRawData rawData = g_swamp->_animationManager->GetDataInBundle(shortname);
        _anim = (const GatorAnimationFile*)rawData.data;
    }
    if (_anim) {
        // !FAKE: c2 block layout; redundant test; keeps the failure block first, as retail (match_one 90.05 -> 100.0)
        if (_anim && (_anim->id != 0x4D494E41 || _anim->version != 0xB)) { // retail constants
            BeepFree(_myAnim);
            _myAnim = 0;
            _anim = 0;
        } else {
            _length = _anim->duration;
            _triggers = _anim->numTriggers;
            _valid = true;
        }
    }
}

// FUNCTION: USA 0x000D2AE9
// PROTOTYPE: PREVIEW 0x004BDDA4 identical
// NAME: proto-pdb:default.pdb
void AnimationResource::GetTriggerName(int index, SwampString& name, unsigned int& boneindex, unsigned int& flags) const
{
    if (!_anim) {
        name = "";
        boneindex = 0;
        flags = 0;
        return;
    }
    const GatorAnimTrigger& v1 = _anim->GetTriggers()[index];
    name = _anim->GetString(v1.name);
    boneindex = v1.bone;
    flags = v1.flags;
}

// FUNCTION: USA 0x000D2B53
// PROTOTYPE: PREVIEW 0x004BDE0E identical
// NAME: proto-pdb:default.pdb
AnimationManager::AnimationManager()
{
    _soundTriggerCallback = 0;
    SetExtension(".anim");
}
