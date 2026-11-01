// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampGatorNode.cpp OBJ: Xbox\Official_Release\SwampGatorNode.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class T> class allocator;
template <class T> struct less;
template <class T1, class T2> struct pair;
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits {};
template <class Tr> class _Tree {
public:
    ~_Tree() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    void* _Myhead;                     // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > > class map
    : public _Tree<_Tmap_traits<K, T, Pr, A, false> > {
};
template <class T, class A = allocator<T> > class vector {
public:
    ~vector() { _Tidy(); }
    class const_iterator {
    public:
        const_iterator(T* ptr) : _Myptr(ptr) {}
        const T& operator*() const { return *_Myptr; }
        const_iterator& operator+=(int off) { _Myptr += off; return *this; }
        const_iterator operator+(int off) const { const_iterator tmp = *this; return tmp += off; }
        T* _Myptr;
    };
    const_iterator begin() const { return const_iterator(_Myfirst); }
    const T& operator[](unsigned int pos) const { return *(begin() + pos); }

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
template <class T, class A = allocator<T> > class deque {
public:
    ~deque() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _pad00[0x14];
};
template <class T, class C = deque<T> > class stack { // src proto-pdb:default.pdb stack
protected:
    C c;                               // 0x00
};
}

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

struct AABox { // NAME: proto-pdb:default.pdb
    bool Empty() const;

    Vector3 low;                       // 0x00
    Vector3 high;                      // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

struct SwampDeformation { // NAME: proto-pdb:default.pdb
    void Identity();

    Vector3 scaling;                   // 0x00
    Vector3 rotation;                  // 0x0C
    Vector3 translation;               // 0x18
    float overlayWeight;               // 0x24
};
DECOMP_SIZE_ASSERT(SwampDeformation, 0x28);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();
    void SetAnimationSpeed(float speed);
    float GetAnimationSpeed() const { return _speed; }

    char field_24[0x2C - 0x24];        // 0x24
    float _speed;                      // 0x2C
    char field_30[0x68 - 0x30];        // 0x30
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

template <class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) ++_Resource->_RefCount; }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    bool IsValid() const { return _Resource != 0; }
    T& operator()() { return *_Resource; }
    const T& operator()() const { return *_Resource; }
    ResourceHandle& operator=(const ResourceHandle& h);

    T* _Resource;                      // 0x00
};

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
    char field_24[0x34 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

class SwampScript;     // NAME: proto-pdb:default.pdb
class TextureResource; // NAME: proto-pdb:default.pdb

struct Matrix { // NAME: proto-pdb:default.pdb
    char field_0[0x40];                // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void GetBoneOrientation(int boneIndex, const Matrix& deform, Matrix& result) const;

    char field_24[0x60 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(ModelResource, 0x60);
struct Quaternion { // NAME: proto-pdb:default.pdb
    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetGameTime() const { return _gameTimePublic; }

    char field_0[0x8];                 // 0x00
    float _gameTimePublic;             // 0x08
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb
class SwampShadow;     // NAME: proto-pdb:default.pdb
class LightNode;       // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { DeferredButNotSorted = 0x20 };
    void SetOption(unsigned int option, bool on, bool recurse);
    void AddChild(SwampSceneNode* child);
    virtual ~SwampSceneNode();
    virtual const AABox& GetBounds() const;
    virtual Vector3 GetWorldPosition() const;
    virtual Quaternion GetWorldRotation() const;
    virtual void SetWorldPosition(const Vector3&);
    virtual void Invalidate();

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

// src proto-pdb:default.pdb GatorNode base CollidableCylinder : CollidableNode : SwampSceneNode, all at 0x0; only SwampSceneNode modelled
class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetShadowTexture(const ResourceHandle<TextureResource>& tex);
    void SetProjectedShadow(const ResourceHandle<ModelResource>& mh);
    void SetEdgeGlowParams(const ResourceHandle<ModelResource>& mh, const Vector3& color, float sizeMin, float sizeMax,
                           float pulseFactor);
    void DoLazyAnimationUpdating(unsigned char numGuaranteedBones);
    void SetAnimSpeed(float speed);
    float GetAnimSpeed();
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>& anim, bool a, bool b);
    void StopAnim();
    ResourceHandle<AnimationControl> GetCurrentAnimation() const { return _currentAnim; }
    virtual void Invalidate();
    Matrix GetBoneOrientation(int boneIndex) const;
    void UpdatePlatformPos();
    void SetShadow(bool b, float radius);

    char field_A4[0xB4 - 0xA4];        // 0xA4
    ResourceHandle<ModelResource> _model; // 0xB4
    char field_B8[0xC7 - 0xB8];        // 0xB8
    unsigned char _lazyUpdateBoneCount; // 0xC7
    char field_C8[0xEC - 0xC8];        // 0xC8
    SwampShadow* _shadow;              // 0xEC
    ResourceHandle<TextureResource> _shadowTex; // 0xF0
    char field_F4[0xF8 - 0xF4];        // 0xF4
    ResourceHandle<ModelResource> _projectedShadow; // 0xF8
    char field_FC[0x114 - 0xFC];       // 0xFC
    ResourceHandle<ModelResource> _edgeGlowModel; // 0x114
    Vector3 _edgeColor;                // 0x118
    float _edgeSizeMin;                // 0x124
    float _edgeSizeMax;                // 0x128
    float _edgePulseFactor;            // 0x12C
    char field_130[0x138 - 0x130];     // 0x130
    LightNode* _light;                 // 0x138
    char field_13C[0x140 - 0x13C];     // 0x13C
    ResourceHandle<AnimationControl> _currentAnim; // 0x140
    char field_144[0x16C - 0x144];     // 0x144
    std::vector<Matrix> _deformMatrices; // 0x16C
    char field_17C[0x1A0 - 0x17C];     // 0x17C
    Vector3 _platformPosLast;          // 0x1A0
    Quaternion _platformRotLast;       // 0x1AC
    float _platformPosTime;            // 0x1BC
    char field_1C0[0x200 - 0x1C0];     // 0x1C0
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

// src proto-pdb:default.pdb LightNode bases SwampSceneNode at 0x0, LightDefinition at 0xA4; neither reached // src proto-pdb:default.pdb LightDefinition
class LightNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl> StartPlayback(const ResourceHandle<AnimationResource>& anim, bool a, bool b);
    void StopPlayback();

    char field_A4[0x160 - 0xA4];       // 0xA4
};
DECOMP_SIZE_ASSERT(LightNode, 0x160);

void* operator new(unsigned int size, const char* file, int line);

class SwampShadow : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampShadow(GatorNode* parent, float radius);

    char field_A4[0x134 - 0xA4];       // 0xA4
};
DECOMP_SIZE_ASSERT(SwampShadow, 0x134);

class TextureManager { // NAME: proto-pdb:default.pdb
public:
    void SetAnimatedTextureControl(const ResourceHandle<AnimationControl>& ach, SwampScript* script);

    char field_0[0x70];                // 0x00
    ResourceHandle<AnimationControl> _animControl; // 0x70
    SwampScript* _animScript;          // 0x74
    char field_78[0x84 - 0x78];        // 0x78
};
DECOMP_SIZE_ASSERT(TextureManager, 0x84);

// FUNCTION: USA 0x000AE1AD
// NAME: proto-pdb:default.pdb
void GatorNode::Invalidate()
{
    if (_shadow) {
        _shadow->Invalidate();
        _shadow = 0;
    }
    if (_light) {
        _light->Invalidate();
        _light = 0;
    }
    SwampSceneNode::Invalidate();
}

// FUNCTION: USA 0x000AE1E2
// PROTOTYPE: PREVIEW 0x00499069 identical
// NAME: proto-pdb:default.pdb
void GatorNode::DoLazyAnimationUpdating(unsigned char numGuaranteedBones)
{
    _lazyUpdateBoneCount = numGuaranteedBones;
}

// FUNCTION: USA 0x000AE1EF
// PROTOTYPE: PREVIEW 0x00499076 identical
// NAME: proto-pdb:default.pdb
void GatorNode::SetShadow(bool b, float radius)
{
    bool v1 = _shadow != 0;
    if (b != v1) {
        if (b) {
            // !FAKE: retail string literal; the original passes its build's __FILE__/__LINE__, ours differs
            _shadow = new("SwampGatorNode.cpp", 971) SwampShadow(this, radius);
            AddChild(_shadow);
        } else {
            _shadow->Invalidate();
            _shadow = 0;
        }
    }
}

// FUNCTION: USA 0x000AE33E
// PROTOTYPE: PREVIEW 0x004991B2 identical
// NAME: proto-pdb:default.pdb
bool AABox::Empty() const
{
    return low.x == 3.40282347e+38f;
}

// FUNCTION: USA 0x000AE356
// NAME: proto-pdb:default.pdb
void SwampDeformation::Identity()
{
    scaling.x = 1.0f;
    scaling.y = 1.0f;
    scaling.z = 1.0f;
    rotation.x = 0.0f;
    rotation.y = 0.0f;
    rotation.z = 0.0f;
    translation.x = 0.0f;
    translation.y = 0.0f;
    translation.z = 0.0f;
    overlayWeight = 0.0f;
}

// FUNCTION: USA 0x000AE3C4
// PROTOTYPE: PREVIEW 0x00499238 identical
// NAME: proto-pdb:default.pdb
void GatorNode::UpdatePlatformPos()
{
    if (_platformPosTime < g_swamp->_input->GetGameTime()) {
        _platformPosTime = g_swamp->_input->GetGameTime();
        _platformPosLast = SwampSceneNode::GetWorldPosition();
        _platformRotLast = SwampSceneNode::GetWorldRotation();
    }
}

// FUNCTION: USA 0x000AE420
// NAME: proto-pdb:default.pdb
void GatorNode::SetShadowTexture(const ResourceHandle<TextureResource>& tex)
{
    _shadowTex = tex;
}

// FUNCTION: USA 0x000AE5D8
// NAME: proto-pdb:default.pdb
void LightNode::StopPlayback()
{
    StartPlayback(ResourceHandle<AnimationResource>(), false, false);
}

// FUNCTION: USA 0x000AE60A
// PROTOTYPE: PREVIEW 0x00499421 identical
// NAME: proto-pdb:default.pdb
void GatorNode::SetAnimSpeed(float speed)
{
    if (_currentAnim.IsValid())
        GetCurrentAnimation()().SetAnimationSpeed(speed);
}

#ifdef NON_MATCHING
// FUNCTION: USA 0x000AE632
// PROTOTYPE: PREVIEW 0x00499449 identical
// NAME: proto-pdb:default.pdb
// best: objdiff 85.875, bytes mismatch
float GatorNode::GetAnimSpeed()
{
    if (_currentAnim.IsValid())
        return GetCurrentAnimation()().GetAnimationSpeed();
    return 0.0f;
}
#endif

// FUNCTION: USA 0x000AE65B
// NAME: proto-pdb:default.pdb
void GatorNode::SetProjectedShadow(const ResourceHandle<ModelResource>& mh)
{
    _projectedShadow = mh;
    SetOption(DeferredButNotSorted, mh.IsValid(), false);
}

// FUNCTION: USA 0x000AE686
// PROTOTYPE: PREVIEW 0x0049949D identical
// NAME: proto-pdb:default.pdb
void GatorNode::SetEdgeGlowParams(const ResourceHandle<ModelResource>& mh, const Vector3& color, float sizeMin,
                                  float sizeMax, float pulseFactor)
{
    _edgeGlowModel = mh;
    _edgeColor = color;
    _edgeSizeMin = sizeMin;
    _edgeSizeMax = sizeMax;
    _edgePulseFactor = pulseFactor;
}

// FUNCTION: USA 0x000AE91C
// PROTOTYPE: PREVIEW 0x00499707 identical
// NAME: proto-pdb:default.pdb
void TextureManager::SetAnimatedTextureControl(const ResourceHandle<AnimationControl>& ach, SwampScript* script)
{
    _animControl = ach;
    _animScript = script;
}

// FUNCTION: USA 0x000AEB8F
// NAME: proto-pdb:default.pdb
Matrix GatorNode::GetBoneOrientation(int boneIndex) const
{
    Matrix result;
    _model().GetBoneOrientation(boneIndex, _deformMatrices[boneIndex], result);
    return result;
}

// FUNCTION: USA 0x000AF132
// PROTOTYPE: PREVIEW 0x00499F1D identical
// NAME: proto-pdb:default.pdb
void GatorNode::StopAnim()
{
    StartAnimation(ResourceHandle<AnimationResource>(), false, false);
}

// FUNCTION: USA 0x000B04FE
// NAME: proto-pdb:default.pdb
class ProfileBar { // NAME: proto-pdb:default.pdb
public:
    struct ProfileEntry; // NAME: proto-pdb:default.pdb

    char field_0[0x10];                // 0x00
    std::map<unsigned int, ProfileEntry*> _entries; // 0x10
    std::vector<unsigned int> _renderOrder;         // 0x1C
    std::stack<unsigned int> _accum;                // 0x2C
};
DECOMP_SIZE_ASSERT(ProfileBar, 0x40);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1ProfileBar@@QAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_ProfileBar_dtor(ProfileBar* p)
{
    p->ProfileBar::~ProfileBar();
}
