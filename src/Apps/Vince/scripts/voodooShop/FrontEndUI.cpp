// UNIT: src/Apps/Vince/scripts/voodooShop/FrontEndUI.cpp OBJ: Xbox\OfficialRelease\FrontEndUI.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

struct AABox;                          // NAME: proto-pdb:default.pdb
struct Vector3;                        // NAME: proto-pdb:default.pdb
struct Quaternion;                     // NAME: proto-pdb:default.pdb
struct Matrix;                         // NAME: proto-pdb:default.pdb

namespace std { // NAME: proto-pdb:default.pdb
template <class T> class allocator {
public:
	void deallocate(T *_Ptr, unsigned int)
		{	// deallocate object at _Ptr, ignore size
		operator delete(_Ptr);
		}
};
template<class _Ty,
	class _Alloc>
	void _Destroy_range(_Ty *_First, _Ty *_Last, _Alloc& _Al); // retail calls it out of line (0x0001A5FD)
template<class _InIt,
	class _FwdIt,
	class _Alloc>
	_FwdIt _Uninitialized_copy(_InIt _First, _InIt _Last, _FwdIt _Dest,
		_Alloc& _Al); // retail calls it out of line (0x00030B9B)
template<class _InIt,
	class _OutIt>
	_OutIt copy(_InIt _First, _InIt _Last, _OutIt _Dest); // retail calls it out of line (0x00035B4A)
template <class E> struct char_traits;
template <class E, class T = char_traits<E>, class A = allocator<E> > class basic_string {
public:
    basic_string() { _Myres = 15; _Eos(0); }
    basic_string(const basic_string& _Right);
    basic_string(const E* _Ptr);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, (unsigned int)-1); }
    basic_string& operator=(const E* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    basic_string& assign(const E* _Ptr);
    int compare(const basic_string& _Right) const;
    unsigned int size() const { return _Mysize; }

protected:
    void _Eos(unsigned int _Newsize);
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    unsigned int _pad00[5];            // 0x00
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class E, class T, class A>
basic_string<E, T, A> operator+(const E* _Left, const basic_string<E, T, A>& _Right);
template <class E, class T, class A>
inline bool operator!=(const basic_string<E, T, A>& _Left, const basic_string<E, T, A>& _Right)
{
    return _Left.compare(_Right) != 0;
}
template <class T1, class T2> struct pair {
    T1 first;                          // 0x00
    T2 second;
};
template <class T, class A = allocator<T> > class vector {
public:
    class iterator {
    public:
        iterator(T* _Ptr) : _Myptr(_Ptr) {}
        T& operator*() const { return *_Myptr; }
        T* operator->() const { return _Myptr; }
        iterator& operator++() { ++_Myptr; return *this; }
        iterator operator+(int _Off) const { iterator _Tmp = *this; return (_Tmp += _Off); }
        iterator& operator+=(int _Off) { _Myptr += _Off; return (*this); }
        bool operator!=(const iterator& _Right) const { return _Myptr != _Right._Myptr; }
        T* _Myptr;
    };
    vector() : _Myfirst(0), _Mylast(0), _Myend(0) {}
    vector(const vector& _Right);
    ~vector() { _Tidy(); }
    iterator begin() { return iterator(_Myfirst); }
    iterator end() { return iterator(_Mylast); }
    unsigned int size() const;
    unsigned int capacity() const;
    T& operator[](unsigned int _Pos) { return (*(begin() + _Pos)); }
    void push_back(const T& _Val);
    void clear() { _Tidy(); }
	vector& operator=(const vector& _Right)
		{	// assign _Right
		if (this != &_Right)
			{	// worth doing


			if (_Right.size() == 0)
				clear();	// new sequence empty, free storage
			else if (_Right.size() <= size())
				{	// enough elements, copy new and destroy old
				T *_Ptr = copy(_Right._Myfirst, _Right._Mylast,
					_Myfirst);	// copy new
				_Destroy(_Ptr, _Mylast);	// destroy old
				_Mylast = _Myfirst + _Right.size();
				}
			else if (_Right.size() <= capacity())
				{	// enough room, copy and construct new
				T *_Ptr = _Right._Myfirst + size();
				copy(_Right._Myfirst, _Ptr, _Myfirst);
				_Mylast = _Ucopy(_Ptr, _Right._Mylast, _Mylast);
				}
			else
				{	// not enough room, allocate new array and construct new
				if (_Myfirst != 0)
					{	// discard old array
					_Destroy(_Myfirst, _Mylast);
					this->_Alval.deallocate(_Myfirst, _Myend - _Myfirst);
					}
				if (_Buy(_Right.size()))
					_Mylast = _Ucopy(_Right._Myfirst, _Right._Mylast,
						_Myfirst);
				}
			}
		return (*this);
		}

protected:
    bool _Buy(unsigned int _Capacity);
	void _Destroy(T *_First, T *_Last)
		{	// destroy [_First, _Last) using allocator
		_Destroy_range(_First, _Last, this->_Alval);
		}
    void _Tidy();
	template<class _Iter>
		T *_Ucopy(_Iter _First, _Iter _Last, T *_Ptr)
		{	// copy initializing [_First, _Last), using allocator
		return (_Uninitialized_copy(_First, _Last,
			_Ptr, this->_Alval));
		}

private:
    A _Alval;                          // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
}

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                             // vtable+0x00
    virtual const AABox& GetBounds() const;                // vtable+0x04
    virtual Vector3 GetWorldPosition() const;              // vtable+0x08
    virtual Quaternion GetWorldRotation() const;           // vtable+0x0C
    virtual void SetWorldPosition(const Vector3&);         // vtable+0x10
    virtual void Invalidate();                             // vtable+0x14
protected:
    virtual void UpdateAnimation(unsigned int);            // vtable+0x18
    virtual void Update();                                 // vtable+0x1C
    virtual void Render(const Matrix&) const;              // vtable+0x20
public:

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class FrontEndMenu : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    FrontEndMenu();
    virtual ~FrontEndMenu();
    virtual void Update();
    virtual void Render(const Matrix&) const;
    virtual void Invalidate();

    bool _valid;                       // 0xA4
    bool _captionHidden;               // 0xA5
    bool _ctrlMissing;                 // 0xA6
    char field_A7[0xB0 - 0xA7];        // 0xA7
    std::basic_string<unsigned short> _wLevelName; // 0xB0
    std::basic_string<unsigned short> _wAreaName; // 0xCC
    char field_E8[0xFC - 0xE8];        // 0xE8
};
DECOMP_SIZE_ASSERT(FrontEndMenu, 0xFC);

