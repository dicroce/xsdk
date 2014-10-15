//=============================================================================
//Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: OS.cpp $
// $Revision:$
// $Date:$
// $Author: Tom Distler $
//=============================================================================

#include "XSDK/OS.h"
#include <errno.h>

namespace XSDK
{

    XString GetLastErrorMsg()
    {
#ifdef IS_WINDOWS
        return GetErrorMsg(GetLastError());
#else
        return GetErrorMsg(errno);
#endif
    }


    XString GetErrorMsg(int error)
    {
#ifdef IS_WINDOWS
        LPVOID pStr = 0;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY,
                      NULL,
                      error,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      (LPTSTR)&pStr,
                      0,
                      0);
        XString str((LPTSTR)pStr);
        LocalFree(pStr);
        return str;
#else
        return XString(strerror(error));
#endif
    }

}
