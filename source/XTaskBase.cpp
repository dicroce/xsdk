
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XTaskBase.h"
#include "XSDK/Errors.h"
#include "XSDK/XGuard.h"
#include "XSDK/OS.h"

#ifdef IS_LINUX
#include <sys/syscall.h>
#endif

using namespace std;
using namespace XSDK;

#ifdef IS_LINUX
pthread_key_t XTaskBase::_tlsKey;
#elif defined(IS_WINDOWS)
DWORD XTaskBase::_tlsKey = 0;
#else
    #error ">> Unknown OS!"
#endif

XTaskBase::XTaskBase(const XString& threadName) :
    _thread(),
    _threadID(invalidThreadID),
    _threadName(threadName),
    _running(false)
{
    if(_threadName.length()>16)
        X_THROW(("XSDK thread names must be 16 characters or less (thread name=%s)",_threadName.c_str()));
}

XTaskBase::~XTaskBase() throw()
{
    try
    {
        Join();
    }
    catch(...) {}
}

XTaskBase* XTaskBase::CurrThread()
{
#ifdef IS_LINUX
    return (XTaskBase*)pthread_getspecific(_tlsKey);
#elif defined(IS_WINDOWS)
    return (XTaskBase*)TlsGetValue(_tlsKey);
#else
    #error ">> Unknown OS!"
#endif
}

int XTaskBase::CurrThreadID()
{
#ifdef IS_LINUX
    const XTaskBase* taskBase = (XTaskBase*)pthread_getspecific(_tlsKey);

    if(taskBase)
        return taskBase->ThreadID();

    return syscall(SYS_gettid);
#elif defined(IS_WINDOWS)
    const XTaskBase* taskBase = (XTaskBase*)TlsGetValue(_tlsKey);

    if(taskBase)
        return taskBase->ThreadID();

    return GetCurrentThreadId();
#else
    #error ">> Unknown OS!"
#endif
}

void XTaskBase::_InitID()
{
#ifdef IS_LINUX
    _threadID = syscall(SYS_gettid);
#elif defined(IS_WINDOWS)
    _threadID = GetCurrentThreadId();
#else
    #error ">> Unknown OS!"
#endif
}

void XTaskBase::Start()
{
#ifdef IS_LINUX
    int err = pthread_create(&_thread, 0, _Start, this);

    if(err < 0)
        X_THROW(("Unable to create thread: Code %d", err));
#elif defined(IS_WINDOWS)
    DWORD id = 0;

    _thread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)_Start, this, 0, &id);

    if(!_thread)
        X_THROW(("Unable to create thread: Code %d", GetLastError()));
#else
    #error ">> Unknown OS!"
#endif

    _running = true;
    FULL_MEM_BARRIER();
}

void* XTaskBase::Join()
{
    void* _retval = NULL;

    if((int)_thread != XTaskBase::invalidThreadID)
    {
#ifdef IS_LINUX
        const int err = pthread_join(_thread, &_retval);
        _thread = XTaskBase::invalidThreadID;

        if(err < 0)
            X_THROW(("Unable to join thread: Code %d", err));
#elif defined(IS_WINDOWS)
        WaitForSingleObject(_thread, INFINITE);
        GetExitCodeThread(_thread, (LPDWORD)&_retval);
        CloseHandle(_thread);
        _thread = XTaskBase::invalidThreadID;
#else
    #error ">> Unknown OS!"
#endif

        _running = false;
        FULL_MEM_BARRIER();
    }

    return _retval;
}

bool XTaskBase::IsRunning() const
{
    return _running;
}

void XTaskBase::CancelThread()
{
#ifdef IS_LINUX
    pthread_cancel(_thread);
#elif defined(IS_WINDOWS)
    TerminateThread(_thread, 0);
#else
    #error ">> Unknown OS!"
#endif

    _running = false;
    FULL_MEM_BARRIER();
}

void XTaskBase::Finish()
{
    _running = false;
}

void* XTaskBase::_Start(void* context)
{
    void* retval = 0;

    if(context)
    {
        XTaskBase* taskBase = (XTaskBase*)context;

#ifdef IS_LINUX
        pthread_setspecific(_tlsKey, context);
        prctl( PR_SET_NAME, taskBase->_threadName.c_str(), 0, 0, 0 );
#elif defined(IS_WINDOWS)
        TlsSetValue(_tlsKey, context);
#else
    #error ">> Unknown OS!"
#endif

        taskBase->_InitID();

//        taskBase->_Begin();
        retval = taskBase->EntryPoint();
//        taskBase->_End(retval);
//        taskBase->Finish();

        taskBase->_running = false;
        // FULL_MEM_BARRIER()

#ifdef IS_LINUX
        pthread_setspecific(_tlsKey, 0);
#elif defined(IS_WINDOWS)
        TlsSetValue(_tlsKey, 0);
#else
    #error ">> Unknown OS!"
#endif
    }

    return retval;
}

XTaskBase::StaticInit XTaskBase::_cStaticInit;

XTaskBase::StaticInit::StaticInit()
{
#ifdef IS_LINUX
    pthread_key_create(&_tlsKey, 0);
#elif defined(IS_WINDOWS)
    _tlsKey = TlsAlloc();
#else
    #error ">> Unknown OS!"
#endif
}

XTaskBase::StaticInit::~StaticInit() throw()
{
#ifdef IS_LINUX
    pthread_key_delete(_tlsKey);
#elif defined(IS_WINDOWS)
    TlsFree(_tlsKey);
    _tlsKey = 0;
#else
    #error ">> Unknown OS!"
#endif
}
