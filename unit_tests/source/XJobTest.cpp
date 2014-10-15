//=============================================================================
// Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XJobTest.cpp$
// $Revision:$
// $Date:$
// $Author: tdistler$
//=============================================================================
#include "XJobTest.h"
#include "XSDK/TimeUtils.h"

#define THREAD_RUNTIME    2000 //ms
#define THREAD_SLEEP_TIME 100  //ms
#define WATCHDOG_INTERVAL 1000 //ms
#define MAX_RUNTIME       4000 //ms

using namespace XSDK;

REGISTER_TEST_FIXTURE(XJobTest);

#include "XSDK/XJob.h"


// Derived job for testing
class XMyJob : public XSDK::XJob
{
public:
    typedef enum {
        XMYJOB_PROGRESS = 0x01,
        XMYJOB_STROBE   = 0x02,
        XMYJOB_EXPIRE   = 0x04,
    }Flags;

    XMyJob(int wdInterval, int maxRuntime, unsigned int flags) :
      XSDK::XJob(wdInterval, maxRuntime),
      _flags(flags),
      _val(0)
      {}
    virtual ~XMyJob() throw() {}
    unsigned int _flags;
    int _val;
protected:
    virtual void* EntryPoint()
    {
        // Extend the runtime if we want to simulate an expired thread.
        int extend = (_flags & XMYJOB_EXPIRE) ? 10 : 1;
        int threadLoopCount = (THREAD_RUNTIME * extend) / THREAD_SLEEP_TIME;

        struct timeval prev = {0,0};
        x_gettimeofday( &prev );
        for( int ii=1; ii<=threadLoopCount; ii++ )
        {
            // Wait 100 ms between loops. We have to compensate for slow
            // threads do to valgrind and other tools... we can't just
            // sleep 100ms.
            struct timeval interval = {0,100000};
            struct timeval wakeTime = {0,0};
            timeradd( &prev, &interval, &wakeTime );
            int sleepTime = 0;
            struct timeval now = {0,0};
            x_gettimeofday( &now );
            timerdiff( &wakeTime, &now, sleepTime ); // Determine how long to actually wait
            timercopy( &prev, &wakeTime );
            if( sleepTime > 0 )
                x_usleep( sleepTime );

            if( _flags & XMYJOB_STROBE )
                StrobeWatchdog();
            if( _flags & XMYJOB_PROGRESS )
                UpdateProgress( (int)(((float)ii / (float)threadLoopCount) * 100.0) );
        }

        // Set something so we know we completed.
        this->_val = 42;
        return 0;
    }
};


void XJobTest::setup()
{
}

void XJobTest::teardown()
{
}

