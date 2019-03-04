#pragma once

#include <vector>
#include <string>
#include <sstream>


namespace expression_fetcher
{
    namespace
    {
        // It's used to control the way how value is dumping
        class InternalDumpStream
        {
        public:
            InternalDumpStream() = default;
            ~InternalDumpStream() = default;

            template<typename T>
            InternalDumpStream &operator <<(const T &value)
            {
                sstr << value;
                return *this;
            }

            std::string GetValue() const
            {
                return sstr.str();
            }

        private:
            std::stringstream sstr;
        };

        
        template<>
        InternalDumpStream &InternalDumpStream::operator <<(const bool &value)
        {
            if(value)
            {
                sstr << "true";
            }
            else
            {
                sstr << "false";
            }
            return *this;
        }
    }

    class Fetcher
    {
    public:
        Fetcher(bool isLeftFetcher)
            : isLeft(isLeftFetcher)
        {}
    
        template<typename T>
        void dump(const T &value, const char *operatorString)
        {
            if(is_stopped())
            {
                return;
            }

            InternalDumpStream stream;
            if(isLeft)
            {
                if(!values.empty())
                {
                    stream << operatorString;
                }
                stream << value;
            }
            else
            {
                stream << value;
                if(!values.empty())
                {
                    stream << operatorString;
                }
            }
            
            values.push_back(stream.GetValue());
        }

        bool is_stopped() const
        {
            return stopped;
        }

        std::string get_string() const
        {
            std::string str;
            if(isLeft)
            {
                for(auto it = values.begin(); it != values.end(); ++it)
                {
                    str += *it;
                }
            }
            else
            {
                for(auto it = values.rbegin(); it != values.rend(); ++it)
                {
                    str += *it;
                }
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

        void reverse()
        {
            std::reverse(values.begin(), values.end());
        }

    private:
        std::vector<std::string> values;
        bool isLeft = false;
        bool stopped = false;
    };

    #define DEFINE_LEFT_OPERATOR_DUMPER(OPERATOR) \
        template <typename T> auto & operator OPERATOR(Fetcher& ifetcher, const T& lvalue) \
        {  \
            ifetcher.dump(lvalue, " "#OPERATOR" ");  \
            return ifetcher;  \
        }

    #define DEFINE_RIGHT_OPERATOR_DUMPER(OPERATOR) \
        template <typename T> auto & operator OPERATOR(const T& rvalue, Fetcher& ifetcher) \
        { \
            ifetcher.dump(rvalue, " "#OPERATOR" "); \
            return ifetcher; \
        }

    #define DEFINE_LEFT_STOP_OPERATOR_DUMPER(OPERATOR) \
        template <typename T> auto & operator OPERATOR(Fetcher& ifetcher, const T& lvalue) \
        {  \
            ifetcher.stop(" "#OPERATOR" "); \
            return ifetcher; \
        }

    #define DEFINE_RIGHT_STOP_OPERATOR_DUMPER(OPERATOR) \
        template <typename T> auto & operator OPERATOR(const T& rvalue, Fetcher& ifetcher) \
        { \
            ifetcher.stop(nullptr); \
            return ifetcher; \
        }

    #define CREATE_DUMP_FOR_OPERATOR(OPERATOR) \
        DEFINE_LEFT_OPERATOR_DUMPER(OPERATOR) \
        DEFINE_RIGHT_OPERATOR_DUMPER(OPERATOR)

    #define CREATE_STOP_FOR_OPERATOR(OPERATOR) \
        DEFINE_LEFT_STOP_OPERATOR_DUMPER(OPERATOR) \
        DEFINE_RIGHT_STOP_OPERATOR_DUMPER(OPERATOR)

    // left
    CREATE_DUMP_FOR_OPERATOR(+)
    CREATE_DUMP_FOR_OPERATOR(-)
    CREATE_DUMP_FOR_OPERATOR(*)
    CREATE_DUMP_FOR_OPERATOR(/)
    CREATE_DUMP_FOR_OPERATOR(%)
    CREATE_DUMP_FOR_OPERATOR(==)
    CREATE_DUMP_FOR_OPERATOR(!=)
    CREATE_DUMP_FOR_OPERATOR(<)
    CREATE_DUMP_FOR_OPERATOR(>)
    CREATE_DUMP_FOR_OPERATOR(<=)
    CREATE_DUMP_FOR_OPERATOR(>=)
#if __cplusplus > 201902L // for future release
    CREATE_DUMP_FOR_OPERATOR(<==>)
#endif
    CREATE_STOP_FOR_OPERATOR(&&)
    CREATE_STOP_FOR_OPERATOR(||)
}


#define EVAL_CONDITION(expr) \
    [&](){ \
        using namespace expression_fetcher; \
        Fetcher left(true); \
        left % expr; \
        if(left.is_stopped()) \
        { \
            Fetcher right(false); \
            expr % right; \
            return left.get_string() + right.get_string(); \
        } \
        else \
        { \
            return left.get_string(); \
        } \
    }()

#define DUMP_EXPRESSION(expr) \
    std::string(#expr)
