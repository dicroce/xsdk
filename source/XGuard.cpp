
//=============================================================================
//Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XGuard.cpp $
// $Revision:$
// $Date:$
// $Author: Tony Di Croce $
//=============================================================================


#include "XSDK/XGuard.h"

using namespace XSDK;

XGuard::XGuard( XMutex& lok ) :
    _lok( lok ),
    _lokCount( 0 )
{
    _lok.Acquire();
    _lokCount++;
}

XGuard::~XGuard() throw()
{
    if( _lokCount )
    {
        _lokCount--;
        _lok.Release();
    }
}

void XGuard::Acquire()
{
    if( !_lokCount )
    {
        _lok.Acquire();
        _lokCount++;
    }
}

void XGuard::Release()
{
    if( _lokCount )
    {
        _lokCount--;
        _lok.Release();
    }
}








