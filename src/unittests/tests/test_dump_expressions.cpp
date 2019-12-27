#include "unittests.h"

#include "expression_fetcher.h"

using expression_fetcher::tests::Check;

FETCHER_TEST(test_const_char_equal_nullptr)
{
    Check(EVAL_CONDITION("test" == nullptr), "\"test\" == nullptr");
}

FETCHER_TEST(test_string_equal_nullptr)
{
    Check(EVAL_CONDITION(std::string("test") == nullptr), "\"test\" == nullptr");
}

struct TestStreamStruct2
{};

FETCHER_TEST(test_nullptr_equal_struct_without_stream_operator)
{
    TestStreamStruct2 testObj;
    Check(EVAL_CONDITION(nullptr == testObj), "nullptr == #val");
}

FETCHER_TEST(test_nullptr_equal_pointer)
{
    TestStreamStruct2 *testObj = reinterpret_cast<TestStreamStruct2 *>(uintptr_t(0xdeadbeaf));
    Check(EVAL_CONDITION(nullptr == testObj), "nullptr == 0xdeadbeaf");
}

FETCHER_TEST(test_less)
{
    int valA = 42;
    float valB = 30.4f;
    Check(EVAL_CONDITION(valA < valB), "42 < 30.4");
}

FETCHER_TEST(test_less_and_equal)
{
    int valA = 42;
    float valB = 30.4f;
    Check(EVAL_CONDITION(valA <= valB), "42 <= 30.4");
}

FETCHER_TEST(test_more)
{
    int valA = 55;
    float valB = 35.4f;
    Check(EVAL_CONDITION(valA > valB), "55 > 35.4");
}

FETCHER_TEST(test_more_and_equal)
{
    int valA = 42;
    float valB = 30.4f;
    Check(EVAL_CONDITION(valA >= valB), "42 >= 30.4");
}

FETCHER_TEST(test_and)
{
    bool valA = true;
    bool valB = false;
    Check(EVAL_CONDITION(valA && valB), "true && false");
}

FETCHER_TEST(test_or)
{
    bool valA = true;
    bool valB = false;
    Check(EVAL_CONDITION(valA || valB), "true || false");
}

FETCHER_TEST(test_equal)
{
    bool valA = true;
    bool valB = false;
    Check(EVAL_CONDITION(valA == valB), "true == false");
}

FETCHER_TEST(test_not_equal)
{
    int valA = 12;
    int valB = 16;
    Check(EVAL_CONDITION(valA != valB), "12 != 16");
}

FETCHER_TEST(test_plus)
{
    int valA = 12;
    int valB = 16;
    Check(EVAL_CONDITION(valA + valB), "12 + 16");
}

FETCHER_TEST(test_minus)
{
    int valA = 12;
    int valB = 16;
    Check(EVAL_CONDITION(valA - valB), "12 - 16");
}

FETCHER_TEST(test_multiply)
{
    int valA = 12;
    int valB = 16;
    Check(EVAL_CONDITION(valA * valB), "12 * 16");
}

FETCHER_TEST(test_divide)
{
    int valA = 12;
    int valB = 16;
    Check(EVAL_CONDITION(valA / valB), "12 / 16");
}

FETCHER_TEST(test_more_and_less)
{
    int valA = 50;
    float valB = 33.3f;
    float valC = 22.22f;
    float valD = 11.11f;
    Check(EVAL_CONDITION(valA > valB && valC < valD), "50 > 33.3 && 22.22 < 11.11");
}

FETCHER_TEST(test_equal_and_equal)
{
    int valA = 50;
    int valB = 33;
    int valC = 22;
    int valD = 11;
    Check(EVAL_CONDITION(valA == valB && valC == valD), "50 == 33 && 22 == 11");
}

FETCHER_TEST(test_not_equal_or_not_equal)
{
    int valA = 50;
    int valB = 33;
    int valC = 22;
    int valD = 11;
    Check(EVAL_CONDITION(valA != valB || valC != valD), "50 != 33 || 22 != 11");
}

FETCHER_TEST(test_more_equal_or_more_equal)
{
    int valA = 50;
    int valB = 33;
    int valC = 22;
    int valD = 11;
    Check(EVAL_CONDITION(valA >= valB || valC >= valD), "50 >= 33 || 22 >= 11");
}

FETCHER_TEST(test_less_equal_and_less_equal)
{
    int valA = 50;
    int valB = 33;
    int valC = 22;
    int valD = 11;
    Check(EVAL_CONDITION(valA <= valB && valC <= valD), "50 <= 33 && 22 <= 11");
}

FETCHER_TEST(test_plus_or_plus)
{
    int valA = 50;
    int valB = 33;
    int valC = 22;
    int valD = 11;
    Check(EVAL_CONDITION(valA + valB && valC + valD), "50 + 33 && 22 + 11");
}

FETCHER_TEST(test_minus_or_minus)
{
    int valA = 50;
    int valB = 33;
    int valC = 22;
    int valD = 11;
    Check(EVAL_CONDITION(valA - valB && valC - valD), "50 - 33 && 22 - 11");
}

FETCHER_TEST(test_divide_or_divide)
{
    int valA = 50;
    int valB = 33;
    int valC = 22;
    int valD = 11;
    // there is no chance to catch right part
    Check(EVAL_CONDITION(valA / valB && valC / valD), "50 / 33 && 2");
}

FETCHER_TEST(test_mult_or_mult)
{
    int valA = 50;
    int valB = 33;
    int valC = 22;
    int valD = 0;
    // there is no chance to catch right part
    Check(EVAL_CONDITION(valA * valB && valC * valD), "50 * 33 && 0");
}

FETCHER_TEST(test_expression_with_2_stop_and)
{
    int valA = 50;
    int valB = 33;
    int valC = 22;
    int valD = 0;
    int valE = 23;
    int valF = 45;

    Check(EVAL_CONDITION(valA - valB && valC < valD && valE > valF), 
        "50 - 33 && false && 23 > 45");
}

FETCHER_TEST(test_expression_with_2_stop_or)
{
    int valA = 50;
    int valB = 33;
    int valC = 22;
    int valD = 0;
    int valE = 23;
    int valF = 45;

    Check(EVAL_CONDITION(valA - valB || valC < valD || valE > valF), 
        "50 - 33 || false || 23 > 45");
}
