// UNIT: src/Apps/Vince/core/PowerIcon.cpp OBJ: core\XBox\OfficialRelease\PowerIcon.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"
namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    basic_string(const E* _Ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& operator+=(const basic_string& _Right) { return append(_Right); }
    basic_string& append(const basic_string& _Right) { return append(_Right, 0, (unsigned int)-1); }
    basic_string& append(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    const E* c_str() const { return _Myptr(); }
    unsigned int size() const { return _Mysize; }
    int compare(const E* _Ptr) const;
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
inline bool operator==(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return _Left.compare(_Right) == 0;
}
template <class E, class T, class A>
basic_string<E, T, A> operator+(const basic_string<E, T, A>& _Left, const E* _Right);
template <class E, class T, class A>
basic_string<E, T, A> operator+(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right);
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;
const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    const SwampString GetStringRecord(const SwampString& id, const SwampString& def) const;
    int GetIntRecord(const SwampString& id, int def) const;
};
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb
struct Vector3 { // NAME: proto-pdb:default.pdb
    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
struct AABox;                // NAME: proto-pdb:default.pdb
class SwampSceneNode;        // NAME: proto-pdb:default.pdb

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

    char field_4[0x1C];                // 0x04 (_name; not reached)
    SwampSceneNode* _node;             // 0x20
    char field_24[0x28];               // 0x24 (_parent .. _saveLoadVars; not reached)
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class CritterMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~CritterMain();
    virtual void Message(SwampScript*, int, void*);
    virtual bool CanChaseTo(const Vector3&);                       // vtable+0x64
    virtual bool CanPatrolTo(const Vector3&);                      // vtable+0x68
    virtual void SetState(int);                                    // vtable+0x6C
    virtual SwampString Save();

    int _state;                        // 0x4C
    char field_50[0x6C];               // 0x50 (_wanderPatrolCenter .. _chaseRangeOverride; not reached)
};
DECOMP_SIZE_ASSERT(CritterMain, 0xBC);

void* operator new(unsigned int size, const char* file, int line); // retail ??2@YAPAXIPBDH@Z

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    const Vector3& SetLocation(const Vector3&);
    virtual ~SwampSceneNode();                                   // vtable+0x00
    virtual const AABox& GetBounds() const;                      // vtable+0x04
    virtual Vector3 GetWorldPosition() const;                    // vtable+0x08
};

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
};

class Vince : public Critter { // NAME: proto-pdb:default.pdb
public:
    void AddWhammySkill(const SwampString&, bool);
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    Vince* GetVince() const { return _vinceCache; }

    char field_0[0x28];                // 0x00 (not reached)
    Vince* _vinceCache;                // 0x28
};
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class FireFlyNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    void SetDeleteWhenDone(bool b) { _deleteWhenDone = b; }

    char field_4[0xCE - 0x4];          // 0x04 (not reached)
    bool _deleteWhenDone;              // 0xCE
    char field_CF[0xE8 - 0xCF];        // 0xCF (not reached)
};
DECOMP_SIZE_ASSERT(FireFlyNode, 0xE8);

class SwampParticleManager { // NAME: proto-pdb:default.pdb
public:
    FireFlyNode* Add(const SwampString&, const SwampString&, unsigned int, bool);
};
extern SwampParticleManager* g_particleManager; // NAME: proto-pdb:default.pdb

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    static void PlaySound(const SwampString&, unsigned int, SwampSceneNode*, bool, float, float);
};

class MusicManager { // NAME: proto-pdb:default.pdb
public:
    void PushOverlay(const SwampString&, bool, SwampScript*);
};
extern MusicManager* g_MusicManager; // NAME: proto-pdb:default.pdb

class AreaStats { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x28];                // 0x00 (_clustersRemaining .. _juneBugsMax; not reached)
    int _whammysFound;                 // 0x28
    int _whammysRemaining;             // 0x2C
    SwampString _name;                 // 0x30
};
DECOMP_SIZE_ASSERT(AreaStats, 0x4C);

class Level { // NAME: proto-pdb:default.pdb
public:
    void FoundWhammy()
    {
        _stats._whammysRemaining--;
        _stats._whammysFound++;
    }

    char field_0[0x3C];                // 0x00 (_zones .. _levelStats; not reached)
    AreaStats _stats;                  // 0x3C
};

class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    static LevelLoader& Instance();
    Level* GetCurLevel() { return _curLevel; }

    char field_0[0x4];                 // 0x00 (not reached)
    Level* _curLevel;                  // 0x04
};

class LocalizedStrings { // NAME: proto-pdb:default.pdb
public:
    static LocalizedStrings& Instance();
    const unsigned short* GetGameString(const SwampString&);
};

class HUDUI { // NAME: proto-pdb:default.pdb
public:
    static HUDUI& Instance() { return *_self; }
    void SetQuickMessage(const unsigned short* ptr, int fontsize);

private:
    static HUDUI* _self;
};

class SwampScriptManager { // NAME: proto-pdb:default.pdb
public:
    typedef SwampScript* (*CreationFunc)(SwampSceneNode*);
    static SwampScriptManager& Instance();
    SwampScript* Create(const SwampString&, SwampSceneNode*) const;
    void Add(const SwampString& name, CreationFunc func);
};

class HoldText : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    HoldText(SwampSceneNode* node);

    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);
};
DECOMP_SIZE_ASSERT(HoldText, 0x4C);

