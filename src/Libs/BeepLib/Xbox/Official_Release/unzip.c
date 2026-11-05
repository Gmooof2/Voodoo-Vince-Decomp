// UNIT: src/Libs/BeepLib/Xbox/Official_Release/unzip.c OBJ: Xbox\Official_Release\unzip.obj OPT: /O1 /Oy
/* Public zlib contrib/minizip unzip.c source, altered: declarations in-file, unmatched procs out.
 * unzip.c -- IO on .zip files using zlib
 * Version 0.15 beta, Mar 19th, 1998, Copyright (C) 1998 Gilles Vollant
 */
#include "../../../../include/decomp.h"
#include <stdio.h>
#include <string.h>
#pragma intrinsic(strlen) // retail inlines strlen in unzLocateFile under /O1 /Oy (C0181)

typedef unsigned char  Byte;
typedef unsigned int   uInt;
typedef unsigned long  uLong;
typedef Byte           Bytef;
typedef void          *voidpf;
typedef void          *voidp;

typedef voidp unzFile;

#define UNZ_OK          (0)
#define UNZ_PARAMERROR  (-102)
#define UNZ_CRCERROR    (-105)

typedef voidpf (*alloc_func)(voidpf opaque, uInt items, uInt size);
typedef void   (*free_func)(voidpf opaque, voidpf address);

struct internal_state;

typedef struct z_stream_s { // NAME: proto-pdb:default.pdb
    Bytef    *next_in;                 // 0x00
    uInt     avail_in;                 // 0x04
    uLong    total_in;                 // 0x08
    Bytef    *next_out;                // 0x0C
    uInt     avail_out;                // 0x10
    uLong    total_out;                // 0x14
    char     *msg;                     // 0x18
    struct internal_state *state;      // 0x1C
    alloc_func zalloc;                 // 0x20
    free_func  zfree;                  // 0x24
    voidpf     opaque;                 // 0x28
    int     data_type;                 // 0x2C
    uLong   adler;                     // 0x30
    uLong   reserved;                  // 0x34
} z_stream;
DECOMP_SIZE_ASSERT(z_stream, 0x38);

int inflateEnd(z_stream *z); // NAME: proto-pdb:default.pdb
int inflate(z_stream *z, int f); // NAME: proto-pdb:default.pdb
uLong crc32(uLong crc, const Bytef *buf, uInt len); // NAME: proto-pdb:default.pdb

#define Z_OK            0
#define Z_STREAM_END    1
#define Z_ERRNO        (-1)
#define Z_SYNC_FLUSH    2
void BeepZLIB_Free(void *p); // NAME: proto-pdb:default.pdb

#define TRYFREE(p) BeepZLIB_Free(p)

typedef struct file_in_zip_read_info_s { // NAME: proto-pdb:default.pdb
    char  *read_buffer;                // 0x00
    z_stream stream;                   // 0x04
    uLong pos_in_zipfile;              // 0x3C
    uLong stream_initialised;          // 0x40
    uLong offset_local_extrafield;     // 0x44
    uInt  size_local_extrafield;       // 0x48
    uLong pos_local_extrafield;        // 0x4C
    uLong crc32;                       // 0x50
    uLong crc32_wait;                  // 0x54
    uLong rest_read_compressed;        // 0x58
    uLong rest_read_uncompressed;      // 0x5C
    FILE* file;                        // 0x60
    uLong compression_method;          // 0x64
    uLong byte_before_the_zipfile;     // 0x68
} file_in_zip_read_info_s;
DECOMP_SIZE_ASSERT(file_in_zip_read_info_s, 0x6C);

typedef struct unz_global_info_s { // NAME: proto-pdb:default.pdb
    uLong number_entry;                // 0x00
    uLong size_comment;                // 0x04
} unz_global_info;
DECOMP_SIZE_ASSERT(unz_global_info, 0x8);

typedef struct tm_unz_s { // NAME: proto-pdb:default.pdb
    uInt tm_sec;                       // 0x00
    uInt tm_min;                       // 0x04
    uInt tm_hour;                      // 0x08
    uInt tm_mday;                      // 0x0C
    uInt tm_mon;                       // 0x10
    uInt tm_year;                      // 0x14
} tm_unz;
DECOMP_SIZE_ASSERT(tm_unz, 0x18);

