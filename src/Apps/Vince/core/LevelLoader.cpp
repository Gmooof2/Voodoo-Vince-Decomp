// UNIT: src/Apps/Vince/core/LevelLoader.cpp OBJ: core\XBox\OfficialRelease\LevelLoader.obj OPT: /O1 /Oy /Oi
#include "../../../include/decomp.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#pragma warning(disable: 4530) // no /EH in the unit flags; as soundnode.cpp
#include <xstring>
#include <string>
#include <vector>
#include <map>
#include <set>
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;
typedef std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> > SwampWString;
class AreaStats; // NAME: proto-pdb:default.pdb

#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specializations (C0132, C0231); retail calls these out of line
// (_Copy 0x000112F5, _Uninitialized_copy 0x00103737 / 0x00103754); their header bodies would emit here with
// __ehhandler$ funclets (gate *objects* extra) and reach the unnamed retail _Uninit_copy (relink refusal) (5.2 T3.c4)
template <> void std::basic_string<char, std::char_traits<char>, std::allocator<char> >::_Copy(unsigned int, unsigned int);
// the wstring _Copy too (GenerateSaveGameName's operator+ chain reaches it; __ehhandler$ funclet) (5.2 T3.c5)
template <> void SwampWString::_Copy(unsigned int, unsigned int);
namespace std {
template <> AreaStats* _Uninitialized_copy<AreaStats*, AreaStats*, allocator<AreaStats> >(AreaStats*, AreaStats*,
                                                                                         AreaStats*, allocator<AreaStats>&);
template <> pair<SwampString, SwampString>* _Uninitialized_copy<pair<SwampString, SwampString>*,
                                                              pair<SwampString, SwampString>*,
                                                              allocator<pair<SwampString, SwampString> > >(
    pair<SwampString, SwampString>*, pair<SwampString, SwampString>*, pair<SwampString, SwampString>*,
    allocator<pair<SwampString, SwampString> >&);
// another unit's cut object defines this _Tidy (relink: our objects and a cut object both define it) (5.2 T3.c4)
template <> void vector<pair<SwampString, SwampString> >::_Tidy();
// HUDUI owns _Buy 0x000E84B1 (our copy stays visible: the vector() ctor inlines _Buy(0), ??0LevelStats); its body
// would emit _Xlen<AreaStats>, a second copy of the 0x00139A45 fold that relink appends past retail .text (vsize +0x35)
// (5.2 T3.c4)
template <> void vector<AreaStats>::_Xlen() const;
}
#endif

const SwampString format(const char*, ...); // NAME: proto-pdb:default.pdb

extern "C" int __cdecl _strnicmp(const char*, const char*, unsigned int); // src proto-pdb:default.pdb _strnicmp

struct _FILETIME { // NAME: proto-pdb:default.pdb
    unsigned long dwLowDateTime;       // 0x00
    unsigned long dwHighDateTime;      // 0x04
};
DECOMP_SIZE_ASSERT(_FILETIME, 0x8);
struct _WIN32_FIND_DATAA { // NAME: proto-pdb:default.pdb
    unsigned long dwFileAttributes;    // 0x00
    _FILETIME ftCreationTime;          // 0x04
    _FILETIME ftLastAccessTime;        // 0x0C
    _FILETIME ftLastWriteTime;         // 0x14
    char field_1C[0x140 - 0x1C];       // 0x1C
};
DECOMP_SIZE_ASSERT(_WIN32_FIND_DATAA, 0x140);
struct _XGAME_FIND_DATA { // NAME: proto-pdb:default.pdb
    _WIN32_FIND_DATAA wfd;             // 0x000
    char szSaveGameDirectory[260];     // 0x140
    unsigned short szSaveGameName[128]; // 0x244
};
DECOMP_SIZE_ASSERT(_XGAME_FIND_DATA, 0x344);

union _ULARGE_INTEGER { // NAME: proto-pdb:default.pdb
    unsigned __int64 QuadPart;         // 0x00
};
extern "C" void* __stdcall FindFirstFileA(const char*, _WIN32_FIND_DATAA*); // NAME: proto-pdb:default.pdb
extern "C" int __stdcall FindNextFileA(void*, _WIN32_FIND_DATAA*); // NAME: proto-pdb:default.pdb
extern "C" int __stdcall CloseHandle(void*); // NAME: proto-pdb:default.pdb
extern "C" int __stdcall GetDiskFreeSpaceExA(const char*, _ULARGE_INTEGER*, _ULARGE_INTEGER*, _ULARGE_INTEGER*); // NAME: proto-pdb:default.pdb
struct XCALCSIG_SIGNATURE { // NAME: proto-pdb:default.pdb
    unsigned char Signature[20];       // 0x00
};
DECOMP_SIZE_ASSERT(XCALCSIG_SIGNATURE, 0x14);
extern "C" void* __stdcall XCalculateSignatureBegin(unsigned long); // NAME: proto-pdb:default.pdb
extern "C" unsigned long __stdcall XCalculateSignatureUpdate(void*, const unsigned char*, unsigned long); // NAME: proto-pdb:default.pdb
extern "C" unsigned long __stdcall XCalculateSignatureEnd(void*, XCALCSIG_SIGNATURE*); // NAME: proto-pdb:default.pdb
extern "C" void* __stdcall XFindFirstSaveGame(const char*, _XGAME_FIND_DATA*); // NAME: proto-pdb:default.pdb
extern "C" int __stdcall XFindNextSaveGame(void*, _XGAME_FIND_DATA*); // NAME: proto-pdb:default.pdb
extern "C" int __stdcall XFindClose(void*); // NAME: proto-pdb:default.pdb

namespace BeepLib { // NAME: proto-pdb:default.pdb
SwampString MakeLower(const SwampString&);
SwampString StripPath(const SwampString&);
std::vector<SwampString> SplitString(const SwampString&, const SwampString&);
SwampString StripDigits(const SwampString&);
double RandomFloat(float, float);
}

class BeepFile { // NAME: proto-pdb:default.pdb
public:
    BeepFile();
    ~BeepFile();
    bool Open(const SwampString&);
    bool OpenForWrite(const SwampString&);
    bool Read(void*, unsigned int);
    bool Write(const void*, unsigned int);
    unsigned int Size();

    char field_0[0x20];                // 0x00
};
DECOMP_SIZE_ASSERT(BeepFile, 0x20);

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    struct Record { // NAME: proto-pdb:default.pdb
        SwampString name;              // 0x00
        SwampString stringdata;        // 0x1C
        int intdata;                   // 0x38
        double floatdata;              // 0x40
        bool booldata;                 // 0x48
    };
    const SwampString GetBlockName() const { return _blockname; }
    const std::vector<BeepTDF*>& GetBlocks() const { return _blocklist; }
    const BeepTDF* FindBlock(const SwampString&, bool) const;
    bool GetBoolRecord(const SwampString&, bool) const;
    double GetFloatRecord(const SwampString&, double) const;
    const SwampString GetStringRecord(const SwampString&, const SwampString&) const;
    int GetIntRecord(const SwampString&, int) const;

    SwampString _blockname;            // 0x00
    bool _valid;                       // 0x1C
    std::vector<Record*> _recordlist;  // 0x20
    char field_30[0xC];                // 0x30
    std::vector<BeepTDF*> _blocklist;  // 0x3C
    char field_4C[0xC];                // 0x4C
};
DECOMP_SIZE_ASSERT(BeepTDF, 0x58);

class SwampSceneNode;        // NAME: proto-pdb:default.pdb
class CollisionResultObject; // NAME: proto-pdb:default.pdb
template <class T> class Ptr; // NAME: proto-pdb:default.pdb

struct Vector3 { // NAME: proto-pdb:default.pdb
    Vector3() {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    unsigned int GetRGB() const;

    float x;                           // 0x00
    float y;                           // 0x04
    float z;                           // 0x08
};
DECOMP_SIZE_ASSERT(Vector3, 0xC);
#ifdef NON_MATCHING // D78, D117: default.pdb records no free Vector3 operator (5.2 T20)
inline Vector3 operator*(const Vector3& v, float f) { return Vector3(v.x * f, v.y * f, v.z * f); }
#endif

struct Quaternion { // NAME: proto-pdb:default.pdb
    Quaternion() {}
    Quaternion(float x, float y, float z) { Set(x, y, z); }
    Quaternion(float x, float y, float z, float _w) : v(x, y, z), w(_w) {}
    void Set(float, float, float);

    Vector3 v;                         // 0x00
    float w;                           // 0x0C
};
DECOMP_SIZE_ASSERT(Quaternion, 0x10);

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

class Locator { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x44];                // 0x00
    SwampScript* _script;              // 0x44
};
DECOMP_SIZE_ASSERT(Locator, 0x48);

class LocatorMain : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    const SwampString& GetLoadName() const { return _loadName; }

    Locator* _locator;                 // 0x4C
    SwampString _loadName;             // 0x50
    SwampString _instanceName;         // 0x6C
};
DECOMP_SIZE_ASSERT(LocatorMain, 0x88);

class AreaLoadLocatorMain : public LocatorMain { // NAME: proto-pdb:default.pdb
public:
    bool _didFlyThrough;               // 0x88
    bool _chapterStart;                // 0x89
    char field_8A[0x72];               // 0x8A
};
DECOMP_SIZE_ASSERT(AreaLoadLocatorMain, 0xFC);

class AnimCam { // NAME: proto-pdb:default.pdb
public:
    static void Reset();

private:
    static bool _letterbox;
    static SwampString s_returnCamera;
};

class LoadScreen { // NAME: proto-pdb:default.pdb
public:
    virtual ~LoadScreen();
    void DisplayLoadScreen();
};

class AmbientSound { // NAME: proto-pdb:default.pdb
public:
    static void CreateAmbientSound(const BeepTDF*);
};

class Diary { // NAME: proto-pdb:default.pdb
public:
    Diary();
    void New(const unsigned short*, const SwampString&);
    bool Save(const unsigned short*, SwampString&);

    char field_0[0x14C];               // 0x00
};
DECOMP_SIZE_ASSERT(Diary, 0x14C);

