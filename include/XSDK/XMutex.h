
//=============================================================================
//Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XMutex.h $
// $Revision:$
// $Date:$
// $Author: Tony Di Croce $
//=============================================================================


#ifndef XSDK_XMutex_h
#define XSDK_XMutex_h

#include "XSDK/Platform.h"

#ifndef WIN32
#include <pthread.h>
#endif

#include "XSDK/Module.h"
#include "XSDK/XSharedLib.h"

namespace XSDK
{

class XCondition;

class XMutex
{
    friend class XCondition;

public:
    X_API XMutex();
    X_API ~XMutex() throw();

    X_API void Acquire();
    X_API void Release();
    X_API bool TryAcquire();

 private:
    XMutex( const XMutex& obj );
    XMutex& operator = ( const XMutex& obj );

#ifdef WIN32
    CRITICAL_SECTION _lok;
    int _lokCount;
#else
    pthread_mutex_t _lok;
#endif // WIN32
};

}

#endif
