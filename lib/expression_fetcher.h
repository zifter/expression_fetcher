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
        Fetcher()
        {}
    
        template<typename T>
        void dump_right(const char *operatorString, const T &rvalue)
        {
            if(!values.empty()) // skip first %
            {
                values.push_back(operatorString);
            }

            InternalDumpStream stream;
            stream << rvalue;
            values.push_back(stream.GetValue());
        }

        template<typename T>
        void dump_left(const T &lvalue, const char *operatorString)
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

            InternalDumpStream stream;
            stream << lvalue;
            values.push_back(stream.GetValue());
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

    private:
        std::vector<std::string> values;
        size_t rightIndex = 0;
        bool stopped = false;
    };

    #define CREATE_LEFT_OPERATOR_DUMPER(OPERATOR) \
        template <typename T> auto & operator OPERATOR(Fetcher& ifetcher, const T& lvalue) \
        {  \
            ifetcher.dump_right(" "#OPERATOR" ", lvalue);  \
            return ifetcher;  \
        }

    #define CREATE_RIGHT_OPERATOR_DUMPER(OPERATOR) \
        template <typename T> auto & operator OPERATOR(const T& rvalue, Fetcher& ifetcher) \
        { \
            ifetcher.dump_left(rvalue, " "#OPERATOR" "); \
            return ifetcher; \
        }

    #define CREATE_LEFT_STOP_OPERATOR_DUMPER(OPERATOR) \
        template <typename T> auto & operator OPERATOR(Fetcher& ifetcher, const T& lvalue) \
        {  \
            ifetcher.dump_right(" "#OPERATOR" ", lvalue); \
            ifetcher.keep_right_position(); \
            return ifetcher; \
        }

    #define CREATE_RIGHT_STOP_OPERATOR_DUMPER(OPERATOR) \
        template <typename T> auto & operator OPERATOR(const T& rvalue, Fetcher& ifetcher) \
        { \
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
    CREATE_DUMP_FOR_OPERATOR(==)
    CREATE_DUMP_FOR_OPERATOR(!=)
    CREATE_DUMP_FOR_OPERATOR(<)
    CREATE_DUMP_FOR_OPERATOR(>)
    CREATE_DUMP_FOR_OPERATOR(<=)
    CREATE_DUMP_FOR_OPERATOR(>=)
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
