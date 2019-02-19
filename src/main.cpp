#include <iostream>

#include "tests.h"

void main()
{
    const bool result = expression_fetcher::tests();

    std::cout << "Tests result: " << result;

    return;
}