class LocalizedStrings { // NAME: proto-pdb:default.pdb
public:
    static LocalizedStrings& Instance();
    const unsigned short* GetGameString(const SwampString& str);
};

struct _SYSTEMTIME { // NAME: proto-pdb:default.pdb
    unsigned short wYear;              // 0x00
    unsigned short wMonth;             // 0x02
    unsigned short wDayOfWeek;         // 0x04
    unsigned short wDay;               // 0x06
    unsigned short wHour;              // 0x08
    unsigned short wMinute;            // 0x0A
    unsigned short wSecond;            // 0x0C
    unsigned short wMilliseconds;      // 0x0E
};
DECOMP_SIZE_ASSERT(_SYSTEMTIME, 0x10);
extern "C" void __stdcall GetSystemTime(_SYSTEMTIME*); // NAME: proto-pdb:default.pdb
SwampWString GetLocalizedTimeString(int hours, int minutes, int seconds); // NAME: proto-pdb:default.pdb
SwampWString GetLocalizedDateString(int year, int month, int day); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x00102435
// PROTOTYPE: PREVIEW 0x004ECF28 identical
// NAME: proto-pdb:default.pdb
class AreaStats { // NAME: proto-pdb:default.pdb
public:
    AreaStats();
    void Copy(const AreaStats& pstats);

    int _clustersRemaining;            // 0x00
    int _clustersFound;                // 0x04
    int _jugRemaining;                 // 0x08
    int _jugMax;                       // 0x0C
    int _pagesFound;                   // 0x10
    int _pagesRemaining;               // 0x14
    int _boxesFound;                   // 0x18
    int _boxesMax;                     // 0x1C
    int _juneBugsFound;                // 0x20
    int _juneBugsMax;                  // 0x24
    int _whammysFound;                 // 0x28
    int _whammysRemaining;             // 0x2C
    SwampString _name;                 // 0x30
};
DECOMP_SIZE_ASSERT(AreaStats, 0x4C);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit AreaStats::operator= in this unit (no matched
// function odr-uses it); this assignment odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_AreaStats_assign(AreaStats* p, const AreaStats& v)
{
    *p = v;
}

// FUNCTION: USA 0x00104810
// NAME: proto-pdb:default.pdb
class LevelStats { // NAME: proto-pdb:default.pdb
public:
    LevelStats();
    ~LevelStats();
    void Load(const BeepTDF& tdf);
    SwampString Save();

    SwampString _name;                 // 0x00
    std::vector<AreaStats> _areaStatsList; // 0x1C
};
DECOMP_SIZE_ASSERT(LevelStats, 0x2C);

class Level { // NAME: proto-pdb:default.pdb
public:
    Level();
    ~Level();
    void AddCluster();
    void SetLevelName(const SwampString& ln);
    void SetAreaName(const SwampString& an);
    void SetSkyName(const SwampString& skyName);
    void SetSkyEffect(const SwampString& skyEffect);

    std::map<SwampString, int> _zones; // 0x00
    SwampScript* _script;              // 0x0C
    LevelStats _levelStats;            // 0x10
    AreaStats _stats;                  // 0x3C
    Vector3 _vinceStart;               // 0x88
    Quaternion _vinceOrient;           // 0x94
    unsigned int _vinceZone;           // 0xA4
    bool _isVoodooShop;                // 0xA8
    SwampString _levelName;            // 0xAC
    SwampString _areaName;             // 0xC8
    SwampString _skyName;              // 0xE4
    SwampString _skyEffect;            // 0x100
    std::map<SwampString, AreaStats> _otherStats; // 0x11C
    std::set<SwampString> _levelWhammys; // 0x128
    Vector3 _vinceAmbient;             // 0x134
};
DECOMP_SIZE_ASSERT(Level, 0x140);

class RaceStandings { // NAME: proto-pdb:default.pdb
public:
    static void Load(BeepTDF*);
    static SwampString Save();
};

class HUDUI { // NAME: proto-pdb:default.pdb
public:
    virtual ~HUDUI();
};

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    enum NodeOptions { // NAME: proto-pdb:default.pdb
        UpdateOnlyOnRender = 0x400
    };
    SwampScript* GetScript() const;
    const Vector3& SetLocation(const Vector3&);
    const Quaternion& SetRotation(const Quaternion&);
    void SetOption(unsigned int option, bool on, bool recurse);
};
class FireFlyNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};
class RegionTrigger : public SwampSceneNode { // NAME: proto-pdb:default.pdb
};

class MusicManager { // NAME: proto-pdb:default.pdb
public:
    void LeavingLevel(const SwampString&);
};
extern MusicManager* g_MusicManager; // NAME: proto-pdb:default.pdb

class LocatorManager { // NAME: proto-pdb:default.pdb
public:
    void RemoveAll();
};
extern LocatorManager* g_locatorManager; // NAME: proto-pdb:default.pdb

class SwampParticleManager { // NAME: proto-pdb:default.pdb
public:
    void RemoveAll();
    FireFlyNode* Add(const SwampString&, const SwampString&, unsigned int, bool);
};
extern SwampParticleManager* g_particleManager; // NAME: proto-pdb:default.pdb

class SwingHookManager { // NAME: proto-pdb:default.pdb
public:
    void RemoveAll();
};
extern SwingHookManager* pSwingHookManager; // NAME: proto-pdb:default.pdb

class CameraManager { // NAME: proto-pdb:default.pdb
public:
    void DestroyAll();

    char field_0[0x44];                // 0x00
    float _farPlane;                   // 0x44
};
extern CameraManager* g_CameraManager; // NAME: proto-pdb:default.pdb

class RegionManager { // NAME: proto-pdb:default.pdb
public:
    RegionTrigger* Find(const SwampString&);
    void DestroyAll();
};

class SwampDebugGeometry { // NAME: proto-pdb:default.pdb
public:
    void DestroyAll();
};

class RootNode { // NAME: proto-pdb:default.pdb
public:
    char field_0[0xA4];                // 0x00
    unsigned int _fogColor;            // 0xA4
    float _fogDistance;                // 0xA8
    float _fogDensity;                 // 0xAC
};
DECOMP_SIZE_ASSERT(RootNode, 0xB0);

class SwampScene { // NAME: proto-pdb:default.pdb
public:
    void DestroyAll();

    RootNode* _root;                   // 0x00
};

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void AddRef() { _RefCount++; }
    void SubRef() { _RefCount--; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
};
class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};
class ModelResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};
class AnimationResource : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    char field_20[0xC];                // 0x20
    float _length;                     // 0x2C
};

template <class T>
class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle() : _Resource(0) {}
    ResourceHandle(const ResourceHandle& h) : _Resource(h._Resource) { if (_Resource) _Resource->AddRef(); }
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }
    ResourceHandle& operator=(const ResourceHandle&);
    bool IsValid() const { return _Resource != 0; }
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

class AnimationControl : public ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef();

    char field_20[0x4];                // 0x20
    ResourceHandle<AnimationResource> _anim; // 0x24
    char field_28[0x10];               // 0x28
    float _time;                       // 0x38
};

template <class T>
class ResourceManager { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<T> Load(const SwampString&);
};
class AnimationManager : public ResourceManager<AnimationResource> { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x54];                // 0x00
    bool _missingResourceMessages;     // 0x54
};
class ModelManager : public ResourceManager<ModelResource> { // NAME: proto-pdb:default.pdb
};

class GatorNode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle<AnimationControl>& StartAnimation(const ResourceHandle<AnimationResource>&, bool, bool);

    char field_0[0x140];               // 0x00
    ResourceHandle<AnimationControl> _currentAnim; // 0x140
    char field_144[0x40];              // 0x144
    float _animSoundRange;             // 0x184
    float _animSoundVolume;            // 0x188
};
class Critter : public GatorNode { // NAME: proto-pdb:default.pdb
public:
    void SetDefaultIdleAnim(ResourceHandle<AnimationResource>);
    void SetLoadedNames(const SwampString&, const SwampString&);
};
class Vince : public Critter { // NAME: proto-pdb:default.pdb
};

class CritterManager { // NAME: proto-pdb:default.pdb
public:
    void DestroyAll();
    Critter* Create(const SwampString&, const SwampString&, const char*, ResourceHandle<ModelResource>, const BeepTDF&);

    char field_0[0x28];                // 0x00
    Vince* _vinceCache;                // 0x28
};
extern CritterManager* pManager; // NAME: proto-pdb:default.pdb

class MaterialManager { // NAME: proto-pdb:default.pdb
public:
    void ClearFixupEnvmaps();

    char field_0[0x5C];                // 0x00
    ResourceHandle<TextureResource> _defaultEMap; // 0x5C
};

class SwampCamera; // NAME: proto-pdb:default.pdb

class Swamp { // NAME: proto-pdb:default.pdb
public:
    AnimationManager& GetAnimationManager() const { return *_animationManager; }
    ModelManager& GetModelManager() const { return *_modelManager; }

    char field_0[0xC];                 // 0x00
    SwampScene* _scene;                // 0x0C
    SwampDebugGeometry* _debugGeometry; // 0x10
    char field_14[0x4];                // 0x14
    AnimationManager* _animationManager; // 0x18
    char field_1C[0x8];                // 0x1C
    MaterialManager* _materialManager; // 0x24
    ModelManager* _modelManager;       // 0x28
    char field_2C[0x4];                // 0x2C
    RegionManager* _regionManager;     // 0x30
    char field_34[0xC];                // 0x34
    SwampCamera* _currentCamera;       // 0x40
};
extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class Globals { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x118];               // 0x00
    Vector3 _vinceAmbientColor;        // 0x118
};
extern Globals gSettings; // NAME: proto-pdb:default.pdb

class TrolleyManager { // NAME: proto-pdb:default.pdb
public:
    void Reset();
};
extern TrolleyManager* g_TrolleyManager; // NAME: proto-pdb:default.pdb

class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    class Slot { // NAME: proto-pdb:default.pdb
    public:
        Slot();
        bool IsChapterFinished(const SwampString& areaname);
        void Load(const BeepTDF& tdf, const _XGAME_FIND_DATA& gameData);
        void Save(SwampString& ff) const;
        void DeclareChapterFinished(const SwampString& a, const SwampString& l);

