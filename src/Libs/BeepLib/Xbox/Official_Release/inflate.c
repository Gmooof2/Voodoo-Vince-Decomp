// UNIT: src/Libs/BeepLib/Xbox/Official_Release/inflate.c OBJ: Xbox\Official_Release\inflate.obj OPT: /O1 /Oy
/* Public zlib 1.1.x source, altered: declarations in-file, unmatched procs out.
 * inflate.c -- zlib interface to inflate modules
 * Copyright (C) 1995-2002 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

typedef unsigned char  Byte;
typedef unsigned int   uInt;
typedef unsigned long  uLong;
typedef Byte           Bytef;
typedef void          *voidpf;

#define Z_NULL 0
#define Z_OK            0
#define Z_STREAM_ERROR (-2)
#define Z_MEM_ERROR    (-4)
#define Z_VERSION_ERROR (-6)

#define ZLIB_VERSION "1.1.4"
#define DEF_WBITS 15 /* MAX_WBITS */

typedef voidpf (*alloc_func)(voidpf opaque, uInt items, uInt size);
typedef void   (*free_func)(voidpf opaque, voidpf address);

struct internal_state;

typedef struct z_stream_s { // NAME: proto-pdb:default.pdb
    Bytef    *next_in;  /* next input byte */
    uInt     avail_in;  /* number of bytes available at next_in */
    uLong    total_in;  /* total nb of input bytes read so far */

    Bytef    *next_out; /* next output byte should be put there */
    uInt     avail_out; /* remaining free space at next_out */
    uLong    total_out; /* total nb of bytes output so far */

    char     *msg;      /* last error message, NULL if no error */
    struct internal_state *state; /* not visible by applications */

    alloc_func zalloc;  /* used to allocate the internal state */
    free_func  zfree;   /* used to free the internal state */
    voidpf     opaque;  /* private data object passed to zalloc and zfree */

    int     data_type;  /* best guess about the data type: ascii or binary */
    uLong   adler;      /* adler32 value of the uncompressed data */
    uLong   reserved;   /* reserved for future use */
} z_stream;

typedef z_stream *z_streamp;

#define ZALLOC(strm, items, size) \
           (*((strm)->zalloc))((strm)->opaque, (items), (size))
#define ZFREE(strm, addr)  (*((strm)->zfree))((strm)->opaque, (voidpf)(addr))

typedef uLong (*check_func)(uLong check, const Bytef *buf, uInt len);

struct inflate_blocks_state;
typedef struct inflate_blocks_state inflate_blocks_statef;

voidpf zcalloc(voidpf opaque, unsigned items, unsigned size);
void   zcfree(voidpf opaque, voidpf ptr);
uLong  adler32(uLong adler, const Bytef *buf, uInt len);
inflate_blocks_statef *inflate_blocks_new(z_streamp z, check_func c, uInt w);
void inflate_blocks_reset(inflate_blocks_statef *s, z_streamp z, uLong *c);
int  inflate_blocks_free(inflate_blocks_statef *s, z_streamp z);

typedef enum { // NAME: proto-pdb:default.pdb
      METHOD,   /* waiting for method byte */
      FLAG,     /* waiting for flag byte */
      DICT4,    /* four dictionary check bytes to go */
      DICT3,    /* three dictionary check bytes to go */
      DICT2,    /* two dictionary check bytes to go */
      DICT1,    /* one dictionary check byte to go */
      DICT0,    /* waiting for inflateSetDictionary */
      BLOCKS,   /* decompressing blocks */
      CHECK4,   /* four check bytes to go */
      CHECK3,   /* three check bytes to go */
      CHECK2,   /* two check bytes to go */
      CHECK1,   /* one check byte to go */
      DONE,     /* finished check, done */
      BAD}      /* got an error--stay here */
inflate_mode;

/* inflate private state */
struct internal_state { // NAME: proto-pdb:default.pdb

  /* mode */
  inflate_mode  mode;   /* current inflate mode */

  /* mode dependent information */
  union {
    uInt method;        /* if FLAGS, method byte */
    struct {
      uLong was;                /* computed check value */
      uLong need;               /* stream check value */
    } check;            /* if CHECK, check values to compare */
    uInt marker;        /* if BAD, inflateSync's marker bytes count */
  } sub;        /* submode */

  /* mode independent information */
  int  nowrap;          /* flag for no wrapper */
  uInt wbits;           /* log2(window size)  (8..15, defaults to 15) */
  inflate_blocks_statef
    *blocks;            /* current inflate_blocks state */

};


// FUNCTION: USA 0x000391B1
// PROTOTYPE: PREVIEW 0x004278A8 identical
// NAME: proto-pdb:default.pdb
int inflateReset(z)
z_streamp z;
{
  if (z == Z_NULL || z->state == Z_NULL)
    return Z_STREAM_ERROR;
  z->total_in = z->total_out = 0;
  z->msg = Z_NULL;
  z->state->mode = z->state->nowrap ? BLOCKS : METHOD;
  inflate_blocks_reset(z->state->blocks, z, Z_NULL);
  return Z_OK;
}


// FUNCTION: USA 0x000391F0
// PROTOTYPE: PREVIEW 0x004278E7 identical
// NAME: proto-pdb:default.pdb
int inflateEnd(z)
z_streamp z;
{
  if (z == Z_NULL || z->state == Z_NULL || z->zfree == Z_NULL)
    return Z_STREAM_ERROR;
  if (z->state->blocks != Z_NULL)
    inflate_blocks_free(z->state->blocks, z);
  ZFREE(z, z->state);
  z->state = Z_NULL;
  return Z_OK;
}


// FUNCTION: USA 0x0003922E
// PROTOTYPE: PREVIEW 0x00427925 identical
// NAME: proto-pdb:default.pdb
int inflateInit2_(z, w, version, stream_size)
z_streamp z;
int w;
const char *version;
int stream_size;
{
  if (version == Z_NULL || version[0] != ZLIB_VERSION[0] ||
      stream_size != sizeof(z_stream))
      return Z_VERSION_ERROR;

  /* initialize state */
  if (z == Z_NULL)
    return Z_STREAM_ERROR;
  z->msg = Z_NULL;
  if (z->zalloc == Z_NULL)
  {
    z->zalloc = zcalloc;
    z->opaque = (voidpf)0;
  }
  if (z->zfree == Z_NULL) z->zfree = zcfree;
  if ((z->state = (struct internal_state *)
       ZALLOC(z,1,sizeof(struct internal_state))) == Z_NULL)
    return Z_MEM_ERROR;
  z->state->blocks = Z_NULL;

  /* handle undocumented nowrap option (no zlib header or check) */
  z->state->nowrap = 0;
  if (w < 0)
  {
    w = - w;
    z->state->nowrap = 1;
  }

  /* set window size */
  if (w < 8 || w > 15)
  {
    inflateEnd(z);
    return Z_STREAM_ERROR;
  }
  z->state->wbits = (uInt)w;

  /* create inflate_blocks state */
  if ((z->state->blocks =
      inflate_blocks_new(z, z->state->nowrap ? Z_NULL : adler32, (uInt)1 << w))
      == Z_NULL)
  {
    inflateEnd(z);
    return Z_MEM_ERROR;
  }

  /* reset state */
  inflateReset(z);
  return Z_OK;
}


// FUNCTION: USA 0x00039313
// PROTOTYPE: PREVIEW 0x00427A0A identical
// NAME: proto-pdb:default.pdb
int inflateInit_(z, version, stream_size)
z_streamp z;
const char *version;
int stream_size;
{
  return inflateInit2_(z, DEF_WBITS, version, stream_size);
}
