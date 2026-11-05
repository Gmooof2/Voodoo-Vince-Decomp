// UNIT: src/Libs/BeepLib/Xbox/Official_Release/BeepTDF.cpp OBJ: Xbox\Official_Release\BeepTDF.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#pragma intrinsic(memcmp) // !FAKE: compiler directive; retail basic_string::find inlines char_traits<char>::compare as repe cmpsb

namespace std { // NAME: proto-pdb:default.pdb
template <class _Elem> struct char_traits {
    static int __cdecl compare(const _Elem* _First1, const _Elem* _First2, size_t _Count) { return (::memcmp(_First1, _First2, _Count)); }
    static const _Elem* __cdecl find(const _Elem* _First, size_t _Count, const _Elem& _Ch); // src proto-pdb:default.pdb find
};
template <class _Ty> class allocator {};
template <class _Ty> struct less;
template <class _Ty1, class _Ty2> struct pair;

template <class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
class basic_string {
public:
    typedef unsigned int size_type;

    basic_string();
    basic_string(const basic_string& _Right);
    basic_string(const _Elem* _Ptr);
    basic_string(const basic_string& _Right, size_type _Roff, size_type _Count = npos);
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const basic_string& _Right) { return (assign(_Right)); }
    basic_string& assign(const basic_string& _Right) { return (assign(_Right, 0, npos)); }
    basic_string& assign(const basic_string& _Right, size_type _Roff, size_type _Count);
    size_type find_first_not_of(const basic_string& _Right, size_type _Off = 0) const; // src proto-pdb:default.pdb find_first_not_of
    size_type find_last_not_of(const basic_string& _Right, size_type _Off = npos) const; // src proto-pdb:default.pdb find_last_not_of
    const _Elem* c_str() const { return (_Myptr()); }
    basic_string substr(size_type _Off = 0, size_type _Count = npos) const { return (basic_string(*this, _Off, _Count)); }
    const _Elem& operator[](size_type _Off) const;
    _Elem& operator[](size_type _Off) { return (_Myptr()[_Off]); } // VC 7.1 body: out-of-line COMDAT folds with retail 0x220EB (C0233 Fix 1)
    size_type find(const _Elem* _Ptr, size_type _Off = 0) const; // src proto-pdb:default.pdb find
    size_type find_first_of(const _Elem* _Ptr, size_type _Off = 0) const; // src proto-pdb:default.pdb find_first_of

    size_type find(const _Elem* _Ptr, size_type _Off, size_type _Count) const
    { // src proto-pdb:default.pdb find
        if (_Count == 0 && _Off <= _Mysize)
            return (_Off);

        size_type _Nm;
        if (_Off < _Mysize && _Count <= (_Nm = _Mysize - _Off)) {
            const _Elem *_Uptr, *_Vptr;
            for (_Nm -= _Count - 1, _Vptr = _Myptr() + _Off;
                 (_Uptr = _Traits::find(_Vptr, _Nm, *_Ptr)) != 0;
                 _Nm -= _Uptr - _Vptr + 1, _Vptr = _Uptr + 1)
                if (_Traits::compare(_Uptr, _Ptr, _Count) == 0)
                    return (_Uptr - _Myptr());
        }

        return (npos);
    }

    size_type find_first_of(const _Elem* _Ptr, size_type _Off, size_type _Count) const
    { // src proto-pdb:default.pdb find_first_of
        if (0 < _Count && _Off < _Mysize) {
            const _Elem* const _Vptr = _Myptr() + _Mysize;
            for (const _Elem* _Uptr = _Myptr() + _Off; _Uptr < _Vptr; ++_Uptr)
                if (_Traits::find(_Ptr, _Count, *_Uptr) != 0)
                    return (_Uptr - _Myptr());
        }

        return (npos);
    }

    size_type find_first_not_of(const _Elem* _Ptr, size_type _Off, size_type _Count) const
    { // src proto-pdb:default.pdb find_first_not_of
        if (_Off < _Mysize) {
            const _Elem* const _Vptr = _Myptr() + _Mysize;
            for (const _Elem* _Uptr = _Myptr() + _Off; _Uptr < _Vptr; ++_Uptr)
                if (_Traits::find(_Ptr, _Count, *_Uptr) == 0)
                    return (_Uptr - _Myptr());
        }
        return (npos);
    }

