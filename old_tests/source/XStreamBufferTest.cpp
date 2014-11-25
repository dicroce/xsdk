
#include "XStreamBufferTest.h"

#include "XSDK/XSocket.h"
#include "XSDK/XGuard.h"
#include "XSDK/TimeUtils.h"
#include "XSDK/XStreamBuffer.h"
#include "XSDK/OS.h"

#ifndef WIN32
#include <unistd.h>
#endif

using namespace XSDK;
using namespace std;

REGISTER_TEST_FIXTURE(XStreamBufferTest);

const static XString TEST_ZERO("zero");
const static XString TEST_ONE("one");
const static XString TEST_TWO("two");
const static XString TEST_THREE("three");
const static XString TEST_PATTERN("0X");


bool XStreamBufferTest::RecvTimeoutHandler( void* opaque )
{
    XStreamBufferTest* st = (XStreamBufferTest*)opaque;
    st->_recieveTimedOut = true;
    return false;
}

void XStreamBufferTest::setup()
{
    _streamIO = new XStreamBuffer;
    _testSucceed = false;
    _recieveTimedOut = false;

}

void XStreamBufferTest::teardown()
{
    _streamIO.Clear();
}

void* XStreamBufferTest::EntryPoint()
{
    switch( _testNum )
    {
    case 0:
    {
        x_sleep(3);
        _streamIO->Send(TEST_PATTERN.c_str(),TEST_PATTERN.length());
    }
    break;
    case 1:
    {
        x_sleep(3);
        _streamIO->Send(TEST_ONE.c_str(),TEST_ONE.length());
    }
    break;
    default:
        break;
    };
    return NULL;
}


void XStreamBufferTest::TestWaitSent()
{
    int temp = 1234;
    UT_ASSERT( !_streamIO->WaitSend(temp) );
    UT_ASSERT( temp == 1234 );
}

void XStreamBufferTest::TestReadyToSend()
{
    UT_ASSERT( _streamIO->ReadyToSend() );
}




void XStreamBufferTest::TestWaitRecv()
{

    {
        int temp = 7000;
        UT_ASSERT( _streamIO->WaitRecv(temp) );
                UT_ASSERT( (temp >= -250) && (temp <= 250) );
    }
    {
        int temp = 7000;
        _testNum = 1;
        Start();
        UT_ASSERT( !_streamIO->WaitRecv(temp) );
        UT_ASSERT( temp >= 3900 && temp <= 5000 );
        char buffer[5];
        memset(buffer,0,5);
        size_t count = _streamIO->RawRecv(buffer,5);
        UT_ASSERT( count == TEST_ONE.length());
    }
}

void XStreamBufferTest::TestReadyToRecv()
{
    UT_ASSERT( !_streamIO->ReadyToRecv() );
    size_t count = _streamIO->RawSend(TEST_PATTERN.c_str(),TEST_PATTERN.length());
    UT_ASSERT( count == TEST_PATTERN.length() );
    char buffer[5];
    memset(buffer,0,5);
    UT_ASSERT( _streamIO->ReadyToRecv() );
    count = _streamIO->RawRecv(buffer,5);
    UT_ASSERT( count == TEST_PATTERN.length() );
}


void XStreamBufferTest::TestSetRecvTimeout()
{
    _streamIO->AttachRecvTimeoutHandler( XStreamBufferTest::RecvTimeoutHandler,this);
    {//Test No data call back generated
        _recieveTimedOut = false;
        timeval before, after;
        x_gettimeofday(&before);
        char buffer[5];
        memset(buffer,0,5);
        _streamIO->SetRecvTimeout(8000);
        size_t count = _streamIO->Recv(buffer,TEST_ZERO.length());
        x_gettimeofday(&after);
        int64_t result = 0;
        timerdiff(&after,&before,result);
        UT_ASSERT( _recieveTimedOut );
        UT_ASSERT( result/1000000 == 8 );
        UT_ASSERT( count == 0);
    }
}

