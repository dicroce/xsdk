
//=============================================================================
//Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XConnBase.cpp $
// $Revision:$
// $Date:$
// $Author: Tony Di Croce $
//=============================================================================


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