    size_type find_last_not_of(const _Elem* _Ptr, size_type _Off, size_type _Count) const
    { // src proto-pdb:default.pdb find_last_not_of
        if (0 < _Mysize)
            for (const _Elem* _Uptr = _Myptr() + (_Off < _Mysize ? _Off : _Mysize - 1);; --_Uptr)
                if (_Traits::find(_Ptr, _Count, *_Uptr) == 0)
                    return (_Uptr - _Myptr());
                else if (_Uptr == _Myptr())
                    break;
        return (npos);
    }

    static const size_type npos;

    enum { _BUF_SIZE = 16 / sizeof(_Elem) < 1 ? 1 : 16 / sizeof(_Elem) };

protected:
    void _Tidy(bool _Built = false, size_type _Newsize = 0);

    _Ax _Alval;                        // 0x00
    union _Bxty {
        _Elem _Buf[_BUF_SIZE];
        _Elem* _Ptr;
    } _Bx;                             // 0x04

public:
    _Elem* _Myptr() { return (_BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf); }
    const _Elem* _Myptr() const { return (_BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf); }

    size_type _Mysize;                 // 0x14
    size_type _Myres;                  // 0x18
};
template <class _Elem, class _Traits, class _Alloc>
const typename basic_string<_Elem, _Traits, _Alloc>::size_type basic_string<_Elem, _Traits, _Alloc>::npos =
    (typename basic_string<_Elem, _Traits, _Alloc>::size_type)(-1);

template <class _Ty, class _Ax = allocator<_Ty> > class vector {
public:
    typedef _Ty* iterator;
    vector() { _Myfirst = 0; _Mylast = 0; _Myend = 0; }
    ~vector() { _Tidy(); }
    iterator begin() { return _Myfirst; }
    iterator end() { return _Mylast; }
    void push_back(const _Ty& _Val);
    void clear() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    _Ty* _Myfirst;                     // 0x04
    _Ty* _Mylast;                      // 0x08
    _Ty* _Myend;                       // 0x0C
};

template <class _Ty1, class _Ty2> struct pair {
    pair(const _Ty1& _Val1, const _Ty2& _Val2) : first(_Val1), second(_Val2) {}

    _Ty1 first;
    _Ty2 second;
};

template <class _Kty, class _Ty, class _Pr, class _Alloc, bool _Mfl> class _Tmap_traits {
public:
    typedef _Kty key_type;
    typedef pair<const _Kty, _Ty> value_type;
};

template <class _Traits> class _Tree {
public:
    typedef typename _Traits::key_type key_type;
    typedef typename _Traits::value_type value_type;

    struct _Node {
        _Node* _Left;                  // 0x00
        _Node* _Parent;                // 0x04
        _Node* _Right;                 // 0x08
        value_type _Myval;             // 0x0C
    };
    typedef _Node* _Nodeptr;

    class const_iterator {
    public:
        const_iterator(_Nodeptr _Pnode) : _Ptr(_Pnode) {}
        const value_type& operator*() const { return (_Ptr->_Myval); }
        const value_type* operator->() const { return (&**this); }
        bool operator==(const const_iterator& _Right) const { return (_Ptr == _Right._Ptr); }
        bool operator!=(const const_iterator& _Right) const { return (!(*this == _Right)); }

        _Nodeptr _Ptr;
    };

    class iterator : public const_iterator {
    public:
        iterator(_Nodeptr _Pnode) : const_iterator(_Pnode) {}
    };

    _Tree() { _Init(); }
    ~_Tree() { _Tidy(); }
    const_iterator end() const { return (const_iterator(_Myhead)); }
    pair<iterator, bool> insert(const value_type& _Val);
    const_iterator find(const key_type& _Keyval) const;
    void clear();

protected:
    void _Init();
    void _Tidy();

private:
    char _Alval;                       // 0x00
    _Nodeptr _Myhead;                  // 0x04
    unsigned int _Mysize;              // 0x08
};

