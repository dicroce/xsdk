//=============================================================================
//Copyright (c) 2011 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XBitsyTest.h $
// $Revision:$
// $Date:$
// $Author: Jonathan M Davis $
//=============================================================================

#ifndef _XBitsyTest_H_
#define _XBitsyTest_H_

#include "framework.h"

class XBitsyTest : public test_fixture
{
public:
    TEST_SUITE(XBitsyTest);
        TEST(XBitsyTest::TestConstructor);
        TEST(XBitsyTest::TestSetBuffer);
        TEST(XBitsyTest::TestReset);
        TEST(XBitsyTest::TestGetBits);
        TEST(XBitsyTest::TestGetExactBits);
    TEST_SUITE_END();

    virtual ~XBitsyTest() throw()
    {}

    void setup() {}
    void teardown() {}

protected:

    void TestConstructor();
    void TestSetBuffer();
    void TestReset();
    void TestGetBits();
    void TestGetExactBits();
};

#endif

