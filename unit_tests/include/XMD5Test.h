
#ifndef _XSDK_XMD5Test_h_
#define _XSDK_XMD5Test_h_

#include "framework.h"
#include "XSDK/XTaskBase.h"

class XMD5Test : public test_fixture
{
public:
    TEST_SUITE(XMD5Test);
        TEST(XMD5Test::TestConstructor);
    TEST_SUITE_END();

    ~XMD5Test() throw() {}
    void setup();
    void teardown();

protected:
    void TestConstructor();
};

#endif
