//=============================================================================
//Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: Files.h $
// $Revision:$
// $Date:$
// $Author: mspaulding $
//=============================================================================

#ifndef _XSDK_FILES_H_
#define _XSDK_FILES_H_

#include "XSDK/Errors.h"
#include "XSDK/XString.h"

#ifndef WIN32
#include <sys/stat.h>
#include <sys/types.h>
#else
#include <direct.h>
#endif


namespace XSDK
{

#ifdef WIN32
const static char WRITE_MODE[] = "wb";
const static char READ_MODE[] = "rb";
const static char READ_MODE_PLUS[] = "rb+";
#else
const static char READ_MODE_PLUS[] = "rb+, ccs=UTF-8";
const static char READ_MODE[] = "rb, ccs=UTF-8";
const static char WRITE_MODE[] = "wb, ccs=UTF-8";
#endif//WIN32

#ifndef WIN32

inline int x_mkdir(const XString& path)
{
    return mkdir(path.c_str(),0777);
}


inline FILE *x_fopen( const XString& path, const XString& mode )
{
    FILE* fp;
    fp = fopen( path.c_str(), mode.c_str() );

    if ( !fp )
        X_THROW(( "Unable to open file: %s\n", path.c_str() ));

    return fp;
}
#else
inline int x_mkdir( const XString& path )
{
    return _wmkdir(path.get_wide_string().data());
}

inline FILE *x_fopen( const XString& path, const XString& mode )
{
    errno_t err;
    FILE* fp;

    if ( err = _wfopen_s( &fp, path.get_wide_string().c_str(), mode.get_wide_string().c_str() ) != 0 )
        X_THROW(( "fopen_s returned error code: %d.\n", (int)err ));

    if ( !fp )
        X_THROW(( "Unable to open file: %s\n", path.c_str() ));

    return fp;
}
#endif

}

#endif /* _XSDK_FILES_H_ */
