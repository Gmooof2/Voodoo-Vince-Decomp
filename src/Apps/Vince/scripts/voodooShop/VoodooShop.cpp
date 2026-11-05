// UNIT: src/Apps/Vince/scripts/voodooShop/VoodooShop.cpp OBJ: Xbox\OfficialRelease\VoodooShop.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

// Class records: VoodooShopLevelMain, LevelScript, LevelScript::ProgressHint and AnimCam have two PDB records each;
// the later ones (0x98, 0x64, ti 0x1408f, 0x128) match retail's offsets (_frontEndUI 0x88, _errorState 0x8C).

inline void* operator new(unsigned int, void* _Where) { return (_Where); }

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T = char_traits<E>, class A = allocator<E> > class basic_string {
public:
    basic_string();
    basic_string(const E* _Ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    unsigned int _pad00[5];            // 0x00
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class E, class T, class A>
basic_string<E, T, A> operator+(const E* _Left, const basic_string<E, T, A>& _Right); // retail out of line (0x000223D6)
template <class T> inline void _Destroy(T* _Ptr) { (_Ptr)->~T(); }
template <class T> class allocator { // VC 7.1 xmemory, the members the rows use
public:
    typedef T* pointer;
    typedef unsigned int size_type;
    typedef int difference_type;
    void destroy(pointer _Ptr) { _Destroy(_Ptr); }
};
struct _Nonscalar_ptr_iterator_tag {};
template <class _Ty, class _Alloc> inline void _Destroy_range(_Ty* _First, _Ty* _Last, _Alloc& _Al, _Nonscalar_ptr_iterator_tag)
{
    for (; _First != _Last; ++_First)
        _Al.destroy(_First);
}
template <class _FwdIt, class _Ty> inline void fill(_FwdIt _First, _FwdIt _Last, const _Ty& _Val)
{
    for (; _First != _Last; ++_First)
        *_First = _Val;
}
template <class _BidIt1, class _BidIt2> inline _BidIt2 _Copy_backward_opt(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest, _Nonscalar_ptr_iterator_tag)
{
    while (_First != _Last)
        *--_Dest = *--_Last;
    return (_Dest);
}
template <class _FwdIt, class _Diff, class _Tval, class _Alloc>
void _Uninitialized_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al); // retail calls it out of line (0x0013A09C)
template <class _Ty, class _Ax = allocator<_Ty> > class vector {
public:
    typedef typename _Ax::size_type size_type;
    typedef typename _Ax::difference_type difference_type;
    typedef typename _Ax::pointer pointer;

    class const_iterator {
    public:
        const_iterator(pointer _Ptr) { _Myptr = _Ptr; }
        difference_type operator-(const const_iterator& _Right) const { return (_Myptr - _Right._Myptr); }
        pointer _Myptr;
    };
    class iterator : public const_iterator {
    public:
        iterator(pointer _Ptr) : const_iterator(_Ptr) {}
        _Ty& operator*() const { return (*this->_Myptr); }
        iterator& operator+=(difference_type _Off) { this->_Myptr += _Off; return (*this); }
        iterator operator+(difference_type _Off) const
        {
            iterator _Tmp = *this;
            return (_Tmp += _Off);
        }
        difference_type operator-(const const_iterator& _Right) const { return ((const_iterator)*this - _Right); }
    };

    ~vector() { _Tidy(); }
    iterator begin() { return (iterator(_Myfirst)); }
    size_type size() const
    {
        return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst);
    }
    _Ty& operator[](size_type _Pos) { return (*(begin() + _Pos)); }
    iterator insert(iterator _Where, const _Ty& _Val)
    {
        size_type _Off = size() == 0 ? 0 : _Where - begin();
        _Insert_n(_Where, (size_type)1, _Val);
        return (begin() + _Off);
    }

protected:
    void _Insert_n(iterator _Where, size_type _Count, const _Ty& _Val);
    void _Tidy();
    pointer _Ufill(pointer _Ptr, size_type _Count, const _Ty& _Val)
    {
        _Uninitialized_fill_n(_Ptr, _Count, _Val, this->_Alval);
        return (_Ptr + _Count);
    }

    _Ax _Alval;                        // 0x00
    pointer _Myfirst;                  // 0x04
    pointer _Mylast;                   // 0x08
    pointer _Myend;                    // 0x0C
};
}

