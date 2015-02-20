
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XConnBase.h"

using namespace XSDK;

XConnBase::XConnBase() :
    XTaskBase(),
    _clientConn(new XSocket),
    _exited( false )
{
}

XConnBase::~XConnBase() throw()
{
}

void* XConnBase::EntryPoint()
{
    try
    {
        ServiceClient();
    }

    catch( ... )
    {
        // XXX - What do we do at the global level with an exception? Something cool I hope!
    }

    _exited = true;

    return &_exited;
}
