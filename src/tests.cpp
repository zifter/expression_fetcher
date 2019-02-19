#include "tests.h"

#include <assert.h>
#include <string>
#include <vector>
#include <functional>
#include <iostream>

#include "expression_fetcher.h"

std::string quote(const std::string &val)
{
    return "'" + val + "'";
}

bool _check(const std::string &v1, const std::string &v2)
{
    if(v1 == v2)
    {
        std::cout << quote(v1) << " == " << quote(v2) << std::endl;
        return true;
    }
    else
    {
        std::cout << quote(v1) << " != " << quote(v2) << std::endl;
        assert(v1 == v2);
        return false;
    }
}

bool test_1()
{
    int a = 30;
    return _check(FETCH(a), "30");
}

namespace expression_fetcher
{

bool tests()
{
    std::vector<std::function<bool()>> tests_fn = {
            test_1,
    };

    bool result = true;
    for(const auto &fn : tests_fn)
    {
        result = fn() && result;
    }

    return result;
}

}
