
//=============================================================================
//Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XStackStopWatch.h $
// $Revision:$
// $Date:$
// $Author: Tom Distler $
//=============================================================================

#ifndef XSDK_XStackStopWatch_h
#define XSDK_XStackStopWatch_h

#include "XSDK/XString.h"
#include "XSDK/XStopWatch.h"
#include "XSDK/XLogger.h"

namespace XSDK
{

    class XStackStopWatch
    {
    public:
        explicit X_API XStackStopWatch(XString message,
                                       //The stopwatch will only print out if at least
                                       //this amount of time has elapsed when it's destroyed.
                                       XDuration minTimeElapsed = XDuration(SECONDS,0),
                                       LOG_LEVEL level = LOGLEVEL_DEBUG);

        X_API virtual ~XStackStopWatch() throw();

    private:
        X_API XStackStopWatch(const XStackStopWatch& obj);
        X_API XStackStopWatch& operator = (const XStackStopWatch& obj);
        XStopWatch _stopWatch;
        XString _message;
        XDuration _minTimeElapsed;
        LOG_LEVEL _level;
    };

};

#endif
