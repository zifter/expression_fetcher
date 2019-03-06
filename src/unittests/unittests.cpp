#include "unittests.h"

#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <sstream>

std::string quote(const std::string &val)
{
    return "'" + val + "'";
}

namespace expression_fetcher
{
    namespace tests
    {
        class TestFailedError
            : public std::runtime_error
        {
        public:
            TestFailedError(const std::string &str)
                : std::runtime_error(str)
            {}
        };

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
                std::cout << "- " << descr.name << "...";
                try
                {
                    descr.fn();
                    std::cout << "ok";
                }
                catch(const TestFailedError& e)
                {
                    result = false;
                    std::cout << "failed" << std::endl << e.what();
                }
                catch(...)
                {
                    result = false;
                    std::cout << "Uncought error";
                }
                std::cout << std::endl;
            }
            std::cout << "Tests processed: " << tests_fn.size() 
                << " with result " << result << std::endl;

            return result;
        }

        void Check(const std::string &v1, const std::string &v2)
        {
            if(v1 != v2)
            {
                std::stringstream sstr;
                sstr << quote(v1) << " == " << quote(v2) << std::endl;
                throw TestFailedError(sstr.str());
            }
        }
    }
}
