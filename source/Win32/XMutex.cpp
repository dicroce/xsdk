
//=============================================================================
//Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XMutex.cpp $
// $Revision:$
// $Date:$
// $Author: Tony Di Croce $
//=============================================================================


#include "XSDK/XMutex.h"
#include "XSDK/Errors.h"


using namespace XSDK;

XMutex::XMutex()
: _lokCount(0)
{
    InitializeCriticalSection( &_lok );
}

XMutex::~XMutex() throw()
{
    DeleteCriticalSection( &_lok );
}

void XMutex::Acquire()
{
    EnterCriticalSection( &_lok );
    _lokCount++;
}

void XMutex::Release()
{
    if( (_lokCount - 1) < 0 )
        X_THROW(( "Attempted to Leave already left critical section." ));
    _lokCount--;
    LeaveCriticalSection( &_lok );
}

bool XMutex::TryAcquire()
{
    BOOL entered = TryEnterCriticalSection( &_lok );

    if( entered )
    {
        _lokCount++;
        return true;
    }

    return false;
}
