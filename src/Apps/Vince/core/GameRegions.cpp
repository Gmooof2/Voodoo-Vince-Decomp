// UNIT: src/Apps/Vince/core/GameRegions.cpp OBJ: core\XBox\OfficialRelease\GameRegions.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
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
template <class E, class T, class A>
basic_string<E, T, A> operator+(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right);
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

const SwampString format(const char* formatstring, ...); // NAME: proto-pdb:default.pdb

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    const SwampString GetStringRecord(const SwampString& id, const SwampString& def) const;
    bool GetBoolRecord(const SwampString& id, bool def) const;
};
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion() {}
    Quaternion(const Vector3& v) { Set(v); }
    void Set(const Vector3&);

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

class Locator { // NAME: proto-pdb:default.pdb
public:
    const Vector3& GetLocation() const { return _pos; }
    Quaternion GetRotation() const { return Quaternion(_basis[2]); }

    Vector3 _pos;                      // 0x00
    Quaternion _rot;                   // 0x0C
    Vector3 _basis[3];                 // 0x1C
    char field_40[0x48 - 0x40];        // 0x40
};
DECOMP_SIZE_ASSERT(Locator, 0x48);

class LocatorManager { // NAME: proto-pdb:default.pdb
public:
    Locator* Find(const SwampString& s) const;
};
extern LocatorManager* g_locatorManager; // NAME: proto-pdb:default.pdb

class Level { // NAME: proto-pdb:default.pdb
public:
    const SwampString& GetLevelName() const { return _levelName; }

    char field_0[0xAC];                // 0x00
    SwampString _levelName;            // 0xAC
    char field_C8[0x140 - 0xC8];       // 0xC8
};
DECOMP_SIZE_ASSERT(Level, 0x140);

class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    static LevelLoader& Instance();
    Level* GetCurLevel() { return _curLevel; }

    char field_0[0x4];                 // 0x00
    Level* _curLevel;                  // 0x04
};

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

    char field_4[0x1C];                // 0x04
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class AreaLoadRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_SETCHAPTERACTIVATE = 0x14,
        MESSAGE_AREALOAD_REGION_USER = 0x15
    };
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();

    SwampString _nextLevel;           // 0x4C
    SwampString _nextArea;             // 0x68
    SwampString _targetLocation;       // 0x84
    SwampString _animVince;            // 0xA0
    bool _activateTrolleys;            // 0xBC
    bool _activateChapter;             // 0xBD
};
DECOMP_SIZE_ASSERT(AreaLoadRegionMain, 0xC0);

// FUNCTION: USA 0x0011F3DA
// PROTOTYPE: PREVIEW 0x00506A7A identical
// NAME: proto-pdb:default.pdb
void AreaLoadRegionMain::Message(SwampScript* source, int message, void* data)
{
    if (message == MESSAGE_SETCHAPTERACTIVATE) {
        _activateChapter = true;
    }
}

// FUNCTION: USA 0x0011F4A5
// NAME: proto-pdb:default.pdb
SwampString AreaLoadRegionMain::Save()
{
    return SwampScript::Save()
        + format("\t\tnextLevel = %s;\n", _nextLevel.c_str())
        + format("\t\tnextArea = %s;\n", _nextArea.c_str())
        + format("\t\ttargetLocation = %s;\n", _targetLocation.c_str())
        + format("\t\tvinceAnimation = %s;\n", _animVince.c_str())
        + format("\t\tactivateTrolleys = %d;\n", _activateTrolleys)
        + format("\t\tactivateChapter = %d;\n", _activateChapter);
}

class HealthWaypointRegionMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();

    SwampString _respawnAt;           // 0x4C
    Vector3 _startPos;                 // 0x68
    Quaternion _startOrient;           // 0x74
};
DECOMP_SIZE_ASSERT(HealthWaypointRegionMain, 0x84);

// FUNCTION: USA 0x0011F67F
// PROTOTYPE: PREVIEW 0x00506D1F identical
// NAME: proto-pdb:default.pdb
void HealthWaypointRegionMain::Load(const BeepTDF& tdf)
{
    _respawnAt = tdf.GetStringRecord("respawnAt", "");
    // !FAKE: c2 scheduling; pointer local; retail reads g_locatorManager before building the string temps
    // (match_one 96.90 without it)
    LocatorManager* p = g_locatorManager;
    Locator* l = p->Find(SwampString("locator_") + _respawnAt);
    _startPos = l->GetLocation();
    _startOrient = l->GetRotation();
}

// FUNCTION: USA 0x0011F757
// NAME: proto-pdb:default.pdb
SwampString HealthWaypointRegionMain::Save()
{
    return SwampScript::Save() + format("\t\trespawnAt = %s;\n", _respawnAt.c_str());
}

// FUNCTION: USA 0x0011FA25
// PROTOTYPE: PREVIEW 0x005070C5 identical
// NAME: proto-pdb:default.pdb
void AreaLoadRegionMain::Load(const BeepTDF& tdf)
{
    _nextLevel = tdf.GetStringRecord("nextLevel", LevelLoader::Instance().GetCurLevel()->GetLevelName());
    _nextArea = tdf.GetStringRecord("nextArea", "");
    _targetLocation = tdf.GetStringRecord("targetLocation", "");
    _animVince = tdf.GetStringRecord("vinceAnimation", "");
    _activateTrolleys = tdf.GetBoolRecord("activateTrolleys", false);
    _activateChapter = tdf.GetBoolRecord("activateChapter", false);
}
