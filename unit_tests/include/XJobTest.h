#ifndef XJobTest_h
#define XJobTest_h

#include "framework.h"

class XJobTest : public test_fixture
{
public:
    TEST_SUITE(XJobTest);
        TEST(XJobTest::TestGetProgress);
        TEST(XJobTest::TestHasGetStatus);
        TEST(XJobTest::TestCompletion);
        TEST(XJobTest::TestIsUnresponsive);
        TEST(XJobTest::TestIsExpired);
        TEST(XJobTest::TestJobReuse);
    TEST_SUITE_END();

    void setup();
    void teardown();

protected:
    void TestGetProgress();
    void TestHasGetStatus();
    void TestCompletion();
    void TestIsUnresponsive();
    void TestIsExpired();
    void TestJobReuse();
};

#endif
