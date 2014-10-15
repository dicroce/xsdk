//=============================================================================
// Copyright (c) 2013 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File:$ Rand.h
// $Revision:$
// $Date:$
// $Author:$ Jonathan M Davis
//=============================================================================

#include "XSDK/XSharedLib.h"

namespace XSDK
{

/// Wrapper around rand which makes it thread-safe.
X_API int x_rand();

/// Wrapper around srand which makes it thread-safe.
X_API void x_srand(unsigned int seed);

/// Calls x_srand with the seed but only if x_srand has not previously been called.
X_API void x_srand_if(unsigned int seed);

};