template <class _Kty, class _Ty, class _Pr = less<_Kty>, class _Alloc = allocator<pair<const _Kty, _Ty> > >
class map : public _Tree<_Tmap_traits<_Kty, _Ty, _Pr, _Alloc, false> > {
public:
    map() {}
};
}

typedef std::basic_string<char> BeepString;

void* operator new(unsigned int size, const char* file, int line);

class FileHandler; // NAME: proto-pdb:default.pdb

class BeepFile { // NAME: proto-pdb:default.pdb
public:
    BeepFile();
    ~BeepFile();
    bool Open(const BeepString& filename);
    const void* GetFileData();

private:
    FileHandler* _fh;                  // 0x00
    BeepString _filename;              // 0x04
};
DECOMP_SIZE_ASSERT(BeepFile, 0x20);

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    struct Record { // NAME: proto-pdb:default.pdb
        BeepString name;               // 0x00
        BeepString stringdata;         // 0x1C
        int intdata;                   // 0x38
        double floatdata;              // 0x40
        bool booldata;                 // 0x48

        Record(const BeepString& id, const BeepString& data);
        ~Record();
    };

    BeepTDF(const BeepString& blockdata, const BeepString& trackingname, int startline);
    BeepTDF(BeepFile& f, const BeepString& trackingname);
    BeepTDF(const BeepString& filename);
    BeepTDF();
    ~BeepTDF();
    bool LoadFile(BeepFile& f, const BeepString& trackingname);
    bool LoadFile(const BeepString& filename);
    bool ParseRawData(const BeepString& blockdata, const BeepString& trackingname, int startline);
    const BeepTDF* FindBlock(const BeepString& blockname, bool canfail) const;
    const Record* FindRecord(const BeepString& id, bool canfail) const;
    const BeepString GetStringRecord(const BeepString& id, const BeepString& def) const;
    int GetIntRecord(const BeepString& id, int def) const;
    double GetFloatRecord(const BeepString& id, double def) const;
    bool GetBoolRecord(const BeepString& id, bool def) const;

private:
    void Cleanup();
    bool Preprocess(BeepString& data);
    int GetLineNumber(const BeepString& s, unsigned int loc, int startline);
    unsigned int FindMatchingBrace(const BeepString& s, unsigned int openbrace);

    BeepString _blockname;             // 0x00
    bool _valid;                       // 0x1C
    std::vector<Record*> _recordlist;  // 0x20
    std::map<BeepString, Record*> _recordlookup; // 0x30
    std::vector<BeepTDF*> _blocklist;  // 0x3C
    std::map<BeepString, BeepTDF*> _blocklookup; // 0x4C
};
DECOMP_SIZE_ASSERT(BeepTDF, 0x58);
DECOMP_SIZE_ASSERT(BeepTDF::Record, 0x50);

class BeepState { // NAME: proto-pdb:default.pdb
    friend class BeepTDF;
    struct TDFState { // NAME: proto-pdb:default.pdb
        BeepTDF* _DefaultBlock;        // 0x00
        BeepTDF::Record* _DefaultRecord; // 0x04
    };
    char field_0[0x14];                // 0x00
    TDFState _TDFState;                // 0x14
    char field_1C[0xC3C];              // 0x1C
};
DECOMP_SIZE_ASSERT(BeepState, 0xC58);

extern BeepState beepState; // NAME: proto-pdb:default.pdb

namespace BeepLib { // NAME: proto-pdb:default.pdb
BeepString MakeLower(const BeepString& s);
}

// FUNCTION: USA 0x000341D1
// PROTOTYPE: PREVIEW 0x0042293C identical
// NAME: proto-pdb:default.pdb
bool BeepTDF::Preprocess(BeepString& data)
{
    {
        int currentpos = 0;
        for (;;) {
            int v0 = data.find("/*", currentpos);
            if (v0 == -1)
                break;
            int v1 = data.find("*/", v0);
            if (v1 == -1)
                return false;
            currentpos = v1 + 2;
            for (int v2 = v0; v2 < currentpos; v2++) {
                char& v3 = data[v2];
                if (v3 != '\r' && v3 != '\n')
                    v3 = ' ';
            }
        }
    }
    {
        unsigned int currentpos = 0;
        unsigned int v0;
        while ((v0 = data.find("//", currentpos)) != BeepString::npos) {
            unsigned int v1 = v0;
            char* v2;
            while (*(v2 = &data[v1]) != 0) {
                if (*v2 == '\r' || *v2 == '\n')
                    break;
                *v2 = ' ';
                currentpos = ++v1;
            }
        }
    }
    return true;
}

