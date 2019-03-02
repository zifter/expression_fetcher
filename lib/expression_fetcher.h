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

    // left
    template <typename T> auto & operator %(Fetcher& ifetcher, const T& lvalue)
    {
        ifetcher.dump(lvalue, " % ");
        return ifetcher;
    }
    template <typename T> auto & operator <(Fetcher& ifetcher, const T& lvalue)
    {
        ifetcher.dump(lvalue, " < ");
        return ifetcher;
    }
    template <typename T> auto & operator >(Fetcher& ifetcher, const T& lvalue)
    {
        ifetcher.dump(lvalue, " > ");
        return ifetcher;
    }
    template <typename T> auto & operator &&(Fetcher& ifetcher, const T& lvalue)
    {
        ifetcher.stop(" && ");
        return ifetcher;
    }
    template <typename T> auto & operator ||(Fetcher& ifetcher, const T& lvalue)
    {
        ifetcher.stop(" || ");
        return ifetcher;
    }

    // right
    template <typename T> auto & operator %(const T& lvalue, Fetcher& ifetcher)
    {
        ifetcher.dump(lvalue, " % ");
        return ifetcher;
    }
    template <typename T> auto & operator <(const T& lvalue, Fetcher& ifetcher)
    {
        ifetcher.dump(lvalue, " < ");
        return ifetcher;
    }
    template <typename T> auto & operator >(const T& lvalue, Fetcher& ifetcher)
    {
        ifetcher.dump(lvalue, " > ");
        return ifetcher;
    }
    template <typename T> auto & operator &&(const T& lvalue, Fetcher& ifetcher)
    {
        ifetcher.stop(nullptr);
        return ifetcher;
    }
    template <typename T> auto & operator ||(const T& lvalue, Fetcher& ifetcher)
    {
        ifetcher.stop(nullptr);
        return ifetcher;
    }
}


#define FETCH(expr) \
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