        SwampString level;             // 0x00
        SwampString area;              // 0x1C
        unsigned long _gameTime;       // 0x38
        _FILETIME _saveTime;           // 0x3C
        std::vector<std::pair<SwampString, SwampString> > _chaptersFinished; // 0x44
        SwampString _savePath;         // 0x54
        SwampWString _gameName;        // 0x70
    };
    enum LoadType { // NAME: proto-pdb:default.pdb
        LOAD_VOODOOSHOP = 0,
        LOAD_AREA = 1,
        LOAD_CHAPTER = 2
    };

    static LevelLoader& Instance();
    static void EnumerateSavedGames(std::vector<_XGAME_FIND_DATA>& saves);
    static bool GenerateFileSignature(const SwampString& filename, bool roamable);
    static bool VerifyFileSignature(const SwampString& filename, bool roamable);
    static bool CalculateFileSignature(XCALCSIG_SIGNATURE* sig, BeepFile& file, bool roamable);
    static bool FreeSpaceCheck(unsigned int* availableBlocks, unsigned int* metadataBlocksNeeded, unsigned int numGamesToReserve);
    LevelStats* GetLevelStats(const SwampString& levelname);
    AreaStats* GetAreaStats(const SwampString& levelname, const SwampString& areaname);
    void FlushLocalStats();
    void CloneMasterStats();
    void LoadLevelStats(const BeepTDF& tdf);
    void RequestVoodooShop(LoadScreen* loadScreen);
    void RequestChapter(const SwampString& levelname, const SwampString& areaname);
    void RequestArea(const SwampString&, const SwampString&, bool, float, bool);
    void SaveGlobalData(const SwampString&);
    void NewGame();
    void DestroyCurrentLevel();
    void InitializeCurrentSave(const unsigned short* name);
    SwampWString GenerateSaveGameName(const SwampString& area);
    void ClearMasterStats();

private:
    void LoadAmbientSounds(Level* plevel, const BeepTDF& summary, LoadScreen& loadScreen);
    void LoadZones(Level* plevel, const BeepTDF& summary, LoadScreen& loadScreen);
    void LoadParticles(Level* plevel, const BeepTDF& summary, LoadScreen& loadScreen);
    void LoadSettings(Level* plevel, const BeepTDF& summary, LoadScreen& loadScreen);
    void LoadObjects(Level* plevel, const BeepTDF& summary, LoadScreen& loadScreen);

public:
    HUDUI* _hudUI;                     // 0x00
    Level* _curLevel;                  // 0x04
    SwampString _requestLevel;         // 0x08
    SwampString _requestArea;          // 0x24
    float _requestTime;                // 0x40
    LoadType _loadType;                // 0x44
    int _currentSavegameSlot;          // 0x48
    std::vector<Slot> _slots;          // 0x4C
    Diary _currSave;                   // 0x5C
    bool _requestPending;              // 0x1A8
    bool _savePending;                 // 0x1A9
    bool _loadingFromVoodooShop;       // 0x1AA
    bool _newGameFlag;                 // 0x1AB
    LoadScreen* _loadScreen;           // 0x1AC
    std::vector<LevelStats> _levelStatsList; // 0x1B0
    std::map<SwampString, SwampSceneNode*> _regionScriptNodeList; // 0x1C0

private:
    LevelLoader();
};
DECOMP_SIZE_ASSERT(LevelLoader::Slot, 0x8C);
DECOMP_SIZE_ASSERT(LevelLoader, 0x1CC);

#ifndef NON_MATCHING
// !FAKE: c1xx front end; declaration-only explicit specializations (C0132, C0231); these rows stay guarded (their EH
// funclets and unnamed callees wall them, 5.2 T3.c4); the matched callers call them out of line as retail does, and the
// header bodies would emit here with __ehhandler$ funclets (gate *objects* extra) and .text$x (relink refusal)
namespace std {
template <> _XGAME_FIND_DATA * _Uninit_copy<_XGAME_FIND_DATA *,_XGAME_FIND_DATA *,std::allocator<_XGAME_FIND_DATA> >(_XGAME_FIND_DATA *,_XGAME_FIND_DATA *,_XGAME_FIND_DATA *,std::allocator<_XGAME_FIND_DATA> &,std::_Nonscalar_ptr_iterator_tag);
template <> std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,AreaStats,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,AreaStats> >,0> >::_Node * _Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,AreaStats,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,AreaStats> >,0> >::_Buynode();
template <> std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node * _Tree<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Buynode();
template <> void _Uninit_fill_n<_XGAME_FIND_DATA *,unsigned int,_XGAME_FIND_DATA,std::allocator<_XGAME_FIND_DATA> >(_XGAME_FIND_DATA *,unsigned int,_XGAME_FIND_DATA const &,std::allocator<_XGAME_FIND_DATA> &,std::_Nonscalar_ptr_iterator_tag);
template <> std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node * _Tree<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Buynode(std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node *,std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node *,std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node *,std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,char);
template <> void vector<_XGAME_FIND_DATA,std::allocator<_XGAME_FIND_DATA> >::_Insert_n(std::vector<_XGAME_FIND_DATA,std::allocator<_XGAME_FIND_DATA> >::iterator,unsigned int,_XGAME_FIND_DATA const &);
template <> void _Uninit_fill_n<AreaStats *,unsigned int,AreaStats,std::allocator<AreaStats> >(AreaStats *,unsigned int,AreaStats const &,std::allocator<AreaStats> &,std::_Nonscalar_ptr_iterator_tag);
template <> void _Uninit_fill_n<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,unsigned int,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > >(std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,unsigned int,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > const &,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > &,std::_Nonscalar_ptr_iterator_tag);
template <> void vector<AreaStats,std::allocator<AreaStats> >::_Insert_n(std::vector<AreaStats,std::allocator<AreaStats> >::iterator,unsigned int,AreaStats const &);
template <> void vector<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > >::_Insert_n(std::vector<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > >::iterator,unsigned int,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > const &);
template <> std::vector<AreaStats,std::allocator<AreaStats> > & vector<AreaStats,std::allocator<AreaStats> >::operator=(std::vector<AreaStats,std::allocator<AreaStats> > const &);
template <> std::vector<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > > & vector<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > >::operator=(std::vector<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > > const &);
// GetLevelStats calls push_back<LevelStats> out of line (0x00105BA2, pinned); its header body would emit
// _Insert_n<LevelStats> with __ehhandler$ funclets (gate relink refusal) (5.2 T12.c1)
template <> void vector<LevelStats,std::allocator<LevelStats> >::push_back(LevelStats const &);
// LoadZones calls map<string,int>::insert out of line (fold 0x000F0BAB, pinned); its header body would emit
// _Buynode with an __ehhandler$ funclet and the _Node ctor (gate relink refusal) (5.2 T12.c1)
template <> std::pair<std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,int,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,int> >,0> >::iterator,bool> _Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,int,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,int> >,0> >::insert(std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,int> const &);
// Level::Level calls map<string,int>'s _Init out of line (fold 0x000EDA1B, another unit's); its header body would
// emit _Buynode with an __ehhandler$ funclet (gate relink refusal) (5.2 T12.c16)
template <> void _Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,int,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,int> >,0> >::_Init();
}
#endif

// FUNCTION: USA 0x0010049F
// PROTOTYPE: PREVIEW 0x004EB01E identical
// NAME: proto-pdb:default.pdb
void Level::AddCluster()
{
    _stats._clustersFound++;
    _stats._clustersRemaining--;
}

// FUNCTION: USA 0x001004A6
// PROTOTYPE: PREVIEW 0x004EB025 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1): equal outside the mask; the stack probe callee at
// 0x00141C10 is in no symbol file (reloc-code)
bool LevelLoader::CalculateFileSignature(XCALCSIG_SIGNATURE* sig, BeepFile& file, bool roamable)
{
    unsigned char data[16384];
    void* hSig = XCalculateSignatureBegin(roamable ? 0 : 1);
    if (hSig == (void*)-1)
        return false;
    unsigned int v0 = file.Size();
    for (unsigned int i = 0; i < v0; i += 16384) {
        unsigned int v1 = 16384;
        if (i + 16384 > v0)
            v1 = v0 - i;
        if (!file.Read(data, v1) || XCalculateSignatureUpdate(hSig, data, v1) != 0) {
            XCalculateSignatureEnd(hSig, 0);
            return false;
        }
    }
    return XCalculateSignatureEnd(hSig, sig) == 0 ? true : false;
}
#endif

// FUNCTION: USA 0x00100545
// PROTOTYPE: PREVIEW 0x004EB0C4 identical
// NAME: proto-pdb:default.pdb
bool LevelLoader::FreeSpaceCheck(unsigned int* availableBlocks, unsigned int* metadataBlocksNeeded, unsigned int numGamesToReserve)
{
    _WIN32_FIND_DATAA fd;
    _ULARGE_INTEGER availableBytes;
    *metadataBlocksNeeded = numGamesToReserve * 21;
    int i = 0;
    void* p = FindFirstFileA("u:\\*", &fd);
    if (p != (void*)-1) {
        do {
            i++;
        } while (FindNextFileA(p, &fd));
        CloseHandle(p);
    }
    GetDiskFreeSpaceExA("u:\\", &availableBytes, 0, 0);
    *availableBlocks = (unsigned int)(availableBytes.QuadPart / 16384);
    return i < 4096 ? true : false;
}

// FUNCTION: USA 0x0010090A
// PROTOTYPE: PREVIEW 0x004EB459 identical
// NAME: proto-pdb:default.pdb
bool FindChapterStartLocator(const std::pair<SwampString, Locator*>& p)
{
    // !FAKE: c2 register allocation; stand-in local; retail holds the script in esi across the call
    SwampScript* v0 = p.second->_script;
    if (!v0)
        return false;
    if (_strnicmp(((LocatorMain*)v0)->GetLoadName().c_str(), "locator_areaload", 16))
        return false;
    // !FAKE: c2 block layout; split test with a separate `return true`; one shared return-false block first as retail (T12.c29 probe: ternary 4 diff lines -> byte-equal)
    if (!((AreaLoadLocatorMain*)v0)->_chapterStart)
        return false;
    return true;
}

