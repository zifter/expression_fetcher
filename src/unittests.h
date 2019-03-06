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
            std::function<void()> fn;
        };

        void Register(Descr &&descr);

        bool Run();

        void Check(const std::string &v1, const std::string &v2);
    }
}


#define FETCHER_TEST(FN_NAME) \
    void FN_NAME(); \
    struct FN_NAME##_register \
    { \
        FN_NAME##_register() \
        { \
            std::function<void()> fn = FN_NAME; \
            expression_fetcher::tests::Register({#FN_NAME, fn}); \
        } \
    }; \
    static FN_NAME##_register FN_NAME##_value; \
    void FN_NAME()
