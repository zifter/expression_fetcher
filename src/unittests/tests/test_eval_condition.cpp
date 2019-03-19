#include "unittests.h"

#include "expression_fetcher.h"

using expression_fetcher::tests::Check;

FETCHER_TEST(test_eval_int)
{
    int a = 30;
    Check(EVAL_CONDITION(a), "[30] is true");
}

FETCHER_TEST(test_eval_or)
{
    bool valA = true;
    bool valB = false;
    Check(EVAL_CONDITION(valA || valB), "[true || false] is true");
}

FETCHER_TEST(test_eval_and)
{
    bool valA = true;
    bool valB = false;
    Check(EVAL_CONDITION(valA && valB), "[true && false] is false");
}

FETCHER_TEST(test_eval_more)
{
    int valA = 10;
    int valB = 20;
    Check(EVAL_CONDITION(valA > valB), "[10 > 20] is false");
}

FETCHER_TEST(test_eval_less)
{
    int valA = 10;
    int valB = 20;
    Check(EVAL_CONDITION(valA < valB), "[10 > 20] is true");
}

FETCHER_TEST(test_condition_evaluated_only_once)
{
    int valA = 0;
    int valB = 1;
    Check(EVAL_CONDITION(valA++ < valB), "[0 < 1] is true");
}