// FUNCTION: USA 0x00100BF4
// PROTOTYPE: PREVIEW 0x004EB743 identical
// NAME: proto-pdb:default.pdb
bool LevelLoader::Slot::IsChapterFinished(const SwampString& areaname)
{
    SwampString testname = BeepLib::MakeLower(areaname);
    for (std::vector<std::pair<SwampString, SwampString> >::iterator i = _chaptersFinished.begin(); i != _chaptersFinished.end(); ++i) {
        if (!i->second.compare(testname))
            return true;
    }
    return false;
}

// FUNCTION: USA 0x00100DA2
// PROTOTYPE: PREVIEW 0x004EB8B2 identical
// NAME: proto-pdb:default.pdb
void AnimCam::Reset()
{
    s_returnCamera = "";
    _letterbox = false;
}

// FUNCTION: USA 0x00100DB9
// PROTOTYPE: PREVIEW 0x004EB8C9 identical
// NAME: proto-pdb:default.pdb
void Level::SetLevelName(const SwampString& ln)
{
    _levelName = ln;
}

// FUNCTION: USA 0x00100DCF
// PROTOTYPE: PREVIEW 0x004EB8DF identical
// NAME: proto-pdb:default.pdb
void Level::SetAreaName(const SwampString& an)
{
    _areaName = an;
}

// FUNCTION: USA 0x00100DE5
// PROTOTYPE: PREVIEW 0x004EB8F5 identical
// NAME: proto-pdb:default.pdb
void Level::SetSkyName(const SwampString& skyName)
{
    _skyName = skyName;
}

// FUNCTION: USA 0x00100DFB
// PROTOTYPE: PREVIEW 0x004EB90B identical
// NAME: proto-pdb:default.pdb
void Level::SetSkyEffect(const SwampString& skyEffect)
{
    _skyEffect = skyEffect;
}

// FUNCTION: USA 0x00100E11
// PROTOTYPE: PREVIEW 0x004EB921 identical
// NAME: proto-pdb:default.pdb
SwampString LevelStats::Save()
{
    SwampString ff;
    ff += "\t\t[Areas]\n\t\t{\n";
    for (std::vector<AreaStats>::const_iterator ait = _areaStatsList.begin(); ait != _areaStatsList.end(); ++ait) {
        AreaStats as = *ait;
        ff += format("\t\t\t[%s]\n\t\t\t{\n", as._name.c_str());
        if (as._name.compare("jimmysfanboat") == 0) {
            ff += RaceStandings::Save();
        } else {
            ff += format("\t\t\t\tClustersFound = %d;\n", as._clustersFound);
            ff += format("\t\t\t\tClustersRemaining = %d;\n", as._clustersRemaining);
            ff += format("\t\t\t\tJugsMax = %d;\n", as._jugMax);
            ff += format("\t\t\t\tJugsRemaining = %d;\n", as._jugRemaining);
            ff += format("\t\t\t\tWhammysFound = %d;\n", as._whammysFound);
            ff += format("\t\t\t\tWhammysRemaining = %d;\n", as._whammysRemaining);
            ff += format("\t\t\t\tPagesFound = %d;\n", as._pagesFound);
            ff += format("\t\t\t\tPagesRemaining = %d;\n", as._pagesRemaining);
            ff += format("\t\t\t\tJuneBugsFound = %d;\n", as._juneBugsFound);
        }
        ff += "\t\t\t}\n";
    }
    ff += "\t\t}\n";
    return ff;
}

// FUNCTION: USA 0x001010BC
// PROTOTYPE: PREVIEW 0x004EBBCC identical
// NAME: proto-pdb:default.pdb
void LevelLoader::Slot::Save(SwampString& ff) const
{
    ff += format("level = %s;\n", level.c_str());
    ff += format("area = %s;\n", area.c_str());
    ff += format("gametime = %u;\n", _gameTime);
    ff += "[chaptersfinished]\n{\n";
    for (std::vector<std::pair<SwampString, SwampString> >::const_iterator i = _chaptersFinished.begin();
         i != _chaptersFinished.end(); ++i)
        ff += format("\tarea = %s %s;\n", i->first.c_str(), i->second.c_str());
    ff += "}\n";
}

// FUNCTION: USA 0x001011E5
// PROTOTYPE: PREVIEW 0x004EBCF5 identical
// NAME: proto-pdb:default.pdb
void LevelLoader::LoadSettings(Level* plevel, const BeepTDF& summary, LoadScreen& loadScreen)
{
    const BeepTDF* settings = summary.FindBlock("Settings", false);
    float farPlane = settings->GetFloatRecord("areafarplane", 10000.0) * 0.01f;
    float fogNear = settings->GetFloatRecord("areafognear", 100000.0) * 0.01f;
    float fogFar = settings->GetFloatRecord("areafogfar", 100000.0) * 0.01f;
    Vector3 clr(0.0f, 0.0f, 0.0f);
    sscanf(settings->GetStringRecord("areafogcolor", "").c_str(), "%f %f %f", &clr.x, &clr.y, &clr.z);
    RootNode* root = g_swamp->_scene->_root;
    float range = fogFar - fogNear;
    range = range > 0.0f ? range : 0.0f;
    if (range > 0.0f) {
        root->_fogDistance = fogNear;
        root->_fogDensity = farPlane / range;
        // !FAKE: inliner; hand form of a Vector3 scale through the PDB ctor (no free operator in the PDB, D78)
        root->_fogColor = Vector3(clr.x * (1.0f / 255.0f), clr.y * (1.0f / 255.0f), clr.z * (1.0f / 255.0f)).GetRGB();
    }
    g_CameraManager->_farPlane = farPlane;
    plevel->SetSkyName(settings->GetStringRecord("sky", format("sky_%s", BeepLib::StripPath(plevel->_levelName).c_str())));
    plevel->SetSkyEffect(settings->GetStringRecord("skyEffect", ""));
    SwampString s = settings->GetStringRecord("areavinceambient", "");
    if (!s.empty()) {
        sscanf(s.c_str(), "%f %f %f", &clr.x, &clr.y, &clr.z);
        // !FAKE: inliner; hand form of a Vector3 scale through the PDB ctor (no free operator in the PDB, D78)
        plevel->_vinceAmbient = Vector3(clr.x * (1.0f / 255.0f), clr.y * (1.0f / 255.0f), clr.z * (1.0f / 255.0f));
    } else {
        plevel->_vinceAmbient = gSettings._vinceAmbientColor;
    }
}

// FUNCTION: USA 0x00101556
// PROTOTYPE: PREVIEW 0x004EC066 identical
// NAME: proto-pdb:default.pdb
void LevelLoader::LoadParticles(Level* plevel, const BeepTDF& summary, LoadScreen& loadScreen)
{
    const BeepTDF* particles = summary.FindBlock("Particles", false);
    if (particles) {
        SwampParticleManager& mgr = *g_particleManager;
        for (std::vector<BeepTDF*>::const_iterator i = particles->GetBlocks().begin(); i != particles->GetBlocks().end(); ++i) {
            loadScreen.DisplayLoadScreen();
            SwampString instanceName = (*i)->GetStringRecord("instancename", "");
            if (instanceName.empty())
                instanceName = (*i)->GetBlockName();
            unsigned int zone = (*i)->GetIntRecord("Zone", 0);
            bool autostart = (*i)->GetBoolRecord("autostart", true);
            FireFlyNode* node = mgr.Add(instanceName, (*i)->GetBlockName(), zone, autostart);
            if (!node)
                continue;
            Vector3 loc;
            sscanf((*i)->GetStringRecord("Location", "").c_str(), "%f %f %f", &loc.x, &loc.y, &loc.z);
            Quaternion rot;
            sscanf((*i)->GetStringRecord("Rotation", "").c_str(), "%f %f %f %f", &rot.v.x, &rot.v.y, &rot.v.z, &rot.w);
            node->SetLocation(loc);
            node->SetRotation(rot);
            node->SetOption(SwampSceneNode::UpdateOnlyOnRender, true, false);
        }
    }
}

// FUNCTION: USA 0x0010180A
// PROTOTYPE: PREVIEW 0x004EC31A identical
// NAME: proto-pdb:default.pdb
void LevelLoader::LoadAmbientSounds(Level* plevel, const BeepTDF& summary, LoadScreen& loadScreen)
{
    const BeepTDF* p = summary.FindBlock("AmbientSounds", false);
    if (p) {
        for (std::vector<BeepTDF*>::const_iterator i = p->GetBlocks().begin(); i != p->GetBlocks().end(); ++i) {
            loadScreen.DisplayLoadScreen();
            AmbientSound::CreateAmbientSound(*i);
        }
    }
}

// FUNCTION: USA 0x00101FF5
// PROTOTYPE: PREVIEW 0x004ECAE8 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 90.47619, bytes mismatch (masked 2): the two assigned values are unnamed data words in no
// symbol file (reloc-data); retail pushes the first before `mov esi, ecx`; this draft leaves them out

void LevelLoader::RequestVoodooShop(LoadScreen* loadScreen)
{
    _loadType = LOAD_VOODOOSHOP;
    _requestPending = true;
    delete _loadScreen;
    _loadScreen = loadScreen;
}
#endif

// FUNCTION: USA 0x0010203B
// PROTOTYPE: PREVIEW 0x004ECB2E identical
// NAME: proto-pdb:default.pdb
void LevelLoader::RequestChapter(const SwampString& levelname, const SwampString& areaname)
{
    _newGameFlag = false;
    _loadType = LOAD_CHAPTER;
    _requestLevel = levelname;
    _requestArea = areaname;
    _requestTime = 0.0f;
    _requestPending = true;
}

// FUNCTION: USA 0x0010207C
// PROTOTYPE: PREVIEW 0x004ECB6F identical
// NAME: proto-pdb:default.pdb
void LevelLoader::InitializeCurrentSave(const unsigned short* name)
{
    _currSave.New(name, "d:\\saves\\initialize.diary");
}

