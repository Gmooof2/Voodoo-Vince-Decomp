// UNIT: src/Apps/Vince/scripts/Bayou/SwitchBoat.cpp OBJ: Xbox\OfficialRelease\SwitchBoat.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator {};
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    int compare(const E* _Ptr) const;
    const E& operator[](unsigned int _Off) const { return (_Myptr()[_Off]); }

    enum { _BUF_SIZE = 16 / sizeof(E) < 1 ? 1 : 16 / sizeof(E) };

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    A _Alval;                          // 0x00
    union _Bxty {
        E _Buf[_BUF_SIZE];
        E* _Ptr;
    } _Bx;                             // 0x04

public:
    const E* _Myptr() const { return (_BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf); }

    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class E, class T, class A>
inline bool operator==(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return _Left.compare(_Right) == 0;
}
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class SwampScript;

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    const SwampString& GetName() const { return _Name; }
    SwampScript* GetScript() const;

    char field_0[0x8];                 // 0x00
    SwampString _Name;                 // 0x08
    char field_24[0xA4 - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Critter* Find(const SwampString& name);

    char field_0[0x2C];                // 0x00
};
DECOMP_SIZE_ASSERT(CritterManager, 0x2C);

extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class BeepTDF;               // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum CollisionType { // NAME: proto-pdb:default.pdb
        HorizontalCollision = 0,
        VerticalCollision = 1
    };
    virtual ~SwampScript();                                        // vtable+0x00
    virtual bool TestActivate() const;                             // vtable+0x04
    virtual void Activate();                                       // vtable+0x08
    virtual void Deactivate();                                     // vtable+0x0C
    virtual void FrameTick();                                      // vtable+0x10
    virtual void TimerEvent(unsigned int);                         // vtable+0x14
    virtual void AnimationFinished();                              // vtable+0x18
    virtual void SecondaryAnimationFinished();                     // vtable+0x1C
    virtual void AnimationInterrupted();                           // vtable+0x20
    virtual void SecondaryAnimationInterrupted();                  // vtable+0x24
    virtual void AnimationTrigger(const SwampString&, float, float, unsigned int); // vtable+0x28
    virtual bool LosingControl(const SwampString&);                // vtable+0x2C
    virtual void GainingControl(SwampScript*);                     // vtable+0x30
    virtual void Message(SwampScript*, int, void*);                // vtable+0x34
    virtual void EnteringRegion(SwampSceneNode*);                  // vtable+0x38
    virtual void LeavingRegion(SwampSceneNode*);                   // vtable+0x3C
    virtual void SoundFinished(const SwampString&);                // vtable+0x40
    virtual void SoundStreamerFinished(const SwampString&);        // vtable+0x44
    virtual void MovieFinished(const SwampString&);                // vtable+0x48
    virtual void CollidedWith(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x4C
    virtual void HitBy(CollisionType, SwampSceneNode*, const Ptr<CollisionResultObject>&); // vtable+0x50
    virtual void Load(const BeepTDF&);                             // vtable+0x54
    virtual SwampString Save();                                    // vtable+0x58
    virtual void SetNode(SwampSceneNode*);                         // vtable+0x5C
    virtual void InvalidateScript();                               // vtable+0x60

    char field_4[0x48];                // 0x04
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    int GetState() const { return _state; }

    int _state;                        // 0x4C
    char field_50[0xBC - 0x50];        // 0x50
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

class SwitchMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    virtual void Activate();

    char field_BC[0x138 - 0xBC];       // 0xBC
    bool _popupOK;                     // 0x138
    char field_139[0x13C - 0x139];     // 0x139
};
DECOMP_SIZE_ASSERT(SwitchMain, 0x13C);

class SwitchBoatMain : public SwitchMain { // NAME: proto-pdb:default.pdb
public:
    virtual void HitBy(CollisionType type, SwampSceneNode* critterHitBy, const Ptr<CollisionResultObject>& cr);

    bool _didOpenCam;                  // 0x13C
    bool _didCloseCam;                 // 0x13D
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(SwitchBoatMain, 0x140);

class BoatgateRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void LeavingRegion(SwampSceneNode* node);

    SwampString _target;               // 0x4C
    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(BoatgateRegionMain, 0x68);

struct Vector3 { // NAME: proto-pdb:default.pdb
public:
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct AABox { // NAME: proto-pdb:default.pdb
public:
    Vector3 low;                       // 0x00
    Vector3 high;                      // 0x0C
};
DECOMP_SIZE_ASSERT(AABox, 0x18);

class RegionTrigger { // NAME: proto-pdb:default.pdb
public:
    virtual const AABox& GetBounds() const;

    char field_4[0xA4 - 0x4];          // 0x04
    int _shape;                        // 0xA4 // src proto-pdb:default.pdb RegionShape
    AABox _bounds;                     // 0xA8
    char field_C0[0x124 - 0xC0];       // 0xC0
};
DECOMP_SIZE_ASSERT(RegionTrigger, 0x124);

// FUNCTION: USA 0x000220BF
// PROTOTYPE: PREVIEW 0x004110A0 identical
// NAME: proto-pdb:default.pdb
const AABox& RegionTrigger::GetBounds() const
{
    return _bounds;
}

// FUNCTION: USA 0x000220EB
// PROTOTYPE: PREVIEW 0x004110E8 identical
// NAME: proto-pdb:default.pdb
template char const & std::basic_string<char,std::char_traits<char>,std::allocator<char> >::operator[](unsigned int)const;

// FUNCTION: USA 0x000221AC
// PROTOTYPE: PREVIEW 0x0041118B identical
// NAME: proto-pdb:default.pdb
void BoatgateRegionMain::LeavingRegion(SwampSceneNode* node)
{
    if (node->GetName() == "fanboat")
        pManager->Find(_target)->GetScript()->Message(this, 0x19, 0); // src proto-pdb:default.pdb MESSAGE_DRIVE
}

// FUNCTION: USA 0x000222AE
// PROTOTYPE: PREVIEW 0x004112A3 identical
// NAME: proto-pdb:default.pdb
void SwitchBoatMain::HitBy(CollisionType type, SwampSceneNode* critterHitBy, const Ptr<CollisionResultObject>& cr)
{
    if (_popupOK) {
        if (critterHitBy != pManager->Find("fanboat"))
            return;
        if (GetState() == 0)
            Activate();
    }
}

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    void Add(const SwampString& name, CreationFunc func);
};

// FUNCTION: USA 0x0002276A
// NAME: proto-pdb:default.pdb
SwitchBoatMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("SwitchBoatMain", creationFunc);
}

// FUNCTION: USA 0x0002281E
// NAME: proto-pdb:default.pdb
BoatgateRegionMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("BoatgateRegionMain", creationFunc);
}
