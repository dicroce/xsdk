
//=============================================================================
//Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XConnBase.h $
// $Revision:$
// $Date:$
// $Author: Tony Di Croce $
//=============================================================================

#ifndef XSDK_XConnBase_h
#define XSDK_XConnBase_h

#include "XSDK/XTaskBase.h"
#include "XSDK/XSocket.h"

#include "XSDK/XException.h"

namespace XSDK
{

class XServer;

/// XConnBase is an abstract class that defines the interface for
/// a set of objects that "handle" incoming socket connection requests.
/// Essentially, ServerSocket is a generic server, and as such it's only
/// job is to wait for incoming connection, and when it recieves one allocate
/// the appropriate handler object and let it actually service the connection.
/// XConnBase defines the interface for these handler objects.
/// The steps to set up a server then are as follows:
/// Subclass XConnBase and implement all the required methods.
/// Create prototypical instance of your new subclassed connection object.
/// Create a ServerSocket object and pass your prototypical connection
/// object into it.
/// Call XServer::Start().
/// At this point, when an incoming connection is detected a new instance of
/// your connection object is created (by ServerSocket, via a call to
/// clone()). A thread is then created, and set to execute the new handler
/// objects "ServiceClient()" method. All threads are kept track of and
/// cleaned up automatically by XServer when the thread sets its running
/// flag to false.

class XConnBase : public XTaskBase
{
    friend class XServer;
public:

    /// No argument constructor
    X_API XConnBase();

    /// Destructor.
    X_API virtual ~XConnBase() throw();

    // XTaskBase Interface...
    X_API virtual void* EntryPoint();

    /// Creates a new object of the same type this method is a member.
    X_API virtual XConnBase* Clone() const = 0;

    /// General function that is called to handle (service) an incoming connection.
    X_API virtual void ServiceClient() = 0;

    /// Sets the server associated with this XConnBase object.
    X_API virtual void SetServer(XServer* server) = 0;

protected:
    XRef<XSDK::XSocket> _clientConn;
    bool _exited;
};

}

#endif

