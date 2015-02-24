
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XTaskBase_h
#define XSDK_XTaskBase_h

#include "XSDK/Platform.h"
#include "XSDK/Types.h"
#include "XSDK/XString.h"

#ifdef IS_LINUX
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/prctl.h>
#else
#ifdef IS_64BIT
#include <intrin.h>
#endif
#endif

#include <list>
#include <map>
#include <vector>

namespace XSDK
{

static const XString DEFAULT_THREAD_NAME = "defaulty";

class XTaskBase
{
public:
    static const int invalidThreadID = 0;

    /// Creates an XTaskBase which doesn't use a thread pool.
    X_API XTaskBase(const XString& threadName=DEFAULT_THREAD_NAME);

    X_API virtual ~XTaskBase() throw();

    X_API virtual void Start();

    X_API virtual void* Join();

    /// This is where the thread work gets done. Derived classes need to
    /// override this method.
    X_API virtual void* EntryPoint()=0;

    /// Returns the ID of this thread.
    X_API int ThreadID() const { return _threadID; }

    /// Whether the thread is currently running.
    X_API bool IsRunning() const;

    /// @note This method should <b>not</b> be called in normal shutdown circumstances.
    X_API void CancelThread();

    /// @note Do _not_ delete or free returned pointer.
    static X_API XTaskBase* CurrThread();

    /// Returns the ID of the current thread. The difference between
    /// this and calling ThreadID() on the XTaskBase returned from CurrThread()
    /// is that this works even when the current thread was not started
    /// with XTaskBase.
    static X_API int CurrThreadID();

   // Returns the CPU ID assigned to this thread
#ifdef IS_LINUX
    static X_API int GetCPU() { return sched_getcpu(); }

#elif defined(IS_WINDOWS)
  #ifdef IS_64BIT

    static X_API DWORD GetCPU()
    {
        int CPUInfo[4];
        __cpuid( CPUInfo, 1 );

        return (unsigned)CPUInfo[1] >> 24;
    }

  #else

    //This also works for WindowsXP, since Vista and above provide an API call.
    static X_API DWORD GetCPU() {   _asm {mov eax, 1}
                                    _asm {cpuid}
                                    _asm {shr ebx, 24}
                                    _asm {mov eax, ebx}
                                }
  #endif
#endif

private:
    X_API XTaskBase(const XTaskBase&);
    X_API XTaskBase& operator = (const XTaskBase&);

    static void* _Start(void* context);

    void _InitID();

    X_API void Finish();

#ifdef IS_LINUX
    pthread_t _thread;
    static pthread_key_t _tlsKey;
#elif defined(IS_WINDOWS)
    HANDLE _thread;
    static DWORD _tlsKey;
#else
    error ">> Unknown OS!"
#endif

    int _threadID;

    XString _threadName;

    bool _running;

    class StaticInit
    {
    public:
        StaticInit();
        ~StaticInit() throw();
    };

    static StaticInit _cStaticInit;
};

}

#endif