// FUNCTION: USA 0x00034281
// PROTOTYPE: PREVIEW 0x004229EC identical
// NAME: proto-pdb:default.pdb
int BeepTDF::GetLineNumber(const BeepString& s, unsigned int loc, int startline)
{
    int lines = 0;
    unsigned int pos = 0;
    while ((pos = s.find_first_of("\n", pos + 1)) < loc)
        lines++;
    return startline + lines;
}

// FUNCTION: USA 0x000342AD
// PROTOTYPE: PREVIEW 0x00422A18 identical
// NAME: proto-pdb:default.pdb
unsigned int BeepTDF::FindMatchingBrace(const BeepString& s, unsigned int openbrace)
{
    int depth = 1;
    unsigned int pos = openbrace;
    for (;;) {
        pos = s.find_first_of("{}", pos + 1);
        if (pos == BeepString::npos)
            return BeepString::npos;
        if (s[pos] == '}') {
            if (--depth == 0)
                return pos;
        } else {
            depth++;
        }
    }
}

// FUNCTION: USA 0x000343A1
// PROTOTYPE: PREVIEW 0x00422B0C identical
// NAME: proto-pdb:default.pdb
BeepTDF::Record::Record(const BeepString& id, const BeepString& data) : name(id), stringdata(data)
{
    intdata = strtoul(data.c_str(), 0, 0);
    floatdata = atof(data.c_str());
    if (floatdata == 0.0 && intdata != 0)
        floatdata = intdata;
    if (_stricmp(data.c_str(), "true") == 0) {
        booldata = true;
        floatdata = 1.0;
        intdata = 1;
    } else if (_stricmp(data.c_str(), "false") == 0) {
        booldata = false;
    } else {
        booldata = intdata != 0;
    }
}

// FUNCTION: USA 0x00034475
// PROTOTYPE: PREVIEW 0x00422BE0 identical
// NAME: proto-pdb:default.pdb
const BeepTDF* BeepTDF::FindBlock(const BeepString& blockname, bool canfail) const
{
    unsigned int v0 = blockname.find_first_of("/\\");
    if (v0 != BeepString::npos) {
        const BeepTDF* v1 = FindBlock(blockname.substr(0, v0), canfail);
        if (v1)
            return v1->FindBlock(blockname.substr(v0 + 1), canfail);
        return 0;
    // !FAKE: c2 stack packing; else block; `it` takes the blockname param slot only inside it (99.74 -> 100)
    } else {
        std::map<BeepString, BeepTDF*>::const_iterator it = _blocklookup.find(BeepLib::MakeLower(blockname));
        if (it != _blocklookup.end())
            return it->second;
        return canfail ? 0 : beepState._TDFState._DefaultBlock;
    }
}

// FUNCTION: USA 0x0003453E
// PROTOTYPE: PREVIEW 0x00422CA9 identical
// NAME: proto-pdb:default.pdb
const BeepTDF::Record* BeepTDF::FindRecord(const BeepString& id, bool canfail) const
{
    unsigned int v0 = id.find_first_of("/\\");
    if (v0 != BeepString::npos) {
        const BeepTDF* v1 = FindBlock(id.substr(0, v0), canfail);
        if (v1)
            return v1->FindRecord(id.substr(v0 + 1), canfail);
        return 0;
    // !FAKE: c2 stack packing; else block; `it` takes the id param slot only inside it (99.74 -> 100)
    } else {
        std::map<BeepString, Record*>::const_iterator it = _recordlookup.find(BeepLib::MakeLower(id));
        if (it != _recordlookup.end())
            return it->second;
        return canfail ? 0 : beepState._TDFState._DefaultRecord;
    }
}

// FUNCTION: USA 0x00034607
// PROTOTYPE: PREVIEW 0x00422D72 identical
// NAME: proto-pdb:default.pdb
const BeepString BeepTDF::GetStringRecord(const BeepString& id, const BeepString& def) const
{
    const Record* r = FindRecord(id, true);
    if (r)
        return r->stringdata;
    return def;
}

