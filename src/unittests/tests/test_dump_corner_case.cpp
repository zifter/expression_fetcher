#include "unittests.h"

#include "expression_fetcher.h"

using expression_fetcher::tests::Check;

FETCHER_TEST(test_2_or_2)
{
    Check(DUMP_CONDITION(2 || 2), "2 || 2");
}

FETCHER_TEST(test_true_and_false)
{
    Check(DUMP_CONDITION(true && false), "true && false");
}

FETCHER_TEST(test_case1)
{
    Check(DUMP_CONDITION(2 == 2 && "2 doesnt equal 2"), "2 == 2 && \"2 doesnt equal 2\"");
}

FETCHER_TEST(test_case2)
{
    Check(DUMP_CONDITION(2 == 2 && 1 == 1), "2 == 2 && 1 == 1");
}

// FETCHER_TEST(test_case3)
// {
//     Check(DUMP_CONDITION(true ? 1 : 0), "1");
// }

// FETCHER_TEST(test_case4)
// {
//     const char *a = reinterpret_cast<const char *>(1);
//     const char *b = "b";
//     Check(DUMP_CONDITION(a == b), "a == \"b\"");
// }

FETCHER_TEST(test_case5_do_not_generate_warning)
{
    std::vector<int> v = {1, 2};
    Check(DUMP_CONDITION(v.size() > 0), "2 > 0");
}

FETCHER_TEST(test_case6_static_consntant)
{
    static const int a = 10;
    Check(DUMP_CONDITION(a > 20), "10 > 20");
}

