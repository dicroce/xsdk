
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XJob.h"
#include "XSDK/XGuard.h"

using namespace XSDK;

XJob::XJob(unsigned int watchdogInterval, unsigned int maxRuntime) :
    XTaskBase(),
    _wdInterval( watchdogInterval ),
    _maxRuntime( maxRuntime ),
    _progress( 0 ),
    _isExpired( false ),
    _isUnresponsive( false ),
    _wdStrobed( false )
{
}

XJob::~XJob() throw()
{
    try
    {
        // [tdistler] Must manually stop the timer for the CancelThread()
        // code to work properly in the event of an error.
        _watchdog.StopTimer();

        // Kill the thread if we're being destroyed but things went wrong.
        if( this->IsRunning() && ( _isExpired || _isUnresponsive ) )
            this->CancelThread();

        // We have to join the thread here to keep the mutex from going away
        // before the thread is dead.
        XTaskBase::Join();
    }
    catch (...) {}
}

void* XJob::EntryPoint()
{
    // OVERRIDEN BY DERIVED OBJECT
    return 0;
}

void XJob::Start()
{
    {
        XGuard lok( _lok );

        // Reset all the variables.
        _progress = 0;
        _isExpired = false;
        _isUnresponsive = false;
        _wdStrobed = false;
        while( _status.NonBlockingPeek() )
            _status.Pop();
        x_gettimeofday( &_startTime );
    }

    XTaskBase::Start();
}

void XJob::_Begin()
{
    unsigned int timerInterval = 0;

    {
        XGuard lok( _lok );
        if( _wdInterval > 0 )
            timerInterval = _wdInterval;
        else
            timerInterval = _maxRuntime;
    }

    // It makes no sense to start the timer if both watchdogInterval and
    // maxRuntime are 0.
    if( timerInterval > 0 )
        _watchdog.StartTimer( timerInterval, false, _WatchdogCallback, this );
}

void XJob::_End(void* retVal)
{
    _watchdog.StopTimer();

    XGuard lok( _lok );
    _status.Push( XJOBSTATUS_COMPLETE );
}

// Called by the watchdog timer.
void XJob::_WatchdogCallback( void* data )
{
    XJob* job = (XJob*)data;

    XGuard lok( job->_lok );

    // Check if watchdog was strobed.
    if( job->_wdInterval > 0 && !job->_wdStrobed && !job->_isUnresponsive )
    {
        job->_isUnresponsive = true;
        job->_status.Push( XJOBSTATUS_UNRESPONSIVE );
    }
    job->_wdStrobed = false;

    // Check if job expired.
    if( job->_maxRuntime > 0 )
    {
        struct timeval now = {0,0};
        x_gettimeofday( &now );
        struct timeval runtime = {0,0};
        timersub( &now, &job->_startTime, &runtime );
        unsigned int runtime_ms = (runtime.tv_sec * 1000) + (runtime.tv_usec / 1000);
        if( runtime_ms >= job->_maxRuntime && !job->_isExpired )
        {
            job->_isExpired = true;
            job->_status.Push( XJOBSTATUS_EXPIRED );
        }
    }
}

bool XJob::HasStatus()
{
    return _status.NonBlockingPeek();
}

XJob::Status XJob::GetStatus()
{
    return _status.Pop();
}

bool XJob::IsExpired() const
{
    XGuard lok( _lok );
    return _isExpired;
}

bool XJob::IsUnresponsive() const
{
    XGuard lok( _lok );
    return _isUnresponsive;
}

int XJob::GetProgress() const
{
    XGuard lok( _lok );
    return _progress;
}

void XJob::StrobeWatchdog()
{
    XGuard lok( _lok );
    _wdStrobed = true;
}

void XJob::UpdateProgress(int percent)
{
    XGuard lok( _lok );
    _progress = percent;
    _status.Push( XJOBSTATUS_PROGRESS );
}
