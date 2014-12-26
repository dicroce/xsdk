//=============================================================================
// Copyright (c) 2011 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: NewDeleteTest.cpp$
// $Revision:$
// $Date:$
// $Author: tdistler$
//=============================================================================
#include "NewDeleteTest.h"
#include "XSDK/XBaseObject.h"
#include "XSDK/XIRef.h"

using namespace XSDK;

REGISTER_TEST_FIXTURE(NewDeleteTest);

void NewDeleteTest::TestNewDeleteAligned()
{
    size_t size = 1234;
    size_t alignment = 0x10;
    unsigned int mask = 0xF;
    uint8_t* p = 0;
    UT_ASSERT_NO_THROW( X_NEW_ALIGNED_BUFFER( p, size, alignment ) );
    UT_ASSERT( p != 0 );
    UT_ASSERT( ((size_t)p & mask) == 0 ); // Verify alignment
    UT_ASSERT_NO_THROW( X_DELETE_ALIGNED_BUFFER( p ) );
}
