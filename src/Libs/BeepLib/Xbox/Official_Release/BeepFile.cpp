// UNIT: src/Libs/BeepLib/Xbox/Official_Release/BeepFile.cpp OBJ: Xbox\Official_Release\BeepFile.obj OPT: /O1 /Oy /Oi
#include "../../../../include/decomp.h"
#include <string.h>

namespace std {

template<class _Elem>
struct char_traits {};

template<class _Ty>
class allocator {};

template<class _Elem, class _Traits = char_traits<_Elem>, class _Ax = allocator<_Elem> >
class basic_string {
public:
    class const_iterator { // NAME: proto-pdb:default.pdb
    public:
        const_iterator(const _Elem* _Ptr) { _Myptr = _Ptr; } // src proto-pdb:default.pdb const_iterator
        const _Elem* _Myptr;
    };
    class iterator : public const_iterator { // NAME: proto-pdb:default.pdb
    public:
        iterator(_Elem* _Ptr) : const_iterator(_Ptr) {}
    };

    basic_string() { _Myres = _BUF_SIZE - 1; _Eos(0); }
    basic_string(const basic_string& _Right); // src proto-pdb:default.pdb basic_string
    iterator begin() { return (iterator(_Myptr())); } // src proto-pdb:default.pdb begin
    // VC 7.1 body: cl keeps it out of line and emits the fold alias of BeepLib's const end (0x0003559F) byte-equal (C0233)
    iterator end() { return (iterator(_Myptr() + _Mysize)); } // src proto-pdb:default.pdb end
    ~basic_string() { _Tidy(true); }
    basic_string& operator=(const _Elem* _Ptr) { return assign(_Ptr); }
    basic_string& assign(const _Elem* _Ptr);
    basic_string& operator=(const basic_string& _Right) { return assign(_Right, 0, npos); }
    basic_string& assign(const basic_string& _Right, unsigned int _Roff, unsigned int _Count);
    static const unsigned int npos = (unsigned int)(-1);
    const _Elem* c_str() const { return _Myptr(); }
    unsigned int size() const { return _Mysize; }

    enum { _BUF_SIZE = 16 / sizeof(_Elem) < 1 ? 1 : 16 / sizeof(_Elem) };

protected:
    void _Tidy(bool _Built = false, unsigned int _Newsize = 0);
    void _Eos(unsigned int _Newsize);

    _Ax _Alval;         // 0x00
    union _Bxty { // src proto-pdb:default.pdb _Bxty
        _Elem _Buf[_BUF_SIZE];
        _Elem* _Ptr;
    } _Bx;              // 0x04

public:
    _Elem* _Myptr() { return (_BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf); }
    const _Elem* _Myptr() const { return (_BUF_SIZE <= _Myres ? _Bx._Ptr : _Bx._Buf); }

    unsigned int _Mysize; // 0x14
    unsigned int _Myres;  // 0x18
};

template<class _FwdIt, class _Ty>
void replace(_FwdIt _First, _FwdIt _Last, const _Ty& _Oldval, const _Ty& _Newval); // src proto-pdb:default.pdb replace

}

class FileHandler { // NAME: proto-pdb:default.pdb
public:
    virtual bool Open(const std::basic_string<char>& name, int mode) = 0;                         // vtable+0x00
    virtual void Close() = 0;                                                                      // vtable+0x04
    virtual bool Read(void* buf, unsigned int size) = 0;                                           // vtable+0x08
    virtual bool Write(const void* buf, unsigned int size) = 0;                                    // vtable+0x0C
    virtual bool IsOperationComplete() const;                                                      // vtable+0x10
    virtual unsigned int Size() = 0;                                                               // vtable+0x14
    virtual int Pos() = 0;                                                                         // vtable+0x18
    virtual void Seek(int pos) = 0;                                                                // vtable+0x1C
    virtual void SeekFromEnd(int pos) = 0;                                                         // vtable+0x20
    virtual void SeekRelative(int pos) = 0;                                                        // vtable+0x24
    virtual const void* GetFileData(const std::basic_string<char>& name);                          // vtable+0x28
    virtual void ReadAsync(unsigned int offset, void* data, unsigned int bytes, void* notification);// vtable+0x2C
    virtual void WriteAsync(unsigned int offset, const void* data, unsigned int bytes, void* notification); // vtable+0x30
    virtual ~FileHandler();                                                                        // vtable+0x34

