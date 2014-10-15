
//=============================================================================
// Copyright (c) 2013 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XStackTrace.h$
// $Revision:$
// $Date:$
// $Author: Tony Di Croce$
//=============================================================================

#ifndef XSDK_XStackTrace_h
#define XSDK_XStackTrace_h

//Using std::string instead of XSDK::XString because of header circular dependencies.
#include <string>
#include <vector>

namespace XSDK
{

class XStackTrace
{
public:

    X_API static std::string GetStack();
    X_API static void GetStack( std::vector<std::string>& stack );
    X_API static std::string StackToString( const std::vector<std::string>& stack );

private:
    XStackTrace();
    XStackTrace( const XStackTrace& obj );
    XStackTrace& operator = ( const XStackTrace& obj );
};

}

#endif