// FUNCTION: USA 0x001020B2
// PROTOTYPE: PREVIEW 0x004ECBA5 identical
// NAME: proto-pdb:default.pdb
SwampWString LevelLoader::GenerateSaveGameName(const SwampString& area)
{
    SwampWString locArea = LocalizedStrings::Instance().GetGameString(area);
    _SYSTEMTIME st;
    GetSystemTime(&st);
    SwampWString locDate = GetLocalizedDateString(st.wYear, st.wMonth, st.wDay);
    SwampWString locTime = GetLocalizedTimeString(st.wHour, st.wMinute, st.wSecond);
    SwampWString gmt = LocalizedStrings::Instance().GetGameString("gmt");
    SwampWString result = locArea + L" " + locDate + L" " + locTime + L" " + gmt;
    return result;
}

// FUNCTION: USA 0x00102235
// PROTOTYPE: PREVIEW 0x004ECD28 identical
// NAME: proto-pdb:default.pdb
bool LevelLoader::GenerateFileSignature(const SwampString& filename, bool roamable)
{
    SwampString sigFilename = filename + ".sig";
    BeepFile dataFile;
    BeepFile sigFile;
    XCALCSIG_SIGNATURE signature;
    if (!dataFile.Open(filename))
        return false;
    if (!sigFile.OpenForWrite(sigFilename))
        return false;
    if (!CalculateFileSignature(&signature, dataFile, roamable))
        return false;
    if (!sigFile.Write(&signature, sizeof(signature)))
        return false;
    return true;
}

// FUNCTION: USA 0x001022CF
// PROTOTYPE: PREVIEW 0x004ECDC2 identical
// NAME: proto-pdb:default.pdb
bool LevelLoader::VerifyFileSignature(const SwampString& filename, bool roamable)
{
    SwampString sigFilename = filename + ".sig";
    BeepFile dataFile;
    BeepFile sigFile;
    XCALCSIG_SIGNATURE signatureSaved;
    XCALCSIG_SIGNATURE signatureCalculated;
    if (!dataFile.Open(filename))
        return false;
    if (!sigFile.Open(sigFilename))
        return false;
    if (sigFile.Size() != sizeof(signatureSaved))
        return false;
    if (!sigFile.Read(&signatureSaved, sizeof(signatureSaved)))
        return false;
    if (!CalculateFileSignature(&signatureCalculated, dataFile, roamable))
        return false;
    // !FAKE: c1xx front end; int stand-in local; the intrinsic memcmp keeps its three-way result (repe cmpsb, sbb) as
    // retail only when the result is held (match_one 93.85 -> 100.0 under /Oi)
    int v0 = memcmp(&signatureSaved, &signatureCalculated, sizeof(signatureSaved));
    if (v0 != 0)
        return false;
    return true;
}