class FrontEndUI : public FrontEndMenu { // NAME: proto-pdb:default.pdb
public:
    FrontEndUI();
    // inline: retail ??_GFrontEndUI 0x0013E262 stores the vptr, calls EndMenu, then ??1FrontEndMenu; no ??1FrontEndUI public
    virtual ~FrontEndUI() { EndMenu(); }
    void EndMenu();
    virtual void Update();
    virtual void Render(const Matrix&) const;

    FrontEndMenu* _menu;               // 0xFC
};
DECOMP_SIZE_ASSERT(FrontEndUI, 0x100);

class SwampInput { // NAME: proto-pdb:default.pdb
public:
    void SetJpadPipedToDpad(bool v1) { _pipeJpadToDpad = v1; }
    bool IsPadPresent(int) const;
    float GetElapsedTime() const { return _elapsed; }

    float _elapsed;                    // 0x00
    char field_4[0x3A - 0x4];          // 0x04
    bool _pipeJpadToDpad;              // 0x3A
    char field_3B[0x44 - 0x3B];        // 0x3B
    int _activePad[4];                 // 0x44
    char field_54[0x120 - 0x54];       // 0x54
};
DECOMP_SIZE_ASSERT(SwampInput, 0x120);

class SoundManager { // NAME: proto-pdb:default.pdb
public:
    void StopAllSounds();
    void StopAllStreams();
    void SetSoundVolume(float v);
    void SetMusicVolume(float v);
    void SetCinematicVolume(float v);
};

class Swamp { // NAME: proto-pdb:default.pdb
public:
    char field_0[0x4];                 // 0x00
    SwampInput* _input;                // 0x04
    char field_8[0x34 - 0x8];          // 0x08
    SoundManager* _soundManager;       // 0x34
    char field_38[0x68 - 0x38];        // 0x38
};
DECOMP_SIZE_ASSERT(Swamp, 0x68);

extern Swamp* g_swamp; // NAME: proto-pdb:default.pdb

class HUDUI { // NAME: proto-pdb:default.pdb
public:
    static HUDUI& Instance() { return *_self; }
    void DisplayMissingControllerMessageThisFrame();

private:
    static HUDUI* _self;
};

