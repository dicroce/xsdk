
//=============================================================================
//Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XJob.h $
// $Revision:$
// $Date:$
// $Author: Tom Distler $
//=============================================================================
#ifndef XSDK_XJob_h
#define XSDK_XJob_h

#include "XSDK/XTaskBase.h"
#include "XSDK/XTimer.h"
#include "XSDK/XBlockingQueue.h"
#include "XSDK/XMutex.h"
#include "XSDK/TimeUtils.h"

namespace XSDK
{

class XJob : public XTaskBase
{
public:
    typedef enum _Status
    {
        XJOBSTATUS_COMPLETE=1,
        XJOBSTATUS_EXPIRED,
        XJOBSTATUS_UNRESPONSIVE,
        XJOBSTATUS_PROGRESS,
    }Status;

    X_API XJob(unsigned int watchdogInterval, unsigned int maxRuntime);
    X_API virtual ~XJob() throw();

    X_API void Start();

    X_API bool HasStatus();
    X_API XJob::Status GetStatus();
    X_API bool IsExpired() const;
    X_API bool IsUnresponsive() const;
    X_API int GetProgress() const;

protected:

    /// This is where the thread work gets done. Derived classes need to
    /// override this method.
    X_API virtual void* EntryPoint();

    X_API void StrobeWatchdog();
    X_API void UpdateProgress(int percent);

private:

    // Hide these methods.
    X_API virtual void _Begin();
    X_API virtual void _End(void* retVal);

    /// Implements the watchdog functionality. Called from the timer.
    static void _WatchdogCallback( void* data );

    unsigned int _wdInterval;      ///< Watchdog interval in milliseconds.
    unsigned int _maxRuntime;      ///< Maximum thread runtime in milliseconds.
    int _progress;                 ///< 0-100, the job progress.
    volatile bool _isExpired;      ///< Indicates if the job expired.
    volatile bool _isUnresponsive; ///< Indicates if the job is unresponsive.

    XSDK::XTimer _watchdog;     ///< The watchdog timer.
    volatile bool _wdStrobed;   ///< Set to true each time the watchdog is strobed.
    struct timeval _startTime;  ///< The time the job started.

    mutable XSDK::XMutex _lok;

    XSDK::XBlockingQueue< XJob::Status > _status; ///< Job status queue.

};

}

#endif
