//=============================================================================
//Copyright (c) 2012 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XObjCountTest.h $
// $Revision:$
// $Date:$
// $Author: Jonathan M Davis $
//=============================================================================

#ifndef _XObjCountTest_h_
#define _XObjCountTest_h_

#include "framework.h"

class XObjCountTest : public test_fixture
{
public:
    TEST_SUITE(XObjCountTest);
        TEST(XObjCountTest::TestCountRefs_AddAndRemove);
        TEST(XObjCountTest::TestCountRefs_XRef);
        TEST(XObjCountTest::TestCountRefs_XBaseObject);
        TEST(XObjCountTest::TestTrackStack_AddAndRemove);
        TEST(XObjCountTest::TestTrackStack_XRef);
        TEST(XObjCountTest::TestTrackStack_XBaseObject);
    TEST_SUITE_END();

    virtual ~XObjCountTest() throw()
    {}

    void setup() {}
    void teardown() {}

protected:

    void TestCountRefs_AddAndRemove();
    void TestCountRefs_XRef();
    void TestCountRefs_XBaseObject();
    void TestTrackStack_AddAndRemove();
    void TestTrackStack_XRef();
    void TestTrackStack_XBaseObject();
};

#endif

