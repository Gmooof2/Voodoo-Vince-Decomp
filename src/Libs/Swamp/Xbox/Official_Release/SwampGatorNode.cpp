// UNIT: src/Libs/Swamp/Xbox/Official_Release/SwampGatorNode.cpp OBJ: Xbox\Official_Release\SwampGatorNode.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

#pragma warning(disable: 4530) // no /EH in the unit flags; as soundnode.cpp
#include <xstring>
#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specialization (C0132); retail calls the string copy ctor out of line; declared
// before <vector>/<stdexcept> so their bodies call it rather than emit it (4.4 T4.c4 setup for the throw rows)
template <> std::basic_string<char,std::char_traits<char>,std::allocator<char> >::basic_string(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
#endif
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <xmmintrin.h>

#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specialization (C0132, C0154); retail calls basic_string::_Copy in another unit;
// its visible body would emit an __ehhandler$ funclet here (C0090: gate objects extra, relink .text$x refusal)
template <> void std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Copy(unsigned int,
                                                                                               unsigned int);
// !FAKE: c1xx front end; declaration-only explicit specializations (C0090); retail calls the string ctor and _Tidy out of line
// (GatorNode::UpdateAnimLight); their header bodies would emit here with the memcpy/strlen externals
template <> std::basic_string<char, std::char_traits<char>, std::allocator<char> >::basic_string(const char*);
template <> void std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Tidy(bool, unsigned int);
#endif

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    unsigned int GetRGB() const;
    float Length() const;
    Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};

struct AABox { // NAME: proto-pdb:default.pdb
    bool Empty() const;
    void AddPointPair(const Vector3* pts);

    Vector3 low;                       // 0x00
    Vector3 high;                      // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

struct SwampDeformation { // NAME: proto-pdb:default.pdb
    SwampDeformation() {}
    void Identity();

    Vector3 scaling;                   // 0x00
    Vector3 rotation;                  // 0x0C
    Vector3 translation;               // 0x18
    float overlayWeight;               // 0x24
};
DECOMP_SIZE_ASSERT(SwampDeformation, 0x28);

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { ++_RefCount; }
    void SubRef() { --_RefCount; }
    const std::string& GetName() const { return _Name; }

    std::string _Name;                 // 0x00
    unsigned int _RefCount;            // 0x1C
    bool _valid;                       // 0x20
};
DECOMP_SIZE_ASSERT(ResourceBase, 0x24);

template <class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    // retail (GatorNode::StartAnimation 0x000AF0F1) increments the new pointer's count with no null test
    ResourceHandle(T* r) : _Resource(r) { _Resource->AddRef(); }
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) ++_Resource->_RefCount; }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    bool IsValid() const { return _Resource != 0; }
    T& operator()() { return *_Resource; }
    const T& operator()() const { return *_Resource; }
    bool operator==(const ResourceHandle& h) const { return _Resource == h._Resource; }
    ResourceHandle& operator=(const ResourceHandle& h);

    T* _Resource;                      // 0x00
};

// visible body: cl 13.10 /O1 elides a null temporary's dtor after operator= only with it (C0136)
template <class T> ResourceHandle<T>& ResourceHandle<T>::operator=(const ResourceHandle& h)
{
    if (_Resource)
        _Resource->SubRef();
    _Resource = h._Resource;
    if (_Resource)
        _Resource->AddRef();
    return *this;
}

class AnimationResource;
class SwampSceneNode;
class GatorNode;
class SwampScript;
class LightNode;

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
    // access: the retail ctor symbol is protected (??0AnimationControl@@IAE...); GatorNode constructs it
    friend class GatorNode;

public:
    void SubRef();
    void SetAnimationSpeed(float speed);
    bool Update(SwampScript*, std::vector<SwampDeformation>&, unsigned int,
                bool secondary);
    float GetAnimationSpeed() const { return _speed; }
    float GetAnimationTime() const { return _time; }
    void SetAnimationTime(float t) { _time = t; }
    bool IsLooped() const { return _looped; }
    bool IsReverse() const { return !_forward; }
    bool IsLightEnabled() const { return _lightEnabled; }
    const ResourceHandle<AnimationResource>& GetAnimHandle() { return _anim; }

protected:
    AnimationControl(const ResourceHandle<AnimationResource>& anim, SwampSceneNode* owner, bool looped, bool reverse,
                     bool secondary);

public:
    ResourceHandle<AnimationResource> _anim; // 0x24
    SwampSceneNode* _owner;            // 0x28
    float _speed;                      // 0x2C
    bool _paused;                      // 0x30
    bool _looped;                      // 0x31
    bool _forward;                     // 0x32
    char field_33[0x36 - 0x33];        // 0x33
    bool _lightEnabled;                // 0x36
    char field_37[0x38 - 0x37];        // 0x37
    float _time;                       // 0x38
    char field_3C[0x68 - 0x3C];        // 0x3C
};
DECOMP_SIZE_ASSERT(AnimationControl, 0x68);

class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    unsigned int NumTracks() const;
    LightNode* CreateLight() const;

    char field_24[0x34 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(AnimationResource, 0x34);

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum { // NAME: proto-pdb:default.pdb
        ANIMATION_FINISHED = 0x0,
        ANIMATION2_FINISHED = 0x1,
        ANIMATION_INTERRUPTED = 0x2,
        ANIMATION2_INTERRUPTED = 0x3
    };
    void QueueAnimationMessage(unsigned int msg);
    void RemoveMatchingQueuedMessages(unsigned int msg);
};

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
    char field_24[0x30 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(TextureResource, 0x30);

struct Vector2 { // NAME: proto-pdb:default.pdb
    Vector2() {}
    Vector2(float _u, float _v) : u(_u), v(_v) {}
    void Zero() { u = 0.0f; v = 0.0f; }

    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

class TextureOverrides { // NAME: proto-pdb:default.pdb
public:
    struct Override { // NAME: proto-pdb:default.pdb
        ResourceHandle<TextureResource> original;    // 0x00
        ResourceHandle<TextureResource> replacement; // 0x04
    };

    void AddOverride(const ResourceHandle<TextureResource>& originalTex,
                     const ResourceHandle<TextureResource>& replacementTex);
    bool empty() const;

    std::vector<Override> _overrides;  // 0x00
};
DECOMP_SIZE_ASSERT(TextureOverrides::Override, 0x8);
DECOMP_SIZE_ASSERT(TextureOverrides, 0x10);

class LightManager { // NAME: proto-pdb:default.pdb
public:
    struct LightLocation { // NAME: proto-pdb:default.pdb
        float xPos;                    // 0x00
        const class LightNode* light;  // 0x04
    };
};
DECOMP_SIZE_ASSERT(LightManager::LightLocation, 0x8);

struct Vector4 { // NAME: proto-pdb:default.pdb
    operator Vector3() const { return *(const Vector3*)this; }

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Vector4, 0x10);

struct Matrix { // NAME: proto-pdb:default.pdb
    const Vector4& operator[](int i) const { return row[i]; }
    void BatchApply(const Vector3* in, Vector3* out, int count) const;

    Vector4 row[4];                    // 0x00
};
DECOMP_SIZE_ASSERT(Matrix, 0x40);

class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void GetBoneOrientation(int boneIndex, const Matrix& deform, Matrix& result) const;
    void ApplyDeformationsToSkeletion(std::vector<Matrix>&, const std::vector<SwampDeformation>&,
                                      unsigned int) const;
    void ApplyDeformationsToSkeletion(std::vector<Matrix>&, const std::vector<SwampDeformation>&,
                                      const std::vector<SwampDeformation>&,
                                      unsigned int) const;
    void ApplyDeformationsToSkeletion(std::vector<Matrix>&, const std::vector<SwampDeformation>&,
                                      const std::vector<SwampDeformation>&, float,
                                      unsigned int) const;
    unsigned int GetStripCount() const;
    unsigned int GetBoneCount() const;
    int GetBoneIndex(const std::string& name, bool) const;
    const AABox& GetBoneBounds(unsigned int bone) const;

    char field_24[0x60 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(ModelResource, 0x60);
struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion() {}
    Quaternion(float x, float y, float z) { Set(x, y, z); }
    void Set(float x, float y, float z);
    void Apply(const Vector3& in, Vector3& out) const;

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    float GetElapsedTime() const { return _elapsed; }
    float GetGameTime() const { return _gameTimePublic; }

    float _elapsed;                    // 0x00
    char field_4[0x8 - 0x4];           // 0x04
    float _gameTimePublic;             // 0x08
};

template <class T> class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const std::string& name);
};