    unsigned char* _filedata;          // 0x04
};
DECOMP_SIZE_ASSERT(FileHandler, 0x8);

struct _iobuf; // src proto-pdb:default.pdb _iobuf
extern "C" long __cdecl ftell(_iobuf* stream); // src proto-pdb:default.pdb ftell
extern "C" int __cdecl fseek(_iobuf* stream, long offset, int origin); // src proto-pdb:default.pdb fseek
extern "C" int __cdecl fclose(_iobuf* stream); // src proto-pdb:default.pdb fclose
extern "C" unsigned int __cdecl fread(void* buf, unsigned int size, unsigned int count, _iobuf* stream); // src proto-pdb:default.pdb fread
extern "C" unsigned int __cdecl fwrite(const void* buf, unsigned int size, unsigned int count, _iobuf* stream); // src proto-pdb:default.pdb fwrite
extern "C" _iobuf* __cdecl fopen(const char* filename, const char* mode); // src proto-pdb:default.pdb fopen

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

extern "C" int unzGetCurrentFileInfo(void* file, unz_file_info_s* pfile_info, char* szFileName, unsigned long fileNameBufferSize, void* extraField, unsigned long extraFieldBufferSize, char* szComment, unsigned long commentBufferSize); // NAME: proto-pdb:default.pdb
extern "C" int unzOpenCurrentFile(void* file); // NAME: proto-pdb:default.pdb
extern "C" int unzReadCurrentFile(void* file, void* buf, unsigned int len); // NAME: proto-pdb:default.pdb
extern "C" int unzCloseCurrentFile(void* file); // NAME: proto-pdb:default.pdb
void* BeepMalloc(unsigned int size, const char* file, int line); // NAME: proto-pdb:default.pdb

void BeepFree(void* p); // NAME: proto-pdb:default.pdb
void* operator new(unsigned int size, const char* file, int line);

class FileSystem { // NAME: proto-pdb:default.pdb
public:
    std::basic_string<char> FindDiskFile(const std::basic_string<char>& name) const;
    void* FindFileInBundles(const std::basic_string<char>& name) const;
    bool DiskFilesAllowed() const { return _DiskFilesAllowed; }

    char field_0[0x1C];                // 0x00
    bool _DiskFilesAllowed;            // 0x1C
    char field_1D[0x3];                // 0x1D
};
DECOMP_SIZE_ASSERT(FileSystem, 0x20);

class BeepState { // NAME: proto-pdb:default.pdb
    friend class BeepFile;
    char field_0[0x1C];                // 0x00
    FileSystem* _FileSystem;           // 0x1C
    char field_20[0xC38];              // 0x20
};
DECOMP_SIZE_ASSERT(BeepState, 0xC58);

extern BeepState beepState; // NAME: proto-pdb:default.pdb

class StandardFileHandler : public FileHandler { // NAME: proto-pdb:default.pdb
public:
    StandardFileHandler();
    virtual bool Open(const std::basic_string<char>& filename, int flags); // vtable+0x00
    virtual void Close();              // vtable+0x04
    virtual bool Read(void* data, unsigned int bytes); // vtable+0x08
    virtual bool Write(const void* data, unsigned int bytes); // vtable+0x0C
    virtual unsigned int Size();       // vtable+0x14
    virtual int Pos();                 // vtable+0x18
    virtual void Seek(int pos);        // vtable+0x1C
    virtual void SeekFromEnd(int pos); // vtable+0x20
    virtual void SeekRelative(int pos); // vtable+0x24

private:
    bool GenericOpen(const std::basic_string<char>& filename, const char* mode);

public:
    _iobuf* _f;                        // 0x08
    unsigned int _filesize;            // 0x0C
    bool _sizevalid;                   // 0x10
    bool _readonly;                    // 0x11
};
DECOMP_SIZE_ASSERT(StandardFileHandler, 0x14);

