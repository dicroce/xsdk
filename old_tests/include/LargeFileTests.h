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
#ifndef _LARGE_FILE_TEST_H_
#define _LARGE_FILE_TEST_H_

#include "framework.h"
#include "XSDK/OS.h"

class LargeFileTests: public test_fixture
{
public:
    //========================================================================
    // Test declarations
    //========================================================================
    TEST_SUITE(LargeFileTests);
        TEST(LargeFileTests::TestAsciiFile);
        TEST(LargeFileTests::TestUnicodeFile);
        TEST(LargeFileTests::TestSeekTell);
        TEST(LargeFileTests::TestPreAllocatedFile);
    TEST_SUITE_END();

    void setup();
    void teardown();

protected:
    void TestAsciiFile();
    void TestUnicodeFile();
    void TestSeekTell();
    void TestPreAllocatedFile();
};


#endif//_LARGE_FILE_TEST_H_