// FUNCTION: USA 0x00034630
// PROTOTYPE: PREVIEW 0x00422D9B identical
// NAME: proto-pdb:default.pdb
int BeepTDF::GetIntRecord(const BeepString& id, int def) const
{
    const Record* r = FindRecord(id, true);
    return r ? r->intdata : def;
}

// FUNCTION: USA 0x0003464B
// PROTOTYPE: PREVIEW 0x00422DB6 identical
// NAME: proto-pdb:default.pdb
double BeepTDF::GetFloatRecord(const BeepString& id, double def) const
{
    const Record* r = FindRecord(id, true);
    return r ? r->floatdata : def;
}

// FUNCTION: USA 0x00034666
// PROTOTYPE: PREVIEW 0x00422DD1 identical
// NAME: proto-pdb:default.pdb
bool BeepTDF::GetBoolRecord(const BeepString& id, bool def) const
{
    const Record* r = FindRecord(id, true);
    return r ? r->booldata : def;
}

// FUNCTION: USA 0x00034F0D
// NAME: proto-pdb:default.pdb
BeepTDF::~BeepTDF()
{
    Cleanup();
}

// FUNCTION: USA 0x00034F42
// NAME: proto-pdb:default.pdb
BeepTDF::BeepTDF() : _valid(false)
{
}

// FUNCTION: USA 0x00034F77
// PROTOTYPE: PREVIEW 0x004236E2 identical
// NAME: proto-pdb:default.pdb
void BeepTDF::Cleanup()
{
    for (std::vector<Record*>::iterator i = _recordlist.begin(); i != _recordlist.end(); ++i)
        delete *i;
    for (std::vector<BeepTDF*>::iterator i = _blocklist.begin(); i != _blocklist.end(); ++i)
        delete *i;
    _recordlist.clear();
    _recordlookup.clear();
    _blocklist.clear();
    _recordlookup.clear();
}

// FUNCTION: USA 0x00034FE2
// PROTOTYPE: PREVIEW 0x0042374D identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 94.82, bytes mismatch (this/blockdata esi/edi swapped, local slot packing, sub not cached in ebx)
bool BeepTDF::ParseRawData(const BeepString& blockdata, const BeepString& trackingname, int startline)
{
    Cleanup();
    _valid = false;
    _blockname = trackingname;
    BeepString whitespace(" \t\r\n");
    unsigned int v0 = 0;
    for (;;) {
        v0 = blockdata.find_first_not_of(whitespace, v0);
        if (v0 == BeepString::npos)
            break;
        if (blockdata[v0] == '[') {
            unsigned int namebegin = blockdata.find_first_not_of(whitespace, v0 + 1);
            if (namebegin == BeepString::npos)
                return false;
            v0 = blockdata.find("]", namebegin);
            if (v0 == BeepString::npos)
                return false;
            unsigned int v1 = blockdata.find_last_not_of(whitespace, v0 - 1);
            if (v1 == BeepString::npos)
                return false;
            {
                BeepString blockname(blockdata, namebegin, v1 - namebegin + 1);
                unsigned int openbrace = v0 = blockdata.find("{", v0 + 1);
                if (openbrace == BeepString::npos)
                    return false;
                unsigned int endbrace = FindMatchingBrace(blockdata, openbrace);
                if (endbrace == BeepString::npos)
                    return false;
                BeepTDF* sub = new("BeepTDF.cpp", 166) BeepTDF;
                v0++;
                if (sub->ParseRawData(blockdata.substr(v0, endbrace - openbrace - 1), trackingname, GetLineNumber(blockdata, v0, startline))) {
                    _blocklist.push_back(sub);
                    _blocklookup.insert(std::map<BeepString, BeepTDF*>::value_type(BeepLib::MakeLower(blockname), sub));
                    sub->_blockname = blockname;
                    v0 = endbrace + 1;
                } else {
                    delete sub;
                    return false;
                }
            }
        } else if (isalnum(blockdata[v0]) || blockdata[v0] == '_') {
            unsigned int eq = blockdata.find("=", v0 + 1);
            if (eq == BeepString::npos)
                return false;
            unsigned int v1 = blockdata.find_last_not_of(whitespace, eq - 1);
            if (v1 == BeepString::npos)
                return false;
            BeepString varname(blockdata, v0, v1 - v0 + 1);
            unsigned int v2 = blockdata.find_first_not_of(whitespace, eq + 1);
            if (v2 == BeepString::npos)
                return false;
            if (blockdata[v2] == ';') {
                v0 = v2 + 1;
            } else {
                unsigned int terminator = blockdata.find(";", v2 + 1);
                if (terminator == BeepString::npos)
                    return false;
                unsigned int v3 = blockdata.find_last_not_of(whitespace, terminator - 1);
                if (v3 == BeepString::npos)
                    return false;
                BeepString vartext(blockdata, v2, v3 - v2 + 1);
                Record* r = new("BeepTDF.cpp", 228) Record(varname, vartext);
                _recordlist.push_back(r);
                _recordlookup.insert(std::map<BeepString, Record*>::value_type(BeepLib::MakeLower(varname), r));
                v0 = terminator + 1;
            }
        } else {
            return false;
        }
    }
    _valid = true;
    return true;
}
#endif