class ZipFileHandler : public FileHandler { // NAME: proto-pdb:default.pdb
public:
    ZipFileHandler(void* zip);
    virtual void Close();              // vtable+0x04
    virtual bool Open(const std::basic_string<char>& filename, int flags); // vtable+0x00
    virtual bool Read(void* data, unsigned int bytes); // vtable+0x08
    virtual bool Write(const void* data, unsigned int bytes); // vtable+0x0C
    virtual unsigned int Size();       // vtable+0x14
    virtual int Pos();                 // vtable+0x18
    virtual void Seek(int pos);        // vtable+0x1C
    virtual void SeekFromEnd(int pos); // vtable+0x20
    virtual void SeekRelative(int pos); // vtable+0x24

    void* _zip;                        // 0x08
    int _filesize;                     // 0x0C
    int _currentpos;                   // 0x10
};
DECOMP_SIZE_ASSERT(ZipFileHandler, 0x14);

class MemoryFileHandler : public FileHandler { // NAME: proto-pdb:default.pdb
public:
    MemoryFileHandler(const void* data, unsigned int size);
    MemoryFileHandler(unsigned int size); // retail 0x3729B (no accepted retail name)
    virtual bool Open(const std::basic_string<char>& filename, int flags); // vtable+0x00
    virtual void Close();              // vtable+0x04
    virtual bool Read(void* data, unsigned int bytes); // vtable+0x08
    virtual bool Write(const void* data, unsigned int bytes); // vtable+0x0C
    virtual unsigned int Size();       // vtable+0x14
    virtual int Pos();                 // vtable+0x18
    virtual void Seek(int pos);        // vtable+0x1C
    virtual void SeekFromEnd(int pos); // vtable+0x20
    virtual void SeekRelative(int pos); // vtable+0x24
    virtual const void* GetFileData(const std::basic_string<char>& name); // vtable+0x28

    const void* _data;                 // 0x08
    unsigned int _filesize;            // 0x0C
    unsigned int _pos;                 // 0x10
    bool _myMemory;                    // 0x14
};
DECOMP_SIZE_ASSERT(MemoryFileHandler, 0x18);

extern "C" int __stdcall CloseHandle(void* hObject); // src proto-pdb:default.pdb CloseHandle
extern "C" void* __stdcall CreateFileA(const char* lpFileName, unsigned long dwDesiredAccess, unsigned long dwShareMode, void* lpSecurityAttributes, unsigned long dwCreationDisposition, unsigned long dwFlagsAndAttributes, void* hTemplateFile); // NAME: proto-pdb:default.pdb
extern "C" unsigned long __stdcall SetFilePointer(void* hFile, long lDistanceToMove, long* lpDistanceToMoveHigh, unsigned long dwMoveMethod); // NAME: proto-pdb:default.pdb
extern "C" int __stdcall SetEndOfFile(void* hFile); // NAME: proto-pdb:default.pdb
extern "C" unsigned long __stdcall GetFileSize(void* hFile, unsigned long* lpFileSizeHigh); // NAME: proto-pdb:default.pdb

struct _OVERLAPPED { // NAME: proto-pdb:default.pdb
    unsigned long Internal;            // 0x00
    unsigned long InternalHigh;        // 0x04
    unsigned long Offset;              // 0x08
    unsigned long OffsetHigh;          // 0x0C
    void* hEvent;                      // 0x10
};

