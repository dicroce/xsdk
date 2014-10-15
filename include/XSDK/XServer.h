
//=============================================================================
//Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XServer.h $
// $Revision:$
// $Date:$
// $Author: Tony Di Croce $
//=============================================================================


#ifndef XSDK_XServer_h
#define XSDK_XServer_h


#include "XSDK/XException.h"
#include "XSDK/XSocket.h"
#include "XSDK/XConnBase.h"
#include "XSDK/XInternalList.h"
#include "XSDK/XString.h"

namespace XSDK
{

/// XServer is a general purpose server class. It can be used to create
/// nearly any type of tcp server. Check out the doxygen for XConnBase for a
/// more detailed description of how to use this object. Also, check out test_k2.cpp
/// for sample code.
class XServer
{
public:

  /// @param addrFamily Determines the interface to bind to.
  X_API XServer();

  X_API virtual ~XServer() throw();

  /// Set the tcp port this server listens on.
  X_API void SetPortNum( int p );

  /// Get the port that this server is listening on.
  X_API int GetPortNum() const;

  /// Set the network interface this server is listening on.
  /// @param ifAddr The interface IP address or ip4_addr_any/ip6_addr_any
  X_API void SetInterface(const XString& ifAddr);

  /// Get the network interface this server is listening on.
  X_API XString GetInterface() const;

  /// Attach a prototypical XConnBase object. WARNING: This object MUST be
  /// allocated on the heap.
  /// \param proto A pointer to the heap allocated prototypical XConnBase
  /// object that will be cloned upon new connection reciept.
  X_API void AttachConnectionPrototype( XSDK::XConnBase* proto );

  /// This method causes this XServer to begin listening for connection
  /// attempts on the passed port.
  X_API void Start();

  /// This method causes this XServer to stop listening for connection
  /// attempts on the passed port.
  X_API void Stop();

  /// This method returns whether or not we are running.
  X_API bool Running() const;

private:

  // no default implementations of these please.
  XServer( const XServer& obj );
  XServer& operator = ( const XServer& obj );

  /// Create a socket, and bind it to the HALServer port.
  void _Prepare( void );

  /// Removes any connection objects that are no longer servicing their client.
  void _PruneOldConnections();

  /// Used to launch client service threads.
  static void* _ConnectionLauncher( void* arg );

  XString _ifAddr;  ///< The interface family to listen on
  XRef<XSDK::XSocket> _serverSok;
  XSDK::XConnBase* _prototype;
  int _port;
  XSDK::XInternalList<XSDK::XConnBase*> _connections;
  bool _running;
};

}

#endif