void XStreamBufferTest::TestRecvWithTimeoutCallBack()
{
    _streamIO->AttachRecvTimeoutHandler( XStreamBufferTest::RecvTimeoutHandler,this);
    {//Test No data call back generated
        _recieveTimedOut = false;
        timeval before, after;
        x_gettimeofday(&before);
        char buffer[5];
        memset(buffer,0,5);
        size_t count = _streamIO->Recv(buffer,TEST_ZERO.length(),7000);
        x_gettimeofday(&after);
        int64_t result = 0;
        timerdiff(&after,&before,result);
        UT_ASSERT( _recieveTimedOut );
        UT_ASSERT( result/1000000 == 7 );
        UT_ASSERT( count == 0);
    }
}
void XStreamBufferTest::TestRecvCallBack()
{
    _streamIO->AttachRecvTimeoutHandler( XStreamBufferTest::RecvTimeoutHandler,this);
    {//Test Recieve data there no call back generated
        _recieveTimedOut = false;
        size_t count = _streamIO->RawSend(TEST_ZERO.c_str(),TEST_ZERO.length());
        UT_ASSERT( count == TEST_ZERO.length());
        char buffer[5];
        memset(buffer,0,5);
        count = _streamIO->Recv(buffer,TEST_ZERO.length());
        UT_ASSERT( count == TEST_ZERO.length());
        UT_ASSERT( TEST_ZERO == buffer );
    }
    {//Test No data call back generated
        _recieveTimedOut = false;
        timeval before, after;
        x_gettimeofday(&before);
        char buffer[5];
        memset(buffer,0,5);
        size_t count = _streamIO->Recv(buffer,TEST_ZERO.length());
        x_gettimeofday(&after);
        int64_t result = 0;
        timerdiff(&after,&before,result);
        UT_ASSERT( _recieveTimedOut );
        UT_ASSERT( result/1000000 == 5 );
        UT_ASSERT( count == 0);
    }
    {//Test partial data call back generated
        _recieveTimedOut = false;
        size_t count = _streamIO->RawSend(TEST_ZERO.c_str(),TEST_ZERO.length());
        UT_ASSERT( count == TEST_ZERO.length());
        char buffer[5];
        memset(buffer,0,5);
        timeval before, after;
        x_gettimeofday(&before);
        count = _streamIO->Recv(buffer,TEST_ZERO.length()+1);
        x_gettimeofday(&after);
        int64_t result = 0;
        timerdiff(&after,&before,result);
        UT_ASSERT( _recieveTimedOut );
        UT_ASSERT( result/1000000 == 5 );
        UT_ASSERT( count == TEST_ZERO.length());
        UT_ASSERT( TEST_ZERO == buffer );  }
    {//Test no data for part time then data no call back generated
        _recieveTimedOut = false;
        _testNum = 0;
        XString final = TEST_PATTERN+TEST_PATTERN+TEST_PATTERN;
        size_t count = _streamIO->RawSend(TEST_PATTERN.c_str(),TEST_PATTERN.length());
        count += _streamIO->RawSend(TEST_PATTERN.c_str(),TEST_PATTERN.length());
        UT_ASSERT( count == TEST_PATTERN.length()*2);
        char buffer[7];
        memset(buffer,0,7);
        Start();
        timeval before, after;
        x_gettimeofday(&before);
        count = _streamIO->Recv(buffer,final.length());
        x_gettimeofday(&after);
        int64_t result = 0;
        timerdiff(&after,&before,result);
        UT_ASSERT( !_recieveTimedOut );
        UT_ASSERT( result/1000000 == 3 );
        UT_ASSERT( count == final.length());
        UT_ASSERT( final == buffer );
    }
}