typedef struct _LAUNCH_DATA { // NAME: proto-pdb:default.pdb
    unsigned char Data[0xC00]; // 0x000
} LAUNCH_DATA;
DECOMP_SIZE_ASSERT(LAUNCH_DATA, 0xC00);

extern "C" unsigned long __stdcall XLaunchNewImageA(const char*, LAUNCH_DATA*); // NAME: proto-pdb:default.pdb
extern "C" void* __cdecl memset(void*, int, unsigned int); // NAME: proto-pdb:default.pdb
#pragma intrinsic(memset)

// FUNCTION: USA 0x0013D44D
// PROTOTYPE: PREVIEW 0x00525DBD identical
// NAME: proto-pdb:default.pdb
void FrontEndMenu::Update()
{
    if (!g_swamp->_input->IsPadPresent(0)) {
        if (g_swamp->_input->_activePad[0] > 0)
            _ctrlMissing = true;
    } else if (_ctrlMissing) {
        _ctrlMissing = false;
    }
    if (_ctrlMissing)
        HUDUI::Instance().DisplayMissingControllerMessageThisFrame();
}

typedef std::basic_string<char> SwampString;
typedef std::basic_string<unsigned short> SwampWString;

namespace BeepLib { // NAME: proto-pdb:default.pdb
SwampString StripPath(const SwampString& s);
SwampWString MakeWide(const SwampString& s);
}

class LocalizedStrings { // NAME: proto-pdb:default.pdb
public:
    static LocalizedStrings& Instance();
    const unsigned short* GetGameString(const SwampString&);
};

struct Vector2 { // NAME: proto-pdb:default.pdb
    Vector2(float u_, float v_) : u(u_), v(v_) {}

    float u;                           // 0x00
    float v;                           // 0x04
};
DECOMP_SIZE_ASSERT(Vector2, 0x8);

class TextureResource; // NAME: proto-pdb:default.pdb

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ResourceHandle& operator=(const ResourceHandle& h);
    bool IsValid() const { return _Resource != 0; }

    T* _Resource;                      // 0x00
};

class Sprite { // NAME: proto-pdb:default.pdb
public:
    void SetTexture(const SwampString&);
    void SetScale(const Vector2& scale) { _scale = scale; }
    void SetAlpha(float alpha) { _alpha = alpha; }
    bool IsValid() const { return _texture.IsValid(); }

    char field_0[0x10];                // 0x00
    Vector2 _scale;                    // 0x10
    ResourceHandle<TextureResource> _texture; // 0x18
    char field_1C[0x20 - 0x1C];        // 0x1C
    float _alpha;                      // 0x20
    char field_24[0x4C - 0x24];        // 0x24
};
DECOMP_SIZE_ASSERT(Sprite, 0x4C);

// Retail layout: LevelLoader::Slot is 0xA4 in retail (imul 0xA4; proto PDB 0x8C); _chaptersFinished
// stays at 0x44, the grown span is opaque.
class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    class Slot { // NAME: proto-pdb:default.pdb
    public:
        char field_0[0x44];            // 0x00
        std::vector<std::pair<SwampString, SwampString> > _chaptersFinished; // 0x44
        char field_54[0xA4 - 0x54];    // 0x54
    };

    static LevelLoader& Instance();
    Slot& GetSlot(int slot) { return _slots[slot]; }

    char field_0[0x4C];                // 0x00
    std::vector<Slot> _slots;          // 0x4C
};

class ChaptersMenu : public FrontEndMenu { // NAME: proto-pdb:default.pdb
public:
    // FUNCTION: USA 0x0013DEFD
    // NAME: proto-pdb:default.pdb
    class LevelEntry { // NAME: proto-pdb:default.pdb
    public:
        LevelEntry();
        ~LevelEntry();

        SwampString _name;             // 0x00
        std::vector<SwampString> _areas; // 0x1C
    };

private:
    void NextChapter();
    void PrevChapter();
    void BuildChapterList();
    void UpdatePic();

    int _slotNum;                      // 0xFC
    Sprite _oldPic;                    // 0x100
    Sprite _newPic;                    // 0x14C
    char field_198[0x238 - 0x198];     // 0x198
    int _curLevel;                     // 0x238
    int _curArea;                      // 0x23C
    std::vector<LevelEntry> _chapterList; // 0x240
    int _totalChapters;                // 0x250
};
DECOMP_SIZE_ASSERT(ChaptersMenu, 0x254);