typedef struct unz_file_info_s { // NAME: proto-pdb:default.pdb
    uLong version;                     // 0x00
    uLong version_needed;              // 0x04
    uLong flag;                        // 0x08
    uLong compression_method;          // 0x0C
    uLong dosDate;                     // 0x10
    uLong crc;                         // 0x14
    uLong compressed_size;             // 0x18
    uLong uncompressed_size;           // 0x1C
    uLong size_filename;               // 0x20
    uLong size_file_extra;             // 0x24
    uLong size_file_comment;           // 0x28
    uLong disk_num_start;              // 0x2C
    uLong internal_fa;                 // 0x30
    uLong external_fa;                 // 0x34
    tm_unz tmu_date;                   // 0x38
} unz_file_info;
DECOMP_SIZE_ASSERT(unz_file_info, 0x50);

typedef struct unz_file_info_internal_s { // NAME: proto-pdb:default.pdb
    uLong offset_curfile;              // 0x00
} unz_file_info_internal;
DECOMP_SIZE_ASSERT(unz_file_info_internal, 0x4);

typedef struct unz_s { // NAME: proto-pdb:default.pdb
    FILE* file;                       // 0x00
    unz_global_info gi;               // 0x04
    uLong byte_before_the_zipfile;    // 0x0C
    uLong num_file;                   // 0x10
    uLong pos_in_central_dir;         // 0x14
    uLong current_file_ok;            // 0x18
    uLong central_pos;                // 0x1C
    uLong size_central_dir;           // 0x20
    uLong offset_central_dir;         // 0x24
    unz_file_info cur_file_info;      // 0x28
    unz_file_info_internal cur_file_info_internal; // 0x78
    file_in_zip_read_info_s* pfile_in_zip_read; // 0x7C
} unz_s;
DECOMP_SIZE_ASSERT(unz_s, 0x80);

#define UNZ_END_OF_LIST_OF_FILE (-100)
#define UNZ_EOF         (0)
#define UNZ_ERRNO       (Z_ERRNO)
#define UNZ_BUFSIZE     (16384)
#define SIZECENTRALDIRITEM (0x2e)

// retail 0x000388BB; not matched (its static callees are unnamed), so declared only
int unzlocal_GetCurrentFileInfoInternal(unzFile file, unz_file_info *pfile_info, // NAME: proto-pdb:default.pdb
    unz_file_info_internal *pfile_info_internal, char *szFileName, uLong fileNameBufferSize,
    void *extraField, uLong extraFieldBufferSize, char *szComment, uLong commentBufferSize);

// FUNCTION: USA 0x00038B65
// PROTOTYPE: PREVIEW 0x0042725C identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 86.6, bytes mismatch (retail passes file in eax: callee's static register convention, PDB file r18)
int unzGetCurrentFileInfo(unzFile file, unz_file_info *pfile_info, char *szFileName, uLong fileNameBufferSize,
    void *extraField, uLong extraFieldBufferSize, char *szComment, uLong commentBufferSize)
{
    return unzlocal_GetCurrentFileInfoInternal(file, pfile_info, NULL, szFileName, fileNameBufferSize,
                                               extraField, extraFieldBufferSize, szComment, commentBufferSize);
}
#endif

// FUNCTION: USA 0x00038B8C
// PROTOTYPE: PREVIEW 0x00427283 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 87.125, bytes mismatch (push file vs retail mov eax,file: callee's static register convention)
int unzGoToFirstFile(unzFile file)
{
    // !FAKE: provenance; neutral stand-ins for minizip's err and s; the PDB records no local
    int v1 = UNZ_OK;
    unz_s* v0;
    if (file == NULL)
        return UNZ_PARAMERROR;
    v0 = (unz_s*)file;
    v0->pos_in_central_dir = v0->offset_central_dir;
    v0->num_file = 0;
    v1 = unzlocal_GetCurrentFileInfoInternal(file, &v0->cur_file_info, &v0->cur_file_info_internal,
                                             NULL, 0, NULL, 0, NULL, 0);
    v0->current_file_ok = (v1 == UNZ_OK);
    return v1;
}
#endif

