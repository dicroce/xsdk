
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XRWLock.h"
#include "XSDK/Errors.h"
#include "XSDK/XMutex.h"
#include "XSDK/XCondition.h"

using namespace XSDK;

XRWLock::XRWLock() : _lok(),_waitingReadersCond(_lok),_waitingWritersCond(_lok),
                     _numWaitingReaders(0), _numWaitingWriters(0), _countLockers(0)
{
    X_LOG_INFO("Entering: XRWLock::XRWLock");
    // Initializes the mutexes and condition variables
    X_LOG_INFO("Exiting: XRWLock::XRWLock");
}

XRWLock::~XRWLock() throw()
{
    X_LOG_INFO("Entering: XRWLock::~XRWLock");
    // Destroy the mutexes and condition variables
    X_LOG_INFO("Exiting: XRWLock::~XRWLock");
}

inline void XRWLock::Acquire()
{
    X_LOG_INFO("Entering: XRWLock::Aquire");
    AcquireWrite();
    X_LOG_INFO("Exiting: XRWLock::Aquire");
}

inline bool XRWLock::TryAcquire()
{
    X_LOG_INFO("Entering: XRWLock::TryAquire");
    return TryAcquireWrite();
    X_LOG_INFO("Exiting: XRWLock::TryAquire");
}

void XRWLock::AcquireRead()
{
    X_LOG_INFO("Entering: XRWLock::AcquireRead");

    //Aquire the mutex before doing operations
    _lok.Acquire();

    // Loop the reader calls so as to prefer Writers
    while (_countLockers < 0 || _numWaitingWriters > 0)
    {
        _numWaitingReaders++;

        // Condition may throw, and it will release the mutex in that case
        try
        {
            _waitingReadersCond.Wait();
        }
        catch (XSDK::XException &e)
        {
            // Condition releases the mutex and then throws the exception
            _lok.Acquire();
            _numWaitingReaders--;
            _lok.Release();
            throw e;
        }

        _numWaitingReaders--;
    }
    _countLockers++;
    _lok.Release();

    X_LOG_INFO("Exiting: XRWLock::AcquireRead");
}

void XRWLock::AcquireWrite()
{
    X_LOG_INFO("Entering: XRWLock::AcquireWrite");

    //Aquire the mutex before doing operations
    _lok.Acquire();

    //Loop through untill there there are no writers
    while (_countLockers != 0)
    {
        _numWaitingWriters++;

        // Condition may throw, and it will release the mutex in that case
        try
        {
            _waitingWritersCond.Wait();
        }
        catch (XSDK::XException&)
        {
            // Condition releases the mutex and then throws the exception
            _lok.Acquire();
            _numWaitingWriters--;
            _lok.Release();
            throw;
        }
        _numWaitingWriters--;
    }
    // _countLockers = -1 implies a writer is holding a lock.
    // _countLockers = 0 implies no reader or writer is holding a lock
    // _countLockers = 1 to n implies n readers are holding the lock.
    _countLockers = -1;

    _lok.Release();
    X_LOG_INFO("Exiting: XRWLock::AcquireWrite");
}

void XRWLock::Release()
{
    X_LOG_INFO("Entering: XRWLock::Release");

    //Aquire the mutex before doing operations
    _lok.Acquire();

    if (_countLockers > 0) // Releasing a reader.
        _countLockers--;
    else if (_countLockers == -1) // Releasing a writer.
        _countLockers = 0;
    else
    {
        _lok.Release();
        X_LOG_INFO("Exiting: XRWLock::Release");
        return;
    }

    // Preferring writers
    // If there are writers waiting and no readers reading.
    if (_numWaitingWriters > 0 && _countLockers == 0)
    {
        _waitingWritersCond.Signal();
    }
    else if ( _numWaitingReaders > 0 && _numWaitingWriters < 0)
    {
        _waitingReadersCond.Broadcast();
    }
    else
    {
        // Nobody is waiting. Signal Nothing
    }

    _lok.Release();
    X_LOG_INFO("Exiting: XRWLock::Release");
}

bool XRWLock::TryAcquireRead()
{
    X_LOG_INFO("Entering: XRWLock::TryAquireRead");

    //Aquire the mutex before doing operations
    bool result = false;
    if(_lok.TryAcquire() == true)
    {
        // Check if a writer holds a lock or that nay write lock requests are pending.
        if (_countLockers == -1 || _numWaitingWriters > 0)
        {
            result = false;
        }
        else
        {
            _countLockers++;
            result = true;
        }
        _lok.Release();
    }

    X_LOG_INFO("Exiting: XRWLock::TryAquireRead");
    return result;
}

bool XRWLock::TryAcquireWrite()
{
    X_LOG_INFO("Entering: XRWLock::TryAquireWrite");

    //Aquire the mutex before doing operations
    bool result = false;
    if(_lok.TryAcquire() == true)
    {
        // Check if nobody is holding a lock
        if (_countLockers == 0)
        {
            _countLockers = -1;
            result = true;
        }
        else
        {
            result = false;
        }
        _lok.Release();
    }

    X_LOG_INFO("Exiting: XRWLock::TryAquireWrite");
    return result;

}