// FUNCTION: USA 0x00035370
// PROTOTYPE: PREVIEW 0x00423ADB identical
// NAME: proto-pdb:default.pdb
BeepTDF::BeepTDF(const BeepString& blockdata, const BeepString& trackingname, int startline)
{
    _valid = ParseRawData(blockdata, trackingname, startline);
}

// FUNCTION: USA 0x000353BA
// PROTOTYPE: PREVIEW 0x00423B25 identical
// NAME: proto-pdb:default.pdb
bool BeepTDF::LoadFile(BeepFile& f, const BeepString& trackingname)
{
    Cleanup();
    _valid = false;
    BeepString textdata((const char*)f.GetFileData());
    if (!Preprocess(textdata))
        return false;
    return _valid = ParseRawData(textdata, trackingname, 1);
}

// FUNCTION: USA 0x0003541A
// PROTOTYPE: PREVIEW 0x00423B85 identical
// NAME: proto-pdb:default.pdb
BeepTDF::BeepTDF(BeepFile& f, const BeepString& trackingname)
{
    _valid = LoadFile(f, trackingname);
}

// FUNCTION: USA 0x00035460
// PROTOTYPE: PREVIEW 0x00423BCB identical
// NAME: proto-pdb:default.pdb
bool BeepTDF::LoadFile(const BeepString& filename)
{
    Cleanup();
    _valid = false;
    BeepFile f;
    if (f.Open(filename))
        _valid = LoadFile(f, filename);
    return _valid;
}

// FUNCTION: USA 0x000354AE
// PROTOTYPE: PREVIEW 0x00423C19 identical
// NAME: proto-pdb:default.pdb
BeepTDF::BeepTDF(const BeepString& filename)
{
    _valid = LoadFile(filename);
}

// FUNCTION: USA 0x00033F69
// PROTOTYPE: PREVIEW 0x004226D4 identical
// NAME: proto-pdb:default.pdb
template unsigned int std::basic_string<char,std::char_traits<char>,std::allocator<char> >::find(char const *,unsigned int,unsigned int)const;

// FUNCTION: USA 0x00034002
// PROTOTYPE: PREVIEW 0x0042276D identical
// NAME: proto-pdb:default.pdb
template unsigned int std::basic_string<char,std::char_traits<char>,std::allocator<char> >::find_first_of(char const *,unsigned int,unsigned int)const;

// FUNCTION: USA 0x00034072
// PROTOTYPE: PREVIEW 0x004227DD identical
// NAME: proto-pdb:default.pdb
template unsigned int std::basic_string<char,std::char_traits<char>,std::allocator<char> >::find_first_not_of(char const *,unsigned int,unsigned int)const;

// FUNCTION: USA 0x000340DC
// PROTOTYPE: PREVIEW 0x00422847 identical
// NAME: proto-pdb:default.pdb
template unsigned int std::basic_string<char,std::char_traits<char>,std::allocator<char> >::find_last_not_of(char const *,unsigned int,unsigned int)const;
