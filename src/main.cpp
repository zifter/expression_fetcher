#include <iostream>
#include <assert.h>

#include "tests.h"

#include "expression_fetcher.h"

using expression_fetcher::tests::Check;

TEST(test_30)
{
    int a = 30;
    return Check(FETCH(a), "30");
}

TEST(test_less)
{
    int valA = 42;
    float valB = 30.4f;
    return Check(FETCH(valA < valB), "42 < 30.4");
}

TEST(test_more)
{
    int valA = 55;
    float valB = 35.4f;
    return Check(FETCH(valA > valB), "55 > 35.4");
}

TEST(test_more_and_less)
{
    int valA = 50;
    float valB = 33.3f;
    float valC = 22.22f;
    float valD = 11.11f;
    return Check(FETCH(valA > valB && valC < valD), "50 > 33.3 && 22.22 < 11.11");
}

TEST(test_or)
{
    bool valA = true;
    bool valB = false;
    return Check(FETCH(valA || valB), "true || false");
}

void main()
{
    const bool result = expression_fetcher::tests::Run();

    assert(result == true);

    return;
}