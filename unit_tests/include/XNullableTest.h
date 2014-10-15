//=============================================================================
//Copyright (c) 2013 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XNullableTest.h $
// $Revision:$
// $Date:$
// $Author: Jonathan M Davis $
//=============================================================================

#ifndef _XNullableTest_H_
#define _XNullableTest_H_

#include "framework.h"

class XNullableTest : public test_fixture
{
public:
    TEST_SUITE(XNullableTest);
        TEST(XNullableTest::TestBool);
        TEST(XNullableTest::TestInt);
        TEST(XNullableTest::TestXString);
        TEST(XNullableTest::TestXRef);
    TEST_SUITE_END();

    virtual ~XNullableTest() throw()
    {}

    void setup() {}
    void teardown() {}

protected:

    void TestBool();
    void TestInt();
    void TestXString();
    void TestXRef();
};

#endif

