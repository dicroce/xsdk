
//=============================================================================
//Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XMonoClock.h $
// $Revision:$
// $Date:$
// $Author: Tom Distler $
//=============================================================================

#ifndef XSDK_XMonoClock_h
#define XSDK_XMonoClock_h

#include "XSDK/XSharedLib.h"

namespace XSDK
{
    // The XMonoClock class provides access to a high-performance, monotonically
    // increasing counter that can be used for interval timing. The counter value
    // returned from GetTime() increments at the rate returned by GetFrequency().
    class XMonoClock
    {
    public:

        // @return The current value of the clock.
        X_API static uint64_t GetTime();

        // @return The number of ticks per second that the clock increments.
        X_API static uint64_t GetFrequency();

        // @param start The clock value at the start of the interval we are measuring.
        // @param end The clock value at the end of the interval we are measuring.
        // @return The elapsed time (in seconds) from 'start' until 'end'.
        X_API static double GetElapsedTime(uint64_t start, uint64_t end);
    };
}

#endif
