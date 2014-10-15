//=============================================================================
// Copyright (c) 2013 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File:$ Rand.cpp
// $Revision:$
// $Date:$
// $Author:$ Jonathan M Davis
//=============================================================================

#include "XSDK/OS/Rand.h"
#include "XSDK/XGuard.h"
#include <cstdlib>

using namespace std;
using namespace XSDK;

static XMutex& GetMutex()
{
    static XMutex mutex;
    return mutex;
}

static bool& GetRandSeeded()
{
    static bool seeded;
    return seeded;
}

int XSDK::x_rand()
{
    XGuard lock(GetMutex());
    return rand();
}

void XSDK::x_srand(unsigned int seed)
{
    XGuard lock(GetMutex());
    srand(seed);
    GetRandSeeded() = true;
}

void XSDK::x_srand_if(unsigned int seed)
{
    XGuard lock(GetMutex());
    if(!GetRandSeeded())
        x_srand(seed);
}