// FUNCTION: USA 0x00038BCB
// PROTOTYPE: PREVIEW 0x004272C2 identical
// NAME: proto-pdb:default.pdb
#ifdef NON_MATCHING
// best: match_one objdiff 89.023254, bytes mismatch (push file vs retail mov eax,file: callee's static register convention)
int unzGoToNextFile(unzFile file)
{
    // !FAKE: provenance; neutral stand-ins for minizip's s and err; the PDB records no local
    unz_s* v0;
    int v1;
    if (file == NULL)
        return UNZ_PARAMERROR;
    v0 = (unz_s*)file;
    if (!v0->current_file_ok)
        return UNZ_END_OF_LIST_OF_FILE;
    if (v0->num_file + 1 == v0->gi.number_entry)
        return UNZ_END_OF_LIST_OF_FILE;
    v0->pos_in_central_dir += SIZECENTRALDIRITEM + v0->cur_file_info.size_filename +
                              v0->cur_file_info.size_file_extra + v0->cur_file_info.size_file_comment;
    v0->num_file++;
    v1 = unzlocal_GetCurrentFileInfoInternal(file, &v0->cur_file_info, &v0->cur_file_info_internal,
                                             NULL, 0, NULL, 0, NULL, 0);
    v0->current_file_ok = (v1 == UNZ_OK);
    return v1;
}
#endif

#define UNZ_MAXFILENAMEINZIP (256)

int unzStringFileNameCompare(const char *fileName1, const char *fileName2, int iCaseSensitivity); // NAME: proto-pdb:default.pdb
#ifndef NON_MATCHING
int unzGetCurrentFileInfo(unzFile file, unz_file_info *pfile_info, char *szFileName, uLong fileNameBufferSize,
    void *extraField, uLong extraFieldBufferSize, char *szComment, uLong commentBufferSize);
int unzGoToFirstFile(unzFile file);
int unzGoToNextFile(unzFile file);
#endif

// FUNCTION: USA 0x00038C26
// NAME: proto-pdb:default.pdb
int unzLocateFile(unzFile file, const char *szFileName, int iCaseSensitivity)
{
    // !FAKE: provenance; neutral stand-ins for minizip's s and err; the PDB records no local
    unz_s* v0;
    int v1;
    uLong num_fileSaved;
    uLong pos_in_central_dirSaved;
    if (file == NULL)
        return UNZ_PARAMERROR;
    if (strlen(szFileName) >= UNZ_MAXFILENAMEINZIP)
        return UNZ_PARAMERROR;
    v0 = (unz_s*)file;
    if (!v0->current_file_ok)
        return UNZ_END_OF_LIST_OF_FILE;
    num_fileSaved = v0->num_file;
    pos_in_central_dirSaved = v0->pos_in_central_dir;
    v1 = unzGoToFirstFile(file);
    while (v1 == UNZ_OK) {
        char szCurrentFileName[UNZ_MAXFILENAMEINZIP + 1];
        unzGetCurrentFileInfo(file, NULL, szCurrentFileName, sizeof(szCurrentFileName) - 1, NULL, 0, NULL, 0);
        if (unzStringFileNameCompare(szCurrentFileName, szFileName, iCaseSensitivity) == 0)
            return UNZ_OK;
        v1 = unzGoToNextFile(file);
    }
    v0->num_file = num_fileSaved;
    v0->pos_in_central_dir = pos_in_central_dirSaved;
    return v1;
}

