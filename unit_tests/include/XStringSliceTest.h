//=============================================================================
//Copyright (c) 2013 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XStringSliceTest.h $
// $Revision:$
// $Date:$
// $Author: Jonathan M Davis $
//=============================================================================

#ifndef _XStringSliceTest_h_
#define _XStringSliceTest_h_

#include "framework.h"

class XStringSliceTest : public test_fixture
{
public:
    TEST_SUITE(XStringSliceTest);
        TEST(XStringSliceTest::TestConstructor);
        TEST(XStringSliceTest::TestConstructorUnicode);
        TEST(XStringSliceTest::TestCopyConstructor);
        TEST(XStringSliceTest::TestCopyConstructorUnicode);
        TEST(XStringSliceTest::TestAssignmentOperator);
        TEST(XStringSliceTest::TestAssignmentOperatorUnicode);
        TEST(XStringSliceTest::TestEqualityOperator);
        TEST(XStringSliceTest::TestInequalityOperator);
        TEST(XStringSliceTest::TestPopX);
        TEST(XStringSliceTest::TestIterators);
        TEST(XStringSliceTest::TestStartsWith);
        TEST(XStringSliceTest::TestStartsWithUnicodeVersion);
        TEST(XStringSliceTest::TestEndsWith);
        TEST(XStringSliceTest::TestEndsWithUnicodeVersion);
        TEST(XStringSliceTest::TestToString);
        TEST(XStringSliceTest::TestSlice);
        TEST(XStringSliceTest::TestFind);
        TEST(XStringSliceTest::TestFindUntil);
    TEST_SUITE_END();

    virtual ~XStringSliceTest() throw()
    {}

    void setup() {}
    void teardown() {}

protected:
    void TestConstructor();
    void TestConstructorUnicode();
    void TestCopyConstructor();
    void TestCopyConstructorUnicode();
    void TestAssignmentOperator();
    void TestAssignmentOperatorUnicode();
    void TestEqualityOperator();
    void TestInequalityOperator();
    void TestPopX();
    void TestIterators();
    void TestStartsWith();
    void TestStartsWithUnicodeVersion();
    void TestEndsWith();
    void TestEndsWithUnicodeVersion();
    void TestToString();
    void TestSlice();
    void TestFind();
    void TestFindUntil();
};

#endif