class AsyncFileHandler : public FileHandler { // NAME: proto-pdb:default.pdb
public:
    AsyncFileHandler(unsigned int desiredsize);
    virtual bool Open(const std::basic_string<char>& filename, int flags); // vtable+0x00
    virtual void Close();              // vtable+0x04
    virtual bool Read(void* data, unsigned int bytes); // vtable+0x08
    virtual bool Write(const void* data, unsigned int bytes); // vtable+0x0C
    virtual unsigned int Size();       // vtable+0x14
    virtual int Pos();                 // vtable+0x18
    virtual void Seek(int pos);        // vtable+0x1C
    virtual void SeekFromEnd(int pos); // vtable+0x20
    virtual void SeekRelative(int pos); // vtable+0x24

    void* _fh;                         // 0x08
    unsigned int _filesize;            // 0x0C
    unsigned int _pos;                 // 0x10
    _OVERLAPPED _overlappedInfo;       // 0x14
    unsigned int _desiredsize;         // 0x28
};
DECOMP_SIZE_ASSERT(AsyncFileHandler, 0x2C);

class BeepFile { // NAME: proto-pdb:default.pdb
public:
    BeepFile();
    ~BeepFile();
    void Close();
    bool Open(const std::basic_string<char>& filename);
    bool Open(const void* data, unsigned int size);
    void* Open(unsigned int size);
    bool OpenForWrite(const std::basic_string<char>& filename);
    bool OpenForAsync(const std::basic_string<char>& filename);
    bool Write(const void* buf, unsigned int size);
    bool Write(const std::basic_string<char>& s);
    unsigned int Size();
    int Pos();
    void Seek(int pos);
    void SeekRelative(int pos);
    const void* GetFileData();

private:
    FileHandler* _fh;                  // 0x00
    std::basic_string<char> _filename; // 0x04
};
DECOMP_SIZE_ASSERT(BeepFile, 0x20);

// FUNCTION: USA 0x00036FAF
// PROTOTYPE: PREVIEW 0x004256BD identical
// NAME: proto-pdb:default.pdb
void StandardFileHandler::Close()
{
    if (_f) {
        fclose(_f);
        _f = 0;
    }
    _sizevalid = false;
}

// FUNCTION: USA 0x00036FCA
// PROTOTYPE: PREVIEW 0x004256D8 identical
// NAME: proto-pdb:default.pdb
bool StandardFileHandler::Read(void* data, unsigned int bytes)
{
    return bytes == fread(data, 1, bytes, _f);
}

// FUNCTION: USA 0x00036FED
// PROTOTYPE: PREVIEW 0x004256FB identical
// NAME: proto-pdb:default.pdb
bool StandardFileHandler::Write(const void* data, unsigned int bytes)
{
    _sizevalid = false;
    return bytes == fwrite(data, 1, bytes, _f);
}

// FUNCTION: USA 0x00037014
// PROTOTYPE: PREVIEW 0x00425722 identical
// NAME: proto-pdb:default.pdb
unsigned int StandardFileHandler::Size()
{
    if (!_sizevalid) {
        int pos = Pos();
        SeekFromEnd(0);
        _filesize = Pos();
        Seek(pos);
        _sizevalid = true;
    }
    return _filesize;
}

// FUNCTION: USA 0x0003704A
// PROTOTYPE: PREVIEW 0x00425758 identical
// NAME: proto-pdb:default.pdb
int StandardFileHandler::Pos()
{
    return ftell(_f);
}

// FUNCTION: USA 0x00037054
// PROTOTYPE: PREVIEW 0x00425762 identical
// NAME: proto-pdb:default.pdb
void StandardFileHandler::Seek(int location)
{
    fseek(_f, location, 0);
}

// FUNCTION: USA 0x00037068
// PROTOTYPE: PREVIEW 0x00425776 identical
// NAME: proto-pdb:default.pdb
void StandardFileHandler::SeekFromEnd(int location)
{
    fseek(_f, location, 2);
}

// FUNCTION: USA 0x0003707C
// PROTOTYPE: PREVIEW 0x0042578A identical
// NAME: proto-pdb:default.pdb
void StandardFileHandler::SeekRelative(int offset)
{
    fseek(_f, offset, 1);
}

