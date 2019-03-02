#include "tests.h"

#include <string>
#include <vector>
#include <functional>
#include <iostream>

std::string quote(const std::string &val)
{
    return "'" + val + "'";
}

namespace expression_fetcher
{
    namespace tests
    {
        std::vector<Descr> tests_fn;

        void Register(Descr &&descr)
        {
            tests_fn.emplace_back(descr);
        }

        bool Run()
        {
            bool result = true;
            for(const auto &descr : tests_fn)
            {
                std::cout << "- " << descr.name << "..." << std::endl;
                const bool test_result = descr.fn();
                std::cout << (test_result ? "Ok" : "error") << std::endl;

                result = test_result && result;
            }
            std::cout << "Tests processed: " << tests_fn.size() 
                << " with result " << result << std::endl;

            return result;
        }

        bool Check(const std::string &v1, const std::string &v2)
        {
            if(v1 == v2)
            {
                std::cout << quote(v1) << " == " << quote(v2) << std::endl;
                return true;
            }
            else
            {
                std::cout << quote(v1) << " != " << quote(v2) << std::endl;
                return false;
            }
        }
    }
}