// !FAKE: linker COMDAT folding; VC 7.1 vector::size body verbatim for this alias only (C0233 Fix 1); the COMDAT lands at the fold VA
// 0x000D39FF and relink merges it into the SwampDebugGeometry vector<LevelStats>::size row
namespace std {
template <> inline unsigned int vector<ChaptersMenu::LevelEntry>::size() const
{
    return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst);
}
}

// FUNCTION: USA 0x0013D49D
// NAME: proto-pdb:default.pdb
void ChaptersMenu::NextChapter()
{
    int level = ++_curLevel;
    if (level == _chapterList.size())
        _curLevel = 0;
    _curArea = 0;
}

// FUNCTION: USA 0x0013D4C9
// NAME: proto-pdb:default.pdb
void ChaptersMenu::PrevChapter()
{
    if (_curLevel == 0)
        _curLevel = _chapterList.size();
    _curArea = 0;
    --_curLevel;
}

// FUNCTION: USA 0x0013D4F1
// PROTOTYPE: PREVIEW 0x00525E61 identical
// NAME: proto-pdb:default.pdb
void FrontEndUI::Update()
{
    if (!_menu) {
        FrontEndMenu::Update();
    }
}

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampScript();                                        // vtable+0x00
    virtual bool TestActivate() const;                             // vtable+0x04
    virtual void Activate();                                       // vtable+0x08
    virtual void Deactivate();                                     // vtable+0x0C
    virtual void FrameTick();                                      // vtable+0x10
    virtual void TimerEvent(unsigned int);                         // vtable+0x14
    void UnRegisterFrameTick();

    char field_4[0x4C - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class DemoLauncher : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    enum Timers { // NAME: proto-pdb:default.pdb
        TIMER_FADE = 0x0,
        TIMER_STOPSTREAMS = 0x1
    };
    virtual void TimerEvent(unsigned int timerId);
    virtual void FrameTick();

    char field_4C[0x50 - 0x4C];        // 0x4C
    float _sound;                      // 0x50
    float _music;                      // 0x54
    float _cutscene;                   // 0x58
    float _deltaSound;                 // 0x5C
    float _deltaMusic;                 // 0x60
    float _deltaCutscene;              // 0x64
};
DECOMP_SIZE_ASSERT(DemoLauncher, 0x68);

// FUNCTION: USA 0x0013D5ED
// PROTOTYPE: PREVIEW 0x00525F51 identical
// NAME: proto-pdb:default.pdb
void DemoLauncher::FrameTick()
{
    _sound -= g_swamp->_input->GetElapsedTime() * _deltaSound;
    _music -= g_swamp->_input->GetElapsedTime() * _deltaMusic;
    _cutscene -= g_swamp->_input->GetElapsedTime() * _deltaCutscene;
    g_swamp->_soundManager->SetSoundVolume(0.0f > _sound ? 0.0f : _sound);
    g_swamp->_soundManager->SetMusicVolume(0.0f > _music ? 0.0f : _music);
    g_swamp->_soundManager->SetCinematicVolume(0.0f > _cutscene ? 0.0f : _cutscene);
}

// FUNCTION: USA 0x0013D697
// PROTOTYPE: PREVIEW 0x00525FFB identical
// NAME: proto-pdb:default.pdb
void DemoLauncher::TimerEvent(unsigned int timerId)
{
    if (timerId == TIMER_FADE) {
        g_swamp->_soundManager->StopAllSounds();
        g_swamp->_soundManager->StopAllStreams();
        UnRegisterFrameTick();
        LAUNCH_DATA data;
        memset(&data, 0, sizeof(data));
        XLaunchNewImageA("d:\\cdx\\cdx.xbe", &data);
    }
}

// FUNCTION: USA 0x0013D92A
// PROTOTYPE: PREVIEW 0x00526282 identical
// NAME: proto-pdb:default.pdb
void FrontEndUI::Render(const Matrix& m) const
{
    if (_menu)
        _menu->Render(m);
    else
        FrontEndMenu::Render(m);
}