// FUNCTION: USA 0x00038E68
// PROTOTYPE: PREVIEW 0x0042755F identical
// NAME: proto-pdb:default.pdb
int unzReadCurrentFile(unzFile file, voidp buf, unsigned len)
{
    int err = UNZ_OK;
    uInt iRead = 0;
    // !FAKE: provenance; neutral stand-ins for minizip's s and pfile_in_zip_read_info; the PDB records no local
    unz_s* v0;
    file_in_zip_read_info_s* v1;
    if (file == NULL)
        return UNZ_PARAMERROR;
    v0 = (unz_s*)file;
    v1 = v0->pfile_in_zip_read;
    if (v1 == NULL)
        return UNZ_PARAMERROR;
    if (v1->read_buffer == NULL)
        return UNZ_END_OF_LIST_OF_FILE;
    if (len == 0)
        return 0;
    v1->stream.next_out = (Bytef*)buf;
    v1->stream.avail_out = (uInt)len;
    if (len > v1->rest_read_uncompressed)
        v1->stream.avail_out = (uInt)v1->rest_read_uncompressed;
    while (v1->stream.avail_out > 0) {
        if (v1->stream.avail_in == 0 && v1->rest_read_compressed > 0) {
            // !FAKE: provenance; neutral stand-in for minizip's uReadThis; the PDB records no local
            uInt v2 = UNZ_BUFSIZE;
            if (v1->rest_read_compressed < v2)
                v2 = (uInt)v1->rest_read_compressed;
            if (v2 == 0)
                return UNZ_EOF;
            if (fseek(v1->file, v1->pos_in_zipfile + v1->byte_before_the_zipfile, SEEK_SET) != 0)
                return UNZ_ERRNO;
            if (fread(v1->read_buffer, v2, 1, v1->file) != 1)
                return UNZ_ERRNO;
            v1->pos_in_zipfile += v2;
            v1->rest_read_compressed -= v2;
            v1->stream.next_in = (Bytef*)v1->read_buffer;
            v1->stream.avail_in = (uInt)v2;
        }
        if (v1->compression_method == 0) {
            // !FAKE: provenance; neutral stand-ins for minizip's uDoCopy and i; the PDB records no local
            uInt v3, v4;
            if (v1->stream.avail_out < v1->stream.avail_in)
                v3 = v1->stream.avail_out;
            else
                v3 = v1->stream.avail_in;
            for (v4 = 0; v4 < v3; v4++)
                *(v1->stream.next_out + v4) = *(v1->stream.next_in + v4);
            v1->crc32 = crc32(v1->crc32, v1->stream.next_out, v3);
            v1->rest_read_uncompressed -= v3;
            v1->stream.avail_in -= v3;
            v1->stream.avail_out -= v3;
            v1->stream.next_out += v3;
            v1->stream.next_in += v3;
            v1->stream.total_out += v3;
            iRead += v3;
        } else {
            // !FAKE: provenance; neutral stand-ins for minizip's uTotalOutBefore, uTotalOutAfter, uOutThis; the PDB records only bufBefore
            uLong v5, v6;
            const Bytef *bufBefore;
            uLong v7;
            int flush = Z_SYNC_FLUSH;
            v5 = v1->stream.total_out;
            bufBefore = v1->stream.next_out;
            err = inflate(&v1->stream, flush);
            v6 = v1->stream.total_out;
            v7 = v6 - v5;
            v1->crc32 = crc32(v1->crc32, bufBefore, (uInt)v7);
            v1->rest_read_uncompressed -= v7;
            iRead += (uInt)(v6 - v5);
            if (err == Z_STREAM_END)
                return (iRead == 0) ? UNZ_EOF : iRead;
            if (err != Z_OK)
                break;
        }
    }
    if (err == Z_OK)
        return iRead;
    return err;
}

// FUNCTION: USA 0x00038FE0
// PROTOTYPE: PREVIEW 0x004276D7 identical
// NAME: proto-pdb:default.pdb
int unzCloseCurrentFile(unzFile file)
{
    // !FAKE: provenance; neutral stand-ins for minizip's err, s and pfile_in_zip_read_info; the PDB records no local
    int v2 = UNZ_OK;
    unz_s* v0;
    file_in_zip_read_info_s* v1;
    if (file == NULL)
        return UNZ_PARAMERROR;
    v0 = (unz_s*)file;
    v1 = v0->pfile_in_zip_read;
    if (v1 == NULL)
        return UNZ_PARAMERROR;
    if (v1->rest_read_uncompressed == 0) {
        if (v1->crc32 != v1->crc32_wait)
            v2 = UNZ_CRCERROR;
    }
    TRYFREE(v1->read_buffer);
    v1->read_buffer = NULL;
    if (v1->stream_initialised)
        inflateEnd(&v1->stream);
    v1->stream_initialised = 0;
    TRYFREE(v1);
    v0->pfile_in_zip_read = NULL;
    return v2;
}

// FUNCTION: USA 0x0003903E
// PROTOTYPE: PREVIEW 0x00427735 identical
// NAME: proto-pdb:default.pdb
int unzClose(unzFile file)
{
    // !FAKE: provenance; neutral stand-in for minizip's s; the PDB records no local
    unz_s* v0;
    if (file == NULL)
        return UNZ_PARAMERROR;
    v0 = (unz_s*)file;
    if (v0->pfile_in_zip_read != NULL)
        unzCloseCurrentFile(file);
    fclose(v0->file);
    TRYFREE(v0);
    return UNZ_OK;
}
