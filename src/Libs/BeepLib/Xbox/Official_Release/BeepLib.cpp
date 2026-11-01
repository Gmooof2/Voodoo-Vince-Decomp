// UNIT: src/Libs/BeepLib/Xbox/Official_Release/BeepLib.cpp OBJ: Xbox\Official_Release\BeepLib.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#include <stdlib.h>

namespace std { // NAME: proto-pdb:default.pdb
template <class _Elem> struct char_traits {};
template <class _Ty> class allocator {};

template <class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
class basic_string {
public:
    typedef basic_string<_Elem, _Traits, _Ax> _Myt;
    typedef unsigned int size_type;

    basic_string(const _Myt& _Right);
    basic_string(const _Myt& _Right, size_type _Roff, size_type _Count = npos);
    basic_string(const _Elem* _Ptr);
    const _Elem& operator[](size_type _Off) const;
    const _Elem* c_str() const { return _Myptr(); }
    size_type size() const { return _Mysize; }
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

public:
    const _Elem* _Myptr() const { return (_BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf); }

    size_type _Mysize;  // 0x14
    size_type _Myres;   // 0x18
};
template <class _Elem, class _Traits, class _Alloc>
const typename basic_string<_Elem, _Traits, _Alloc>::size_type basic_string<_Elem, _Traits, _Alloc>::npos =
    (typename basic_string<_Elem, _Traits, _Alloc>::size_type)(-1);
}

typedef std::basic_string<char> BeepString;
typedef std::basic_string<unsigned short> BeepWString;

namespace BeepLib { // NAME: proto-pdb:default.pdb
bool HasExtension(const BeepString& s);
BeepString StripExtension(const BeepString& s);
BeepString StripPath(const BeepString& s);
BeepString StripDigits(const BeepString& s);
BeepWString MakeWide(const BeepString& s);
BeepString ReplaceExtension(const BeepString& s, const BeepString& ext); // src proto-pdb:default.pdb ReplaceExtension
BeepString AddExtension(const BeepString& s, const BeepString& ext);
}

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

// FUNCTION: USA 0x00035B07
// PROTOTYPE: PREVIEW 0x00424272 identical
// NAME: proto-pdb:default.pdb
BeepWString BeepLib::MakeWide(const BeepString& s)
{
    wchar_t buffer[1024];
    mbstowcs(buffer, s.c_str(), s.size() + 1); // src proto-pdb:default.pdb mbstowcs
    return BeepWString(buffer);
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