class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
};

class TextureManager;

class Swamp { // NAME: proto-pdb:default.pdb
public:
    TextureManager& GetTextureManager() const { return *_textureManager; }

    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x18 - 0x8];          // 0x08
    AnimationManager* _animationManager; // 0x18
    char field_1C[0x38 - 0x1C];        // 0x1C
    TextureManager* _textureManager;   // 0x38
    char field_3C[0x60 - 0x3C];        // 0x3C
    unsigned int _frameID;             // 0x60
};

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb
class SwampShadow;     // NAME: proto-pdb:default.pdb
class LightNode;       // NAME: proto-pdb:default.pdb

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { SortedRender = 0x10, DeferredButNotSorted = 0x20, LocationSorting = 0x2000 };
    enum NodeRenderingSettings { Alpha = 0x0 }; // NAME: proto-pdb:default.pdb
    void SetOption(unsigned int option, bool on, bool recurse);
    void SetRenderingSetting(unsigned int setting, unsigned int value, bool recurse);
    SwampScript* GetScript() const;
    void AddChild(SwampSceneNode* child);
    virtual ~SwampSceneNode();
    virtual const AABox& GetBounds() const;
    virtual Vector3 GetWorldPosition() const;
    virtual Quaternion GetWorldRotation() const;
    virtual void SetWorldPosition(const Vector3&);
    virtual void Invalidate();
protected: // retail ??_7CollidableNode slots name ?Update@SwampSceneNode@@MAEXXZ etc. (C0377)
    virtual void UpdateAnimation(unsigned int pass);
    virtual void Update();
    virtual void Render(const Matrix& m) const;

public:
    char field_4[0x6C - 0x4];          // 0x04
    AABox _BoundingBox;                // 0x6C
    unsigned int _lastRenderFrame;     // 0x84
    char field_88[0xA4 - 0x88];        // 0x88
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

// src proto-pdb:default.pdb GatorNode base CollidableCylinder : CollidableNode : SwampSceneNode, all at 0x0
class CollisionResultObject;       // NAME: proto-pdb:default.pdb
template <class T> class Ptr;      // NAME: proto-pdb:default.pdb

class CollidableNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    CollidableNode(int type);
    virtual ~CollidableNode() {} // retail ??1GatorNode 0x000B051A inlines it (vptr store + ??1SwampSceneNode)
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float) const;
    virtual bool CanJorgeCollideWithMe(const SwampSceneNode*, const Vector3&) const;
    virtual bool DoesJorgeRepelMe(const SwampSceneNode*, bool) const;

    float _sphereRadius;               // 0xA4
    bool _collidable;                  // 0xA8
};
DECOMP_SIZE_ASSERT(CollidableNode, 0xAC);

class CollidableCylinder : public CollidableNode { // NAME: proto-pdb:default.pdb
public:
    CollidableCylinder(int type);
    virtual ~CollidableCylinder() {} // retail ??1GatorNode 0x000B051A inlines it (dead vptr store)
    virtual Ptr<CollisionResultObject> CheckLine(const Vector3&, const Vector3&, float) const;

    float _height;                     // 0xAC
    float _radius;                     // 0xB0
};
DECOMP_SIZE_ASSERT(CollidableCylinder, 0xB4);

class SwampSpookyRenderer { // NAME: proto-pdb:default.pdb
public:
    SwampSpookyRenderer(unsigned int bufferWidth, unsigned int bufferHeight, unsigned int backgroundColor,
                        unsigned int flags);
    ~SwampSpookyRenderer();

    char field_0[0x58];                // 0x00
};
DECOMP_SIZE_ASSERT(SwampSpookyRenderer, 0x58);

// FUNCTION: USA 0x000B0B93
// NAME: proto-pdb:default.pdb
// ??_GGatorNode: compiler-generated with the vtable the ctor emits; calls ??1 out of line (5.3 T6)
class GatorNode : public CollidableCylinder { // NAME: proto-pdb:default.pdb
public:
    enum BlendMode { OVERLAY = 0x0, TWEEN = 0x1 }; // NAME: proto-pdb:default.pdb