void XStreamBufferTest::TestWriteRecieveRaw()
{
    {//Write four seperate, read back four seperate
        size_t count = _streamIO->RawSend(TEST_ZERO.c_str(),TEST_ZERO.length());
        UT_ASSERT( count == TEST_ZERO.length());
        count = _streamIO->RawSend(TEST_ZERO.c_str(),TEST_ZERO.length());
        UT_ASSERT( count == TEST_ZERO.length());
        count = _streamIO->RawSend(TEST_ZERO.c_str(),TEST_ZERO.length());
        UT_ASSERT( count == TEST_ZERO.length());
        count = _streamIO->RawSend(TEST_ZERO.c_str(),TEST_ZERO.length());
        UT_ASSERT( count == TEST_ZERO.length());
        char buffer[5];
        memset(buffer,0,5);
        count = _streamIO->RawRecv(buffer,TEST_ZERO.length());
        UT_ASSERT( count == TEST_ZERO.length());
        UT_ASSERT( TEST_ZERO == buffer );
        memset(buffer,0,5);
        count = _streamIO->RawRecv(buffer,TEST_ZERO.length());
        UT_ASSERT( count == TEST_ZERO.length());
        UT_ASSERT( TEST_ZERO == buffer );
        memset(buffer,0,5);
        count = _streamIO->RawRecv(buffer,TEST_ZERO.length());
        UT_ASSERT( count == TEST_ZERO.length());
        UT_ASSERT( TEST_ZERO == buffer );
        memset(buffer,0,5);
        count = _streamIO->RawRecv(buffer,TEST_ZERO.length());
        UT_ASSERT( count == TEST_ZERO.length());
        UT_ASSERT( TEST_ZERO == buffer );
        count = _streamIO->RawRecv(buffer,TEST_ZERO.length());
        UT_ASSERT( count == 0 );
    }
    {//Write two separate, read back one total
        size_t count = _streamIO->RawSend(TEST_ZERO.c_str(),TEST_ZERO.length());
        UT_ASSERT( count == TEST_ZERO.length());
        count = _streamIO->RawSend(TEST_ZERO.c_str(),TEST_ZERO.length());
        UT_ASSERT( count == TEST_ZERO.length());
        char buffer[9];
        memset(buffer,0,9);
        XString temp = TEST_ZERO;
        temp += TEST_ZERO;
        count = _streamIO->RawRecv(buffer,temp.length());
        UT_ASSERT( count == temp.length());
        UT_ASSERT( temp == buffer );
        count = _streamIO->RawRecv(buffer,TEST_ZERO.length());
        UT_ASSERT( count == 0 );
    }
    {//Write one total, read back two seperate
        XString temp = TEST_ZERO;
        temp += TEST_ZERO;
        size_t count = _streamIO->RawSend(temp.c_str(),temp.length());
        UT_ASSERT( count == temp.length());
        char buffer[5];
        memset(buffer,0,5);
        count = _streamIO->RawRecv(buffer,TEST_ZERO.length());
        UT_ASSERT( count == TEST_ZERO.length());
        UT_ASSERT( TEST_ZERO == buffer );
        memset(buffer,0,5);
        count = _streamIO->RawRecv(buffer,TEST_ZERO.length());
        UT_ASSERT( count == TEST_ZERO.length());
        UT_ASSERT( TEST_ZERO == buffer );
        count = _streamIO->RawRecv(buffer,TEST_ZERO.length());
        UT_ASSERT( count == 0 );
    }
    {//Write 4, read 5, but get 4
        size_t count = _streamIO->RawSend(TEST_ZERO.c_str(),TEST_ZERO.length());
        UT_ASSERT( count == TEST_ZERO.length());
        char buffer[9];
        memset(buffer,0,9);
        count = _streamIO->RawRecv(buffer,9);
        UT_ASSERT( count == TEST_ZERO.length());
        UT_ASSERT( TEST_ZERO == buffer );
        count = _streamIO->RawRecv(buffer,9);
        UT_ASSERT( count == 0 );
    }
    {//Write 10/12/10, Read back 8/8/8/8
        XString value0 = TEST_PATTERN + TEST_PATTERN + TEST_PATTERN + TEST_PATTERN ;
        XString value1 = value0 + TEST_PATTERN;
        XString value2 = value1 + TEST_PATTERN;
        size_t count = _streamIO->RawSend(value1.c_str(),value1.length());
        UT_ASSERT( count == value1.length());
        count = _streamIO->RawSend(value2.c_str(),value2.length());
        UT_ASSERT( count == value2.length());
        count = _streamIO->RawSend(value1.c_str(),value1.length());
        UT_ASSERT( count == value1.length());
        for ( int i = 0; i < 4; ++i )
        {
            char buffer[9];
            memset(buffer,0,9);
            count = _streamIO->RawRecv(buffer,value0.length());
            UT_ASSERT( count == value0.length());
            UT_ASSERT( value0 == buffer );
        }
        char buffer[9];
        memset(buffer,0,9);
        count = _streamIO->RawRecv(buffer,8);
        UT_ASSERT( count == 0 );
    }
}
