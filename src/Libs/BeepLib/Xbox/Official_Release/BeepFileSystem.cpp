// UNIT: src/Libs/BeepLib/Xbox/Official_Release/BeepFileSystem.cpp OBJ: Xbox\Official_Release\BeepFileSystem.obj OPT: /O1 /Oy
#include "../../../../include/decomp.h"

extern "C" int unzClose(void* file); // NAME: proto-pdb:default.pdb

struct tm_unz_s { // NAME: proto-pdb:default.pdb
    unsigned int tm_sec;               // 0x00
    unsigned int tm_min;               // 0x04
    unsigned int tm_hour;              // 0x08
    unsigned int tm_mday;              // 0x0C
    unsigned int tm_mon;               // 0x10
    unsigned int tm_year;              // 0x14
};
DECOMP_SIZE_ASSERT(tm_unz_s, 0x18);

struct unz_file_info_s { // NAME: proto-pdb:default.pdb
    unsigned long version;             // 0x00
    unsigned long version_needed;      // 0x04
    unsigned long flag;                // 0x08
    unsigned long compression_method;  // 0x0C
    unsigned long dosDate;             // 0x10
    unsigned long crc;                 // 0x14
    unsigned long compressed_size;     // 0x18
    unsigned long uncompressed_size;   // 0x1C
    unsigned long size_filename;       // 0x20
    unsigned long size_file_extra;     // 0x24
    unsigned long size_file_comment;   // 0x28
    unsigned long disk_num_start;      // 0x2C
    unsigned long internal_fa;         // 0x30
    unsigned long external_fa;         // 0x34
    tm_unz_s tmu_date;                 // 0x38
};
DECOMP_SIZE_ASSERT(unz_file_info_s, 0x50);

extern "C" int unzLocateFile(void* file, const char* szFileName, int iCaseSensitivity); // NAME: proto-pdb:default.pdb
extern "C" int unzGetCurrentFileInfo(void* file, unz_file_info_s* pfile_info, char* szFileName, unsigned long fileNameBufferSize, void* extraField, unsigned long extraFieldBufferSize, char* szComment, unsigned long commentBufferSize); // NAME: proto-pdb:default.pdb

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
    unsigned long nFileSizeHigh;       // 0x1C
    unsigned long nFileSizeLow;        // 0x20
    unsigned long dwReserved0;         // 0x24
    unsigned long dwReserved1;         // 0x28
    char cFileName[260];               // 0x2C
    char cAlternateFileName[14];       // 0x130
};
DECOMP_SIZE_ASSERT(_WIN32_FIND_DATAA, 0x140);

extern "C" void* __stdcall FindFirstFileA(const char* lpFileName, _WIN32_FIND_DATAA* lpFindFileData); // NAME: proto-pdb:default.pdb
extern "C" int __stdcall CloseHandle(void* hObject); // NAME: proto-pdb:default.pdb

namespace std { // NAME: proto-pdb:default.pdb
template <class E> struct char_traits;
template <class T> class allocator;
template <class E, class T, class A> class basic_string {
public:
    class const_iterator {
    public:
        const_iterator(const E* _Ptr) : _Myptr(_Ptr) {}
        const E* _Myptr;
    };
    class iterator : public const_iterator {
    public:
        iterator(E* _Ptr) : const_iterator(_Ptr) {}
    };
    basic_string(const E* _Ptr);
    basic_string(const basic_string& _Right);
    ~basic_string() { _Tidy(true); }
    iterator begin() { return iterator(_Myptr()); }
    iterator end();
    const E* c_str() const { return _Myptr(); }
    unsigned int find(const E* _Ptr, unsigned int _Off = 0) const;
    static const unsigned int npos = (unsigned int)(-1);

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);
    enum { _BUF_SIZE = 16 / sizeof(E) < 1 ? 1 : 16 / sizeof(E) };
    E* _Myptr() { return (_BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf); }
    const E* _Myptr() const { return (_BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf); }

private:
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
template <class _FwdIt, class _Ty>
void replace(_FwdIt _First, _FwdIt _Last, const _Ty& _Oldval, const _Ty& _Newval);
template <class T> struct less;
template <class T1, class T2> struct pair {
    T1 first;                          // 0x00
    T2 second;
};
template <class K, class T, class Pr, class A, bool Mfl> class _Tmap_traits {
public:
    typedef pair<const K, T> value_type;
};
template <class Tr> class _Tree {
    struct _Node {
        _Node* _Left;                  // 0x00
        _Node* _Parent;                // 0x04
        _Node* _Right;                 // 0x08
        typename Tr::value_type _Myval; // 0x0C
        char _Color;                   // 0x2C
        char _Isnil;                   // 0x2D
    };
public:
    class const_iterator {
    public:
        const_iterator(_Node* _Pnode) : _Ptr(_Pnode) {}
        bool operator!=(const const_iterator& _Right) const { return _Ptr != _Right._Ptr; }
        const typename Tr::value_type& operator*() const { return _Myval(_Ptr); }
        const typename Tr::value_type* operator->() const { return &**this; }
        const_iterator& operator++() { _Inc(); return *this; }
        void _Inc()
        {
            if (_Ptr->_Isnil)
                ;
            else if (!_Ptr->_Right->_Isnil)
                _Ptr = _Min(_Ptr->_Right);
            else {
                _Node* _Pnode;
                while (!(_Pnode = _Ptr->_Parent)->_Isnil && _Ptr == _Pnode->_Right)
                    _Ptr = _Pnode;
                _Ptr = _Pnode;
            }
        }
        _Node* _Ptr;
    };
    class iterator : public const_iterator {
    public:
        iterator(_Node* _Pnode) : const_iterator(_Pnode) {}
        typename Tr::value_type* operator->() const { return &this->_Ptr->_Myval; }
        iterator& operator++() { this->_Inc(); return *this; }
    };
    static typename Tr::value_type& _Myval(_Node* _Pnode) { return _Pnode->_Myval; }
    static _Node* _Min(_Node* _Pnode)
    {
        while (!_Pnode->_Left->_Isnil)
            _Pnode = _Pnode->_Left;
        return (_Pnode);
    }
    _Tree() { _Init(); }
    ~_Tree() { _Tidy(); }
    iterator begin() { return iterator(_Myhead->_Left); }
    iterator end() { return iterator(_Myhead); }
    const_iterator begin() const { return const_iterator(_Myhead->_Left); }
    const_iterator end() const { return const_iterator(_Myhead); }

protected:
    void _Init();
    void _Tidy();

private:
    char _Alval;                       // 0x00
    _Node* _Myhead;                    // 0x04
    unsigned int _Mysize;              // 0x08
};
template <class K, class T, class Pr = less<K>, class A = allocator<pair<const K, T> > > class map
    : public _Tree<_Tmap_traits<K, T, Pr, A, false> > {
};
template <class T, class A = allocator<T> > class vector {
public:
    class const_iterator {
    public:
        const_iterator(T* _Ptr) : _Myptr(_Ptr) {}
        const T& operator*() const { return *_Myptr; }
        const_iterator& operator++() { ++_Myptr; return *this; }
        const_iterator operator-(int _Off) const { return const_iterator(_Myptr - _Off); }
        bool operator!=(const const_iterator& _Right) const { return _Myptr != _Right._Myptr; }
        T* _Myptr;
    };
    vector() : _Myfirst(0), _Mylast(0), _Myend(0) {}
    const_iterator begin() const { return const_iterator(_Myfirst); }
    const_iterator end() const { return const_iterator(_Mylast); }
    ~vector() { _Tidy(); }
    void push_back(const T& _Val);

protected:
    void _Tidy();

private:
    char _Alval;                       // 0x00
    T* _Myfirst;                       // 0x04
    T* _Mylast;                        // 0x08
    T* _Myend;                         // 0x0C
};
}
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > SwampString;