    GatorNode(const ResourceHandle<ModelResource>& mh);
    virtual ~GatorNode();
    virtual void Update();
    virtual void Render(const Matrix& m) const;
    virtual void UpdateAnimation(unsigned int pass);
    virtual Vector3 GetWorldPosition() const;
    virtual void SetWorldPosition(const Vector3& pos);
    virtual void SetModel(const ResourceHandle<ModelResource>& mh);
    ResourceHandle<AnimationControl>& StartSecondaryAnimation(const ResourceHandle<AnimationResource>& anim, bool looped,
                                                              bool reverse);
    ResourceHandle<AnimationControl> TransitionAnimation(ResourceHandle<AnimationResource> anim, float delay,
                                                         bool looped, bool reverse);
    void CalcBounds(unsigned int numBones) const;
    void SetSuperEdgeGlowParams(const ResourceHandle<ModelResource>& mh, const Vector3& color, unsigned int bufferWidth,
                                unsigned int bufferHeight, float pulseFactor);
    void DoSpookyRendering(unsigned int width, unsigned int height, unsigned int backgroundColor, float alpha,
                           bool blur);
    void OverrideTexture(const ResourceHandle<TextureResource>& original,
                         const ResourceHandle<TextureResource>& replacement);
    void StartUVScroll();
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
    Matrix GetBoneOrientation(const std::string& boneName) const;
    const std::vector<Matrix>& GetDeformationMatricesLast() const;
    void OverrideTexture(const std::string& original, const std::string& replacement);
    void UpdatePlatformPos();
    void SetShadow(bool b, float radius);

protected:
    void UpdateModel();
    void UpdateDeformationMatrices();
    void UpdateAnimLight();

public:
    ResourceHandle<ModelResource> _model; // 0xB4
    ResourceHandle<ModelResource> _nextModel; // 0xB8
    bool _newModel;                    // 0xBC
    bool _newParticles;                // 0xBD
    bool _hasCollisionBSP;             // 0xBE
    bool _haveWorldPosLast;            // 0xBF
    bool _movingPlatform;              // 0xC0
    bool _acceptsShadows;              // 0xC1
    bool _useAllLights;                // 0xC2
    bool _uvScrolling;                 // 0xC3
    bool _dirtyAnimation;              // 0xC4
    bool _edgeGlow;                    // 0xC5
    bool _enableAnimatedLight;         // 0xC6
    unsigned char _lazyUpdateBoneCount; // 0xC7
    Vector3 _worldPosLast;             // 0xC8
    int _posBone;                      // 0xD4
    BlendMode _blendMode;              // 0xD8
    float _tweenFactor;                // 0xDC
    Vector3 _ambientColor;             // 0xE0
    SwampShadow* _shadow;              // 0xEC
    ResourceHandle<TextureResource> _shadowTex; // 0xF0
    int _shadowBone;                   // 0xF4
    ResourceHandle<ModelResource> _projectedShadow; // 0xF8
    Vector3 _projectedShadowLightLoc;  // 0xFC
    Vector3 _projectedShadowLightColor; // 0x108
    ResourceHandle<ModelResource> _edgeGlowModel; // 0x114
    Vector3 _edgeColor;                // 0x118
    float _edgeSizeMin;                // 0x124
    float _edgeSizeMax;                // 0x128
    float _edgePulseFactor;            // 0x12C
    SwampSpookyRenderer* _superEdgeGlow; // 0x130
    SwampSpookyRenderer* _spooky;      // 0x134
    LightNode* _light;                 // 0x138
    TextureOverrides* _texOverrides;   // 0x13C
    ResourceHandle<AnimationControl> _currentAnim; // 0x140
    ResourceHandle<AnimationControl> _secondaryAnim; // 0x144
    ResourceHandle<AnimationControl> _textureAnim; // 0x148
    std::vector<SwampDeformation> _deformInfo; // 0x14C
    std::vector<SwampDeformation> _deformInfo_2nd; // 0x15C
    std::vector<Matrix> _deformMatrices; // 0x16C
    float _animTransitionTimeTotal;    // 0x17C
    float _animTransitionTimer;        // 0x180
    float _animSoundRange;             // 0x184
    float _animSoundVolume;            // 0x188
    unsigned int _animTriggerFlags;    // 0x18C
    std::vector<Matrix> _deformMatricesLast; // 0x190
    Vector3 _platformPosLast;          // 0x1A0
    Quaternion _platformRotLast;       // 0x1AC
    float _platformPosTime;            // 0x1BC
    std::vector<bool> _stripControl;   // 0x1C0
    unsigned int _modelRenderFlags;    // 0x1D4
    std::vector<Vector2> _t0Offset;    // 0x1D8
    std::vector<Vector2> _t1Offset;    // 0x1E8
    Vector2 _uvScrollingSpeed;         // 0x1F8
};
DECOMP_SIZE_ASSERT(GatorNode, 0x200);

// src proto-pdb:default.pdb LightNode bases SwampSceneNode at 0x0, LightDefinition at 0xA4; neither reached // src proto-pdb:default.pdb LightDefinition
class LightNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual void UpdateAnimation(unsigned int pass); // retail ?UpdateAnimation@LightNode@@UAEXI@Z 0x000CC61C
    ResourceHandle<AnimationControl> StartPlayback(const ResourceHandle<AnimationResource>& anim, bool a, bool b);
    void StopPlayback();
    bool IsEnabled() const { return _enabled; }
    void Enable();
    void Disable();
    void UpdateCachedWorldPosition();
    void SetAnimSpeed(float speed) { _speed = speed; }

    char field_A4[0x154 - 0xA4];       // 0xA4
    bool _enabled;                     // 0x154
    char field_155[0x158 - 0x155];     // 0x155
    ResourceHandle<AnimationControl> _currentPath; // 0x158
    float _speed;                      // 0x15C
};
DECOMP_SIZE_ASSERT(LightNode, 0x160);

void* operator new(unsigned int size, const char* file, int line);

namespace BeepLib { // NAME: proto-pdb:default.pdb
std::string ReplaceExtension(const std::string& s, const std::string& ext);
}

class SwampShadow : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    SwampShadow(GatorNode* parent, float radius);

    char field_A4[0x134 - 0xA4];       // 0xA4
};
DECOMP_SIZE_ASSERT(SwampShadow, 0x134);

class TextureManager : public ResourceManager<TextureResource> { // NAME: proto-pdb:default.pdb
public:
    void SetAnimatedTextureControl(const ResourceHandle<AnimationControl>& ach, SwampScript* script);

    char field_0[0x70];                // 0x00
    ResourceHandle<AnimationControl> _animControl; // 0x70
    SwampScript* _animScript;          // 0x74
    char field_78[0x84 - 0x78];        // 0x78
};
DECOMP_SIZE_ASSERT(TextureManager, 0x84);

