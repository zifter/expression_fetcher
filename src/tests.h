#pragma once

#include <string>
#include <functional>

namespace expression_fetcher
{
    namespace tests
    {
        struct Descr
        {
            std::string name;
            std::function<bool()> fn;
        };

        void Register(Descr &&descr);

        bool Run();

        bool Check(const std::string &v1, const std::string &v2);
    }
}


#define FETCHER_TEST(FN_NAME) \
    bool FN_NAME(); \
    struct FN_NAME##_register \
    { \
        FN_NAME##_register() \
        { \
            std::function<bool()> fn = FN_NAME; \
            expression_fetcher::tests::Register({#FN_NAME, fn}); \
        } \
    }; \
    static FN_NAME##_register FN_NAME##_value; \
    bool FN_NAME()
