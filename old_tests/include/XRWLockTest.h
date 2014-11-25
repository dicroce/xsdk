#ifndef XRWLockTest_h
#define XRWLockTest_h

#include "framework.h"

#include "XSDK/XTaskBase.h"
#include "XSDK/XRWLock.h"
#include "XSDK/XCommandQueue.h"
#include "XSDK/TimeUtils.h"

// XRWLock test class tests the functionality of XRWLocks.
class XRWLockTest : public test_fixture, public XSDK::XTaskBase
{
public:
    TEST_SUITE(XRWLockTest);
        TEST(XRWLockTest::TestMultipleReadLocks);
        TEST(XRWLockTest::TestMultipleWriteLocks);
        TEST(XRWLockTest::TestWriterWaitsforReaderToFinish);
    TEST_SUITE_END();

    //TEST(TestUnlockWhileNotLocked);
        // The linux implementation does not give an error on unlocking a lock which
        // was not locked previously. The behaviour in this case is undefined.
        // Thus we remove this test case/

        //TEST(TestWritePrefferingLocks);
    // The linux implementation of read write locks prefers readers.
    // So we could see writer starvation and the below test case thus fails.
    // We could use the pthread_rwlockattr_setkind_np() function to set the
    // PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP flag on the attr that we
    // pass to pthread_rwlock_init(). Could not find proper documentation
    // or man page for this attribute and are thus ommiting the write-preferred
    // behaviour test for the RWLocks

        //TEST(TestRecursiveLocks);
        // Windows implementation does not support recursive RWLocks,
        // thus omitting the test case

    virtual ~XRWLockTest() throw(){}
    virtual void setup();
    virtual void teardown();
    virtual void* EntryPoint();

protected:

    void TestMultipleReadLocks();
    void TestMultipleWriteLocks();
    void TestWriterWaitsforReaderToFinish();
    void TestWritePrefferingLocks();
    void TestRecursiveLocks();
    void TestUnlockWhileNotLocked();

private:

    int _testNum;
    int _readValue;
    int _writeValue;
    XSDK::XRWLock _rwLock;
};

class ReadLockWorkQueue : public XSDK::XCommandQueue<XSDK::XRWLock*>
{
public:
        virtual bool Process( XSDK::XRWLock* rwLock )
        {
                rwLock->AcquireRead();

                XSDK::x_sleep(4);

                rwLock->Release();

                return true;
        }
};

class WriteLockWorkQueue : public XSDK::XCommandQueue<XSDK::XRWLock*>
{
public:
        virtual bool Process( XSDK::XRWLock* rwLock )
        {
                rwLock->AcquireWrite();

                XSDK::x_sleep(4);

                rwLock->Release();

                return true;
        }
};

#endif
