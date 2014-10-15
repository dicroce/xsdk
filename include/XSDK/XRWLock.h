
//=============================================================================
//Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XRWLock.h $
// $Revision:$
// $Date:$
// $Author: Hardeep Singh $
//=============================================================================


#ifndef XSDK_XRWLock_h
#define XSDK_XRWlock_h

#include "XSDK/Platform.h"
#include "XSDK/XException.h"
#include "XSDK/Module.h"

#ifdef WIN32
#include "XSDK/XMutex.h"
#include "XSDK/XCondition.h"
#else
#include <pthread.h>
#endif // WIN32

//Note:
// This code does not implement recursive locking for the write lock on Windows.
// Tring to recursively call the lock will cause a deadlock in the application.

namespace XSDK
{

class XRWLock
{

public:
    X_API XRWLock();
    X_API virtual ~XRWLock() throw();

    X_API void Acquire();
    X_API void AcquireRead();
    X_API void AcquireWrite();
    X_API void Release();
    X_API bool TryAcquire();
    X_API bool TryAcquireRead();
    X_API bool TryAcquireWrite();

 private:
    XRWLock( const XRWLock& obj );
    XRWLock& operator = ( const XRWLock& obj );

#ifdef WIN32
    XMutex _lok;
    XCondition _waitingReadersCond;
    XCondition _waitingWritersCond;
    int _numWaitingReaders;
    int _numWaitingWriters;
    int _countLockers;
#else
    pthread_rwlock_t _lok;
#endif // WIN32
};

}

#endif
