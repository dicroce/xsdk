
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef XSDK_XStreamBuffer_h
#define XSDK_XStreamBuffer_h

#include "XSDK/XMemory.h"
#include "XSDK/XStreamIO.h"
#include "XSDK/XCondition.h"

namespace XSDK
{

    class XStreamBuffer : public XStreamIO
    {
    public:
        X_API XStreamBuffer();
        X_API virtual ~XStreamBuffer() {}


        X_API bool Valid()
        {
            X_THROW(("Someone (i.e. Nick) was too lazy to implement me... - Concerned Coworker"));
        }

        /// Send data. May generate send timeout callbacks.
        /// \param msg A pointer to a memory buffer that is the SOURCE of data to send.
        /// \param msgLen The length of the data to send.
        X_API size_t Send( const void* msg, size_t msgLen );

        /// Send data. May generate send timeout callbacks.
        /// \param msg A pointer to a memory buffer that is the SOURCE of data to send.
        /// \param msgLen The length of the data to send.
        /// \param sendTimeout Override send timeout for this call only.
        X_API size_t Send( const void* msg, size_t msgLen, XDuration sendTimeout );
        X_API size_t Send( const void* msg, size_t msgLen, int sendTimeoutMillis )
        { return XStreamIO::Send(msg, msgLen, sendTimeoutMillis); }

        /// Send data. May write less bytes than asked for.
        /// \param msg A pointer to a memory buffer that is the SOURCE of data to send.
        /// \param msgLen The length of the data to send.
        X_API ssize_t RawSend( const void* msg, size_t msgLen );

        /// Receieve data. May generate recv timeout callbacks.
        /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
        /// \param msgLen The length of the data to receive.
        X_API size_t Recv( void* buf, size_t msgLen );

        /// Receieve data. May generate recv timeout callbacks.
        /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
        /// \param msgLen The length of the data to receive.
        /// \param recvTimeout Override recv timeout for this call only.
        X_API size_t Recv( void* buf, size_t msgLen, XDuration recvTimeout );
        X_API size_t Recv( void* buf, size_t msgLen, int recvTimeoutMillis )
        { return XStreamIO::Recv(buf, msgLen, recvTimeoutMillis); }

        /// Receieve data. May read less bytes than asked for.
        /// \param buf A pointer to a memory buffer that is filled in by the incoming data.
        /// \param msgLen The length of the data to receive.
        X_API ssize_t RawRecv( void* buf, size_t msgLen );

        /// This is a nop in this class
        X_API bool BufferedRecv();

        /// Attach a handler function pointer to send notifications about recv timeouts.
        /// \param rtcb A pointer to a function to call when recv times out.
        /// \param opaque User data to be passed to rtcb.
        X_API void AttachRecvTimeoutHandler( TimeoutHandler rtcb, void* opaque );

        /// Explicitly set a recv timeout value.
        /// \param timeout The length of time to attempt a recv before failing.
        X_API void SetRecvTimeout( XDuration timeout );
        X_API virtual void SetRecvTimeout( int timeoutMillis ) { XStreamIO::SetRecvTimeout(timeoutMillis); }

        /// Attach a handler function pointer to send notifications about send timeouts.
        /// \param stcb A pointer to a function to call when send times out.
        /// \param opaque User data to be passed to stcb.
        X_API void AttachSendTimeoutHandler( TimeoutHandler stcb, void* opaque );

        /// Explicitly set a send timeout value.
        /// \param timeout The length of time to attempt a send before failing.
        X_API void SetSendTimeout( XDuration timeout );
        X_API void SetSendTimeout( int timeoutMillis ) { XStreamIO::SetSendTimeout(timeoutMillis); }

        /// Wait for data to be available to be received on the socket.
        X_API bool WaitRecv( XDuration& waitTime );
        X_API bool WaitRecv( int& waitTimeMillis ) { return XStreamIO::WaitRecv(waitTimeMillis); }

        X_API bool WaitSend( XDuration& waitTime );
        X_API bool WaitSend( int& waitTimeMillis ) { return XStreamIO::WaitSend(waitTimeMillis); }

        /// Returns true if a RawRecv() would not block.
        X_API bool ReadyToRecv();

        /// Returns true if a RawSend() would not block.
        X_API bool ReadyToSend();

        /// Get Host used for Connect
        X_API virtual XSDK::XString GetHost() const
        { X_THROW(("Not yet implemnted.")); }

        X_API uint32_t GetHostPort() const
        { X_THROW(("Not yet implemnted.")); }


        X_API virtual XSDK::XString GetPeerIp() const
        { X_THROW(("Not yet implemnted.")); }

        X_API void Close();


    private:
        XStreamBuffer(const XStreamBuffer&);
        XStreamBuffer& operator=(const XStreamBuffer&);

        size_t _Send(const void* msg, size_t msgLen);
        size_t _Receive(void* buffer, size_t msgLen, XDuration timeout);


        std::list< XIRef<XSDK::XMemory> > _buffers;
        size_t _currentIndex;
        TimeoutHandler _receiveHandler;
        void* _opaqueReceieve;
        XDuration _receiveTimeout;
        XSDK::XMutex _conditionLock;
        XSDK::XCondition _condition;
    };

};



#endif//XStreamBuffer
