
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XServer.h"

using namespace XSDK;

XServer::XServer() :
    _ifAddr(),
    _serverSok(new XSocket),
    _prototype( NULL ),
    _port( -1 ),
    _connections(),
    _running( false )
{
}

XServer::~XServer() throw()
{
    // XXX - this should be XIRef<>
    delete _prototype;
}

void XServer::SetPortNum( int p )
{
    _port = p;
}

int XServer::GetPortNum() const
{
    return _port;
}

void XServer::SetInterface(const XString& ifAddr)
{
    _ifAddr = ifAddr;
}

XString XServer::GetInterface() const
{
    return _ifAddr;
}

void XServer::AttachConnectionPrototype( XConnBase* proto )
{
    if( !proto )
        X_THROW(( "NULL connection prototype passed to XSDK::XServer." ));

    delete _prototype;

    _prototype = proto->Clone();
}

void XServer::Start()
{
    if( !_prototype || (_port <= 0) )
        X_THROW(( "XServer needs a connection object and valid port number." ));

    _running = true;

    _Prepare();

    while( _running )
    {
        XConnBase* conn = _prototype->Clone();
        if( !conn )
            X_THROW(( "XServer: Unable to clone connection object." ));

        conn->_clientConn = _serverSok->Accept();

        if( !_running )
        {
            delete conn;
            continue;
        }

        _connections.Append( conn );

        conn->SetServer(this);
        conn->Start();

        _PruneOldConnections();
    }

    x_foreach( _connections )
    {
        XConnBase* conn = *_connections.IterData();

        conn->Join();

        delete conn;
    }

    _connections.Clear();
    _serverSok->Close();
}

void XServer::Stop()
{
    _running = false;

    XSocket conn;

    if( _ifAddr == ip4_addr_any )
        conn.Connect( (char*)"127.0.0.1", _port );
    else if( _ifAddr == ip6_addr_any )
        conn.Connect( (char*)"::1", _port );
    else
        conn.Connect( _ifAddr, _port );
    conn.Close();
}

bool XServer::Running() const
{
    return _running;
}

void XServer::_Prepare()
{
    if( _port == -1 )
        X_THROW(( "XServer: Invalid port number specified." ));

    _serverSok->Bind( _port, _ifAddr );

    _serverSok->Listen();
}

void XServer::_PruneOldConnections()
{
    _connections.IterStart();

    while( _connections.IterValid() )
    {
        bool autoNext = false;

        XConnBase* conn = *_connections.IterData();

        if( conn && (conn->_exited == true) )
        {
            conn->Join();

            _connections.RemoveAtIter();

            delete conn;

            autoNext = true;
        }

        if( autoNext == false )
            _connections.Iterate();
    }
}
