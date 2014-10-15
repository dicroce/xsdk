#ifndef XStreamBufferTest_h
#define XStreamBufferTest_h

#include "framework.h"

#ifdef WIN32
#include <winsock2.h>
#endif // WIN32

#include "XSDK/XTaskBase.h"
#include "XSDK/XSocket.h"
#include "XSDK/XMutex.h"
#include "XSDK/XStreamIO.h"

class XStreamBufferTest : public test_fixture, public XSDK::XTaskBase
{
public:
    TEST_SUITE(XStreamBufferTest);
        TEST(XStreamBufferTest::TestRecvCallBack);
        TEST(XStreamBufferTest::TestWriteRecieveRaw);
        TEST(XStreamBufferTest::TestRecvWithTimeoutCallBack);
        TEST(XStreamBufferTest::TestSetRecvTimeout);
        TEST(XStreamBufferTest::TestWaitRecv);
        TEST(XStreamBufferTest::TestWaitSent);
        TEST(XStreamBufferTest::TestReadyToRecv);
        TEST(XStreamBufferTest::TestReadyToSend);
    TEST_SUITE_END();

    virtual ~XStreamBufferTest() throw()
    {}

    void setup();
    void teardown();

    virtual void* EntryPoint();

    bool _recieveTimedOut;

    static bool RecvTimeoutHandler( void* opaque );

protected:
    void TestWriteRecieveRaw();
    void TestRecvCallBack();
    void TestRecvWithTimeoutCallBack();
    void TestSetRecvTimeout();
    void TestWaitRecv();
    void TestWaitSent();
    void TestReadyToRecv();
    void TestReadyToSend();
private:
    XRef<XSDK::XStreamIO> _streamIO;
    int _testNum;
    bool _testSucceed;
    XSDK::XMutex _lock;
};

#endif

