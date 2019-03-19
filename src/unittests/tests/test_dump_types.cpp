#include "unittests.h"

#include "expression_fetcher.h"

using expression_fetcher::tests::Check;

FETCHER_TEST(test_int)
{
    int a = 30;
    Check(DUMP_CONDITION(a), "30");
}

FETCHER_TEST(test_bool_false)
{
    bool a = false;
    Check(DUMP_CONDITION(a), "false");
}

FETCHER_TEST(test_bool_true)
{
    bool a = true;
    Check(DUMP_CONDITION(a), "true");
}

FETCHER_TEST(test_nullptr)
{
    Check(DUMP_CONDITION((std::nullptr_t)nullptr), "nullptr");
}

FETCHER_TEST(test_nullptr2)
{
    int *p = nullptr;
    Check(DUMP_CONDITION(p), "nullptr");
}

FETCHER_TEST(test_const_char)
{
    Check(DUMP_CONDITION("test"), "\"test\"");
}

FETCHER_TEST(test_string)
{
    std::string p("test");
    Check(DUMP_CONDITION(p), "\"test\"");
}

struct TestStreamStruct
{};

FETCHER_TEST(test_struct_without_stream_operator)
{
    TestStreamStruct testObj;
    Check(DUMP_CONDITION(testObj), "#val");
}

FETCHER_TEST(test_pointer)
{
    TestStreamStruct *testObj = reinterpret_cast<TestStreamStruct *>(0xdeadbeaf);
    Check(DUMP_CONDITION(testObj), "0xdeadbeaf");
}