// FUNCTION: USA 0x0013DD41
// PROTOTYPE: PREVIEW 0x00526699 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 98.59, bytes mismatch (entry ptr in edi, curArea in ebx; retail swaps them)
void ChaptersMenu::UpdatePic()
{
    _oldPic = _newPic;
    _oldPic.SetAlpha(1.0f);
    SwampString levelname = BeepLib::StripPath(_chapterList[_curLevel]._name);
    SwampString areaname;
    // !FAKE: c2 register allocation; stand-in local; retail keeps the entry in ebx across size() (match_one 94.33 -> 98.59)
    LevelEntry* p = &_chapterList[_curLevel];
    if (_curArea < p->_areas.size())
        areaname = p->_areas[_curArea];
    _newPic.SetTexture("savescreen_" + areaname);
    if (!_newPic.IsValid())
        _newPic.SetTexture("missing");
    _newPic.SetScale(Vector2(2.0f, 1.5f));
    _newPic.SetAlpha(0.0f);
    _wLevelName = LocalizedStrings::Instance().GetGameString(levelname);
    if (_wLevelName.size() == 0)
        _wLevelName = BeepLib::MakeWide(levelname);
    _wAreaName = LocalizedStrings::Instance().GetGameString(areaname);
    if (_wAreaName.size() == 0)
        _wAreaName = BeepLib::MakeWide(areaname);
}
#endif

inline void *__cdecl operator new(unsigned int, void *_Where)
{
    return (_Where);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??0LevelEntry@ChaptersMenu@@QAE@ABV01@@Z through
// a caller in this unit (not matched yet); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_LevelEntry_copy(void* p, const ChaptersMenu::LevelEntry& src)
{
    new (p) ChaptersMenu::LevelEntry(src);
}

// FUNCTION: USA 0x0013DF1E
// PROTOTYPE: PREVIEW 0x00526876 identical
// NAME: proto-pdb:default.pdb
template std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > & std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >::operator=(std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > const &);

// FUNCTION: USA 0x0013E055
// PROTOTYPE: PREVIEW 0x005269AD identical
// NAME: proto-pdb:default.pdb
void FrontEndMenu::Invalidate()
{
    _valid = false;
}

// ??_GFrontEndMenu 0x0013E05D, ??_GExtrasMenu 0x0013E46E, ??_GChaptersMenu 0x0013EC13 unmatched, no draft (5.3 T6.c11):
// companion wall: a virtual ??_G is emitted only with its class vtable; ??0/??1 of FrontEndMenu (0x0013E079/0x0013E01B),
// ExtrasMenu (0x0013E119/0x0013E1F9), ChaptersMenu (0x0013F0F1/0x0013EB82) are not passing rows. Lift: match a ctor or dtor.

class MoviePlayer;                      // NAME: proto-pdb:default.pdb

class AttractMode : public FrontEndMenu { // NAME: proto-pdb:default.pdb
public:
    virtual ~AttractMode();
    virtual void Update();
    virtual void Render(const Matrix&) const;

    bool _stopping;                    // 0xFC
    bool _stopped;                     // 0xFD
    char field_FE[0x100 - 0xFE];       // 0xFE
    MoviePlayer* _player;              // 0x100
};
DECOMP_SIZE_ASSERT(AttractMode, 0x104);

// FUNCTION: USA 0x0013E10E
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1: vptr store ??_7AttractMode 0x001FB1D8 unnamed; code bytes equal)
AttractMode::~AttractMode()
{
}
#endif

// FUNCTION: USA 0x0013E21E
// NAME: proto-pdb:default.pdb
ChaptersMenu::LevelEntry::~LevelEntry()
{
}

// FUNCTION: USA 0x0013E236
// PROTOTYPE: PREVIEW 0x00526B8E identical
// NAME: proto-pdb:default.pdb
FrontEndUI::FrontEndUI()
    : _menu(0)
{
    _captionHidden = false;
    g_swamp->_input->SetJpadPipedToDpad(true);
}

// callees/literals pinned 5.4 T25.c1
// FUNCTION: USA 0x0013EC2F
// PROTOTYPE: PREVIEW 0x0052754D near-identical
// NAME: proto-pdb:default.pdb
void ChaptersMenu::BuildChapterList()
{
    _totalChapters = 0;
    std::vector<std::pair<SwampString, SwampString> >& chapters = LevelLoader::Instance().GetSlot(_slotNum)._chaptersFinished;
    if (chapters.size() == 0)
        return;

    LevelEntry le;
    le._name = chapters.begin()->first;
    _chapterList.clear();
    for (std::vector<std::pair<SwampString, SwampString> >::iterator it = chapters.begin(); it != chapters.end(); ++it) {
        if (le._name != it->first) {
            _chapterList.push_back(le);
            le._name = it->first;
            le._areas.clear();
        }
        le._areas.push_back(it->second);
        ++_totalChapters;
    }
    _chapterList.push_back(le);
}
