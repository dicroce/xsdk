#ifndef XServerTest_h
#define XServerTest_h

#ifdef UNICODE
#undef UNICODE
#endif

#include "framework.h"

#ifdef WIN32
#include <winsock2.h>
#endif // WIN32

#include "XSDK/XTaskBase.h"
#include "XSDK/XServer.h"
#include "XSDK/XConnBase.h"

class XServerTest : public test_fixture, public XSDK::XTaskBase
{
public:
    TEST_SUITE(XServerTest);
        TEST(XServerTest::TestStartStop);
        TEST(XServerTest::TestRoundTrip);
    TEST_SUITE_END();

    virtual ~XServerTest() throw()
    {}

    void setup();
    void teardown();

virtual void* EntryPoint();

protected:

    void TestStartStop();
    void TestRoundTrip();

private:
    int _testNum;
    int _val;
    XSDK::XString _interfaceAddr;

    XSDK::XServer _server;
};

class TestConn : public XSDK::XConnBase
{
    friend class XServerTest;

public:

    virtual XConnBase* Clone() const
    {
        TestConn* newTestConn = new TestConn;
        newTestConn->_st = _st;
        newTestConn->_server = _server;
        return newTestConn;
    }

    virtual void ServiceClient()
    {
        unsigned int val = 0;
        _clientConn->Recv( &val, 4 );
        val++;
        _clientConn->Send( &val, 4 );
    }

    virtual void SetServer(XSDK::XServer* server)
    {
        _server = server;
    }

private:

    XServerTest* _st;
    XSDK::XServer* _server;
};
#ifndef UNICODE
#define UNICODE
#endif

#endif
