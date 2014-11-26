//=============================================================================
//Copyright (c) 2011 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XStatisticsTest.h $
// $Revision:$
// $Date:$
// $Author: tdistler $
//=============================================================================

#ifndef _XStatisticsTest_H_
#define _XStatisticsTest_H_

#include "framework.h"

class XStatisticsTest : public test_fixture
{
public:
    TEST_SUITE(XStatisticsTest);
        TEST(XStatisticsTest::TestAverage);
        TEST(XStatisticsTest::TestMedian);
        TEST(XStatisticsTest::TestMaxSamples);
        TEST(XStatisticsTest::TestSampleTimeout);
    TEST_SUITE_END();

    virtual ~XStatisticsTest() throw()
    {}

    void setup() {}
    void teardown() {}

protected:

    void TestAverage();
    void TestMedian();
    void TestMaxSamples();
    void TestSampleTimeout();
};

#endif