// FUNCTION: USA 0x000370A7
// PROTOTYPE: PREVIEW 0x004257B5 identical
// NAME: proto-pdb:default.pdb
bool ZipFileHandler::Open(const std::basic_string<char>& filename, int flags)
{
    Close();
    unz_file_info_s fileinfo;
    unzGetCurrentFileInfo(_zip, &fileinfo, 0, 0, 0, 0, 0, 0);
    _filesize = fileinfo.uncompressed_size;
    unzOpenCurrentFile(_zip);
    _filedata = (unsigned char*)BeepMalloc(_filesize + 1, "BeepFile.cpp", 347);
    // !FAKE: c2 register allocation; dummy local; _filesize cached in edi across the call without it (match_one 87.41 -> 100.0)
    int v0 = unzReadCurrentFile(_zip, _filedata, _filesize);
    if (v0 != _filesize) {
        Close();
        return false;
    }
    _filedata[_filesize] = 0;
    unzCloseCurrentFile(_zip);
    _currentpos = 0;
    return true;
}

// FUNCTION: USA 0x0003712A
// PROTOTYPE: PREVIEW 0x00425838 identical
// NAME: proto-pdb:default.pdb
void ZipFileHandler::Close()
{
    BeepFree(_filedata);
    _filedata = 0;
}

// FUNCTION: USA 0x0003713C
// PROTOTYPE: PREVIEW 0x0042584A identical
// NAME: proto-pdb:default.pdb
bool ZipFileHandler::Read(void* data, unsigned int bytes)
{
    if (_currentpos + bytes > _filesize)
        return false;
    memcpy(data, _filedata + _currentpos, bytes);
    _currentpos += bytes;
    return true;
}

// FUNCTION: USA 0x00037178
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0); matched_set refuses: pin has no size in symbols_pin.txt (lift: size on the pin)
bool ZipFileHandler::Write(const void* data, unsigned int bytes)
{
    return false;
}
#endif

// FUNCTION: USA 0x0003717D
// PROTOTYPE: PREVIEW 0x00425980 identical
// NAME: proto-pdb:default.pdb
void ZipFileHandler::SeekRelative(int offset)
{
    _currentpos += offset;
}

// FUNCTION: USA 0x000371B6
// PROTOTYPE: PREVIEW 0x004258BA identical
// NAME: proto-pdb:default.pdb
void AsyncFileHandler::Close()
{
    if (_fh != (void*)-1) {
        CloseHandle(_fh);
        _fh = (void*)-1;
    }
    _filesize = 0;
    _pos = 0;
}

// FUNCTION: USA 0x00037291
// PROTOTYPE: PREVIEW 0x00425976 identical
// NAME: proto-pdb:default.pdb
void ZipFileHandler::Seek(int location)
{
    _currentpos = location;
}

// FUNCTION: USA 0x00037323
// PROTOTYPE: PREVIEW 0x00425A12 identical
// NAME: proto-pdb:default.pdb
bool MemoryFileHandler::Read(void* data, unsigned int bytes)
{
    if (_pos + bytes > _filesize)
        return false;
    memcpy(data, (const char*)_data + _pos, bytes);
    _pos += bytes;
    return true;
}

// FUNCTION: USA 0x0003735F
// PROTOTYPE: PREVIEW 0x00425A4E identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0); matched_set refuses: pin has no size in symbols_pin.txt:4130 (lift: size 0x4 on the pin)
unsigned int ZipFileHandler::Size()
{
    return _filesize;
}
#endif

// FUNCTION: USA 0x00037363
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes match (masked 0); matched_set refuses: pin has no size in symbols_pin.txt (lift: size on the pin)
int ZipFileHandler::Pos()
{
    return _currentpos;
}
#endif

