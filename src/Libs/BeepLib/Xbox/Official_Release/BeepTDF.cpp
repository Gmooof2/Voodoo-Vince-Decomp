// UNIT: src/Libs/BeepLib/Xbox/Official_Release/BeepTDF.cpp OBJ: Xbox\Official_Release\BeepTDF.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

namespace std { // NAME: proto-pdb:default.pdb
template <class _Elem> struct char_traits;
template <class _Ty> class allocator;
template <class _Ty> struct less;
template <class _Ty1, class _Ty2> struct pair;

template <class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
class basic_string {
public:
    typedef unsigned int size_type;

    basic_string();
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    const _Elem& operator[](size_type _Off) const;
    size_type find_first_of(const _Elem* _Ptr, size_type _Off = 0) const; // src proto-pdb:default.pdb find_first_of

    static const size_type npos;

protected:
    void _Tidy(bool _Built = false, size_type _Newsize = 0);

private:
    char _pad00[0x1C];
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
    void clear() { _Tidy(); }

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    _Ty* _Myfirst;                     // 0x04
    _Ty* _Mylast;                      // 0x08
    _Ty* _Myend;                       // 0x0C
};

template <class _Kty, class _Ty, class _Pr, class _Alloc, bool _Mfl> class _Tmap_traits {};

template <class _Traits> class _Tree {
public:
    _Tree() { _Init(); }
    ~_Tree() { _Tidy(); }
    void clear();

protected:
    void _Init();
    void _Tidy();

private:
    char _Alval;                       // 0x00
    void* _Myhead;                     // 0x04
    unsigned int _Mysize;              // 0x08
};

template <class _Kty, class _Ty, class _Pr = less<_Kty>, class _Alloc = allocator<pair<const _Kty, _Ty> > >
class map : public _Tree<_Tmap_traits<_Kty, _Ty, _Pr, _Alloc, false> > {
public:
    map() {}
};
}

typedef std::basic_string<char> BeepString;

class BeepFile; // NAME: proto-pdb:default.pdb

class BeepTDF { // NAME: proto-pdb:default.pdb
public:
    struct Record { // NAME: proto-pdb:default.pdb
        BeepString name;               // 0x00
        BeepString stringdata;         // 0x1C
        int intdata;                   // 0x38
        double floatdata;              // 0x40
        bool booldata;                 // 0x48

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
    const Record* FindRecord(const BeepString& id, bool canfail) const;
    const BeepString GetStringRecord(const BeepString& id, const BeepString& def) const;
    int GetIntRecord(const BeepString& id, int def) const;
    double GetFloatRecord(const BeepString& id, double def) const;
    bool GetBoolRecord(const BeepString& id, bool def) const;

private:
    void Cleanup();
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

// FUNCTION: USA 0x00035370
// PROTOTYPE: PREVIEW 0x00423ADB identical
// NAME: proto-pdb:default.pdb
BeepTDF::BeepTDF(const BeepString& blockdata, const BeepString& trackingname, int startline)
{
    _valid = ParseRawData(blockdata, trackingname, startline);
}

// FUNCTION: USA 0x0003541A
// PROTOTYPE: PREVIEW 0x00423B85 identical
// NAME: proto-pdb:default.pdb
BeepTDF::BeepTDF(BeepFile& f, const BeepString& trackingname)
{
    _valid = LoadFile(f, trackingname);
}

// FUNCTION: USA 0x000354AE
// PROTOTYPE: PREVIEW 0x00423C19 identical
// NAME: proto-pdb:default.pdb
BeepTDF::BeepTDF(const BeepString& filename)
{
    _valid = LoadFile(filename);
}
