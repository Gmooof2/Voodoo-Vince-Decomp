// UNIT: src/Libs/BeepLib/Xbox/Official_Release/BeepLib.cpp OBJ: Xbox\Official_Release\BeepLib.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#include <stdlib.h>
#include <string.h>
#pragma intrinsic(memset) // !FAKE: compiler directive; retail char_traits<char>::assign has the inline rep-stos memset form (match_one 19.78 -> 100.0); /Oi control also matches

extern "C" wchar_t* __cdecl wmemset(wchar_t* _S, wchar_t _C, unsigned int _N); // NAME: proto-pdb:default.pdb

namespace std { // NAME: proto-pdb:default.pdb
template <class _Elem> struct char_traits;
template <> struct char_traits<char> {
    typedef char _Elem;
    static void __cdecl assign(_Elem& _Left, const _Elem& _Right) { _Left = _Right; } // src proto-pdb:default.pdb assign
    // VC 7.1 iosfwd body; body-less, the copy ctor's _Tidy made cl 13.10 ICE (C1001)
    static _Elem* __cdecl copy(_Elem* _First1, const _Elem* _First2, size_t _Count) { return ((_Elem*)::memcpy(_First1, _First2, _Count)); } // src proto-pdb:default.pdb copy
    static const _Elem* __cdecl find(const _Elem* _First, size_t _Count, const _Elem& _Ch) { return ((const _Elem*)::memchr(_First, _Ch, _Count)); } // src proto-pdb:default.pdb find
    static _Elem* __cdecl assign(_Elem* _First, size_t _Count, _Elem _Ch) { return ((_Elem*)::memset(_First, _Ch, _Count)); } // src proto-pdb:default.pdb assign
};
template <> struct char_traits<unsigned short> {
    typedef unsigned short _Elem;
    static void __cdecl assign(_Elem& _Left, const _Elem& _Right) { _Left = _Right; } // src proto-pdb:default.pdb assign
    static _Elem* __cdecl copy(_Elem* _First1, const _Elem* _First2, size_t _Count); // src proto-pdb:default.pdb copy
    static _Elem* __cdecl assign(_Elem* _First, size_t _Count, _Elem _Ch) { return (::wmemset(_First, _Ch, _Count)); } // src proto-pdb:default.pdb assign
};
template <class _Ty> class allocator {
public:
    void deallocate(_Ty* _Ptr, size_t _Count); // src proto-pdb:default.pdb deallocate
};

class _String_base { // NAME: proto-pdb:default.pdb
public:
    void _Xlen() const; // src proto-pdb:default.pdb _Xlen
};

template <class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
class basic_string : public _String_base {
public:
    typedef basic_string<_Elem, _Traits, _Ax> _Myt;
    typedef unsigned int size_type;

    class const_iterator { // NAME: proto-pdb:default.pdb
    public:
        const_iterator(const _Elem* _Ptr) { _Myptr = _Ptr; } // src proto-pdb:default.pdb const_iterator
        const _Elem& operator*() const { return (*_Myptr); }
        bool operator==(const const_iterator& _Right) const { return (_Myptr == _Right._Myptr); }
        bool operator!=(const const_iterator& _Right) const { return (!(*this == _Right)); }
        const _Elem* _Myptr;
    };
    class iterator : public const_iterator { // NAME: proto-pdb:default.pdb
    public:
        iterator(_Elem* _Ptr) : const_iterator(_Ptr) {}
        _Elem& operator*() const { return ((_Elem&)**(const_iterator*)this); }
        iterator& operator++() { ++this->_Myptr; return (*this); }
    };