// Generated by tools/gen_stl.py (4.3 T7.c1); bodies from the VC 7.1 headers
#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specializations (C0132, C0154); retail calls these out of line; the
// header bodies would emit them here with EH funclets and throw info that relink refuses (C0090)
namespace std {
template <> std::vector<bool,std::allocator<bool> >::iterator copy<std::vector<bool,std::allocator<bool> >::iterator,std::vector<bool,std::allocator<bool> >::iterator>(std::vector<bool,std::allocator<bool> >::iterator,std::vector<bool,std::allocator<bool> >::iterator,std::vector<bool,std::allocator<bool> >::iterator);
template <> std::vector<bool,std::allocator<bool> >::iterator copy_backward<std::vector<bool,std::allocator<bool> >::iterator,std::vector<bool,std::allocator<bool> >::iterator>(std::vector<bool,std::allocator<bool> >::iterator,std::vector<bool,std::allocator<bool> >::iterator,std::vector<bool,std::allocator<bool> >::iterator);
template <> void vector<Matrix,std::allocator<Matrix> >::_Tidy();
template <> void vector<SwampDeformation,std::allocator<SwampDeformation> >::_Insert_n(std::vector<SwampDeformation,std::allocator<SwampDeformation> >::iterator,unsigned int,SwampDeformation const &);
template <> void vector<SwampDeformation,std::allocator<SwampDeformation> >::_Tidy();
template <> void vector<Vector2,std::allocator<Vector2> >::_Insert_n(std::vector<Vector2,std::allocator<Vector2> >::iterator,unsigned int,Vector2 const &);
template <> std::vector<SwampDeformation,std::allocator<SwampDeformation> >::iterator vector<SwampDeformation,std::allocator<SwampDeformation> >::erase(std::vector<SwampDeformation,std::allocator<SwampDeformation> >::iterator,std::vector<SwampDeformation,std::allocator<SwampDeformation> >::iterator);
template <> std::vector<Vector2,std::allocator<Vector2> >::iterator vector<Vector2,std::allocator<Vector2> >::erase(std::vector<Vector2,std::allocator<Vector2> >::iterator,std::vector<Vector2,std::allocator<Vector2> >::iterator);
template <> std::vector<unsigned int,std::allocator<unsigned int> >::iterator vector<unsigned int,std::allocator<unsigned int> >::erase(std::vector<unsigned int,std::allocator<unsigned int> >::iterator,std::vector<unsigned int,std::allocator<unsigned int> >::iterator);
template <> unsigned int vector<Matrix,std::allocator<Matrix> >::capacity()const;
template <> unsigned int vector<Matrix,std::allocator<Matrix> >::size()const;
template <> unsigned int vector<SwampDeformation,std::allocator<SwampDeformation> >::capacity()const;
template <> unsigned int vector<SwampDeformation,std::allocator<SwampDeformation> >::size()const;
template <> unsigned int vector<Vector2,std::allocator<Vector2> >::size()const;
template <> unsigned int vector<unsigned int,std::allocator<unsigned int> >::size()const;
template <> void vector<unsigned int,std::allocator<unsigned int> >::resize(unsigned int,unsigned int);
template <> Matrix * _Uninitialized_copy<Matrix *,Matrix *,std::allocator<Matrix> >(Matrix *,Matrix *,Matrix *,std::allocator<Matrix> &);
template <> Matrix * copy<Matrix *,Matrix *>(Matrix *,Matrix *,Matrix *);
template <> SwampDeformation * _Uninitialized_copy<SwampDeformation *,SwampDeformation *,std::allocator<SwampDeformation> >(SwampDeformation *,SwampDeformation *,SwampDeformation *,std::allocator<SwampDeformation> &);
template <> SwampDeformation * copy<SwampDeformation *,SwampDeformation *>(SwampDeformation *,SwampDeformation *,SwampDeformation *);
template <> void vector<Vector2,std::allocator<Vector2> >::_Tidy();
template <> unsigned int vector<TextureOverrides::Override,std::allocator<TextureOverrides::Override> >::size()const;
template <> void vector<TextureOverrides::Override,std::allocator<TextureOverrides::Override> >::_Tidy();
template <> void vector<TextureOverrides::Override,std::allocator<TextureOverrides::Override> >::_Xlen()const;
}
#endif

#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specialization (C0132); retail ??1GatorNode calls vector<bool>'s inner
// _Tidy out of line at the 0x000AD5F9 fold row
namespace std {
template <> void vector<unsigned int,std::allocator<unsigned int> >::_Tidy();
}
#endif

#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specialization (C0132, C0215); retail calls it out of line and its body is the
// matched Blender.obj row 0x00078D38 (a same-name COMDAT copy here: relink refuses the overlap)
namespace std {
template <> void vector<Matrix,std::allocator<Matrix> >::resize(unsigned int);
}
#endif

inline bool TextureOverrides::empty() const
{
    return _overrides.size() == 0;
}

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
            // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
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

