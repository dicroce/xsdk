
//=============================================================================
//Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: StackTrace.h $
// $Revision:$
// $Date:$
// $Author: Tony Di Croce $
//=============================================================================

#ifndef XSDK_OS_StackTrace_h
#define XSDK_OS_StackTrace_h

#include <cstdio>

#ifndef WIN32

  #include <signal.h>
  #include <execinfo.h>
  #include <cxxabi.h>
  #include <cstdlib>

#endif

namespace XSDK
{

#ifndef WIN32

void x_sigsegv_handler( int sigNum );

void x_install_segv_handler();

#endif

}

#endif