class PowerIconMain : public CritterMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_ABSORB = 0x14,
        MESSAGE_ABSORB_FIRST = 0x15,
        MESSAGE_ABSORB_INSTANT = 0x16,
        MESSAGE_ICONSPECIFIC = 0x17
    };
    PowerIconMain(SwampSceneNode* node);
    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);
    virtual SwampString Save();

    struct RegisterMe { // NAME: proto-pdb:default.pdb
        RegisterMe();
    };
    static RegisterMe registerMe;
private:
    static SwampScript* creationFunc(SwampSceneNode* node);

    SwampString _powerName;            // 0xBC
};
DECOMP_SIZE_ASSERT(PowerIconMain, 0xD8);

class RewardPowerIconMain : public PowerIconMain { // NAME: proto-pdb:default.pdb
public:
    enum Messages { // NAME: proto-pdb:default.pdb
        MESSAGE_SHOW = 0x17
    };
    enum States { // NAME: proto-pdb:default.pdb
        STATE_HIDE = 0,
        STATE_SHOW = 1
    };

    virtual void Message(SwampScript* source, int message, void* data);
    virtual void Load(const BeepTDF& tdf);
};
DECOMP_SIZE_ASSERT(RewardPowerIconMain, 0xD8);

// row-end pin 5.4 T26.c1
// FUNCTION: USA 0x00121AD8
// PROTOTYPE: PREVIEW 0x0050A782 identical
// NAME: proto-pdb:default.pdb
SwampScript* HoldText::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (relative path), ours differs
    return new("scripts\\common\\PowerIcon.cpp", 23) HoldText(node);
}

// FUNCTION: USA 0x0012202C
// PROTOTYPE: PREVIEW 0x0050ACFA identical
// NAME: proto-pdb:default.pdb
SwampScript* PowerIconMain::creationFunc(SwampSceneNode* node)
{
    // !FAKE: retail string literal; __FILE__/__LINE__ text; the original passes its build's __FILE__/__LINE__ (header path), ours differs
    return new("d:\\beep\\apps\\vince\\scripts\\common\\PowerIcon.h", 14) PowerIconMain(node);
}

// FUNCTION: USA 0x00122053
// NAME: proto-pdb:default.pdb
PowerIconMain::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("PowerIconMain", creationFunc);
}

// FUNCTION: USA 0x00122016
// NAME: proto-pdb:default.pdb
void RewardPowerIconMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_SHOW:
        SetState(STATE_SHOW);
        break;
    default:
        PowerIconMain::Message(source, message, data);
        break;
    }
}

// FUNCTION: USA 0x00121C15
// NAME: proto-pdb:default.pdb
void RewardPowerIconMain::Load(const BeepTDF& tdf)
{
    PowerIconMain::Load(tdf);
    // !FAKE: EH state; stand-in local (absent from the PDB); retail destroys the "state" temporary before the SetState call
    int state = tdf.GetIntRecord("state", 0);
    SetState(state);
}

// FUNCTION: USA 0x00121CB3
// NAME: proto-pdb:default.pdb
HoldText::RegisterMe::RegisterMe()
{
    SwampScriptManager::Instance().Add("HoldText", creationFunc);
}

// FUNCTION: USA 0x00121D91
// PROTOTYPE: PREVIEW 0x0050AA5F identical
// NAME: proto-pdb:default.pdb
void PowerIconMain::Message(SwampScript* source, int message, void* data)
{
    switch (message) {
    case MESSAGE_ABSORB:
    case MESSAGE_ABSORB_FIRST: {
        Vince* vince = pManager->GetVince();
        if (message != MESSAGE_ABSORB_FIRST) {
            SoundManager::PlaySound("intpowicon", 0x20, vince, true, 0.0f, 0.0f);
            g_MusicManager->PushOverlay("vinIconPU", false, 0);
        }
        FireFlyNode* n = g_particleManager->Add("", "powericonenterexit", 0, true);
        n->SetLocation(_node->GetWorldPosition());
        n->SetDeleteWhenDone(true);
        if (_powerName.size()) {
            vince->AddWhammySkill(_powerName, true);
            if (message != MESSAGE_ABSORB_FIRST) {
                std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> > text =
                    std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> >(
                        LocalizedStrings::Instance().GetGameString(_powerName)) + L"\n" +
                    std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> >(
                        LocalizedStrings::Instance().GetGameString("powerearned"));
                HUDUI::Instance().SetQuickMessage(text.c_str(), 0);
                if (_powerName == "whammypitchfork")
                    SwampScriptManager::Instance().Create("HoldText", 0);
            }
        }
        InvalidateScript();
        LevelLoader::Instance().GetCurLevel()->FoundWhammy();
        break;
    }
    case MESSAGE_ABSORB_INSTANT:
        if (_powerName.size())
            pManager->GetVince()->AddWhammySkill(_powerName, true);
        InvalidateScript();
        LevelLoader::Instance().GetCurLevel()->FoundWhammy();
        break;
    default:
        CritterMain::Message(source, message, data);
        break;
    }
}

// FUNCTION: USA 0x00121B35
// NAME: proto-pdb:default.pdb
void PowerIconMain::Load(const BeepTDF& tdf)
{
    _powerName = tdf.GetStringRecord("powername", "");
}

// FUNCTION: USA 0x00121BA5
// NAME: proto-pdb:default.pdb
SwampString PowerIconMain::Save()
{
    SwampString str = CritterMain::Save();
    str += format("\t\tpowername = %s;\n", _powerName.c_str());
    return str;
}
