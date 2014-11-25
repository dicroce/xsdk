//=============================================================================
//Copyright (c) 2012 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XSocketAddressTest.h $
// $Revision:$
// $Date:$
// $Author: Tom Distler $
//=============================================================================
#ifndef XSocketAddressTest_h
#define XSocketAddressTest_h

#include "framework.h"

class XSocketAddressTest : public test_fixture
{
public:
    TEST_SUITE(XSocketAddressTest);
        TEST(XSocketAddressTest::TestConstructor);
        TEST(XSocketAddressTest::TestGetSetPortNum);
        TEST(XSocketAddressTest::TestGetSetAddress);
        TEST(XSocketAddressTest::TestGetAddressFamily);
        TEST(XSocketAddressTest::TestSockAddrCast);
        TEST(XSocketAddressTest::TestComparisons);
        TEST(XSocketAddressTest::TestStaticGetAddressFamily);
        TEST(XSocketAddressTest::TestStaticGetSockAddrSize);
        TEST(XSocketAddressTest::TestStaticAddressToString);
        TEST(XSocketAddressTest::TestStaticStringToAddress);
        TEST(XSocketAddressTest::TestIsolateAddress);
        TEST(XSocketAddressTest::TestAddressTypeMethods);
        TEST(XSocketAddressTest::TestIsWildcardAddress);
        TEST(XSocketAddressTest::TestIsHostname);
    TEST_SUITE_END();

    virtual ~XSocketAddressTest() throw() {}

    void setup() {}
    void teardown() {}

protected:
    void TestConstructor();
    void TestGetSetPortNum();
    void TestGetSetAddress();
    void TestGetAddressFamily();
    void TestSockAddrCast();
    void TestComparisons();
    void TestStaticGetAddressFamily();
    void TestStaticGetSockAddrSize();
    void TestStaticAddressToString();
    void TestStaticStringToAddress();
    void TestIsolateAddress();
    void TestAddressTypeMethods();
    void TestIsWildcardAddress();
    void TestIsHostname();
};

#endif