    // !FAKE: inliner; hand form of _Tidy(false, 0); retail inlines it while calling _Tidy(true) out of line (0x000111D6, another unit's row), so _Tidy is decl-only here
    basic_string() { _Myres = _BUF_SIZE - 1; _Eos(0); } // src proto-pdb:default.pdb basic_string
    basic_string(const _Myt& _Right) : _Alval(_Right._Alval) { _Tidy(); assign(_Right, 0, npos); } // src proto-pdb:default.pdb basic_string
    basic_string(const _Myt& _Right, size_type _Roff, size_type _Count = npos);
    basic_string(const _Elem* _Ptr);
    ~basic_string() { _Tidy(true); } // src proto-pdb:default.pdb ~basic_string
    _Myt& append(size_type _Count, _Elem _Ch) // src proto-pdb:default.pdb append
    {
        if (npos - _Mysize <= _Count)
            _String_base::_Xlen();

        size_type _Num;
        if (0 < _Count && _Grow(_Num = _Mysize + _Count)) {
            _Traits::assign(_Myptr() + _Mysize, _Count, _Ch);
            _Eos(_Num);
        }
        return (*this);
    }
    _Myt& operator=(const _Myt& _Right) { return (assign(_Right)); }
    _Myt& assign(const _Myt& _Right) { return (assign(_Right, 0, npos)); }
    _Myt& assign(const _Myt& _Right, size_type _Roff, size_type _Count); // src proto-pdb:default.pdb assign
    const_iterator end() const { return (const_iterator(_Myptr() + _Mysize)); } // src proto-pdb:default.pdb end
    const _Elem& operator[](size_type _Off) const;
    const _Elem* c_str() const { return _Myptr(); }
    size_type size() const { return _Mysize; }
    void resize(size_type _Newsize) { resize(_Newsize, _Elem()); } // src proto-pdb:default.pdb resize
    void resize(size_type _Newsize, _Elem _Ch); // src proto-pdb:default.pdb resize
    size_type find_first_of(const _Myt& _Right, size_type _Off = 0) const; // src proto-pdb:default.pdb find_first_of
    size_type find_first_not_of(const _Myt& _Right, size_type _Off = 0) const; // src proto-pdb:default.pdb find_first_not_of
    size_type find_last_of(const _Elem* _Ptr, size_type _Off, size_type _Count) const // src proto-pdb:default.pdb find_last_of
    {
        if (0 < _Count && 0 < _Mysize)
            for (const _Elem* _Uptr = _Myptr() + (_Off < _Mysize ? _Off : _Mysize - 1);; --_Uptr)
                if (_Traits::find(_Ptr, _Count, *_Uptr) != 0)
                    return (_Uptr - _Myptr());
                else if (_Uptr == _Myptr())
                    break;

        return (npos);
    }
    size_type find_last_of(const _Elem* _Ptr, size_type _Off = npos) const; // src proto-pdb:default.pdb find_last_of
    _Myt substr(size_type _Off = 0, size_type _Count = npos) const { return _Myt(*this, _Off, _Count); } // src proto-pdb:default.pdb substr

    static const size_type npos;

    enum { _BUF_SIZE = 16 / sizeof(_Elem) < 1 ? 1 : 16 / sizeof(_Elem) };

protected:
    _Ax _Alval;         // 0x00
    union _Bxty {
        _Elem _Buf[_BUF_SIZE];
        _Elem* _Ptr;
    } _Bx;              // 0x04

protected:
    void _Eos(size_type _Newsize) { _Traits::assign(_Myptr()[_Mysize = _Newsize], _Elem()); } // src proto-pdb:default.pdb _Eos
    bool _Grow(size_type _Newsize, bool _Trim = false); // src proto-pdb:default.pdb _Grow
    void _Tidy(bool _Built = false, size_type _Newsize = 0) // src proto-pdb:default.pdb _Tidy
    {
        if (!_Built)
            ;
        else if (_BUF_SIZE <= _Myres) {
            _Elem* _Ptr = _Bx._Ptr;
            if (0 < _Newsize)
                _Traits::copy(_Bx._Buf, _Ptr, _Newsize);
            _Alval.deallocate(_Ptr, _Myres + 1);
        }
        _Myres = _BUF_SIZE - 1;
        _Eos(_Newsize);
    }

public:
    _Elem* _Myptr() { return (_BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf); } // src proto-pdb:default.pdb _Myptr
    const _Elem* _Myptr() const { return (_BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf); }