typedef std::basic_string<char> SwampString;
typedef std::basic_string<unsigned short> SwampWString;

struct _FILETIME { // NAME: proto-pdb:default.pdb
    unsigned long dwLowDateTime;       // 0x00
    unsigned long dwHighDateTime;      // 0x04
};
DECOMP_SIZE_ASSERT(_FILETIME, 0x8);

class SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampSceneNode();                             // vtable+0x00

    char field_4[0xA4 - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampSceneNode, 0xA4);

class FrontEndMenu : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    virtual ~FrontEndMenu();
    void SetCaption(const unsigned short*, unsigned int);

    char field_A4[0xFC - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(FrontEndMenu, 0xFC);

class FrontEndUI : public FrontEndMenu { // NAME: proto-pdb:default.pdb
public:
    void EndMenu();

    FrontEndMenu* _menu;               // 0xFC
};
DECOMP_SIZE_ASSERT(FrontEndUI, 0x100);

// FUNCTION: USA 0x001385A4
// PROTOTYPE: PREVIEW 0x005227F7 identical
// NAME: proto-pdb:default.pdb
void FrontEndUI::EndMenu()
{
    if (_menu) {
        delete _menu;
        _menu = 0;
    }
}

class ResourceBase { // NAME: proto-pdb:default.pdb
public:
    void SubRef() { --_RefCount; }

    char field_0[0x1C];                // 0x00
    unsigned int _RefCount;            // 0x1C
};

class TextureResource : public ResourceBase { // NAME: proto-pdb:default.pdb
};

template <class T> class ResourceHandle { // NAME: proto-pdb:default.pdb
public:
    ~ResourceHandle() { if (_Resource) _Resource->SubRef(); }

    T* _Resource;                      // 0x00
};

class Sprite { // NAME: proto-pdb:default.pdb
public:
    void SetTexture(const SwampString&);

    char field_0[0x18];                // 0x00
    ResourceHandle<TextureResource> _texture; // 0x18
    char field_1C[0x4C - 0x1C];        // 0x1C
};
DECOMP_SIZE_ASSERT(Sprite, 0x4C);

class UINode : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0xD4 - 0xA4];        // 0xA4
};
DECOMP_SIZE_ASSERT(UINode, 0xD4);

class ChaptersMenu; // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x0013B7D3
// PROTOTYPE: PREVIEW 0x005254FB identical
// NAME: proto-pdb:default.pdb
class SavedGamesScreen : public UINode { // NAME: proto-pdb:default.pdb
public:
    // FUNCTION: USA 0x00138D27
    // PROTOTYPE: PREVIEW 0x00522DB7 identical
    // NAME: proto-pdb:default.pdb
    // FUNCTION: USA 0x00138D43
    // PROTOTYPE: PREVIEW 0x00522DD3 identical
    // NAME: proto-pdb:default.pdb
    // FUNCTION: USA 0x00139AB1
    // PROTOTYPE: PREVIEW 0x0052398E identical
    // NAME: proto-pdb:default.pdb
    class SavedGameInfo { // NAME: proto-pdb:default.pdb
    public:
        SwampString _area;             // 0x00
        SwampString _level;            // 0x1C
        float _gameTime;               // 0x38
        _FILETIME _timeOfDay;          // 0x3C
        SwampWString _gameName;        // 0x44
    };

    char field_D4[0xE4 - 0xD4];        // 0xD4
private:
    void UpdatePics();
public:

    Sprite _pictures[3];              // 0xE4
    bool _updatePics;                  // 0x1C8
    int _topSel;                       // 0x1CC
    int _lastSel;                      // 0x1D0
    Sprite _selectBar;                // 0x1D4
    Sprite _highlight;                 // 0x220
    char field_26C[0x27C - 0x26C];     // 0x26C
    Sprite _subSelectBar;              // 0x27C
    Sprite _upArrow;                   // 0x2C8
    Sprite _downArrow;                 // 0x314
    ChaptersMenu* _chaptersMenu;       // 0x360
    std::vector<SavedGameInfo> _games; // 0x364
};
DECOMP_SIZE_ASSERT(SavedGamesScreen::SavedGameInfo, 0x60);
DECOMP_SIZE_ASSERT(SavedGamesScreen, 0x374);