// FUNCTION: USA 0x00037367
// PROTOTYPE: PREVIEW 0x00425A56 identical
// NAME: proto-pdb:default.pdb
void ZipFileHandler::SeekFromEnd(int location)
{
    _currentpos = _filesize - location;
}

// FUNCTION: USA 0x00037374
// PROTOTYPE: PREVIEW 0x00425A63 identical
// NAME: proto-pdb:default.pdb
const void* MemoryFileHandler::GetFileData(const std::basic_string<char>& filename)
{
    return _data;
}

// FUNCTION: USA 0x000373AD
// PROTOTYPE: PREVIEW 0x00425A9C identical
// NAME: proto-pdb:default.pdb
bool BeepFile::Write(const void* data, unsigned int bytes)
{
    if (_fh)
        return _fh->Write(data, bytes);
    return false;
}

// FUNCTION: USA 0x000373BD
// PROTOTYPE: PREVIEW 0x00425AAC identical
// NAME: proto-pdb:default.pdb
unsigned int BeepFile::Size()
{
    if (_fh)
        return _fh->Size();
    return 0;
}

// FUNCTION: USA 0x000373DA
// PROTOTYPE: PREVIEW 0x00425ABA identical
// NAME: proto-pdb:default.pdb
int BeepFile::Pos()
{
    if (_fh)
        return _fh->Pos();
    return 0;
}

// FUNCTION: USA 0x000373E8
// PROTOTYPE: PREVIEW 0x00425AC8 identical
// NAME: proto-pdb:default.pdb
void BeepFile::Seek(int location)
{
    if (_fh)
        _fh->Seek(location);
}

// FUNCTION: USA 0x000373F6
// PROTOTYPE: PREVIEW 0x00425AD6 identical
// NAME: proto-pdb:default.pdb
void BeepFile::SeekRelative(int offset)
{
    if (_fh)
        _fh->SeekRelative(offset);
}

// FUNCTION: USA 0x00037404
// PROTOTYPE: PREVIEW 0x00425AE4 identical
// NAME: proto-pdb:default.pdb
const void* BeepFile::GetFileData()
{
    if (_fh)
        return _fh->GetFileData(_filename);
    return 0;
}

// FUNCTION: USA 0x00037527
// PROTOTYPE: PREVIEW 0x00425BFB identical
// NAME: proto-pdb:default.pdb
bool BeepFile::Write(const std::basic_string<char>& s)
{
    return Write(s.c_str(), s.size());
}

// FUNCTION: USA 0x00037546
// PROTOTYPE: PREVIEW 0x00425C1A identical
// NAME: proto-pdb:default.pdb
BeepFile::BeepFile()
    : _fh(0)
{
}

// FUNCTION: USA 0x00037558
// PROTOTYPE: PREVIEW 0x00425C2C identical
// NAME: proto-pdb:default.pdb
bool StandardFileHandler::GenericOpen(const std::basic_string<char>& filename, const char* mode)
{
    Close();
    std::basic_string<char> fname(filename);
    std::replace(fname.begin(), fname.end(), '/', '\\');
    _f = fopen(fname.c_str(), mode);
    return _f != 0;
}

// FUNCTION: USA 0x000375DA
// PROTOTYPE: PREVIEW 0x00425CAE identical
// NAME: proto-pdb:default.pdb
bool StandardFileHandler::Open(const std::basic_string<char>& filename, int flags)
{
    _readonly = (flags & 3) == 0;
    if (flags & 1)
        return GenericOpen(filename, "wb");
    if (flags & 2)
        return GenericOpen(filename, "r+b");
    return GenericOpen(filename, "rb");
}