    size_type _Mysize;  // 0x14
    size_type _Myres;   // 0x18
};
template <class _Elem, class _Traits, class _Alloc>
const typename basic_string<_Elem, _Traits, _Alloc>::size_type basic_string<_Elem, _Traits, _Alloc>::npos =
    (typename basic_string<_Elem, _Traits, _Alloc>::size_type)(-1);
// !FAKE: decl-only specialization; retail calls the char copy ctor (0x00011753) out of line, keeps it unemitted here (C0231)
template <> basic_string<char, char_traits<char>, allocator<char> >::basic_string(const basic_string<char, char_traits<char>, allocator<char> >& _Right);
// !FAKE: decl-only specializations; retail calls _Eos (0x000111BA, 0x0001841B) out of line by name (C0231)
template <> void basic_string<char, char_traits<char>, allocator<char> >::_Eos(size_type _Newsize);
template <> void basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >::_Eos(size_type _Newsize);
template <class _Elem, class _Traits, class _Alloc>
basic_string<_Elem, _Traits, _Alloc> operator+(const basic_string<_Elem, _Traits, _Alloc>& _Left, const basic_string<_Elem, _Traits, _Alloc>& _Right); // src proto-pdb:default.pdb operator+
// !FAKE: decl-only specialization; retail calls char _Tidy (0x000111D6) out of line by name (C0231)
template <> void basic_string<char, char_traits<char>, allocator<char> >::_Tidy(bool _Built, size_type _Newsize);
// !FAKE: decl-only specialization; the dtor's wide _Tidy body is Onion's row (0x000184CB), keep it unemitted here (C0215)
template <> void basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >::_Tidy(bool _Built, size_type _Newsize);

template <class _Ty, class _Ax = allocator<_Ty> >
class vector { // NAME: proto-pdb:default.pdb
public:
    vector() : _Myfirst(0), _Mylast(0), _Myend(0) {} // src proto-pdb:default.pdb vector
    vector(const vector& _Right); // src proto-pdb:default.pdb vector
    ~vector() { _Tidy(); } // src proto-pdb:default.pdb ~vector
    void push_back(const _Ty& _Val); // src proto-pdb:default.pdb push_back

protected:
    void _Tidy(); // src proto-pdb:default.pdb _Tidy

    _Ax _Alval;    // 0x00
    _Ty* _Myfirst; // 0x04
    _Ty* _Mylast;  // 0x08
    _Ty* _Myend;   // 0x0C
};

struct _Nonscalar_ptr_iterator_tag {}; // NAME: proto-pdb:default.pdb
template <class _FwdIt, class _Ty> inline
void replace(_FwdIt _First, _FwdIt _Last, const _Ty& _Oldval, const _Ty& _Newval) // src proto-pdb:default.pdb replace
{
    for (; _First != _Last; ++_First)
        if (*_First == _Oldval)
            *_First = _Newval;
}
template <class _InIt, class _OutIt> inline
_OutIt _Copy_opt(_InIt _First, _InIt _Last, _OutIt _Dest, _Nonscalar_ptr_iterator_tag) // src proto-pdb:default.pdb _Copy_opt
{
    for (; _First != _Last; ++_Dest, ++_First)
        *_Dest = *_First;
    return (_Dest);
}
}

// FUNCTION: USA 0x0003556D
// NAME: proto-pdb:default.pdb
template char* std::char_traits<char>::assign(char*, size_t, char);

// FUNCTION: USA 0x0003559F
// PROTOTYPE: PREVIEW 0x00423D99 identical
// NAME: proto-pdb:default.pdb
template std::basic_string<char,std::char_traits<char>,std::allocator<char> >::const_iterator std::basic_string<char,std::char_traits<char>,std::allocator<char> >::end()const;

// FUNCTION: USA 0x000355BB
// PROTOTYPE: PREVIEW 0x00423D0A identical
// NAME: proto-pdb:default.pdb
template unsigned int std::basic_string<char,std::char_traits<char>,std::allocator<char> >::find_last_of(char const *,unsigned int,unsigned int)const;

// FUNCTION: USA 0x0003562A
// PROTOTYPE: PREVIEW 0x00423D79 identical
// NAME: proto-pdb:default.pdb
template void std::replace<std::basic_string<char,std::char_traits<char>,std::allocator<char> >::iterator,char>(std::basic_string<char,std::char_traits<char>,std::allocator<char> >::iterator,std::basic_string<char,std::char_traits<char>,std::allocator<char> >::iterator,char const &,char const &);

// FUNCTION: USA 0x000356CA
// PROTOTYPE: PREVIEW 0x00423E35 identical
// NAME: proto-pdb:default.pdb
template std::basic_string<char,std::char_traits<char>,std::allocator<char> > & std::basic_string<char,std::char_traits<char>,std::allocator<char> >::append(unsigned int,char);

// FUNCTION: USA 0x0003572A
// PROTOTYPE: PREVIEW 0x00423E95 identical
// NAME: proto-pdb:default.pdb
template std::basic_string<unsigned short,std::char_traits<unsigned short>,std::allocator<unsigned short> > & std::basic_string<unsigned short,std::char_traits<unsigned short>,std::allocator<unsigned short> >::append(unsigned int,unsigned short);