class FileSystem { // NAME: proto-pdb:default.pdb
public:
    typedef std::map<SwampString, void*> ZipFiles; // NAME: proto-pdb:default.pdb
    typedef std::vector<SwampString> Directories; // NAME: proto-pdb:default.pdb

    FileSystem();
    ~FileSystem();
    SwampString FindDiskFile(const SwampString& filename) const;
    void* FindFileInBundles(const SwampString& filename) const;

    ZipFiles _ZipFiles;                // 0x00
    Directories _Dirs;                 // 0x0C
    bool _DiskFilesAllowed;            // 0x1C
};
DECOMP_SIZE_ASSERT(FileSystem, 0x20);

// FUNCTION: USA 0x00037AF2
// NAME: proto-pdb:default.pdb
void* FileSystem::FindFileInBundles(const SwampString& filename) const
{
    void* best_bundle = 0;
    unsigned long best_date = 0;
    for (ZipFiles::const_iterator it = _ZipFiles.begin(); it != _ZipFiles.end(); ++it) {
        if (unzLocateFile(it->second, filename.c_str(), 2) == 0) {
            unz_file_info_s fileinfo;
            unzGetCurrentFileInfo(it->second, &fileinfo, 0, 0, 0, 0, 0, 0);
            if (fileinfo.dosDate > best_date) {
                best_date = fileinfo.dosDate;
                best_bundle = it->second;
            }
        }
    }
    return best_bundle;
}

// FUNCTION: USA 0x00037CD0
// NAME: proto-pdb:default.pdb
SwampString FileSystem::FindDiskFile(const SwampString& filename) const
{
    SwampString name(filename);
    std::replace(name.begin(), name.end(), '\\', '/');
    Directories::const_iterator v0 = _Dirs.begin(); // !FAKE: no PDB local for the directory iterator
    if (name.c_str()[0] == '.' || name.c_str()[0] == '/' || name.find(":") != SwampString::npos)
        v0 = _Dirs.end() - 1;
    // !FAKE: guarded do/while with the replace chars set once: retail hoists them (its replace body is visible,
    // which here would emit another unit's matched row 0x0003562A)
    if (v0 != _Dirs.end()) {
        const char v3 = '\\'; // !FAKE
        const char v4 = '/'; // !FAKE
        do {
            {
                SwampString fullpath = *v0 + name;
                SwampString searchstring(fullpath);
                std::replace(searchstring.begin(), searchstring.end(), v4, v3);
                _WIN32_FIND_DATAA fdata;
                void* v2 = FindFirstFileA(searchstring.c_str(), &fdata); // !FAKE: no PDB local for the handle
                if (v2 != (void*)-1) {
                    CloseHandle(v2);
                    if ((fdata.dwFileAttributes & 0x10) == 0)
                        return fullpath;
                }
            }
            ++v0;
        } while (v0 != _Dirs.end());
    }
    return SwampString("");
}

// FUNCTION: USA 0x000381BF
// PROTOTYPE: PREVIEW 0x004268B6 identical
// NAME: proto-pdb:default.pdb
FileSystem::~FileSystem()
{
    for (ZipFiles::iterator it = _ZipFiles.begin(); it != _ZipFiles.end(); ++it)
        unzClose(it->second);
}

// FUNCTION: USA 0x000381FD
// PROTOTYPE: PREVIEW 0x004268F4 identical
// NAME: proto-pdb:default.pdb
FileSystem::FileSystem() : _DiskFilesAllowed(true)
{
    _Dirs.push_back("");
}