// FUNCTION: USA 0x000AE388
// PROTOTYPE: PREVIEW 0x004991FC identical
// NAME: proto-pdb:default.pdb
const std::vector<Matrix>& GatorNode::GetDeformationMatricesLast() const
{
    if (_movingPlatform && _deformMatricesLast.size() == _model().GetBoneCount())
        return _deformMatricesLast;
    return _deformMatrices;
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

// FUNCTION: USA 0x000AE42B
// NAME: proto-pdb:default.pdb
void GatorNode::DoSpookyRendering(unsigned int width, unsigned int height, unsigned int backgroundColor, float alpha,
                                  bool blur)
{
    delete _spooky;
    // !FAKE: c1xx front end; stand-in local; retail computes the flags word before operator new, no PDB local
    unsigned int v1 = 0;
    if (blur)
        v1 = 1;
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
    _spooky = new("SwampGatorNode.cpp", 1032) SwampSpookyRenderer(width, height, backgroundColor, v1);
    SetOption(SortedRender, true, false);
    SetOption(LocationSorting, true, false);
    // !FAKE: c2 float conversion; stand-in local for the cvttss2si operand (as Sprite.cpp); no PDB local
    float v0 = alpha * 255.5f;
    SetRenderingSetting(Alpha, _mm_cvtt_ss2si(_mm_load_ss(&v0)), false);
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

// FUNCTION: USA 0x000AE632
// PROTOTYPE: PREVIEW 0x00499449 identical
// NAME: proto-pdb:default.pdb
float GatorNode::GetAnimSpeed()
{
    if (_currentAnim.IsValid()) {
        // !FAKE: c2 scheduling; hand form of GetCurrentAnimation() via the T* ctor (no inner null test); the copy-ctor form hoists the _speed load above the incl (match_one 85.875 -> 100.0)
        ResourceHandle<AnimationControl> v0(_currentAnim._Resource);
        return v0().GetAnimationSpeed();
    }
    return 0.0f;
}

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

// FUNCTION: USA 0x000AE6CA
// NAME: proto-pdb:default.pdb
void GatorNode::SetSuperEdgeGlowParams(const ResourceHandle<ModelResource>& mh, const Vector3& color,
                                       unsigned int bufferWidth, unsigned int bufferHeight, float pulseFactor)
{
    _edgeGlowModel = mh;
    _edgeColor = color;
    _edgePulseFactor = pulseFactor;
    // !FAKE: c1xx front end; stand-in local; retail calls GetRGB before the delete, no PDB local
    unsigned int v0 = color.GetRGB();
    delete _superEdgeGlow;
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
    _superEdgeGlow = new("SwampGatorNode.cpp", 1020) SwampSpookyRenderer(bufferWidth, bufferHeight, v0, 3);
}

// FUNCTION: USA 0x000AE91C
// PROTOTYPE: PREVIEW 0x00499707 identical
// NAME: proto-pdb:default.pdb
void TextureManager::SetAnimatedTextureControl(const ResourceHandle<AnimationControl>& ach, SwampScript* script)
{
    _animControl = ach;
    _animScript = script;
}

// FUNCTION: USA 0x000AE936
// PROTOTYPE: PREVIEW 0x00499721 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 79.80, bytes mismatch (Vector3::Length call vs retail inline rsqrtss); wall: inline-asm sqrt, D116 (C0219)
void GatorNode::CalcBounds(unsigned int numBones) const
{
    AABox bounds;
    bounds.low.x = 3.4028235e38f;
    bounds.low.y = 3.4028235e38f;
    bounds.low.z = 3.4028235e38f;
    bounds.high.x = -3.4028235e38f;
    bounds.high.y = -3.4028235e38f;
    bounds.high.z = -3.4028235e38f;
    if (numBones) {
        const float radius2 = _sphereRadius * 2.0f;
        for (unsigned int b = 0; b < numBones; ++b) {
            Vector3 center = _deformMatrices[b][3];
            Vector3 pts[2];
            Vector3 v0;
            v0.x = center.x - radius2;
            v0.y = center.y - radius2;
            v0.z = center.z - radius2;
            pts[0] = v0;
            Vector3 v1;
            v1.x = center.x + radius2;
            v1.y = center.y + radius2;
            v1.z = center.z + radius2;
            pts[1] = v1;
            bounds.AddPointPair(pts);
        }
    } else {
        for (unsigned int b = 0; b < _deformMatrices.size(); ++b) {
            const AABox& v2 = _model().GetBoneBounds(b);
            if (!v2.Empty()) {
                Vector3 pts[2];
                _deformMatrices[b].BatchApply(&v2.low, pts, 2);
                Vector3 center;
                center.x = (pts[1].x + pts[0].x) * 0.5f;
                center.y = (pts[1].y + pts[0].y) * 0.5f;
                center.z = (pts[1].z + pts[0].z) * 0.5f;
                Vector3 v3;
                v3.x = pts[0].x - center.x;
                v3.y = pts[0].y - center.y;
                v3.z = pts[0].z - center.z;
                float v4 = v3.Length();
                Vector3 v0;
                v0.x = center.x - v4;
                v0.y = center.y - v4;
                v0.z = center.z - v4;
                pts[0] = v0;
                Vector3 v1;
                v1.x = center.x + v4;
                v1.y = center.y + v4;
                v1.z = center.z + v4;
                pts[1] = v1;
                bounds.AddPointPair(pts);
            }
        }
    }
    const_cast<GatorNode*>(this)->_BoundingBox = bounds;
}
#endif

// FUNCTION: USA 0x000AEB46
// PROTOTYPE: PREVIEW 0x00499931 identical
// NAME: proto-pdb:default.pdb
Matrix GatorNode::GetBoneOrientation(const std::string& boneName) const
{
    Matrix result;
    // !FAKE: c1xx front end; stand-in local; the bone index feeds two arguments, the PDB records no local for it
    int v0 = _model().GetBoneIndex(boneName, false);
    _model().GetBoneOrientation(v0, _deformMatrices[v0], result);
    return result;
}

// FUNCTION: USA 0x000AEB8F
// NAME: proto-pdb:default.pdb
Matrix GatorNode::GetBoneOrientation(int boneIndex) const
{
    Matrix result;
    _model().GetBoneOrientation(boneIndex, _deformMatrices[boneIndex], result);
    return result;
}

// FUNCTION: USA 0x000AEDEB
// NAME: proto-pdb:default.pdb
Vector3 GatorNode::GetWorldPosition() const
{
    Vector3 pos = SwampSceneNode::GetWorldPosition();
    if (_posBone > 0) {
        Matrix m = GetBoneOrientation(_posBone);
        // !FAKE: inliner; hand form of the inline Quaternion::Apply(v) (C0125, C0191); neutral locals
        Vector3 v0 = m[3];
        Vector3 tmp;
        const Quaternion* p = &GetWorldRotation();
        p->Apply(v0, tmp);
        v0 = tmp;
        pos += v0;
    }
    return pos;
}

// FUNCTION: USA 0x000AEE6E
// NAME: proto-pdb:default.pdb
void GatorNode::SetWorldPosition(const Vector3& pos)
{
    Vector3 newpos;
    _haveWorldPosLast = true;
    if (_posBone > 0) {
        Matrix m = GetBoneOrientation(_posBone);
        // !FAKE: inliner; hand form of the inline Quaternion::Apply(v) (C0125, C0191); neutral locals
        Vector3 tmp;
        {
            Vector3 v0 = m[3];
            const Quaternion* p = &GetWorldRotation();
            p->Apply(v0, tmp);
        }
        Vector3 v1(tmp);
        newpos.x = pos.x - v1.x;
        newpos.y = pos.y - v1.y;
        newpos.z = pos.z - v1.z;
        _worldPosLast = newpos;
        SwampSceneNode::SetWorldPosition(newpos);
    } else {
        _worldPosLast = pos;
        SwampSceneNode::SetWorldPosition(pos);
    }
}

// FUNCTION: USA 0x000AF048
// NAME: proto-pdb:default.pdb
// needs the visible ResourceHandle::operator= body (C0136); its COMDAT ??4?$ResourceHandle@VAnimationControl@@
// copy is byte-identical to the matched Generator.obj row 0x0003B04D and rides it in relink (T3 change, D117)
ResourceHandle<AnimationControl>& GatorNode::StartAnimation(const ResourceHandle<AnimationResource>& anim, bool looped,
                                                            bool reverse)
{
    _dirtyAnimation = true;
    SwampScript* script = GetScript();
    if (script)
        script->RemoveMatchingQueuedMessages(SwampScript::ANIMATION_FINISHED);
    if (_currentAnim.IsValid()) {
        if (looped && _currentAnim().IsLooped() && _currentAnim().GetAnimHandle() == anim)
            return _currentAnim;
        if (script)
            script->QueueAnimationMessage(SwampScript::ANIMATION_INTERRUPTED);
        if (_light) {
            _light->Invalidate();
            _light = 0;
        }
    }
    if (anim.IsValid()) {
        // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
        _currentAnim = ResourceHandle<AnimationControl>(
            new("SwampGatorNode.cpp", 725) AnimationControl(anim, this, looped, reverse, false));
    } else {
        _currentAnim = ResourceHandle<AnimationControl>();
        UpdateAnimation(0);
        CalcBounds(0);
    }
    return _currentAnim;
}

// FUNCTION: USA 0x000AF132
// PROTOTYPE: PREVIEW 0x00499F1D identical
// NAME: proto-pdb:default.pdb
void GatorNode::StopAnim()
{
    StartAnimation(ResourceHandle<AnimationResource>(), false, false);
}

// FUNCTION: USA 0x000AF153
// NAME: proto-pdb:default.pdb
// needs the visible ResourceHandle::operator= body (C0136); its COMDAT ??4?$ResourceHandle@VAnimationControl@@
// copy is byte-identical to the matched Generator.obj row 0x0003B04D and rides it in relink (T3 change, D117)
ResourceHandle<AnimationControl>& GatorNode::StartSecondaryAnimation(const ResourceHandle<AnimationResource>& anim,
                                                                     bool looped, bool reverse)
{
    _dirtyAnimation = true;
    if (_secondaryAnim.IsValid()) {
        if (looped && _secondaryAnim().IsLooped() && _secondaryAnim().GetAnimHandle() == anim)
            return _secondaryAnim;
        // !FAKE: c1xx front end; neutral stand-in local; one GetScript call in retail, no PDB local
        SwampScript* p = GetScript();
        if (p)
            p->QueueAnimationMessage(SwampScript::ANIMATION2_INTERRUPTED);
    }
    if (anim.IsValid()) {
        // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
        _secondaryAnim = new("SwampGatorNode.cpp", 782) AnimationControl(anim, this, looped, reverse, true);
    } else {
        _secondaryAnim = ResourceHandle<AnimationControl>();
    }
    _blendMode = OVERLAY;
    _animTransitionTimer = 0.0f;
    return _secondaryAnim;
}

// FUNCTION: USA 0x000AF22B
// PROTOTYPE: PREVIEW 0x0049A016 identical
// NAME: proto-pdb:default.pdb
void GatorNode::UpdateAnimLight()
{
    if (!_currentAnim.IsValid())
        return;
    if (_currentAnim().IsLightEnabled() && _enableAnimatedLight) {
        if (!_light) {
            // !FAKE: c1xx front end; neutral stand-in local; retail copies the anim handle (edi), no PDB local
            ResourceHandle<AnimationResource> v0 = _currentAnim().GetAnimHandle();
            _light = v0().CreateLight();
            if (_light) {
                _light->Disable();
                AddChild(_light);
            }
        }
        if (_light && !_light->IsEnabled()) {
            _light->Enable();
            std::string name = BeepLib::ReplaceExtension(_currentAnim().GetName(), ".lightpath");
            ResourceHandle<AnimationResource> lp = g_swamp->_animationManager->Load(name);
            if (lp.IsValid()) {
                ResourceHandle<AnimationControl> ctrl =
                    _light->StartPlayback(lp, _currentAnim().IsLooped(), _currentAnim().IsReverse());
                if (ctrl.IsValid()) {
                    _light->SetAnimSpeed(_currentAnim().GetAnimationSpeed());
                    ctrl().SetAnimationTime(_currentAnim().GetAnimationTime());
                    _light->UpdateAnimation(0);
                    _light->UpdateCachedWorldPosition();
                }
            }
        }
    } else if (_light && _light->IsEnabled()) {
        _light->StopPlayback();
        _light->Disable();
    }
}

// FUNCTION: USA 0x000B0050
// NAME: proto-pdb:default.pdb
ResourceHandle<AnimationControl> GatorNode::TransitionAnimation(ResourceHandle<AnimationResource> anim, float delay,
                                                                bool looped, bool reverse)
{
    if (delay > 0.0f && _currentAnim.IsValid() && !_secondaryAnim.IsValid()) {
        if (looped && _currentAnim().IsLooped() && _currentAnim().GetAnimHandle() == anim)
            return _currentAnim;
        _secondaryAnim = _currentAnim;
        _deformInfo_2nd = _deformInfo;
        _secondaryAnim().SetAnimationSpeed(0.0f);
        _animTransitionTimer = delay;
        _animTransitionTimeTotal = delay;
        _blendMode = TWEEN;
        if (_light) {
            _light->Invalidate();
            _light = 0;
        }
    }
    return StartAnimation(anim, looped, reverse);
}

// FUNCTION: USA 0x000B01FC
// NAME: proto-pdb:default.pdb
void GatorNode::OverrideTexture(const ResourceHandle<TextureResource>& original,
                                const ResourceHandle<TextureResource>& replacement)
{
    if (replacement.IsValid()) {
        if (!_texOverrides)
            // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__, ours differs
            _texOverrides = new("SwampGatorNode.cpp", 1061) TextureOverrides;
    } else if (!_texOverrides) {
        return;
    }
    _texOverrides->AddOverride(original, replacement);
    // !FAKE: c2 register allocation; stand-in local; retail loads _texOverrides once for empty() and delete
    TextureOverrides* p = _texOverrides;
    if (p->empty()) {
        delete p;
        _texOverrides = 0;
    }
}

// FUNCTION: USA 0x000B0287
// NAME: proto-pdb:default.pdb
void GatorNode::StartUVScroll()
{
    _uvScrolling = true;
    if (_model.IsValid()) {
        // !FAKE: c1xx front end; stand-in local; one GetStripCount call in retail, no PDB local
        unsigned int v0 = _model().GetStripCount();
        if (_t0Offset.size() != v0) {
            _t0Offset.resize(v0);
            for (std::vector<Vector2>::iterator it = _t0Offset.begin(); it != _t0Offset.end(); ++it)
                it->Zero();
        }
        if (_t1Offset.size() != v0) {
            _t1Offset.resize(v0);
            for (std::vector<Vector2>::iterator it = _t1Offset.begin(); it != _t1Offset.end(); ++it)
                it->Zero();
        }
    } else {
        _t0Offset.clear();
        _t1Offset.clear();
    }
}

// FUNCTION: USA 0x000B032F
// PROTOTYPE: PREVIEW 0x0049B11A identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 98.52, bytes mismatch: retail tests _currentAnim by `mov eax; test eax,eax`, ours `cmp [esi+0x140],0`
// 5.2 T12.c19 at-stop 20: every head spelling (local, ref, Ptr, ==false, bool) and /Oi /Ob1 /Op /Oa /Ow still cmp mem,0
void GatorNode::UpdateDeformationMatrices()
{
    if (!_currentAnim.IsValid())
        return;
    bool initMatricesLast = false;
    // !FAKE: c1xx front end; neutral stand-in local; one GetScript call in retail, no PDB local
    SwampScript* p = GetScript();
    unsigned int boneCount = 0;
    if (_lazyUpdateBoneCount && !_dirtyAnimation && g_swamp->_frameID - _lastRenderFrame > 2)
        boneCount = _lazyUpdateBoneCount;
    bool wasUpdated = _currentAnim().Update(p, _deformInfo, boneCount, false);
    if (_secondaryAnim.IsValid())
        wasUpdated |= _secondaryAnim().Update(p, _deformInfo_2nd, boneCount, true);
    if (wasUpdated) {
        if (_movingPlatform) {
            if (_deformMatricesLast.size() == _deformMatrices.size())
                _deformMatricesLast = _deformMatrices;
            else
                initMatricesLast = true;
        }
        if (!_secondaryAnim.IsValid())
            _model().ApplyDeformationsToSkeletion(_deformMatrices, _deformInfo, boneCount);
        else if (_blendMode == OVERLAY)
            _model().ApplyDeformationsToSkeletion(_deformMatrices, _deformInfo, _deformInfo_2nd, boneCount);
        else
            _model().ApplyDeformationsToSkeletion(_deformMatrices, _deformInfo, _deformInfo_2nd, _tweenFactor,
                                                  boneCount);
        CalcBounds(boneCount);
        if (initMatricesLast) {
            _deformMatricesLast.resize(_deformMatrices.size());
            _deformMatricesLast = _deformMatrices;
        }
    }
}
#endif

// FUNCTION: USA 0x000B0BAF
// NAME: proto-pdb:default.pdb
// needs the visible ResourceHandle::operator= body (C0136); its COMDAT ??4?$ResourceHandle@VAnimationControl@@
// copy is byte-identical to the matched Generator.obj row 0x0003B04D and rides it in relink (T3 change, D117)
void GatorNode::UpdateAnimation(unsigned int pass)
{
    if (pass == 0 || _dirtyAnimation) {
        UpdateModel();
        if (_animTransitionTimer > 0.0f) {
            _animTransitionTimer -= g_swamp->_input->GetElapsedTime();
            _tweenFactor = 1.0f - _animTransitionTimer / _animTransitionTimeTotal;
            if (_animTransitionTimer <= 0.0f) {
                _animTransitionTimeTotal = 0.0f;
                _animTransitionTimer = 0.0f;
                _blendMode = OVERLAY;
                _tweenFactor = 1.0f;
                _secondaryAnim = ResourceHandle<AnimationControl>();
            }
        }
        UpdateDeformationMatrices();
        UpdateAnimLight();
        _dirtyAnimation = false;
    }
}

// FUNCTION: USA 0x000B0C68
// NAME: proto-pdb:default.pdb
void GatorNode::SetModel(const ResourceHandle<ModelResource>& mh)
{
    _nextModel = mh;
    _newModel = true;
    _dirtyAnimation = true;
    if (_nextModel.IsValid()) {
        unsigned int numBones = _nextModel().GetBoneCount();
        if (_currentAnim.IsValid()) {
            // !FAKE: c1xx front end; stand-in locals; retail copies the handle (refcount) and calls NumTracks once
            ResourceHandle<AnimationResource> v0 = _currentAnim().GetAnimHandle();
            if (v0.IsValid()) {
                unsigned int v1 = v0().NumTracks();
                if (v1 && v1 != numBones)
                    StartAnimation(ResourceHandle<AnimationResource>(), false, false);
            }
        }
        if (_secondaryAnim.IsValid()) {
            // !FAKE: c1xx front end; stand-in locals; retail copies the handle (refcount) and calls NumTracks once
            ResourceHandle<AnimationResource> v0 = _secondaryAnim().GetAnimHandle();
            if (v0.IsValid()) {
                unsigned int v1 = v0().NumTracks();
                if (v1 && v1 != numBones)
                    StartSecondaryAnimation(ResourceHandle<AnimationResource>(), false, false);
            }
        }
    }
    if (!_model.IsValid())
        UpdateModel();
}

// FUNCTION: USA 0x000B0DD5
// NAME: proto-pdb:default.pdb
// companion of ??_GGatorNode 0x000B0B93: emits the vtable (5.3 T6)
// !FAKE: inliner; hand form of retail's inlined Quaternion(float, float, float) mem-initializer; cl keeps that ctor out
// of line (match_one bytes provisional, call ??0Quaternion@@QAE@MMM@Z); retail calls Quaternion::Set 0x000DC99B before the
// _platformPosTime store, so the Set call rides in its initializer (cf. LevelLoader.cpp:1178)
GatorNode::GatorNode(const ResourceHandle<ModelResource>& mh)
    : CollidableCylinder(6), _newModel(false), _newParticles(false), _hasCollisionBSP(false),
      _haveWorldPosLast(false), _movingPlatform(false), _acceptsShadows(false), _useAllLights(false),
      _uvScrolling(true), _dirtyAnimation(false), _edgeGlow(false), _enableAnimatedLight(true),
      _lazyUpdateBoneCount(0), _posBone(0), _blendMode(OVERLAY), _tweenFactor(1.0f), _ambientColor(0.0f, 0.0f, 0.0f),
      _shadow(0), _shadowBone(-1), _projectedShadowLightLoc(0.0f, 0.0f, 0.0f),
      _projectedShadowLightColor(0.0f, 0.0f, 0.0f), _edgeColor(0.0f, 1.0f, 0.0f), _edgeSizeMin(0.01f),
      _edgeSizeMax(0.015f), _edgePulseFactor(3.5f), _superEdgeGlow(0), _spooky(0), _light(0), _texOverrides(0),
      _animTransitionTimeTotal(0.0f), _animTransitionTimer(0.0f), _animSoundRange(16.0f), _animSoundVolume(1.0f),
      _animTriggerFlags(0), _platformPosLast(0.0f, 0.0f, 0.0f),
      _platformPosTime((_platformRotLast.Set(0.0f, 0.0f, 0.0f), 0.0f)), _modelRenderFlags(0), _uvScrollingSpeed(1.0f, 1.0f)
{
    if (mh.IsValid())
        SetModel(mh);
}

// FUNCTION: USA 0x000B051A
// NAME: proto-pdb:default.pdb
GatorNode::~GatorNode()
{
    delete _texOverrides;
    delete _superEdgeGlow;
    delete _spooky;
}

// FUNCTION: USA 0x000B063C
// PROTOTYPE: PREVIEW 0x0049B427 identical
// NAME: proto-pdb:default.pdb
void GatorNode::OverrideTexture(const std::string& original, const std::string& replacement)
{
    ResourceHandle<TextureResource> origTex = g_swamp->GetTextureManager().Load(original);
    ResourceHandle<TextureResource> replTex;
    if (replacement.size() != 0)
        replTex = g_swamp->GetTextureManager().Load(replacement);
    OverrideTexture(origTex, replTex);
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

#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specialization (C0154); _Tidy is unmatched (its erase callees are unnamed fold
// VAs); its visible body emits out_of_range throw info (.xdata$x) here, which relink refuses
typedef std::_Tree<std::_Tmap_traits<unsigned int, ProfileBar::ProfileEntry*, std::less<unsigned int>,
                                     std::allocator<std::pair<const unsigned int, ProfileBar::ProfileEntry*> >, false> >
    ProfileTree;
template <> void ProfileTree::_Tidy();
#endif

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1ProfileBar@@QAE@XZ in this unit (no matched
// function odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_ProfileBar_dtor(ProfileBar* p)
{
    p->ProfileBar::~ProfileBar();
}

// FUNCTION: USA 0x000AE259
// PROTOTYPE: PREVIEW 0x004990E0 identical
// NAME: proto-pdb:default.pdb
// best: match_one objdiff 100.0, bytes match (4.3 T7.c1); was walled: provenance_lint nested operator
// (C0212) -- lifted 5.3 T2.c1 (C0369)
template bool std::vector<bool,std::allocator<bool> >::const_iterator::operator==(std::vector<bool,std::allocator<bool> >::const_iterator const &)const;

// FUNCTION: USA 0x000AE275
// PROTOTYPE: PREVIEW 0x004AC361 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tset_traits<LightManager::LightLocation,std::less<LightManager::LightLocation>,std::allocator<LightManager::LightLocation>,1> >::const_iterator::_Inc();

// FUNCTION: USA 0x000AE2AB
// PROTOTYPE: PREVIEW 0x0049911F identical
// NAME: proto-pdb:default.pdb
template void std::fill<Vector2 *,Vector2>(Vector2 *,Vector2 *,Vector2 const &);

// FUNCTION: USA 0x000AE2CD
// PROTOTYPE: PREVIEW 0x00499141 identical
// NAME: proto-pdb:default.pdb
template SwampDeformation * std::_Copy_opt<SwampDeformation *,SwampDeformation *>(SwampDeformation *,SwampDeformation *,SwampDeformation *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x000AE2F7
// PROTOTYPE: PREVIEW 0x0049916B identical
// NAME: proto-pdb:default.pdb
template Vector2 * std::_Copy_opt<Vector2 *,Vector2 *>(Vector2 *,Vector2 *,Vector2 *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x000AE319
// PROTOTYPE: PREVIEW 0x0049918D identical
// NAME: proto-pdb:default.pdb
template Vector2 * std::_Copy_backward_opt<Vector2 *,Vector2 *>(Vector2 *,Vector2 *,Vector2 *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x000AE4D3
// PROTOTYPE: PREVIEW 0x00499347 identical
// NAME: proto-pdb:default.pdb
// best: match_one objdiff 100.0, bytes match (4.3 T7.c1); was walled: provenance_lint nested operator
// (C0212) -- lifted 5.3 T2.c1 (C0369)
template std::vector<bool,std::allocator<bool> >::reference & std::vector<bool,std::allocator<bool> >::reference::operator=(bool);

// FUNCTION: USA 0x000AE59D
// PROTOTYPE: PREVIEW 0x004993B5 identical
// NAME: proto-pdb:default.pdb
// best: match_one objdiff 100.0, bytes match (4.3 T7.c1); was walled: provenance_lint nested operator
// (C0212) -- lifted 5.3 T2.c1 (C0369)
template std::vector<bool,std::allocator<bool> >::reference & std::vector<bool,std::allocator<bool> >::reference::operator=(std::vector<bool,std::allocator<bool> >::reference const &);

// FUNCTION: USA 0x000AE5C1
// PROTOTYPE: PREVIEW 0x00448A7D identical
// NAME: proto-pdb:default.pdb
template void std::_Construct<Vector2,Vector2>(Vector2 *,Vector2 const &);

// FUNCTION: USA 0x000AE7AD
// PROTOTYPE: PREVIEW 0x00499598 identical
// NAME: proto-pdb:default.pdb
template void std::fill<std::vector<bool,std::allocator<bool> >::iterator,bool>(std::vector<bool,std::allocator<bool> >::iterator,std::vector<bool,std::allocator<bool> >::iterator,bool const &);

// FUNCTION: USA 0x000AE803
// PROTOTYPE: PREVIEW 0x004995EE identical
// NAME: proto-pdb:default.pdb
template std::vector<bool,std::allocator<bool> >::iterator std::_Copy_opt<std::vector<bool,std::allocator<bool> >::iterator,std::vector<bool,std::allocator<bool> >::iterator>(std::vector<bool,std::allocator<bool> >::iterator,std::vector<bool,std::allocator<bool> >::iterator,std::vector<bool,std::allocator<bool> >::iterator,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x000AEC29
// PROTOTYPE: PREVIEW 0x00499A14 identical
// NAME: proto-pdb:default.pdb
template std::vector<bool,std::allocator<bool> >::iterator std::vector<bool,std::allocator<bool> >::end();

// FUNCTION: USA 0x000AECBA
// PROTOTYPE: PREVIEW 0x00499AA5 identical
// NAME: proto-pdb:default.pdb
template std::vector<bool,std::allocator<bool> >::iterator std::_Copy_backward_opt<std::vector<bool,std::allocator<bool> >::iterator,std::vector<bool,std::allocator<bool> >::iterator>(std::vector<bool,std::allocator<bool> >::iterator,std::vector<bool,std::allocator<bool> >::iterator,std::vector<bool,std::allocator<bool> >::iterator,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x000AF02F
// PROTOTYPE: PREVIEW 0x00499E1A identical
// NAME: proto-pdb:default.pdb
template void std::_Destroy_range<TextureOverrides::Override,std::allocator<TextureOverrides::Override> >(TextureOverrides::Override *,TextureOverrides::Override *,std::allocator<TextureOverrides::Override> &,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x000AF3AD
// NAME: proto-pdb:default.pdb
template void std::vector<bool,std::allocator<bool> >::_Xlen() const;

// FUNCTION: USA 0x000AF749
// PROTOTYPE: PREVIEW 0x0049A534 identical
// NAME: proto-pdb:default.pdb
template bool std::vector<SwampDeformation,std::allocator<SwampDeformation> >::_Buy(unsigned int);

// FUNCTION: USA 0x000AF791
// PROTOTYPE: PREVIEW 0x0049A57C identical
// NAME: proto-pdb:default.pdb
template bool std::vector<Matrix,std::allocator<Matrix> >::_Buy(unsigned int);

// FUNCTION: USA 0x000AF9AE
// PROTOTYPE: PREVIEW 0x0049A799 identical
// NAME: proto-pdb:default.pdb
template void std::vector<bool,std::allocator<bool> >::_Trim(unsigned int);

// FUNCTION: USA 0x000AFC7F
// PROTOTYPE: PREVIEW 0x0049AA6A identical
// NAME: proto-pdb:default.pdb
template std::vector<SwampDeformation,std::allocator<SwampDeformation> > & std::vector<SwampDeformation,std::allocator<SwampDeformation> >::operator=(std::vector<SwampDeformation,std::allocator<SwampDeformation> > const &);

// FUNCTION: USA 0x000AFD63
// PROTOTYPE: PREVIEW 0x0049AB4E identical
// NAME: proto-pdb:default.pdb
template std::vector<Matrix,std::allocator<Matrix> > & std::vector<Matrix,std::allocator<Matrix> >::operator=(std::vector<Matrix,std::allocator<Matrix> > const &);

// FUNCTION: USA 0x000AFE45
// PROTOTYPE: PREVIEW 0x0049AC30 identical
// NAME: proto-pdb:default.pdb
template void std::vector<SwampDeformation,std::allocator<SwampDeformation> >::resize(unsigned int,SwampDeformation);

// FUNCTION: USA 0x000AFE9D
// PROTOTYPE: PREVIEW 0x0049AC88 identical
// NAME: proto-pdb:default.pdb
template std::vector<bool,std::allocator<bool> >::iterator std::vector<bool,std::allocator<bool> >::erase(std::vector<bool,std::allocator<bool> >::iterator,std::vector<bool,std::allocator<bool> >::iterator);

// FUNCTION: USA 0x000AFF27
// PROTOTYPE: PREVIEW 0x0049AD12 identical
// NAME: proto-pdb:default.pdb
template void std::vector<Vector2,std::allocator<Vector2> >::resize(unsigned int,Vector2);

// FUNCTION: USA 0x000AFFA7
// PROTOTYPE: PREVIEW 0x0049AD92 identical
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<bool,std::allocator<bool> >::_Insert_x(std::vector<bool,std::allocator<bool> >::iterator,unsigned int);

// FUNCTION: USA 0x000B013D
// PROTOTYPE: PREVIEW 0x0049AF28 identical
// NAME: proto-pdb:default.pdb
template void std::vector<SwampDeformation,std::allocator<SwampDeformation> >::resize(unsigned int);

// FUNCTION: USA 0x000B0164
// PROTOTYPE: PREVIEW 0x0049AF4F identical
// NAME: proto-pdb:default.pdb
template void std::vector<bool,std::allocator<bool> >::clear();

// FUNCTION: USA 0x000B0194
// PROTOTYPE: PREVIEW 0x0049AF7F identical
// NAME: proto-pdb:default.pdb
template void std::vector<Vector2,std::allocator<Vector2> >::resize(unsigned int);

// FUNCTION: USA 0x000B01AB
// PROTOTYPE: PREVIEW 0x0049AF96 identical
// NAME: proto-pdb:default.pdb
template void std::vector<bool,std::allocator<bool> >::_Insert_n(std::vector<bool,std::allocator<bool> >::iterator,unsigned int,bool);

// FUNCTION: USA 0x000B0495
// PROTOTYPE: PREVIEW 0x0049B280 identical
// NAME: proto-pdb:default.pdb
template void std::vector<bool,std::allocator<bool> >::resize(unsigned int,bool);
