
//=============================================================================
//Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XRWLock.cpp $
// $Revision:$
// $Date:$
// $Author: Hardeep Singh $
//=============================================================================


#include "XSDK/XRWLock.h"
#include "XSDK/Errors.h"

#include <errno.h>

using namespace XSDK;

XRWLock::XRWLock() :
    _lok()
{
    pthread_rwlockattr_t rwlockAttributes;
    int err = 0;
    if ((err = pthread_rwlockattr_init( &rwlockAttributes )) < 0 )
        X_THROW(( "Unable to allocate RWLock attributes." ));

    if ((err = pthread_rwlock_init( &_lok, &rwlockAttributes )) < 0 )
        X_THROW(( "Unable to allocate a RWLock." ));
}

XRWLock::~XRWLock() throw()
{
    pthread_rwlock_destroy( &_lok );
}

void XRWLock::Acquire()
{
    int err = pthread_rwlock_wrlock( &_lok );

    if( err != 0 )
        X_THROW(( "pthread_rwlock_wrlock() failed." ));
}

void XRWLock::AcquireRead()
{
    int err = pthread_rwlock_rdlock( &_lok );

    if( err != 0 )
        X_THROW(( "pthread_rwlock_rdlock() failed." ));
}

void XRWLock::AcquireWrite()
{
    int err = pthread_rwlock_wrlock( &_lok );

    if( err != 0 )
        X_THROW(( "pthread_rwlock_wrlock() failed." ));
}

void XRWLock::Release()
{
    int err = pthread_rwlock_unlock( &_lok );

    if( err != 0 )
        X_THROW(( "pthread_rwlock_unlock() failed." ));
}

bool XRWLock::TryAcquire()
{
    int err = pthread_rwlock_trywrlock( &_lok );

    if( err == 0 )
        return true;

    if( err == EBUSY )
        return false;

    X_THROW(( "pthread_rwlock_trywrlock() failed." ));
}

bool XRWLock::TryAcquireWrite()
{
    int err = pthread_rwlock_trywrlock( &_lok );

    if( err == 0 )
        return true;

    if( err == EBUSY )
        return false;

    X_THROW(( "pthread_rwlock_trywrlock() failed." ));
}

bool XRWLock::TryAcquireRead()
{
    int err = pthread_rwlock_tryrdlock( &_lok );

    if( err == 0 )
        return true;

    if( err == EBUSY )
        return false;

    X_THROW(( "pthread_rwlock_tryrdlock() failed." ));
}