// FUNCTION: USA 0x00138800
// PROTOTYPE: PREVIEW 0x00522A51 identical
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<SavedGamesScreen::SavedGameInfo,std::allocator<SavedGamesScreen::SavedGameInfo> >::size()const;

class SwampScript { // NAME: proto-pdb:default.pdb
public:
    virtual ~SwampScript();                                // vtable+0x00

    char field_4[0x4C - 0x4];          // 0x04
};
DECOMP_SIZE_ASSERT(SwampScript, 0x4C);

class LevelScript : public SwampScript { // NAME: proto-pdb:default.pdb
public:
    struct ProgressHint { // NAME: proto-pdb:default.pdb
        SwampString _name;             // 0x00
        unsigned int _time;            // 0x1C
    };

    char field_4C[0x54 - 0x4C];        // 0x4C
    std::vector<ProgressHint> _progressHints; // 0x54
};
DECOMP_SIZE_ASSERT(LevelScript::ProgressHint, 0x20);
DECOMP_SIZE_ASSERT(LevelScript, 0x64);

// FUNCTION: USA 0x00138829
// PROTOTYPE: PREVIEW 0x00522A7A identical
// NAME: proto-pdb:default.pdb
template unsigned int std::vector<LevelScript::ProgressHint,std::allocator<LevelScript::ProgressHint> >::size()const;

