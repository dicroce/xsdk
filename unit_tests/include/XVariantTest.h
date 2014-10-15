//============================================================================
// Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File:$
// $Revision:$
// $Date:$
// $Author:$
//============================================================================
#ifndef _X_VARIANT_TEST_H_
#define _X_VARIANT_TEST_H_
#include "framework.h"
#include "XSDK/XSharedLibImp.h"
#include "XSDK/Types.h"
#include "XSDK/XSharedLibDef.h"

class XVariantTest :
    public test_fixture
{
public:
    TEST_SUITE(XVariantTest);
        TEST(XVariantTest::TestDefaultAndCopyConstructor);
        TEST(XVariantTest::TestConversions);
        TEST(XVariantTest::TestAssignmentOperators);
        TEST(XVariantTest::TestCastOperators);
        TEST(XVariantTest::TestClear);
    TEST_SUITE_END();

    void setup();
    void teardown();

protected:
    void TestDefaultAndCopyConstructor();
    void TestConversions();
    void TestAssignmentOperators();
    void TestCastOperators();
    void TestClear();
};


#endif
