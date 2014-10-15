//=============================================================================
//Copyright (c) 2011 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XUuidTest.h $
// $Revision:$
// $Date:$
// $Author:$
//=============================================================================

#ifndef _XSDK_X_UUIDTest_H_
#define _XSDK_X_UUIDTest_H_

#include "framework.h"

class XUuidTest : public test_fixture
{
public:
        TEST_SUITE(XUuidTest);
            TEST(XUuidTest::TestConstructor);
            TEST(XUuidTest::TestGetUuid);
            TEST(XUuidTest::TestIsEmpty);
            TEST(XUuidTest::TestToString);
            TEST(XUuidTest::TestIsStringValid);
            TEST(XUuidTest::TestFromString);
            TEST(XUuidTest::TestGenerate);
            TEST(XUuidTest::TestClear);
        TEST_SUITE_END();

        virtual ~XUuidTest() throw()
        {}

        void setup() {}
        void teardown() {}

protected:
        void TestConstructor();
        void TestGetUuid();
        void TestIsEmpty();
        void TestToString();
        void TestIsStringValid();
        void TestFromString();
        void TestGenerate();
        void TestClear();
};

#endif
