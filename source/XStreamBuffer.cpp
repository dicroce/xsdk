#include "XSDK/XStreamBuffer.h"
#include "XSDK/XLogger.h"
#include "XSDK/OS.h"
#include "XSDK/XMonoClock.h"


using namespace XSDK;


XStreamBuffer::XStreamBuffer():XStreamIO(),
_buffers(),
_currentIndex(0),
_receiveHandler(NULL),
_opaqueReceieve(NULL),
_receiveTimeout(XDuration(SECONDS, 5)),
_conditionLock(),
_condition(_conditionLock)
{
}


size_t XStreamBuffer::_Send(const void* msg, size_t msgLen)
{
    if ( msgLen <= 0 )
    {
        X_LOG_INFO("[XStreamBuffer] message length 0");
        return 0;
    }
    if ( msg == NULL )
    {
        X_LOG_INFO("[XStreamBuffer] msg is null");
        return 0;
    }
    XIRef<XMemory> memory = new XMemory;
    memcpy(&(memory->Extend(msgLen)),msg,msgLen);
    _buffers.push_front(memory);
    XGuard guard(_conditionLock);
    _condition.Signal();
    return memory->GetDataSize();
}

size_t XStreamBuffer::Send( const void* msg, size_t msgLen )
{
    return _Send(msg,msgLen);
}

size_t XStreamBuffer::Send( const void* msg, size_t msgLen, XDuration sendTimeout )
{
    return _Send(msg, msgLen);
}

ssize_t XStreamBuffer::RawSend( const void* msg, size_t msgLen )
{
    return _Send(msg,msgLen);
}

size_t XStreamBuffer::_Receive(void* buffer, size_t msgLen, XDuration timeout)
{
    if ( msgLen <= 0 )
    {
        X_LOG_INFO("[XStreamBuffer] message length 0");
        return 0;
    }

    size_t readCount = 0;
    bool timedOut = false;
    uint64_t timeToWaitUntil = XCondition::FutureMonotonicTime(timeout);

    do
    {
        XGuard guard(_conditionLock);
        XIRef<XMemory> memory = _buffers.empty() ? NULL : _buffers.back();

        while( !timedOut && memory.IsEmpty() )
        {
            timedOut = _condition.WaitUntil(timeToWaitUntil);
            memory = _buffers.empty() ? NULL : _buffers.back();
        }

        if ( timedOut )
        {
            if ( _receiveHandler == NULL )
                return readCount;
            else if ( _receiveHandler(_opaqueReceieve) )
            {
                timedOut = false;
                timeToWaitUntil = XCondition::FutureMonotonicTime(timeout);
                continue;
            }
            else
                return readCount;
        }

        size_t countToRead = ((memory->GetDataSize()-_currentIndex) > (msgLen - readCount)) ? (msgLen - readCount) : (memory->GetDataSize()-_currentIndex);
        memcpy((((unsigned char*)buffer)+readCount),&(memory->Index(_currentIndex)),countToRead);
        readCount += countToRead;
        _currentIndex += countToRead;

        if ( _currentIndex >= memory->GetDataSize() )
        {
            _currentIndex = 0;
            _buffers.pop_back();
            memory = _buffers.empty() ? NULL : _buffers.back();
        }
    }while( readCount < msgLen && !timedOut );
    return readCount;
}

size_t XStreamBuffer::Recv( void* buf, size_t msgLen )
{
    return _Receive(buf, msgLen, _receiveTimeout);
}

size_t XStreamBuffer::Recv( void* buf, size_t msgLen, XDuration recvTimeout )
{
    return _Receive(buf, msgLen, recvTimeout);
}


ssize_t XStreamBuffer::RawRecv( void* buf, size_t msgLen )
{
    if ( msgLen <= 0 )
    {
        X_LOG_INFO("[XStreamBuffer] message length 0");
        return 0;
    }
    XIRef<XMemory> memory = _buffers.empty() ? NULL : _buffers.back();
    if ( memory.IsEmpty() )
    {
        X_LOG_INFO("[XStreamBuffer] no data to read");
        return 0;
    }
    size_t readCount = 0;
    while ( !memory.IsEmpty() && readCount < msgLen )
    {
        size_t countToRead = ((memory->GetDataSize()-_currentIndex) > (msgLen - readCount)) ? (msgLen - readCount) : (memory->GetDataSize()-_currentIndex);
        memcpy((((unsigned char*)buf)+readCount),&(memory->Index(_currentIndex)),countToRead);
        readCount += countToRead;
        _currentIndex += countToRead;
        if ( _currentIndex >= memory->GetDataSize() )
        {
            _currentIndex = 0;
            _buffers.pop_back();
            memory = _buffers.empty() ? NULL : _buffers.back();
        }
    }
    return readCount;
}

bool XStreamBuffer::BufferedRecv()
{
    /// For XStreamIO completeness
    return true;
}

void XStreamBuffer::AttachRecvTimeoutHandler( TimeoutHandler rtcb, void* opaque )
{
    _receiveHandler = rtcb;
    _opaqueReceieve = opaque;
}

void XStreamBuffer::SetRecvTimeout( XDuration timeout )
{
    _receiveTimeout = timeout;
}

void XStreamBuffer::AttachSendTimeoutHandler( TimeoutHandler /*stcb*/, void* /*opaque*/)
{///This function is intentionally empty there is no need to time out sends
}

void XStreamBuffer::SetSendTimeout( XDuration /*timeout*/ )
{///This function is intentionally empty there is no need to time out sends
}

bool XStreamBuffer::WaitRecv( XDuration& waitTime )
{
    const uint64_t before = XMonoClock::GetTime();
    uint64_t timeToWaitUntil = XCondition::FutureMonotonicTime(waitTime);
    bool timedOut = false;

    XGuard guard(_conditionLock);
    XIRef<XMemory> memory = _buffers.empty() ? NULL : _buffers.back();

    while( !timedOut && memory.IsEmpty() )
    {
        timedOut = _condition.WaitUntil(timeToWaitUntil);
        memory = _buffers.empty() ? NULL : _buffers.back();
    }

    waitTime = timedOut ? XDuration::Zero()
                        : waitTime - XDuration(HNSECS, XMonoClock::GetElapsedTime(before, XMonoClock::GetTime()) * 10000000);

    return timedOut;
}

bool XStreamBuffer::WaitSend( XDuration& /*waitTime*/ )
{///This is always true
    return false;
}

bool XStreamBuffer::ReadyToRecv()
{
    return !_buffers.empty();
}

bool XStreamBuffer::ReadyToSend()
{///This is always true
    return true;
}


void XStreamBuffer::Close()
{
    _buffers.clear();
}
