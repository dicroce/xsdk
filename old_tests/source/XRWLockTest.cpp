
#include "XRWLockTest.h"
#include "XSDK/TimeUtils.h"

using namespace XSDK;

REGISTER_TEST_FIXTURE(XRWLockTest);

void XRWLockTest::setup()
{
}

void XRWLockTest::teardown()
{
}

// XXX - READ THIS
//
// These tests require spawning multiple threads. Since their can be only 1 EntryPoint() method,
// _testNum is used to control what actual work is done when the thread starts.
// If another thread is required then command queues are used.
// TODO: Remove inherinting from Xtaskbase and just use multiple command queues to do the work.

void* XRWLockTest::EntryPoint()
{
        switch( _testNum )
        {
        case 0:
        {
                _rwLock.AcquireRead();

                x_sleep( 2 );

                _readValue++;

                _rwLock.Release();
                return &_readValue;
        }
        break;
        case 1:
        {
                _rwLock.AcquireWrite();

                x_sleep( 2 );

                _writeValue = 100;

                _rwLock.Release();

                return &_writeValue;
        }

        break;
        default:
                break;
        };

        return NULL;
}

// Initially, _readValue is 0.
// We then Start our thread and go to x_sleep for 1 second (plenty of time for the thread to start).
// While the main thread x_sleeps, our worker wakes up and grabs the read lock and then goes to x_sleep.
// The main thread then wakes up and attempts to grab a read lock. It succeeds and checks that
// the value has not been changed yet. It then again sleeps for 2 seconds. This give enough time for the
// Start Thread to come out of sleep and increment the read value.Finally, the main thread checks
// that the read value == 1. We are modifying a variable under read lock only to verify that the thread
// was acutally under the read thread.
void XRWLockTest::TestMultipleReadLocks()
{

        try
        {
                _readValue = 0;

                // Spawn first read thread
                _testNum = 0;
                Start();

                // Give enought time for Start Read thread to spawn.
                x_sleep(1);

                //Spawn second read thread
                _rwLock.AcquireRead();

                // Assert value was not updated yet and the Start thread is holding the reader lock, even though
                // we have aquired another reader lock.
                UT_ASSERT( _readValue == 0 );

                x_sleep(2);

                // Assert that value was updated by the thread launched inside Start method
                // Implying that thread also had the read lock
                UT_ASSERT( _readValue == 1 );

                _rwLock.Release();

                Join();
        }

        catch(XSDK::XException&)
        {
                UT_ASSERT( false && "Test threw!" );
        }
}

// Initially, _writeValue is 0.
// We then Start our thread and go to x_sleep for 1 second (plenty of time for the thread to start).
// While the main thread x_sleeps, our worker wakes up and grabs the write lock and then goes to x_sleep.
// The main thread then wakes up and attempts to grab a write lock. It blocks.
// Start Thread comes out of sleep and sets the write value to 100 and relases the lock.
// Finally, the main thread is able to acquire the write lock and it checks that the write value == 100.

void XRWLockTest::TestMultipleWriteLocks()
{

        // Spawn first write thread
        _testNum = 1;
        _writeValue = 0;
        Start();

        x_sleep( 1 );

        //Try to aquire write lock. The process will block waiting for the Start thread
        // to change the value and release the lock.

        _rwLock.AcquireWrite();

        UT_ASSERT( _writeValue == 100 );

        _rwLock.Release();

        Join();
}

// We will have two readers holding the lock when the writer tries to acquire the lock.
// The writer will block untill the reader releases the lock.
void XRWLockTest::TestWriterWaitsforReaderToFinish()
{

                _readValue = 0;
                _writeValue = 0;
                _testNum = 0;

                // Spawn the first read thread which waits for 2 seconds
                Start();
                x_sleep(1);

                // Try write Lock. It should fail because 1 read thread has locked
                UT_ASSERT( _rwLock.TryAcquireWrite() == false );

                // Spawn the second read thread which waits for 4 seconds
                ReadLockWorkQueue readQueue;
                readQueue.Startup();
                readQueue.Post( &_rwLock );

                x_sleep(1);

                // Try write Lock. It should fail.
                UT_ASSERT( _rwLock.TryAcquireWrite() == false );

                x_sleep(1);

                // Assert that first thread has quit, but still we can't lock it
                UT_ASSERT(  _readValue == 1 );
                UT_ASSERT(  _rwLock.TryAcquireWrite() == false );

                x_sleep(3);

                //Assert that now we can acquire the lock.
                UT_ASSERT(  _rwLock.TryAcquireWrite() == true);

                // Release the lock and shutdown.
                _rwLock.Release();

                Join();
                readQueue.Shutdown();
}


// Try to acquire multiple read locks in the same thread.
// Try to acquire multiple write locks in the same thread.
void XRWLockTest::TestRecursiveLocks()
{

                // Should throw, since _rwLock is not locked.
                _rwLock.AcquireRead();
                _rwLock.AcquireRead();

                x_sleep(1);

                _rwLock.Release();
                _rwLock.Release();

                // Test Recursive write lock

                _rwLock.AcquireWrite();
                _rwLock.AcquireWrite();

                x_sleep(1);

                _rwLock.Release();
                _rwLock.Release();
}

// Trying to release a lock without holding it should throw an exception
void XRWLockTest::TestUnlockWhileNotLocked()
{

                // Should throw, since _rwLock is not locked.
                _rwLock.Release();

        UT_ASSERT( false && "Unlocking an unlocked RWLock DID NOT THROW!" );
}

// We will have one reader holding the lock when the writer tries to acquire the lock.
// The writer will block untill the reader releases the lock.
// Another reader also tries to block. This reader gets blocked.
// After first reader releases the writer is allowed to hold the lock
// Only after first writer releases the lock, the second reader is allowed to acquire the lock.
void XRWLockTest::TestWritePrefferingLocks()
{

                _readValue = 0;
                _testNum = 0;

                //Spawn the reader thread which waits for 4 seconds.
                ReadLockWorkQueue readQueue;
                readQueue.Startup();
                readQueue.Post( &_rwLock );
                x_sleep(1);

                // Spawn the write thread, waits for 4 seconds, writes
                WriteLockWorkQueue writeQueue;
                writeQueue.Startup();
                writeQueue.Post( &_rwLock );
                x_sleep(1);

                // Test 1: Try to acquire the read lock

                UT_ASSERT(  _rwLock.TryAcquireRead() == false );

                // Test 2: Wait for the availability of read lock
                // Spawn the reader thread which waits for 2 seconds
                // The first read thread is still holding the lock( it keeps the lock for 4 seconds)
                // This reader thead will not get the lock because a writer is waiting for it.
                // We will confirm it after after the first read thread has quit as well as this
                // thread has had enought time(2 secs) to make the change.
                Start();
                x_sleep(3);

                //Read thread has been active for 3 seconds.
                //Assert that it has not been able to change the value because it did not get the lock

                UT_ASSERT( _readValue == 1 );

                x_sleep(4);

                Join();
                readQueue.Shutdown();
                writeQueue.Shutdown();
}
