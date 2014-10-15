
//=============================================================================
//Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XDynamicLibrary.h $
// $Revision:$
// $Date:$
// $Author: Tony Di Croce $
//=============================================================================


#ifndef _XSDK_Dll_H_
#define _XSDK_Dll_H_

#include "XSDK/Platform.h"
#include "XSDK/XException.h"
#include "XSDK/Errors.h"
#include "XSDK/XString.h"

#ifdef IS_LINUX
#include <dlfcn.h>
#endif

namespace XSDK
{
    #ifdef IS_LINUX
        typedef void*       FARPROC;
        typedef void*       HINSTANCE;
    #endif

    //=====================================================================
    //XDynamicLibrary class - library helper for loading
    //                          .DLL's and .so's
    //=====================================================================
    class XDynamicLibrary
    {
    public:
        X_API XDynamicLibrary();
        X_API XDynamicLibrary(const XSDK::XString& libraryName);
        X_API virtual ~XDynamicLibrary() throw();

        X_API void Load(void);
        X_API void Load(const XSDK::XString& libraryName);

        X_API FARPROC ResolveSymbol(const XSDK::XString& symbolName);
        X_API void Unload(void);

    private:
        // explicitly made private to forbid the user from holding multiple copies
        XDynamicLibrary(const XDynamicLibrary&);
        XDynamicLibrary& operator =(const XDynamicLibrary&);

        HINSTANCE   _libraryInstance;
        XSDK::XString _libraryName;
    }; // XDynamicLibrary

}; // namespace XSDK

#endif // _XSDK_Dll_H_

