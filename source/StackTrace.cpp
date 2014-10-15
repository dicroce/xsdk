
//=============================================================================
//Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: StackTrace.cpp $
// $Revision:$
// $Date:$
// $Author: Tony Di Croce $
//=============================================================================

#include "XSDK/OS/StackTrace.h"
#include "XSDK/XLogger.h"

namespace XSDK
{

#ifndef WIN32

void x_sigsegv_handler( int sigNum )
{
    XLog::LogBacktrace();
    abort();
}

void x_install_segv_handler()
{
    signal( SIGSEGV, x_sigsegv_handler );
}

#endif

}
