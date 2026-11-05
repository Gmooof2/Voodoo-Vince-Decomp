// UNIT: src/Apps/Vince/core/MultiAnimation.cpp OBJ: core\XBox\OfficialRelease\MultiAnimation.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits; // src proto-pdb:default.pdb char_traits
template <class T> class allocator; // src proto-pdb:default.pdb allocator
template <class E, class T, class A> class basic_string {
public:
    ~basic_string() { _Tidy(true); }
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
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};
class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle(const ResourceHandle& rh) : _Resource(rh._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    ResourceHandle& operator=(const ResourceHandle& rh);
    bool IsValid() const { return _Resource != 0; }

    T* _Resource;                      // 0x00
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString& name);
    bool EnableMissingResourceMessages(bool enable)
    {
        bool old = _missingResourceMessages;
        _missingResourceMessages = enable;
        return old;
    }

    char field_0[0x54];                // 0x00
    bool _missingResourceMessages;     // 0x54
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};
class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x18];                // 0x00
    AnimationManager* _animationManager; // 0x18
    char field_1C[0xC];                // 0x1C
    ModelManager* _modelManager;       // 0x28
    char field_2C[0x3C];               // 0x2C
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

template <class T> class SwampShuffleQueue { // NAME: proto-pdb:default.pdb
public:
    void PushBack(T);
    void Shuffle();

    char field_0[0x14];                // 0x00
};

class MultiAnimation { // NAME: proto-pdb:default.pdb
public:
    void Load(const SwampString& baseName, const char* formatString, int startindex);

    SwampShuffleQueue<ResourceHandle<AnimationResource> > _animQueue; // 0x00
};
DECOMP_SIZE_ASSERT(MultiAnimation, 0x14);

class MultiModel { // NAME: proto-pdb:default.pdb
public:
    void Load(const SwampString& baseName, const char* formatString, int startindex);

    SwampShuffleQueue<ResourceHandle<ModelResource> > _modelQueue; // 0x00
};
DECOMP_SIZE_ASSERT(MultiModel, 0x14);

// FUNCTION: USA 0x0010F945
// NAME: proto-pdb:default.pdb
void MultiAnimation::Load(const SwampString& baseName, const char* formatString, int startindex)
{
    bool v0 = g_swamp->_animationManager->EnableMissingResourceMessages(false); // !FAKE: no PDB local for the old flag
    for (int v1 = startindex;; v1++) { // !FAKE: no PDB local for the index
        SwampString filename = format(formatString, baseName.c_str(), v1);
        ResourceHandle<AnimationResource> ah = g_swamp->_animationManager->Load(filename);
        if (!ah.IsValid()) {
            AnimationManager* v2 = g_swamp->_animationManager; // !FAKE: manager loaded before the args as retail
            ah = v2->Load(baseName);
            if (ah.IsValid())
                _animQueue.PushBack(ah);
            break;
        }
        _animQueue.PushBack(ah);
    }
    g_swamp->_animationManager->EnableMissingResourceMessages(v0);
    _animQueue.Shuffle();
}

// FUNCTION: USA 0x0010FA48
// NAME: proto-pdb:default.pdb
void MultiModel::Load(const SwampString& baseName, const char* formatString, int startindex)
{
    bool v0 = g_swamp->_modelManager->EnableMissingResourceMessages(false); // !FAKE: no PDB local for the old flag
    for (int v1 = startindex;; v1++) { // !FAKE: no PDB local for the index
        SwampString filename = format(formatString, baseName.c_str(), v1);
        ResourceHandle<ModelResource> mh = g_swamp->_modelManager->Load(filename);
        if (!mh.IsValid()) {
            ModelManager* v2 = g_swamp->_modelManager; // !FAKE: manager loaded before the args as retail
            mh = v2->Load(baseName);
            if (mh.IsValid())
                _modelQueue.PushBack(mh);
            break;
        }
        _modelQueue.PushBack(mh);
    }
    g_swamp->_modelManager->EnableMissingResourceMessages(v0);
    _modelQueue.Shuffle();
}