// FUNCTION: USA 0x00037616
// PROTOTYPE: PREVIEW 0x00425CEA identical
// NAME: proto-pdb:default.pdb
bool AsyncFileHandler::Open(const std::basic_string<char>& filename, int flags)
{
    Close();
    std::basic_string<char> fname(filename);
    std::replace(fname.begin(), fname.end(), '/', '\\');
    unsigned long access = 0x80000000;
    unsigned long disposition = 3;
    if (flags & 1) {
        access = 0xC0000000;
        disposition = 4;
    }
    _fh = CreateFileA(fname.c_str(), access, 1, 0, disposition, 0x60000000, 0);
    if (_fh != (void*)-1) {
        if ((flags & 1) && _desiredsize) {
            SetFilePointer(_fh, _desiredsize, 0, 0);
            SetEndOfFile(_fh);
            SetFilePointer(_fh, 0, 0, 0);
            _filesize = _desiredsize;
        } else {
            _filesize = GetFileSize(_fh, 0);
        }
        return true;
    }
    return false;
}

// FUNCTION: USA 0x000376F7
// PROTOTYPE: PREVIEW 0x00425DCB identical
// NAME: proto-pdb:default.pdb
void BeepFile::Close()
{
    delete _fh;
    _fh = 0;
    _filename = "";
}

// FUNCTION: USA 0x00037719
// PROTOTYPE: PREVIEW 0x00425DED identical
// NAME: proto-pdb:default.pdb
BeepFile::~BeepFile()
{
    Close();
}

// FUNCTION: USA 0x0003772F
// PROTOTYPE: PREVIEW 0x00425E03 near-identical
// NAME: proto-pdb:default.pdb
bool BeepFile::Open(const std::basic_string<char>& filename)
{
    Close();
    std::basic_string<char> openpath;
    FileHandler* fh = 0;
    if (beepState._FileSystem->DiskFilesAllowed()) {
        openpath = beepState._FileSystem->FindDiskFile(filename);
        if (openpath.size())
            fh = new("BeepFile.cpp", 847) StandardFileHandler;
    }
    if (!fh) {
        void* zip = beepState._FileSystem->FindFileInBundles(filename);
        if (zip)
            fh = new("BeepFile.cpp", 858) ZipFileHandler(zip);
    }
    if (fh) {
        if (fh->Open(openpath, 0)) {
            _fh = fh;
            _filename = openpath;
            return true;
        }
        delete fh;
    }
    return false;
}

// FUNCTION: USA 0x00037832
// PROTOTYPE: PREVIEW 0x00425F06 near-identical
// NAME: proto-pdb:default.pdb
bool BeepFile::Open(const void* data, unsigned int size)
{
    Close();
    _fh = new("BeepFile.cpp", 923) MemoryFileHandler(data, size);
    return true;
}

#ifdef NON_MATCHING
// best: match_one objdiff 100.0, bytes provisional (masked 1): near-callee-unnamed
// FUNCTION: USA 0x0003786D
// PROTOTYPE: PREVIEW 0x00425F41 near-identical
// NAME: proto-pdb:default.pdb
void* BeepFile::Open(unsigned int size)
{
    Close();
    _fh = new("BeepFile.cpp", 931) MemoryFileHandler(size);
    return (void*)_fh->GetFileData(_filename);
}
#endif

// FUNCTION: USA 0x000378AD
// PROTOTYPE: PREVIEW 0x00425F81 near-identical
// NAME: proto-pdb:default.pdb
bool BeepFile::OpenForWrite(const std::basic_string<char>& filename)
{
    Close();
    FileHandler* fh = new("BeepFile.cpp", 942) StandardFileHandler;
    if (fh->Open(filename, 1)) {
        _fh = fh;
        _filename = filename;
        return true;
    }
    delete fh;
    return false;
}

// FUNCTION: USA 0x00037911
// PROTOTYPE: PREVIEW 0x00425FE5 near-identical
// NAME: proto-pdb:default.pdb
bool BeepFile::OpenForAsync(const std::basic_string<char>& filename)
{
    Close();
    FileHandler* fh = new("BeepFile.cpp", 983) AsyncFileHandler(0);
    if (fh->Open(filename, 0)) {
        _fh = fh;
        _filename = filename;
        return true;
    }
    delete fh;
    return false;
}
