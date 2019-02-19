#pragma once

#include <vector>
#include <string>
#include <sstream>

namespace expression_fetcher
{
    class Fetcher
    {
    public:
        bool is_stopped() const
        {
            return _stopped;
        }

        std::string get_string() const
        {
            std::string str;
            for(const auto &val : _values)
            {
                str += val + " ";
            }
            return str;
        }

        template<typename T>
        void grab(const T &lvalue)
        {
            if(!is_stopped())
            {
                std::stringstream sstr;
                sstr << lvalue << " ";
                _values.push_back(sstr.str());
            }
        }

        void stop()
        {
            _stopped = false;
        }

        void reverse()
        {
            std::reverse(_values.begin(), _values.end());
        }

    private:
        std::vector<std::string> _values;
        bool _stopped = false;
    };

    // left
    template <typename T> auto & operator %(Fetcher& ifetcher, const T& lvalue)
    {
        ifetcher.grab(lvalue);
        return ifetcher;
    }

    // right
    template <typename T> auto & operator %(const T& lvalue, Fetcher& ifetcher)
    {
        ifetcher.grab(lvalue);
        return ifetcher;
    }
}

#define FETCH(expr) \
    [&](){ \
        using namespace expression_fetcher; \
        Fetcher left, right; \
        left % expr; \
        expr % right; \
        right.reverse(); \
        if(left.is_stopped()) \
        { \
            return left.get_string() + " " + right.get_string(); \
        } \
        else \
        { \
            return left.get_string(); \
        } \
    }()