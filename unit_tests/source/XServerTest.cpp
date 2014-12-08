
#include "XServerTest.h"
#include "XSDK/TimeUtils.h"

#ifndef WIN32
#include <unistd.h>
#endif

using namespace XSDK;

REGISTER_TEST_FIXTURE(XServerTest);



void XServerTest::setup()
{
    _interfaceAddr.clear();
}

void XServerTest::teardown()
{
}

void* XServerTest::EntryPoint()
{
    switch( _testNum )
    {
    case 0:
    {
        try
        {
            TestConn tc;
            tc._st = this;
            _server.AttachConnectionPrototype( &tc );
            _server.SetPortNum( 4242 );
            _server.SetInterface( _interfaceAddr );
            _server.Start();
        }

        catch( XSDK::XException& ex )
        {
            printf("%s @ %d: %s\n", ex.GetSrcFile(), ex.GetLineNum(), ex.what());
            fflush(stdout);
            UT_ASSERT( false && "Test threw!" );
        }
    }
    break;
    case 1:
    {
        try
        {
            TestConn tc;
            tc._st = this;
            _server.AttachConnectionPrototype( &tc );
            _server.SetPortNum( 4343 );
            _server.SetInterface( _interfaceAddr );
            _server.Start();
        }

        catch( XSDK::XException& ex )
        {
            printf("%s @ %d: %s\n", ex.GetSrcFile(), ex.GetLineNum(), ex.what());
            fflush(stdout);
            UT_ASSERT( false && "Test threw!" );
        }
    }
    break;
    default:
        break;
    };

    return &_val;
}

void XServerTest::TestStartStop()
{

    try
    {
        const char addrs[2][32] = { "127.0.0.1", "::1" };
        for (int ii=0; ii<2; ++ii)
        {

            _testNum = 0;
            _interfaceAddr = addrs[ii];

            Start();

            while(_server.Running() == false)
                x_usleep( 1000000 );

            _server.Stop();

            Join();
        }
    }
    catch(XSDK::XException& ex)
    {
        printf("%s @ %d: %s\n", ex.GetSrcFile(), ex.GetLineNum(), ex.what());
        fflush(stdout);
        UT_ASSERT( false && "Test threw!" );
    }
}

void XServerTest::TestRoundTrip()
{

    try
    {
        const char addrs[2][32] = { "127.0.0.1", "::1" };
        for (int ii=0; ii<2; ++ii)
        {

            _testNum = 1;
            _interfaceAddr = addrs[ii];

            Start();

            x_sleep( 1 );

            XSocket client;
            client.Connect( _interfaceAddr, 4343 );

            unsigned int val = 41;
            client.Send( &val, 4 );

            UT_ASSERT( val == 41 );
            client.Recv( &val, 4 );
            UT_ASSERT( val == 42 );

            _server.Stop();

            Join();
        }
    }

    catch( XSDK::XException& ex )
    {
        printf("%s @ %d: %s\n", ex.GetSrcFile(), ex.GetLineNum(), ex.what());
        fflush(stdout);
        UT_ASSERT( false && "Test threw!" );
    }
}

