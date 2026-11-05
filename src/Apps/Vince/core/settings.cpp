// UNIT: src/Apps/Vince/core/settings.cpp OBJ: core\XBox\OfficialRelease\Settings.obj OPT: /O1 /Oy
#include "../../../include/decomp.h"

inline void* operator new(unsigned int, void* p) { return p; }

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T = char_traits<E>, class A = allocator<E> > class basic_string {
public:
    basic_string() { _Myres = 15; _Eos(0); } // src proto-pdb:default.pdb _Myres
    ~basic_string() { _Tidy(true, 0); }
    bool empty() const { return _Mysize == 0; }
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    int compare(const E* _Ptr) const;
    static const unsigned int npos = (unsigned int)(-1);
protected:
    void _Eos(unsigned int _Newsize);
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);

private:
    char _pad00[0x14];                 // 0x00
    unsigned int _Mysize;              // 0x14
    unsigned int _Myres;               // 0x18
};
template <class T, class A = allocator<T> > class vector {
public:
    vector() : _Myfirst(0), _Mylast(0), _Myend(0) {}
    ~vector() { _Tidy(); }
    unsigned int size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }
protected:
    void _Tidy();

private:
    char _pad00[0x4];                  // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
template <class _T1, class _T2> inline void _Construct(_T1* _Ptr, const _T2& _Val)
{
    new ((void*)_Ptr) _T1(_Val);
}
struct _Nonscalar_ptr_iterator_tag {};
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
template <class E, class T, class A>
inline bool operator==(const basic_string<E, T, A>& _Left, const E* _Right)
{
    return _Left.compare(_Right) == 0;
}
}

enum PARAM_TYPES { // NAME: proto-pdb:default.pdb
    PARAM_FLOAT = 0x0,
    PARAM_STRING = 0x1,
    PARAM_VECTOR2 = 0x2,
    PARAM_VECTOR3 = 0x3,
    PARAM_VECTOR4 = 0x4,
    PARAM_VERTEX = 0x5,
    PARAM_QUATERNION = 0x6,
    PARAM_INT = 0x7,
    PARAM_DOTANGLE = 0x8,
};

class Param { // NAME: proto-pdb:default.pdb
public:
    const char* _Name;                 // 0x00
    PARAM_TYPES _Type;                 // 0x04
    float _Min;                        // 0x08
    float _Max;                        // 0x0C
    const char* _s;                    // 0x10
    void* val;                         // 0x14
    bool _Modify;                      // 0x18
};
DECOMP_SIZE_ASSERT(Param, 0x1C);

class BeepFile { // NAME: proto-pdb:default.pdb
public:
    BeepFile();
    ~BeepFile();
    bool OpenForWrite(const std::basic_string<char>&);
    void Close();
    bool Write(const std::basic_string<char>&);

    char field_0[0x20];                // 0x00
};
DECOMP_SIZE_ASSERT(BeepFile, 0x20);

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    BeepTDF(const std::basic_string<char>&);
    ~BeepTDF();

    char field_0[0x58];                // 0x00
};
DECOMP_SIZE_ASSERT(BeepTDF, 0x58);

class LevelLoader { // NAME: proto-pdb:default.pdb
public:
    static LevelLoader& Instance();
    static bool GenerateFileSignature(const std::basic_string<char>&, bool);
};

class Settings { // NAME: proto-pdb:default.pdb
public:
    Settings();
    ~Settings();
    void SaveParams() const;
    void Save(std::basic_string<char>&) const;
    void Load(const BeepTDF&);
    void Load(const std::basic_string<char>&);
    Param* FindParam(const std::basic_string<char>&) const;
    float GetParamFloat(const std::basic_string<char>&, float) const;

    Param* _params;                            // 0x00
    int _paramCount;                           // 0x04
    std::vector<Param> _editParams;            // 0x08
    std::basic_string<char> _fileName;         // 0x18
};
DECOMP_SIZE_ASSERT(Settings, 0x34);

// FUNCTION: USA 0x001114D9
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING // wall-fold-va-callers (C0233): match_one 100, but 0x001114D9 also folds vector<std::string>::size, which FrontEndUI.obj calls as an external (4.3 T12)
template unsigned int std::vector<Param,std::allocator<Param> >::size()const;
#endif

// FUNCTION: USA 0x00111571
// PROTOTYPE: PREVIEW 0x004FBDAC identical
// NAME: proto-pdb:default.pdb
Param* Settings::FindParam(const std::basic_string<char>& name) const
{
    for (int i = 0; i < _paramCount; i++) {
        if (name == _params[i]._Name)
            return &_params[i];
    }
    return 0;
}

// FUNCTION: USA 0x001115AB
// PROTOTYPE: PREVIEW 0x004FBDE6 identical
// NAME: proto-pdb:default.pdb
float Settings::GetParamFloat(const std::basic_string<char>& name, float def) const
{
    Param* p = FindParam(name);
    if (!p)
        return def;
    return *(float*)p->val;
}

// FUNCTION: USA 0x00111BF8
// PROTOTYPE: PREVIEW 0x004FC433 identical
// NAME: proto-pdb:default.pdb
void Settings::Load(const std::basic_string<char>& filename)
{
    BeepTDF tdf(filename);
    _fileName = filename;
    Load(tdf);
}

// FUNCTION: USA 0x00111E4C
// PROTOTYPE: PREVIEW 0x004FC687 identical
// NAME: proto-pdb:default.pdb
Settings::Settings() : _params(0), _paramCount(0)
{
}

// FUNCTION: USA 0x0011201F
// PROTOTYPE: PREVIEW 0x004FC85A identical
// NAME: proto-pdb:default.pdb
void Settings::SaveParams() const
{
    if (!_fileName.empty()) {
        std::basic_string<char> ff;
        Save(ff);
        BeepFile f;
        if (f.OpenForWrite(_fileName)) {
            f.Write(ff);
            f.Close();
            LevelLoader::Instance().GenerateFileSignature(_fileName, false);
        }
    }
}

// FUNCTION: USA 0x0011209C
// PROTOTYPE: PREVIEW 0x004FC8D7 identical
// NAME: proto-pdb:default.pdb
Settings::~Settings()
{
    SaveParams();
}

// FUNCTION: USA 0x0011148C
// PROTOTYPE: PREVIEW 0x004FBCDD identical
// NAME: proto-pdb:default.pdb
template void std::fill<Param *,Param>(Param *,Param *,Param const &);

// FUNCTION: USA 0x001114AF
// PROTOTYPE: PREVIEW 0x004FBD00 identical
// NAME: proto-pdb:default.pdb
template Param * std::_Copy_backward_opt<Param *,Param *>(Param *,Param *,Param *,std::_Nonscalar_ptr_iterator_tag);

// FUNCTION: USA 0x00111509
// PROTOTYPE: PREVIEW 0x004FBD44 identical
// NAME: proto-pdb:default.pdb
template void std::_Construct<Param,Param>(Param *,Param const &);
