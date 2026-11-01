# Notice for src/

The code in this folder is a reconstruction of copyrighted game code, written so that it compiles to the same bytes as
the retail release of Voodoo Vince. It is not the studio's original source. It is provided for study and preservation,
and **no license is asserted** over it. The MIT license in the repository's `LICENSE` file does not apply to this folder.

This repository holds no game files. To build or check anything you need your own copy of the game.

Literal strings in this code are copied exactly from the retail executable at the address the function references; some
are the original studio's build paths.

## Third-party code

`Libs/BeepLib/Xbox/Official_Release/` includes code from the public zlib library, version 1.1.4 (`adler32.c`,
`infblock.c`, `infcodes.c`, `inflate.c`, `infutil.c`). Those file heads refer to the copyright notice in `zlib.h`, which
is not in this tree, so it is repeated here. That code keeps the zlib license:

    Copyright (C) 1995-2002 Jean-loup Gailly and Mark Adler

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgment in the product documentation would be
       appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.

    Jean-loup Gailly        Mark Adler
    jloup@gzip.org          madler@alumni.caltech.edu
