#include <iostream>
#include <assert.h>

#include "unittests.h"

int main()
{
    const bool result = expression_fetcher::tests::Run();

    assert(result == true);

    return !result;
}