// FUNCTION: USA 0x00138C71
// PROTOTYPE: PREVIEW 0x00522DAA identical
// NAME: proto-pdb:default.pdb
struct Transition { // NAME: proto-pdb:default.pdb
    int stateNext;                     // 0x00
    SwampString camNext;               // 0x04
    bool reverseCamera;                // 0x20
};
DECOMP_SIZE_ASSERT(Transition, 0x24);

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ~Transition in this unit (no matched function
// odr-uses it); this qualified call odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_Transition_dtor(Transition* p)
{
    p->Transition::~Transition();
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit SavedGameInfo default ctor in this unit (no
// matched function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_SavedGameInfo_ctor(void* p)
{
    new (p) SavedGamesScreen::SavedGameInfo;
}

// The compiler generates the SavedGameInfo copy constructor here; it has no body in source.
// FUNCTION: USA 0x00138F64
// PROTOTYPE: PREVIEW 0x00522F62 identical
// NAME: proto-pdb:default.pdb
// !FAKE: c1xx front end; scaffold call; retail emits the implicit SavedGameInfo copy ctor in this unit (no
// matched function odr-uses it); this placement new odr-uses it instead (matched_set [[emitter]] row)
void pin_emit_SavedGameInfo_copy(void* p, const SavedGamesScreen::SavedGameInfo& v)
{
    new (p) SavedGamesScreen::SavedGameInfo(v);
}

class SwampCamera : public SwampSceneNode { // NAME: proto-pdb:default.pdb
public:
    char field_A4[0xC4 - 0xA4];        // 0xA4
    float _speed;                      // 0xC4
    char field_C8[0x10C - 0xC8];       // 0xC8
};
DECOMP_SIZE_ASSERT(SwampCamera, 0x10C);

class GameCam : public SwampCamera { // NAME: proto-pdb:default.pdb
public:
    char field_10C[0x11C - 0x10C];     // 0x10C
};
DECOMP_SIZE_ASSERT(GameCam, 0x11C);

class AnimCam : public GameCam { // NAME: proto-pdb:default.pdb
public:
    static AnimCam* SwitchTo(const SwampString&, float, bool, float);
    static void Return(float);

    char field_11C[0x128 - 0x11C];     // 0x11C
};
DECOMP_SIZE_ASSERT(AnimCam, 0x128);

class VoodooShopLevelMain : public LevelScript { // NAME: proto-pdb:default.pdb
public:
    void SetCamera(const char* name, bool reverse);

private:
    void EnterErrorState(unsigned int errorState, const unsigned short* message);

    char field_64[0x88 - 0x64];        // 0x64
    FrontEndUI* _frontEndUI;           // 0x88
    unsigned int _errorState;          // 0x8C
    char field_90[0x98 - 0x90];        // 0x90
    static bool _controlOff;
};
DECOMP_SIZE_ASSERT(VoodooShopLevelMain, 0x98);

// FUNCTION: USA 0x001390B7
// PROTOTYPE: PREVIEW 0x00523109 identical
// NAME: proto-pdb:default.pdb
void VoodooShopLevelMain::SetCamera(const char* name, bool reverse)
{
    AnimCam::Return(0.0f);
    AnimCam* cam = AnimCam::SwitchTo(name, 0.0f, reverse, 0.0f);
    cam->_speed = reverse ? -3 : 3;
    _controlOff = true;
}

// FUNCTION: USA 0x00139B9E
// PROTOTYPE: PREVIEW 0x00523A7B identical
// NAME: proto-pdb:default.pdb
template void std::_Destroy_range<SavedGamesScreen::SavedGameInfo,std::allocator<SavedGamesScreen::SavedGameInfo> >(SavedGamesScreen::SavedGameInfo *,SavedGamesScreen::SavedGameInfo *,std::allocator<SavedGamesScreen::SavedGameInfo> &,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x00139BB7
// PROTOTYPE: PREVIEW 0x00523A94 identical
// NAME: proto-pdb:default.pdb
template SavedGamesScreen::SavedGameInfo * std::_Copy_backward_opt<SavedGamesScreen::SavedGameInfo *,SavedGamesScreen::SavedGameInfo *>(SavedGamesScreen::SavedGameInfo *,SavedGamesScreen::SavedGameInfo *,SavedGamesScreen::SavedGameInfo *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x00139C60
// NAME: proto-pdb:default.pdb
void SavedGamesScreen::UpdatePics()
{
    int i;
    Sprite* pic = _pictures;
    for (i = _topSel; i <= _lastSel; i++, pic++) {
        if (_games[i]._gameTime >= 0.0f)
            pic->SetTexture("savescreen_" + _games[i]._area);
        else
            pic->SetTexture("");
    }
    for (i = _lastSel + 1; i < 3; i++)
        _pictures[i].SetTexture("");
    _updatePics = false;
}

// FUNCTION: USA 0x00139DAF
// PROTOTYPE: PREVIEW 0x00523C8C identical
// NAME: proto-pdb:default.pdb
template void std::fill<SavedGamesScreen::SavedGameInfo *,SavedGamesScreen::SavedGameInfo>(SavedGamesScreen::SavedGameInfo *,SavedGamesScreen::SavedGameInfo *,SavedGamesScreen::SavedGameInfo const &);

// FUNCTION: USA 0x0013A168
// PROTOTYPE: PREVIEW 0x00524045 identical
// NAME: proto-pdb:default.pdb
template SavedGamesScreen::SavedGameInfo * std::vector<SavedGamesScreen::SavedGameInfo,std::allocator<SavedGamesScreen::SavedGameInfo> >::_Ufill(SavedGamesScreen::SavedGameInfo *,unsigned int,SavedGamesScreen::SavedGameInfo const &);

// FUNCTION: USA 0x0013A400
// PROTOTYPE: PREVIEW 0x005242DD identical
// NAME: proto-pdb:default.pdb
template std::vector<SavedGamesScreen::SavedGameInfo,std::allocator<SavedGamesScreen::SavedGameInfo> >::iterator std::vector<SavedGamesScreen::SavedGameInfo,std::allocator<SavedGamesScreen::SavedGameInfo> >::insert(std::vector<SavedGamesScreen::SavedGameInfo,std::allocator<SavedGamesScreen::SavedGameInfo> >::iterator,SavedGamesScreen::SavedGameInfo const &);

// FUNCTION: USA 0x0013A4C0
// PROTOTYPE: PREVIEW 0x0052439D identical
// NAME: proto-pdb:default.pdb
void VoodooShopLevelMain::EnterErrorState(unsigned int errorState, const unsigned short* message)
{
    _errorState = errorState;
    _frontEndUI->SetCaption(message, 6);
}

// !FAKE: c1xx front end; scaffold call; retail emits the implicit ??1SavedGamesScreen@@UAE@XZ in this unit (no vptr
// store, so no declared dtor; no matched function odr-uses it); this qualified call odr-uses it instead
// (matched_set [[emitter]] row)
void pin_emit_SavedGamesScreen_dtor(SavedGamesScreen* p)
{
    p->SavedGamesScreen::~SavedGamesScreen();
}
