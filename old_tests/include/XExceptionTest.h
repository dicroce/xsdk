//============================================================================
// Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File:$ XExceptionTest.cpp
// $Revision:$
// $Date:$
// $Author:$ Tony Di Croce
//============================================================================
#ifndef _X_EXCEPTION_TEST_H_
#define _X_EXCEPTOIN_TEST_H_

#include "framework.h"
#include "XSDK/XException.h"

class XExceptionTest :
    public test_fixture
{
public:
    TEST_SUITE(XExceptionTest);
        TEST(XExceptionTest::testConstructors);
        TEST(XExceptionTest::testSetThrowPoint);
        TEST(XExceptionTest::testWhat);
        TEST(XExceptionTest::testGetters);
        TEST(XExceptionTest::testAssignment);
    TEST_SUITE_END();

    void setup();
    void teardown();

protected:
    void testConstructors();
    void testSetThrowPoint();
    void testWhat();
    void testGetters();
    void testAssignment();
};

#endif