// FUNCTION: USA 0x0003578B
// PROTOTYPE: PREVIEW 0x00423EF6 identical
// NAME: proto-pdb:default.pdb
// !FAKE: full specialization with _Tidy(false, 0) expanded by hand (retail inlines it); naming _Tidy emitted its
// wide body here, which is Onion's row 0x000184CB (4.3 T12 relink red); decl-only _Tidy gave a call (match_one 80.0)
template <> std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> >::basic_string(const std::basic_string<unsigned short, std::char_traits<unsigned short>, std::allocator<unsigned short> >& _Right) // src proto-pdb:default.pdb basic_string
    : _Alval(_Right._Alval)
{
    _Myres = _BUF_SIZE - 1;
    _Eos(0);
    assign(_Right, 0, npos);
}
template std::basic_string<unsigned short,std::char_traits<unsigned short>,std::allocator<unsigned short> >::basic_string<unsigned short,std::char_traits<unsigned short>,std::allocator<unsigned short> >(std::basic_string<unsigned short,std::char_traits<unsigned short>,std::allocator<unsigned short> > const &);

// FUNCTION: USA 0x000358AF
// PROTOTYPE: PREVIEW 0x0042401A identical
// NAME: proto-pdb:default.pdb
template std::basic_string<char,std::char_traits<char>,std::allocator<char> > * std::_Copy_opt<std::basic_string<char,std::char_traits<char>,std::allocator<char> > *,std::basic_string<char,std::char_traits<char>,std::allocator<char> > *>(std::basic_string<char,std::char_traits<char>,std::allocator<char> > *,std::basic_string<char,std::char_traits<char>,std::allocator<char> > *,std::basic_string<char,std::char_traits<char>,std::allocator<char> > *,std::_Nonscalar_ptr_iterator_tag);

typedef std::basic_string<char> BeepString;
typedef std::basic_string<unsigned short> BeepWString;

class BeepState { // NAME: proto-pdb:default.pdb
public:
    bool IsRDTSCAvailable() { return _RDTSCAvailable; }
    unsigned __int64 GetTSC();
    unsigned __int64 GetStartingTimestamp() { return _StartingTimestamp; }
    double GetInverseCPUSpeed() { return _InverseCPUSpeed; }

private:
    char field_0[0x20];                // 0x00
    unsigned __int64 _StartingTimestamp; // 0x20
    double _CPUSpeed;                  // 0x28
    double _InverseCPUSpeed;           // 0x30
    char field_38[0x3D - 0x38];        // 0x38
    bool _RDTSCAvailable;              // 0x3D
    char field_3E[0xC58 - 0x3E];       // 0x3E
};
DECOMP_SIZE_ASSERT(BeepState, 0xC58);

extern BeepState beepState; // NAME: proto-pdb:default.pdb
extern "C" unsigned long __stdcall GetTickCount(); // NAME: proto-pdb:default.pdb

namespace BeepLib { // NAME: proto-pdb:default.pdb
double GetTime();
bool HasExtension(const BeepString& s);
BeepString StripExtension(const BeepString& s);
BeepString StripPath(const BeepString& s);
BeepString StripDigits(const BeepString& s);
BeepWString MakeWide(const BeepString& s);
BeepString ReplaceExtension(const BeepString& s, const BeepString& ext); // src proto-pdb:default.pdb ReplaceExtension
BeepString AddExtension(const BeepString& s, const BeepString& ext);
std::vector<BeepString> SplitString(const BeepString& str, const BeepString& splitChars); // src proto-pdb:default.pdb SplitString
}

const BeepString format(const char* formatstring, ...); // NAME: proto-pdb:default.pdb
extern "C" int __cdecl _vsnprintf(char* buffer, size_t count, const char* format, char* argptr); // NAME: proto-pdb:default.pdb

// FUNCTION: USA 0x000354F0
// PROTOTYPE: PREVIEW 0x00423C5B identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1): callee _GetTickCount@0 (0x0014A4D9) has no table name (symbols.txt holds ghidra `GetTickCount`); needs a pin
double BeepLib::GetTime()
{
    if (beepState.IsRDTSCAvailable()) {
        // !FAKE: inliner; dummy local; inline loads of _StartingTimestamp/_InverseCPUSpeed hoist above the GetTSC call without it (match_one 54.4 -> 100.0)
        unsigned __int64 v1 = beepState.GetTSC();
        return (__int64)(v1 - beepState.GetStartingTimestamp()) * beepState.GetInverseCPUSpeed();
    }
    return GetTickCount() * 0.001;
}
#endif

