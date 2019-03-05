#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <type_traits>

namespace expression_fetcher
{
    namespace
    {
        template<typename T>
        class IsStreamAvailable
        {
        private:
            template<typename SS, typename TT>
            static auto test(int) -> decltype(std::declval<SS &>() << std::declval<TT>(), std::true_type());
        
            template<typename, typename>
            static auto test(...)->std::false_type;

        public:
            static const bool value = decltype(test<std::stringstream, const T &>(0))::value;
        };

        template<typename T>
        typename std::enable_if<
            IsStreamAvailable<typename std::decay<T>::type>::value && 
            !std::is_pointer<T>::value, std::string>::type 
            to_string(const T &value)
        {
            std::stringstream sstr;
            sstr << value;
            return sstr.str();
        }
        
        template<typename... T>
        std::string to_string(const T &...)
        {
            return "#val";
        }

        template<typename T>
        std::string to_string(T *val)
        {
            std::stringstream sstr;
            sstr << "0x" << std::hex << reinterpret_cast<uintptr_t>(val);
            return sstr.str();
        }

        std::string to_string(bool value)
        {
            if(value)
            {
                return "true";
            }
            else
            {
                return "false";
            }
        }

        std::string to_string(std::nullptr_t)
        {
            return "nullptr";
        }

        std::string to_string(const std::string &val)
        {
            std::stringstream sstr;
            sstr << '"' << val << '"';
            return sstr.str();
        }

        std::string to_string(const char *val)
        {
            std::stringstream sstr;
            sstr << '"' << val << '"';
            return sstr.str();
        }
    }

    class Fetcher
    {
    public:
        Fetcher()
        {}
    
        void dump_right(const char *operatorString, std::string rvalue)
        {
            if(!values.empty()) // skip first %
            {
                values.push_back(operatorString);
            }
            values.push_back(std::move(rvalue));
        }

        void dump_left(std::string lvalue, const char *operatorString)
        {
            if(is_stopped())
            {
                return;
            }

            const bool needAppendOperator = !values.empty();
            if(needAppendOperator)
            {
                values.push_back(operatorString);
            }

            values.push_back(std::move(lvalue));
        }

        bool is_stopped() const
        {
            return stopped;
        }

        bool is_cut() const
        {
            return rightIndex != 0;
        }

        std::string get_left_string() const
        {
            std::string str;
            size_t count = values.size();
            if(is_cut())
            {
                count = rightIndex - 1;
            }
            
            for(size_t i = 0; i < count; ++i)
            {
                str += values[i];
            }
           
            return str;
        }

        std::string get_right_string() const
        {
            std::string str;
            for(auto it = values.rbegin(); it != values.rend(); ++it)
            {
                str += *it;
            }
            return str;
        }

        void stop(const char *stop)
        {
            if(stop)
            {
                values.push_back(stop);
            }
            stopped = true;
        }

        void keep_right_position()
        {
            rightIndex = values.size();
        }

        operator bool() const 
        { 
            // TODO Need to fix ternary operator
            return true; 
        }

    private:
        std::vector<std::string> values;
        size_t rightIndex = 0;
        bool stopped = false;
    };

    #define CREATE_LEFT_OPERATOR_DUMPER(OPERATOR) \
        template <typename T> auto & operator OPERATOR(Fetcher& ifetcher, const T& lvalue) \
        {  \
            ifetcher.dump_right(" "#OPERATOR" ", to_string(lvalue));  \
            return ifetcher;  \
        }

    #define CREATE_RIGHT_OPERATOR_DUMPER(OPERATOR) \
        template <typename T> auto & operator OPERATOR(const T& rvalue, Fetcher& ifetcher) \
        { \
            ifetcher.dump_left(to_string(rvalue), " "#OPERATOR" "); \
            return ifetcher; \
        }

    #define CREATE_LEFT_STOP_OPERATOR_DUMPER(OPERATOR) \
        template <typename T> auto & operator OPERATOR(Fetcher& ifetcher, const T& lvalue) \
        {  \
            ifetcher.dump_right(" "#OPERATOR" ", to_string(lvalue)); \
            ifetcher.keep_right_position(); \
            return ifetcher; \
        }

    #define CREATE_RIGHT_STOP_OPERATOR_DUMPER(OPERATOR) \
        template <typename T> auto & operator OPERATOR(const T& rvalue, Fetcher& ifetcher) \
        { \
            std::ignore = rvalue; \
            ifetcher.stop(nullptr); \
            return ifetcher; \
        }

    #define CREATE_DUMP_FOR_OPERATOR(OPERATOR) \
        CREATE_LEFT_OPERATOR_DUMPER(OPERATOR) \
        CREATE_RIGHT_OPERATOR_DUMPER(OPERATOR)

    #define CREATE_STOP_FOR_OPERATOR(OPERATOR) \
        CREATE_LEFT_STOP_OPERATOR_DUMPER(OPERATOR) \
        CREATE_RIGHT_STOP_OPERATOR_DUMPER(OPERATOR)

    CREATE_LEFT_OPERATOR_DUMPER(*) // right part is not necessary because of priority
    CREATE_LEFT_OPERATOR_DUMPER(/) 
    CREATE_DUMP_FOR_OPERATOR(%)
    CREATE_DUMP_FOR_OPERATOR(+)
    CREATE_DUMP_FOR_OPERATOR(-)
    CREATE_DUMP_FOR_OPERATOR(<)
    CREATE_DUMP_FOR_OPERATOR(>)
    CREATE_DUMP_FOR_OPERATOR(<=)
    CREATE_DUMP_FOR_OPERATOR(>=)
    CREATE_DUMP_FOR_OPERATOR(==)
    CREATE_DUMP_FOR_OPERATOR(!=)
#if __cplusplus > 201902L // for future release standart
    CREATE_DUMP_FOR_OPERATOR(<==>)
#endif
    CREATE_STOP_FOR_OPERATOR(&&)
    CREATE_STOP_FOR_OPERATOR(||)
}


#define EVAL_CONDITION(expr) \
    [&](){ \
        using namespace expression_fetcher; \
        Fetcher left; \
        left % expr; \
        if(left.is_cut()) \
        { \
            Fetcher right; \
            expr % right; \
            return left.get_left_string() + right.get_right_string(); \
        } \
        else \
        { \
            return left.get_left_string(); \
        } \
    }()

#define DUMP_EXPRESSION(expr) \
    std::string(#expr)