void XJobTest::TestGetProgress()
{
    try
    {
        XMyJob job(0, 0, XMyJob::XMYJOB_PROGRESS);

        job.Start();
        job.Join();
        UT_ASSERT( job._val == 42 );

        // Make sure the progress was updated.
        UT_ASSERT( job.GetProgress() == 100 );
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XJobTest::TestHasGetStatus()
{
    try
    {
        // Test HasStatus() using progress updates.
        {
            XMyJob job(0, 0, XMyJob::XMYJOB_PROGRESS);
            UT_ASSERT( job.HasStatus() == false );
            job.Start();
            job.Join();
            UT_ASSERT( job.HasStatus() == true );
        }

        x_sleep(1);

        // Test progress updates.
        {
            XMyJob job(0, 0, XMyJob::XMYJOB_PROGRESS);
            job.Start();
            int count = 0;
            int lastProg = 0;
            while( 1 )
            {
                XJob::Status msg = job.GetStatus();
                if( msg == XJob::XJOBSTATUS_COMPLETE )
                    break;
                count++;
                UT_ASSERT( msg == XJob::XJOBSTATUS_PROGRESS );
                int prog = 0;
                UT_ASSERT_NO_THROW( prog = job.GetProgress() );
                UT_ASSERT( prog > lastProg );
                lastProg = prog;
            }
            UT_ASSERT( count == (THREAD_RUNTIME / THREAD_SLEEP_TIME) );
            UT_ASSERT( job._val == 42 );
        }
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XJobTest::TestCompletion()
{
    try
    {
        // Test job completion *without* watchdog or max runtime.
        {
            XMyJob job(0, 0, 0);
            UT_ASSERT( job.IsRunning() == false );
            UT_ASSERT( job.IsUnresponsive() == false );
            UT_ASSERT( job.IsExpired() == false );
            job.Start();
            XJob::Status msg = job.GetStatus(); // Blocks until message.
            UT_ASSERT( msg == XJob::XJOBSTATUS_COMPLETE );
            UT_ASSERT( job.IsRunning() == false );
            UT_ASSERT( job.IsUnresponsive() == false );
            UT_ASSERT( job.IsExpired() == false );
            UT_ASSERT( job._val == 42 );
        }

        // Test job completion *with* watchdog and *without* max runtime.
        {
            XMyJob job(WATCHDOG_INTERVAL, 0, XMyJob::XMYJOB_STROBE);
            UT_ASSERT( job.IsRunning() == false );
            UT_ASSERT( job.IsUnresponsive() == false );
            UT_ASSERT( job.IsExpired() == false );
            job.Start();
            XJob::Status msg = job.GetStatus(); // Blocks until message.
            UT_ASSERT( msg == XJob::XJOBSTATUS_COMPLETE );
            UT_ASSERT( job.IsRunning() == false );
            UT_ASSERT( job.IsUnresponsive() == false );
            UT_ASSERT( job.IsExpired() == false );
            UT_ASSERT( job._val == 42 );
        }

        // Test job completion *without* watchdog and *with* max runtime.
        {
            XMyJob job(0, MAX_RUNTIME, 0);
            UT_ASSERT( job.IsRunning() == false );
            UT_ASSERT( job.IsUnresponsive() == false );
            UT_ASSERT( job.IsExpired() == false );
            job.Start();
            XJob::Status msg = job.GetStatus(); // Blocks until message.
            UT_ASSERT( msg == XJob::XJOBSTATUS_COMPLETE );
            UT_ASSERT( job.IsRunning() == false );
            UT_ASSERT( job.IsUnresponsive() == false );
            UT_ASSERT( job.IsExpired() == false );
            UT_ASSERT( job._val == 42 );
        }
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XJobTest::TestIsUnresponsive()
{
    try
    {
        // Test unresponsive job *without* max runtime.
        {
            XMyJob job(WATCHDOG_INTERVAL, 0, 0);
            UT_ASSERT( job.IsRunning() == false );
            UT_ASSERT( job.IsUnresponsive() == false );
            UT_ASSERT( job.IsExpired() == false );
            job.Start();
            XJob::Status msg = job.GetStatus(); // Blocks until message.
            UT_ASSERT( msg == XJob::XJOBSTATUS_UNRESPONSIVE );
            msg = job.GetStatus(); // Blocks until message.
            UT_ASSERT( msg == XJob::XJOBSTATUS_COMPLETE );
            UT_ASSERT( job.IsRunning() == false );
            UT_ASSERT( job.IsUnresponsive() == true );
            UT_ASSERT( job.IsExpired() == false );
            UT_ASSERT( job._val == 42 );
        }

        // Test unresponsive job *with* max runtime.
        {
            XMyJob job(WATCHDOG_INTERVAL, MAX_RUNTIME, 0);
            UT_ASSERT( job.IsRunning() == false );
            UT_ASSERT( job.IsUnresponsive() == false );
            UT_ASSERT( job.IsExpired() == false );
            job.Start();
            XJob::Status msg = job.GetStatus(); // Blocks until message.
            UT_ASSERT( msg == XJob::XJOBSTATUS_UNRESPONSIVE );
            msg = job.GetStatus(); // Blocks until message.
            UT_ASSERT( msg == XJob::XJOBSTATUS_COMPLETE );
            UT_ASSERT( job.IsRunning() == false );
            UT_ASSERT( job.IsUnresponsive() == true );
            UT_ASSERT( job.IsExpired() == false );
            UT_ASSERT( job._val == 42 );
        }
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XJobTest::TestIsExpired()
{
    try
    {
        // Test expired job *with* watchdog.
        {
            XMyJob job(WATCHDOG_INTERVAL, MAX_RUNTIME, XMyJob::XMYJOB_EXPIRE | XMyJob::XMYJOB_STROBE);
            UT_ASSERT( job.IsRunning() == false );
            UT_ASSERT( job.IsUnresponsive() == false );
            UT_ASSERT( job.IsExpired() == false );
            job.Start();
            XJob::Status msg = job.GetStatus(); // Blocks until message.
            UT_ASSERT( msg == XJob::XJOBSTATUS_EXPIRED );
            UT_ASSERT( job.IsRunning() == true );
            UT_ASSERT( job.IsUnresponsive() == false );
            UT_ASSERT( job.IsExpired() == true );
        }

        // Test expired job *without* watchdog.
        {
            XMyJob job(0, MAX_RUNTIME, XMyJob::XMYJOB_EXPIRE | XMyJob::XMYJOB_STROBE);
            UT_ASSERT( job.IsRunning() == false );
            UT_ASSERT( job.IsUnresponsive() == false );
            UT_ASSERT( job.IsExpired() == false );
            job.Start();
            XJob::Status msg = job.GetStatus(); // Blocks until message.
            UT_ASSERT( msg == XJob::XJOBSTATUS_EXPIRED );
            UT_ASSERT( job.IsRunning() == true );
            UT_ASSERT( job.IsUnresponsive() == false );
            UT_ASSERT( job.IsExpired() == true );
        }
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}

void XJobTest::TestJobReuse()
{
    try
    {
        XMyJob job(WATCHDOG_INTERVAL, MAX_RUNTIME, XMyJob::XMYJOB_STROBE);
        UT_ASSERT( job.IsRunning() == false );
        UT_ASSERT( job.IsUnresponsive() == false );
        UT_ASSERT( job.IsExpired() == false );
        job.Start();
        XJob::Status msg = job.GetStatus(); // Blocks until message.
        UT_ASSERT( msg == XJob::XJOBSTATUS_COMPLETE );
        UT_ASSERT_NO_THROW( job.Join() );
        UT_ASSERT( job.IsRunning() == false );
        UT_ASSERT( job.IsUnresponsive() == false );
        UT_ASSERT( job.IsExpired() == false );
        UT_ASSERT( job._val == 42 );

        // Run the job again.
        UT_ASSERT_NO_THROW( job.Start() );
        msg = job.GetStatus(); // Blocks until message.
        UT_ASSERT( msg == XJob::XJOBSTATUS_COMPLETE );
        UT_ASSERT( job.IsRunning() == false );
        UT_ASSERT( job.IsUnresponsive() == false );
        UT_ASSERT( job.IsExpired() == false );
        UT_ASSERT( job._val == 42 );
    }
    catch(XSDK::XException&)
    {
        UT_ASSERT(false && "Test threw!");
    }
}