// FUNCTION: USA 0x00102647
// PROTOTYPE: PREVIEW 0x004ED13A identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 98.79027, bytes mismatch (masked 0), 16 attempts: only the stack-slot assignment differs
// (retail instanceName +8, rot -0x24, objects +0x30, name +0x34; ours -0x34, +0x14, +0x4c, +0x30); declaration order
// and scope do not move it (5.2 T3.c5)
// 20 attempts, at-stop (5.2 T12.c1): PDB local order (objects first 96.86; zone/loc/rot hoist no change)
void LevelLoader::LoadObjects(Level* plevel, const BeepTDF& summary, LoadScreen& loadScreen)
{
    bool bGotStartPos = false;
    CritterManager& mgr = *pManager;
    const BeepTDF* objects = summary.FindBlock("Objects", false);
    plevel->_stats._clustersRemaining = 0;
    plevel->_stats._jugRemaining = 0;
    for (std::vector<BeepTDF*>::const_iterator oit = objects->GetBlocks().begin(); oit != objects->GetBlocks().end(); ++oit) {
        loadScreen.DisplayLoadScreen();
        SwampString instanceName = (*oit)->GetStringRecord("instancename", "");
        Vector3 loc;
        sscanf((*oit)->GetStringRecord("Location", "").c_str(), "%f %f %f", &loc.x, &loc.y, &loc.z);
        Quaternion rot;
        sscanf((*oit)->GetStringRecord("Rotation", "").c_str(), "%f %f %f %f", &rot.v.x, &rot.v.y, &rot.v.z, &rot.w);
        unsigned int zone = (*oit)->GetIntRecord("Zone", 0);
        SwampString basename = (*oit)->GetBlockName();
        SwampString name(basename, 0, basename.find_last_of("_"));
        name[0] = tolower(name[0]);
        if (_strnicmp(name.c_str(), "DustCluster", 11) == 0) {
            plevel->_stats._clustersRemaining++;
            Critter* dc = mgr.Create("DustCluster", "DustCluster", "DustCluster",
                                     g_swamp->GetModelManager().Load("dustBag_base"), **oit);
            ResourceHandle<AnimationResource> ah = g_swamp->GetAnimationManager().Load("dustBag_idle");
            dc->SetDefaultIdleAnim(ah);
            dc->StartAnimation(ah, true, false);
            dc->SetLoadedNames(basename, instanceName);
            dc->SetRotation(Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
        } else if (_strnicmp(name.c_str(), "dustmote", 8) == 0) {
            plevel->_stats._clustersRemaining++;
            Critter* dm = mgr.Create("dustmote", "dustmote", "dustmote", ResourceHandle<ModelResource>(), **oit);
            dm->SetLoadedNames(basename, instanceName);
            dm->GetScript()->Message(0, 0x14, 0); // 0x14: retail constant (a Messages enumerator, owner unknown)
        } else if (name.compare("vince") == 0) {
            if (!bGotStartPos) {
                plevel->_vinceStart = loc;
                plevel->_vinceOrient = rot;
                bGotStartPos = true;
                plevel->_vinceZone = zone;
            }
        } else {
            SwampString scriptname = BeepLib::StripDigits(name);
            ResourceHandle<ModelResource> mh = g_swamp->GetModelManager().Load(name + "_base.gator");
            loadScreen.DisplayLoadScreen();
            if (mh.IsValid()) {
                SwampString animname = name + SwampString("_idle.anim");
                if (!instanceName.empty())
                    name = instanceName;
                Critter* critter = mgr.Create(name, scriptname, scriptname.c_str(), mh, **oit);
                critter->SetLoadedNames(basename, instanceName);
                float range = (*oit)->GetFloatRecord("soundrange", 10000.0);
                critter->_animSoundRange = range * 0.01f;
                int vol = (*oit)->GetIntRecord("soundvolume", 100);
                critter->_animSoundVolume = vol * 0.01f;
                AnimationManager& animManager = g_swamp->GetAnimationManager();
                bool missing = animManager._missingResourceMessages;
                animManager._missingResourceMessages = false;
                ResourceHandle<AnimationResource> ah = g_swamp->GetAnimationManager().Load(animname);
                animManager._missingResourceMessages = missing;
                loadScreen.DisplayLoadScreen();
                if (ah.IsValid()) {
                    critter->SetDefaultIdleAnim(ah);
                    if (!critter->_currentAnim.IsValid()) {
                        critter->StartAnimation(ah, true, false);
                        if (!(*oit)->GetBoolRecord("NonRandomIdle", false)) {
                            ResourceHandle<AnimationControl> control = critter->_currentAnim;
                            control._Resource->_time = BeepLib::RandomFloat(0.0f, control._Resource->_anim._Resource->_length);
                        }
                    }
                }
            } else {
                SwampString blockname = (*oit)->GetBlockName();
            }
        }
    }
}
#endif

// FUNCTION: USA 0x00104286
// PROTOTYPE: PREVIEW 0x004EECA6 identical
// NAME: proto-pdb:default.pdb
void LevelLoader::LoadZones(Level* plevel, const BeepTDF& summary, LoadScreen& loadScreen)
{
    const BeepTDF* zones = summary.FindBlock("Zones", false);
    for (std::vector<BeepTDF::Record*>::const_iterator i = zones->_recordlist.begin(); i != zones->_recordlist.end(); ++i)
        plevel->_zones.insert(std::pair<const SwampString, int>((*i)->name, (*i)->intdata));
}

// FUNCTION: USA 0x001042F2
// PROTOTYPE: PREVIEW 0x004EED12 identical
// NAME: proto-pdb:default.pdb
void LevelLoader::EnumerateSavedGames(std::vector<_XGAME_FIND_DATA>& saves)
{
    _XGAME_FIND_DATA data;
    void* p = XFindFirstSaveGame("u:\\", &data);
    if (p != (void*)-1) {
        do {
            saves.push_back(data);
        } while (XFindNextSaveGame(p, &data));
        XFindClose(p);
    }
}

// FUNCTION: USA 0x001046A3
// PROTOTYPE: PREVIEW 0x004EF0B0 identical
// NAME: proto-pdb:default.pdb
LevelStats::LevelStats()
{
}

// FUNCTION: USA 0x001046BA
// PROTOTYPE: PREVIEW 0x004EF0C7 identical
// NAME: proto-pdb:default.pdb
LevelLoader::Slot::Slot() : _gameTime(-1)
{
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit LevelStats::operator= in this unit (no matched
// function odr-uses it); this assignment odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_LevelStats_assign(LevelStats* p, const LevelStats& v)
{
    *p = v;
}

// FUNCTION: USA 0x001048B0
// PROTOTYPE: PREVIEW 0x004EF2AC identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1): the pair<string,string> dtor is folded at 0x00018696
// under other names (as DeclareChapterFinished) (5.2 T3.c5)
void LevelLoader::Slot::Load(const BeepTDF& tdf, const _XGAME_FIND_DATA& gameData)
{
    level = tdf.GetStringRecord("level", "");
    area = tdf.GetStringRecord("area", "");
    _gameTime = tdf.GetIntRecord("gametime", 0);
    _saveTime = gameData.wfd.ftLastWriteTime;
    _gameName = gameData.szSaveGameName;
    const BeepTDF* block = tdf.FindBlock("chaptersfinished", false);
    _chaptersFinished.clear();
    for (std::vector<BeepTDF::Record*>::const_iterator i = block->_recordlist.begin(); i != block->_recordlist.end(); ++i) {
        const BeepTDF::Record* record = *i;
        std::vector<SwampString> res = BeepLib::SplitString(record->stringdata, " ");
        _chaptersFinished.push_back(std::make_pair(BeepLib::MakeLower(res[0]), BeepLib::MakeLower(res[1])));
    }
    _savePath = gameData.szSaveGameDirectory;
}
#endif

// FUNCTION: USA 0x00104B0C
// PROTOTYPE: PREVIEW 0x004EF508 identical
// NAME: proto-pdb:default.pdb
Level::~Level()
{
    if (_script) {
        _script->InvalidateScript();
        _script = 0;
    }
}

// FUNCTION: USA 0x00104C71
// PROTOTYPE: PREVIEW 0x004EF66D identical
// NAME: proto-pdb:default.pdb
// !FAKE: inliner; hand form of retail's inlined Quaternion(float, float, float) mem-initializer: cl keeps that inline
// ctor out of line (call ??0Quaternion@@QAE@MMM@Z, 5.2 T12.c1); retail calls Quaternion::Set 0x000DC99B between the
// _vinceStart and _vinceZone stores, so the Set call rides in _vinceZone's initializer (cf. Whammy.cpp:734) (5.2 T12.c16)
Level::Level() : _script(0), _vinceStart(0.0f, 0.0f, 0.0f), _vinceZone((_vinceOrient.Set(0.0f, 0.0f, 1.0f), 0)), _isVoodooShop(false)
{
}

// FUNCTION: USA 0x00104D14
// PROTOTYPE: PREVIEW 0x004EF710 identical
// NAME: proto-pdb:default.pdb
void LevelLoader::DestroyCurrentLevel()
{
    if (_curLevel) {
        g_MusicManager->LeavingLevel(_curLevel->_areaName);
        Vince* vince = pManager->_vinceCache;
        // !FAKE: c1xx front end; stand-in local; retail loads the region manager before it reads the argument
        RegionManager* regions = g_swamp->_regionManager;
        RegionTrigger* region = regions->Find(_curLevel->_areaName);
        if (region) {
            SwampScript* script = region->GetScript();
            if (script)
                script->LeavingRegion(vince);
        }
        if (_curLevel->_script)
            _curLevel->_script->LeavingRegion(vince);
        delete _curLevel;
        _curLevel = 0;
    }
    g_locatorManager->RemoveAll();
    g_particleManager->RemoveAll();
    pManager->DestroyAll();
    g_swamp->_debugGeometry->DestroyAll();
    g_swamp->_regionManager->DestroyAll();
    pSwingHookManager->RemoveAll();
    g_CameraManager->DestroyAll();
    AnimCam::Reset();
    _regionScriptNodeList.clear();
    g_swamp->_scene->DestroyAll();
    g_swamp->_currentCamera = 0;
    delete _hudUI;
    _hudUI = 0;
    g_swamp->_materialManager->ClearFixupEnvmaps();
    g_swamp->_materialManager->_defaultEMap = ResourceHandle<TextureResource>();
}

// FUNCTION: USA 0x00105BE2
// PROTOTYPE: PREVIEW 0x004F05DE identical
// NAME: proto-pdb:default.pdb
void LevelLoader::ClearMasterStats()
{
    _levelStatsList.clear();
}

// FUNCTION: USA 0x00105BED
// PROTOTYPE: PREVIEW 0x004F05E9 identical
// NAME: proto-pdb:default.pdb
LevelStats* LevelLoader::GetLevelStats(const SwampString& levelname)
{
    SwampString levelkey = BeepLib::MakeLower(BeepLib::StripPath(levelname));
    LevelStats* v0 = 0;
    std::vector<LevelStats>::iterator i = 0;
    i = _levelStatsList.begin();
    while (i != _levelStatsList.end()) {
        SwampString name = i->_name;
        if (!name.compare(levelkey)) {
            v0 = &*i;
            break;
        }
        ++i;
    }
    if (i == _levelStatsList.end()) {
        LevelStats newstats;
        newstats._name = levelname;
        _levelStatsList.push_back(newstats);
        v0 = &_levelStatsList.back();
    }
    return v0;
}

// FUNCTION: USA 0x00105CB3
// PROTOTYPE: PREVIEW 0x004F06AF identical
// NAME: proto-pdb:default.pdb
AreaStats* LevelLoader::GetAreaStats(const SwampString& levelname, const SwampString& areaname)
{
    LevelStats* p = GetLevelStats(levelname);
    AreaStats* v0 = 0;
    SwampString areakey = BeepLib::MakeLower(areaname);
    std::vector<AreaStats>::iterator i = 0;
    for (i = p->_areaStatsList.begin(); i != p->_areaStatsList.end(); ++i) {
        if (!i->_name.compare(areakey)) {
            v0 = &*i;
            break;
        }
    }
    if (i == p->_areaStatsList.end()) {
        AreaStats newstats;
        newstats._name = areakey;
        p->_areaStatsList.push_back(newstats);
        v0 = &p->_areaStatsList.back();
    }
    return v0;
}

// FUNCTION: USA 0x00105D47
// PROTOTYPE: PREVIEW 0x004F0743 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1): vector<Slot>::_Buy 0x001030F8 is in no symbol file
// (reloc-code); frees when it is pinned (5.2 T3.c5)
LevelLoader::LevelLoader()
    : _hudUI(0), _curLevel(0), _currentSavegameSlot(-1), _requestPending(false), _savePending(false),
      _loadingFromVoodooShop(false), _newGameFlag(false), _loadScreen(0)
{
}
#endif

// FUNCTION: USA 0x00105FA0
// PROTOTYPE: PREVIEW 0x004F091E identical
// NAME: proto-pdb:default.pdb
void LevelLoader::FlushLocalStats()
{
    Level* p = _curLevel;
    if (!p->_isVoodooShop) {
        SwampString levelname = BeepLib::MakeLower(BeepLib::StripPath(p->_levelName));
        GetLevelStats(levelname);
        SwampString areaname = BeepLib::MakeLower(p->_areaName);
        // !FAKE: c1xx front end; stand-in local; retail calls GetAreaStats before it computes &p->_stats
        AreaStats* v0 = GetAreaStats(levelname, areaname);
        v0->Copy(p->_stats);
    }
}

// FUNCTION: USA 0x00106032
// PROTOTYPE: PREVIEW 0x004F09B0 identical
// NAME: proto-pdb:default.pdb
void LevelLoader::CloneMasterStats()
{
    Level* p = _curLevel;
    if (!p->_isVoodooShop) {
        SwampString name = BeepLib::MakeLower(BeepLib::StripPath(p->_levelName));
        p->_stats.Copy(*GetAreaStats(name, p->_areaName));
    }
}

// FUNCTION: USA 0x001063CA
// PROTOTYPE: PREVIEW 0x004F0D86 identical
// NAME: proto-pdb:default.pdb
void LevelStats::Load(const BeepTDF& tdf)
{
    const BeepTDF* atdf = tdf.FindBlock("Areas", false);
    if (atdf) {
        for (std::vector<BeepTDF*>::const_iterator i = atdf->GetBlocks().begin(); i != atdf->GetBlocks().end(); ++i) {
            SwampString areaname = BeepLib::MakeLower((*i)->GetBlockName());
            if (areaname.compare("jimmysfanboat") == 0) {
                RaceStandings::Load(*i);
            } else {
                AreaStats* stats = LevelLoader::Instance().GetAreaStats(BeepLib::StripPath(_name), areaname);
                stats->_clustersFound = (*i)->GetIntRecord("ClustersFound", 0);
                stats->_clustersRemaining = (*i)->GetIntRecord("ClustersRemaining", 0);
                stats->_jugMax = (*i)->GetIntRecord("JugsMax", 0);
                stats->_jugRemaining = (*i)->GetIntRecord("JugsRemaining", 0);
                stats->_whammysFound = (*i)->GetIntRecord("WhammysFound", 0);
                stats->_whammysRemaining = (*i)->GetIntRecord("WhammysRemaining", 0);
                stats->_pagesFound = (*i)->GetIntRecord("PagesFound", 0);
                stats->_pagesRemaining = (*i)->GetIntRecord("PagesRemaining", 0);
                stats->_juneBugsFound = (*i)->GetIntRecord("JuneBugsFound", 0);
            }
        }
    }
}

// FUNCTION: USA 0x00106631
// PROTOTYPE: PREVIEW 0x004F0FED identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 92.61719, bytes mismatch (masked 1): ours spills this (retail ebx) and caches npos in edi;
// the pair<string,string> dtor callee is folded at 0x00018696 under other names (masked)
void LevelLoader::Slot::DeclareChapterFinished(const SwampString& a, const SwampString& l)
{
    SwampString area;
    SwampString level;
    area = a.empty() ? LevelLoader::Instance()._curLevel->_areaName : a;
    area = BeepLib::MakeLower(area);
    if (l.empty())
        level = BeepLib::StripPath(LevelLoader::Instance()._curLevel->_levelName);
    else
        level = l;
    level = BeepLib::MakeLower(level);
    if (!IsChapterFinished(area))
        _chaptersFinished.push_back(std::make_pair(level, area));
}
#endif

// FUNCTION: USA 0x0010676F
// PROTOTYPE: PREVIEW 0x004F112B identical
// NAME: proto-pdb:default.pdb
void LevelLoader::LoadLevelStats(const BeepTDF& tdf)
{
    for (std::vector<BeepTDF*>::const_iterator i = tdf.GetBlocks().begin(); i != tdf.GetBlocks().end(); ++i) {
        SwampString levelname = BeepLib::MakeLower((*i)->GetBlockName());
        // !FAKE: c1xx front end; stand-in local; retail calls GetLevelStats before it reads *i for the argument
        LevelStats* v0 = LevelLoader::Instance().GetLevelStats(levelname);
        v0->Load(**i);
    }
}

// FUNCTION: USA 0x001076D9
// PROTOTYPE: PREVIEW 0x004F2082 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 4): TrolleyManager::Reset 0x0012E512 pin DISABLED at the
// cut (R8); two retail data words 0x0021EEF8/0x0021EEFC (const char*) in no symbol file, so no placeholder passes
// provenance_lint (not a data start); the draft that scored passed them where this text passes "" (5.2 T3.c5)
void LevelLoader::NewGame()
{
    g_TrolleyManager->Reset();
    _newGameFlag = true;
    InitializeCurrentSave(L"New Game");
    SwampWString title = GenerateSaveGameName(""); // retail: data word 0x0021EEFC
    SwampString savePath;
    _currSave.Save(title.c_str(), savePath);
    RequestArea("", "", false, 0.0f, false); // retail: data words 0x0021EEF8, 0x0021EEFC
    SaveGlobalData(savePath);
}
#endif


// Generated by tools/gen_stl.py (5.2 T3.c4); bodies from the VC 7.1 headers
// FUNCTION: USA 0x001005D6
// PROTOTYPE: PREVIEW 0x004EB13D identical
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<AreaStats,std::allocator<AreaStats> >::capacity()const;

// FUNCTION: USA 0x001005EC
// PROTOTYPE: PREVIEW 0x004EB153 identical
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > >::capacity()const;

// FUNCTION: USA 0x00100602
// PROTOTYPE: PREVIEW 0x004EB169 identical
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > >::size()const;

// FUNCTION: USA 0x00100630
// PROTOTYPE: PREVIEW 0x004EB17F identical
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<LevelStats,std::allocator<LevelStats> >::capacity()const;

// FUNCTION: USA 0x00100646
// PROTOTYPE: PREVIEW 0x004EB195 identical
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<_XGAME_FIND_DATA,std::allocator<_XGAME_FIND_DATA> >::capacity()const;

// FUNCTION: USA 0x0010065E
// PROTOTYPE: PREVIEW 0x004EB1AD identical
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<_XGAME_FIND_DATA,std::allocator<_XGAME_FIND_DATA> >::size()const;

// FUNCTION: USA 0x00100676
// PROTOTYPE: PREVIEW 0x004EB1C5 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,AreaStats,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,AreaStats> >,0> >::_Lrotate(std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,AreaStats,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,AreaStats> >,0> >::_Node *);

// FUNCTION: USA 0x001006B8
// PROTOTYPE: PREVIEW 0x004EB207 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,AreaStats,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,AreaStats> >,0> >::_Rrotate(std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,AreaStats,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,AreaStats> >,0> >::_Node *);

// FUNCTION: USA 0x001006FC
// PROTOTYPE: PREVIEW 0x004EB24B identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::const_iterator::_Inc();

// FUNCTION: USA 0x00100732
// PROTOTYPE: PREVIEW 0x004EB281 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,AreaStats,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,AreaStats> >,0> >::const_iterator::_Inc();

// FUNCTION: USA 0x00100768
// PROTOTYPE: PREVIEW 0x004EB2B7 identical
// NAME: proto-pdb:default.pdb
template void std::fill<_XGAME_FIND_DATA *,_XGAME_FIND_DATA>(_XGAME_FIND_DATA *,_XGAME_FIND_DATA *,_XGAME_FIND_DATA const &);

// FUNCTION: USA 0x0010078F
// PROTOTYPE: PREVIEW 0x004EB2DE identical
// NAME: proto-pdb:default.pdb
template _XGAME_FIND_DATA * std::_Copy_backward_opt<_XGAME_FIND_DATA *,_XGAME_FIND_DATA *>(_XGAME_FIND_DATA *,_XGAME_FIND_DATA *,_XGAME_FIND_DATA *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x001007BE
// PROTOTYPE: PREVIEW 0x004EB30D identical
// NAME: proto-pdb:default.pdb
template void std::_Construct<_XGAME_FIND_DATA,_XGAME_FIND_DATA>(_XGAME_FIND_DATA *,_XGAME_FIND_DATA const &);

// FUNCTION: USA 0x001007D6
// PROTOTYPE: PREVIEW 0x004EB325 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Lrotate(std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node *);

// FUNCTION: USA 0x00100818
// PROTOTYPE: PREVIEW 0x004EB367 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Rrotate(std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node *);

// FUNCTION: USA 0x0010085C
// PROTOTYPE: PREVIEW 0x004EB3AB identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::const_iterator::_Dec();

// FUNCTION: USA 0x001008B5
// PROTOTYPE: PREVIEW 0x004EB404 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional; gate: bytes provisional (EH __ehhandler$/FuncInfo and unnamed callees masked), pin_rows i10-no-row on its callees, relink .text$x interleave refusal (5.2 T3.c4)
template _XGAME_FIND_DATA * std::_Uninit_copy<_XGAME_FIND_DATA *,_XGAME_FIND_DATA *,std::allocator<_XGAME_FIND_DATA> >(_XGAME_FIND_DATA *,_XGAME_FIND_DATA *,_XGAME_FIND_DATA *,std::allocator<_XGAME_FIND_DATA> &,std::_Nonscalar_ptr_iterator_tag);
#endif

// FUNCTION: USA 0x0010094A
// PROTOTYPE: PREVIEW 0x004EB499 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional; gate: bytes provisional (EH __ehhandler$/FuncInfo and unnamed callees masked), pin_rows i10-no-row on its callees, relink .text$x interleave refusal (5.2 T3.c4)
template std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,AreaStats,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,AreaStats> >,0> >::_Node * std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,AreaStats,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,AreaStats> >,0> >::_Buynode();
#endif

// FUNCTION: USA 0x001009D2
// PROTOTYPE: PREVIEW 0x004EB521 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional; gate: bytes provisional (EH __ehhandler$/FuncInfo and unnamed callees masked), pin_rows i10-no-row on its callees, relink .text$x interleave refusal (5.2 T3.c4)
template std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node * std::_Tree<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Buynode();
#endif

// FUNCTION: USA 0x00100AE2
// PROTOTYPE: PREVIEW 0x004EB631 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional; gate: bytes provisional (EH __ehhandler$/FuncInfo and unnamed callees masked), pin_rows i10-no-row on its callees, relink .text$x interleave refusal (5.2 T3.c4)
template void std::_Uninit_fill_n<_XGAME_FIND_DATA *,unsigned int,_XGAME_FIND_DATA,std::allocator<_XGAME_FIND_DATA> >(_XGAME_FIND_DATA *,unsigned int,_XGAME_FIND_DATA const &,std::allocator<_XGAME_FIND_DATA> &,std::_Nonscalar_ptr_iterator_tag);
#endif

// FUNCTION: USA 0x00100B50
// PROTOTYPE: PREVIEW 0x004EB69F identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,AreaStats,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,AreaStats> >,0> >::_Init();

// FUNCTION: USA 0x00100B75
// PROTOTYPE: PREVIEW 0x004EB6C4 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Init();

// FUNCTION: USA 0x00100C45
// PROTOTYPE: PREVIEW 0x004EB794 identical
// NAME: proto-pdb:default.pdb
template _XGAME_FIND_DATA * std::vector<_XGAME_FIND_DATA,std::allocator<_XGAME_FIND_DATA> >::_Ufill(_XGAME_FIND_DATA *,unsigned int,_XGAME_FIND_DATA const &);

// FUNCTION: USA 0x00100C6A
// PROTOTYPE: PREVIEW 0x004EB7B9 identical
// NAME: proto-pdb:default.pdb
template std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,AreaStats>::~pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,AreaStats>();

// FUNCTION: USA 0x00100CC4
// PROTOTYPE: PREVIEW 0x004EB7F2 identical
// NAME: proto-pdb:default.pdb
template std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node::_Node(std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node *,std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node *,std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node *,std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,char);

// FUNCTION: USA 0x0010239A
// PROTOTYPE: PREVIEW 0x004ECE8D identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional; gate: bytes provisional (EH __ehhandler$/FuncInfo and unnamed callees masked), pin_rows i10-no-row on its callees, relink .text$x interleave refusal (5.2 T3.c4)
template std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node * std::_Tree<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Buynode(std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node *,std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node *,std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node *,std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &,char);
#endif

// FUNCTION: USA 0x00102405
// PROTOTYPE: PREVIEW 0x004ECEF8 identical
// NAME: proto-pdb:default.pdb
template std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > std::make_pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> >);

// FUNCTION: USA 0x00102498
// PROTOTYPE: PREVIEW 0x004ECF8B identical
// NAME: proto-pdb:default.pdb
template std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > & std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >::operator=(std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > const &);

// FUNCTION: USA 0x001024C1
// PROTOTYPE: PREVIEW 0x004ECFB4 identical
// NAME: proto-pdb:default.pdb
template AreaStats * std::_Copy_backward_opt<AreaStats *,AreaStats *>(AreaStats *,AreaStats *,AreaStats *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x001024E8
// PROTOTYPE: PREVIEW 0x004ECFDB identical
// NAME: proto-pdb:default.pdb
template std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > * std::_Copy_backward_opt<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *>(std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x001025FF
// PROTOTYPE: PREVIEW 0x004ED0F2 identical
// NAME: proto-pdb:default.pdb
template AreaStats * std::_Copy_opt<AreaStats *,AreaStats *>(AreaStats *,AreaStats *,AreaStats *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x00102623
// PROTOTYPE: PREVIEW 0x004ED116 identical
// NAME: proto-pdb:default.pdb
template std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > * std::_Copy_opt<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *>(std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x00102F60
// PROTOTYPE: PREVIEW 0x004ED9D3 identical
// NAME: proto-pdb:default.pdb
template std::_Tree<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::iterator std::_Tree<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Insert(bool,std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node *,std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &);

// FUNCTION: USA 0x001030B2
// PROTOTYPE: PREVIEW 0x004EDB25 identical
// NAME: proto-pdb:default.pdb
template bool std::vector<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > >::_Buy(unsigned int);

// FUNCTION: USA 0x00103141
// PROTOTYPE: PREVIEW 0x004EDB6B identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional; gate: bytes provisional (EH __ehhandler$/FuncInfo and unnamed callees masked), pin_rows i10-no-row on its callees, relink .text$x interleave refusal (5.2 T3.c4)
template void std::vector<_XGAME_FIND_DATA,std::allocator<_XGAME_FIND_DATA> >::_Insert_n(std::vector<_XGAME_FIND_DATA,std::allocator<_XGAME_FIND_DATA> >::iterator,unsigned int,_XGAME_FIND_DATA const &);
#endif

// FUNCTION: USA 0x00103341
// PROTOTYPE: PREVIEW 0x004EDD6B identical
// NAME: proto-pdb:default.pdb
template std::_Tree<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::iterator std::_Tree<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::erase(std::_Tree<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::iterator);

// FUNCTION: USA 0x001035BA
// PROTOTYPE: PREVIEW 0x004EDFE4 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Erase(std::_Tree_nod<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::_Node *);

// FUNCTION: USA 0x001035F5
// PROTOTYPE: PREVIEW 0x004EE01F identical
// NAME: proto-pdb:default.pdb
template void std::fill<AreaStats *,AreaStats>(AreaStats *,AreaStats *,AreaStats const &);

// FUNCTION: USA 0x0010362C
// PROTOTYPE: PREVIEW 0x004EE056 identical
// NAME: proto-pdb:default.pdb
template void std::fill<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >(std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > const &);

// FUNCTION: USA 0x00103663
// PROTOTYPE: PREVIEW 0x004EE08D identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional; gate: bytes provisional (EH __ehhandler$/FuncInfo and unnamed callees masked), pin_rows i10-no-row on its callees, relink .text$x interleave refusal (5.2 T3.c4)
template void std::_Uninit_fill_n<AreaStats *,unsigned int,AreaStats,std::allocator<AreaStats> >(AreaStats *,unsigned int,AreaStats const &,std::allocator<AreaStats> &,std::_Nonscalar_ptr_iterator_tag);
#endif

// FUNCTION: USA 0x001036CD
// PROTOTYPE: PREVIEW 0x004EE0F7 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional; gate: bytes provisional (EH __ehhandler$/FuncInfo and unnamed callees masked), pin_rows i10-no-row on its callees, relink .text$x interleave refusal (5.2 T3.c4)
template void std::_Uninit_fill_n<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,unsigned int,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > >(std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,unsigned int,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > const &,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > &,std::_Nonscalar_ptr_iterator_tag);
#endif

// FUNCTION: USA 0x001037A5
// PROTOTYPE: PREVIEW 0x004EE1CF identical
// NAME: proto-pdb:default.pdb
template std::pair<std::_Tree<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::iterator,bool> std::_Tree<std::_Tset_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,0> >::insert(std::basic_string<char,std::char_traits<char>,std::allocator<char> > const &);

// FUNCTION: USA 0x00103846
// PROTOTYPE: PREVIEW 0x004EE270 identical
// NAME: proto-pdb:default.pdb
template std::vector<_XGAME_FIND_DATA,std::allocator<_XGAME_FIND_DATA> >::iterator std::vector<_XGAME_FIND_DATA,std::allocator<_XGAME_FIND_DATA> >::insert(std::vector<_XGAME_FIND_DATA,std::allocator<_XGAME_FIND_DATA> >::iterator,_XGAME_FIND_DATA const &);

// FUNCTION: USA 0x0010388D
// PROTOTYPE: PREVIEW 0x004EE2B7 identical
// NAME: proto-pdb:default.pdb
template std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,AreaStats,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,AreaStats> >,0> >::iterator std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,AreaStats,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,AreaStats> >,0> >::erase(std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,AreaStats,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,AreaStats> >,0> >::iterator);

// FUNCTION: USA 0x00103B2C
// PROTOTYPE: PREVIEW 0x004EE556 identical
// NAME: proto-pdb:default.pdb
template void std::_Tree<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,AreaStats,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,AreaStats> >,0> >::_Erase(std::_Tree_nod<std::_Tmap_traits<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,AreaStats,std::less<std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> > const ,AreaStats> >,0> >::_Node *);

// FUNCTION: USA 0x00103D11
// PROTOTYPE: PREVIEW 0x004EE731 identical
// NAME: proto-pdb:default.pdb
template AreaStats * std::vector<AreaStats,std::allocator<AreaStats> >::_Ufill(AreaStats *,unsigned int,AreaStats const &);

// FUNCTION: USA 0x00103D33
// PROTOTYPE: PREVIEW 0x004EE753 identical
// NAME: proto-pdb:default.pdb
template std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > * std::vector<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > >::_Ufill(std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > *,unsigned int,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > const &);

// FUNCTION: USA 0x00103D55
// PROTOTYPE: PREVIEW 0x004EE775 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1) (5.2 T12.c16): the only masked field is its own
// __ehhandler$ funclet VA (mov eax, 0x001532A8), unnamed; needs that name (C0331). 5.2 T12.c1: 99.49 with the
// explicit instantiation: the decl-only _Xlen<AreaStats> (C0314) hides its throw body, so cl emits a jmp after call _Xlen
// VC 7.1 <vector> body verbatim as an explicit specialization: _Xlen<AreaStats> stays decl-only (vsize fix, C0314)
// and cannot be redeclared noreturn (C2381), so `__assume(0)` restates its throw body's effect (C0237, C0325, !FAKE)
template <> void std::vector<AreaStats,std::allocator<AreaStats> >::_Insert_n(iterator _Where,
    size_type _Count, const AreaStats& _Val)
{	// insert _Count * _Val at _Where
    AreaStats _Tmp = _Val;	// in case _Val is in sequence
    size_type _Capacity = capacity();

    if (_Count == 0)
        ;
    else if (max_size() - size() < _Count) {
        _Xlen();	// result too long
        __assume(0); // !FAKE: c2 noreturn; __assume(0) restating noreturn; decl-only _Xlen emits a jmp after the call (99.49)
    } else if (_Capacity < size() + _Count) {	// not enough room, reallocate
        _Capacity = max_size() - _Capacity / 2 < _Capacity
            ? 0 : _Capacity + _Capacity / 2;	// try to grow by 50%
        if (_Capacity < size() + _Count)
            _Capacity = size() + _Count;
        pointer _Newvec = this->_Alval.allocate(_Capacity);
        pointer _Ptr = _Newvec;

        _TRY_BEGIN
        _Ptr = _Ucopy(_Myfirst, _ITER_BASE(_Where),
            _Newvec);	// copy prefix
        _Ptr = _Ufill(_Ptr, _Count, _Tmp);	// add new stuff
        _Ucopy(_ITER_BASE(_Where), _Mylast, _Ptr);	// copy suffix
        _CATCH_ALL
        _Destroy(_Newvec, _Ptr);
        this->_Alval.deallocate(_Newvec, _Capacity);
        _RERAISE;
        _CATCH_END

        _Count += size();
        if (_Myfirst != 0) {	// destroy and deallocate old array
            _Destroy(_Myfirst, _Mylast);
            this->_Alval.deallocate(_Myfirst, _Myend - _Myfirst);
        }
        _Myend = _Newvec + _Capacity;
        _Mylast = _Newvec + _Count;
        _Myfirst = _Newvec;
    } else if ((size_type)(_Mylast - _ITER_BASE(_Where)) < _Count) {	// new stuff spills off end
        _Ucopy(_ITER_BASE(_Where), _Mylast,
            _ITER_BASE(_Where) + _Count);	// copy suffix

        _TRY_BEGIN
        _Ufill(_Mylast, _Count - (_Mylast - _ITER_BASE(_Where)),
            _Tmp);	// insert new stuff off end
        _CATCH_ALL
        _Destroy(_ITER_BASE(_Where) + _Count, _Mylast + _Count);
        _RERAISE;
        _CATCH_END

        _Mylast += _Count;
        fill(_ITER_BASE(_Where), _Mylast - _Count,
            _Tmp);	// insert up to old end
    } else {	// new stuff can all be assigned
        pointer _Oldend = _Mylast;
        _Mylast = _Ucopy(_Oldend - _Count, _Oldend,
            _Mylast);	// copy suffix
        copy_backward(_ITER_BASE(_Where), _Oldend - _Count,
            _Oldend);	// copy hole
        fill(_ITER_BASE(_Where), _ITER_BASE(_Where) + _Count,
            _Tmp);	// insert into hole
    }
}
#endif

// FUNCTION: USA 0x00103FC8
// PROTOTYPE: PREVIEW 0x004EE9E8 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional; gate: bytes provisional (EH __ehhandler$/FuncInfo and unnamed callees masked), pin_rows i10-no-row on its callees, relink .text$x interleave refusal (5.2 T3.c4)
template void std::vector<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > >::_Insert_n(std::vector<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > >::iterator,unsigned int,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > const &);
#endif

// FUNCTION: USA 0x0010433D
// PROTOTYPE: PREVIEW 0x004EED5D identical
// NAME: proto-pdb:default.pdb
template std::vector<AreaStats,std::allocator<AreaStats> >::iterator std::vector<AreaStats,std::allocator<AreaStats> >::insert(std::vector<AreaStats,std::allocator<AreaStats> >::iterator,AreaStats const &);

// FUNCTION: USA 0x001043AA
// PROTOTYPE: PREVIEW 0x004EEDCA identical
// NAME: proto-pdb:default.pdb
template std::vector<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > >::iterator std::vector<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > >::insert(std::vector<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > >::iterator,std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > const &);

// FUNCTION: USA 0x001044A9
// PROTOTYPE: PREVIEW 0x004EEEB6 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (5.2 T12.c1); unguarded, gate relink refuses "our ranges overlap at 0x000E84B1": its _Buy<AreaStats> call emits a second copy of HUDUI's _Buy (C0314)
template std::vector<AreaStats,std::allocator<AreaStats> > & std::vector<AreaStats,std::allocator<AreaStats> >::operator=(std::vector<AreaStats,std::allocator<AreaStats> > const &);
#endif

// FUNCTION: USA 0x001045A6
// PROTOTYPE: PREVIEW 0x004EEFB3 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (5.2 T12.c1); unguarded, gate relink refuses "our ranges overlap at 0x000E84B1": its _Buy<AreaStats> call emits a second copy of HUDUI's _Buy (C0314)
template std::vector<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > > & std::vector<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > >::operator=(std::vector<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > >,std::allocator<std::pair<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > > const &);
#endif
