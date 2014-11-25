//============================================================================
// Copyright (c) 2011 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: NewDeleteTest.h$
// $Revision:$
// $Date:$
// $Author: tdistler$
//============================================================================
#ifndef _NEW_DELETE_TEST_H_
#define _NEW_DELETE_TEST_H_

#include "framework.h"
#include "XSDK/OS.h"

class NewDeleteTest : public test_fixture
{
public:
    TEST_SUITE(NewDeleteTest);
        TEST(NewDeleteTest::TestNewDeleteAligned);
    TEST_SUITE_END();

    void setup() {}
    void teardown() {}

protected:
    void TestNewDeleteAligned();
};

#endif // _NEW_DELETE_TEST_H_