// FUNCTION: USA 0x00035542
// PROTOTYPE: PREVIEW 0x00423CAD identical
// NAME: proto-pdb:default.pdb
extern "C" wchar_t* __cdecl wmemset(wchar_t* _S, wchar_t _C, unsigned int _N)
{
    wchar_t* _Su = _S;
    for (; 0 < _N; ++_Su, --_N)
        *_Su = _C;
    return _S;
}

// FUNCTION: USA 0x00035690
// PROTOTYPE: PREVIEW 0x00423DFB identical
// NAME: proto-pdb:default.pdb
bool BeepLib::HasExtension(const BeepString& s)
{
    unsigned int pos = s.find_last_of("/\\:.");
    return pos != BeepString::npos && s[pos] == '.' && pos != s.size() - 1;
}

// FUNCTION: USA 0x000358D7
// PROTOTYPE: PREVIEW 0x00424042 identical
// NAME: proto-pdb:default.pdb
BeepString BeepLib::StripExtension(const BeepString& s)
{
    unsigned int pos = s.find_last_of("/\\:.");
    if (pos != BeepString::npos && s[pos] == '.')
        return s.substr(0, pos);
    return s;
}

// FUNCTION: USA 0x0003591F
// PROTOTYPE: PREVIEW 0x0042408A identical
// NAME: proto-pdb:default.pdb
BeepString BeepLib::StripPath(const BeepString& s)
{
    unsigned int pos = s.find_last_of("/\\:");
    if (pos != BeepString::npos)
        return s.substr(pos + 1);
    return s;
}

// FUNCTION: USA 0x00035954
// PROTOTYPE: PREVIEW 0x004240BF identical
// NAME: proto-pdb:default.pdb
BeepString BeepLib::StripDigits(const BeepString& s)
{
    int i = s.size();
    while (--i >= 0) {
        if (s[i] < '0' || s[i] > '9')
            break;
    }
    return s.substr(0, i + 1);
}

// FUNCTION: USA 0x00035991
// NAME: proto-pdb:default.pdb
const BeepString format(const char* formatstring, ...)
{
    char buffer[1024];
    if (_vsnprintf(buffer, sizeof(buffer) - 1, formatstring, (char*)(&formatstring + 1)) >= 0)
        return BeepString(buffer);

    BeepString result;
    result.resize(2048);
    while (_vsnprintf((char*)result.c_str(), result.size() - 1, formatstring, (char*)(&formatstring + 1)) < 0)
        result.resize(result.size() * 2);
    return result;
}

// FUNCTION: USA 0x00035B07
// PROTOTYPE: PREVIEW 0x00424272 identical
// NAME: proto-pdb:default.pdb
BeepWString BeepLib::MakeWide(const BeepString& s)
{
    wchar_t buffer[1024];
    mbstowcs(buffer, s.c_str(), s.size() + 1); // src proto-pdb:default.pdb mbstowcs
    return BeepWString(buffer);
}

// FUNCTION: USA 0x00035BD2
// NAME: proto-pdb:default.pdb
BeepString BeepLib::ReplaceExtension(const BeepString& s, const BeepString& ext)
{
    if (ext.c_str()[0] == '.')
        return StripExtension(s) + ext;
    return StripExtension(s) + BeepString(".") + ext;
}

// FUNCTION: USA 0x00035C6C
// PROTOTYPE: PREVIEW 0x004243D7 identical
// NAME: proto-pdb:default.pdb
BeepString BeepLib::AddExtension(const BeepString& s, const BeepString& ext)
{
    if (!HasExtension(s))
        return ReplaceExtension(s, ext);
    return s;
}

// FUNCTION: USA 0x00035D50
// NAME: proto-pdb:default.pdb
std::vector<BeepString> BeepLib::SplitString(const BeepString& str, const BeepString& splitChars)
{
    std::vector<BeepString> result;
    unsigned int start = str.find_first_not_of(splitChars);
    if (start == BeepString::npos)
        return result;

    unsigned int end;
    while ((end = str.find_first_of(splitChars, start)) != BeepString::npos) {
        result.push_back(str.substr(start, end - start));
        start = str.find_first_not_of(splitChars, end);
    }
    if (start != end)
        result.push_back(str.substr(start, end - start));
    return result;
}
