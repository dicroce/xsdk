
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/Errors.h"
#include "XSDK/XMonoClock.h"
#include "XSDK/TimeUtils.h"

#ifdef IS_WINDOWS
    #include <windows.h>
#elif defined(IS_LINUX)
    #include <time.h>
#else
    #error ">> Unknown OS!"
#endif

using namespace XSDK;

static uint64_t NanoSec = 1000000000;

static uint64_t MonotonicFrequency()
{
    uint64_t freq;

#ifdef IS_WINDOWS
    if(!QueryPerformanceFrequency((LARGE_INTEGER*)&freq))
        X_THROW(("Was not able to get monotonic clock frequency: %d", GetLastError()));
#elif defined(IS_LINUX)
    struct timespec cfreq = {0, 0};
    int ret = 0;
    if(ret = clock_getres(CLOCK_MONOTONIC, &cfreq) || (cfreq.tv_nsec <= 0 && cfreq.tv_sec <= 0))
        X_THROW(("Was not able to get monotonic clock frequency: %d", ret));
    freq = NanoSec / ((uint64_t)cfreq.tv_sec * NanoSec + (uint64_t)cfreq.tv_nsec);
#else
    #error ">> Unknown OS!"
#endif

    return freq;
}

uint64_t XMonoClock::GetTime()
{
    uint64_t now;

#ifdef IS_WINDOWS
    if(!QueryPerformanceCounter((LARGE_INTEGER*)&now))
        X_THROW(("Unable to get monotonic time: %d", GetLastError()));
#elif defined(IS_LINUX)
    struct timespec cnow;
    int ret = 0;
    if(ret = clock_gettime(CLOCK_MONOTONIC, &cnow))
        X_THROW(("Unable to get monotonic time: %d", ret));
    now = (uint64_t)cnow.tv_sec * GetFrequency();
    now += (uint64_t)cnow.tv_nsec * GetFrequency() / NanoSec;
#else
    #error ">> Unknown OS!"
#endif

    return now;
}

uint64_t XMonoClock::GetFrequency()
{
    static const uint64_t Frequency = MonotonicFrequency();
    return Frequency;
}

double XMonoClock::GetElapsedTime(uint64_t start, uint64_t end)
{
    return XSDK::GetElapsedTime(start, end, GetFrequency());
}
