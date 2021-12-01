#pragma once

#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <string>

namespace parser_utils
{
    inline std::vector<std::string> split_by_string(std::string s, const std::string delimiter)
    {
        std::vector<std::string> ret;
        size_t pos = 0;
        std::string token;
        while ((pos = s.find(delimiter)) != std::string::npos)
        {
            token = s.substr(0, pos);
            ret.push_back(token);
            s.erase(0, pos + delimiter.length());
        }
        ret.push_back(s);

        return ret;
    }

    inline std::vector<std::string> unique(std::vector<std::string> ret)
    {
        std::vector<std::string>::iterator ip = std::unique(ret.begin(), ret.end());
        ret.resize(std::distance(ret.begin(), ip));
        return ret;
    }
}

namespace parser
{
    using basic_tuple = std::tuple<bool, std::string>;
    using plus_minus_tuple_type = basic_tuple;
    using plus_minus_token_type = std::vector<plus_minus_tuple_type>;

    inline void get_plus_minus_tokens(const std::string& expression, plus_minus_token_type& plus_minus_tokens)
    {
        std::vector<std::string> tokens_plus = parser_utils::split_by_string(expression, "+");
        for (unsigned int i = 0; i < tokens_plus.size(); i++)
        {
            std::vector<std::string> tokens_minus = parser_utils::split_by_string(tokens_plus[i], "-");
            if (tokens_minus.size() > 1 && tokens_minus.size() < 3)
            {
                plus_minus_tokens.push_back(std::make_tuple(false, tokens_minus[0]));
                plus_minus_tokens.push_back(std::make_tuple(true, tokens_minus[1]));
            }
            else if (tokens_minus.size() == 1)
                plus_minus_tokens.push_back(std::make_tuple(false, tokens_minus[0]));
            else
                throw std::exception();
        }

        auto it = std::remove_if(plus_minus_tokens.begin(), plus_minus_tokens.end(), [](std::tuple<bool, std::string>& t)
            { return std::get<1>(t) == ""; });
        plus_minus_tokens.erase(it, plus_minus_tokens.end());
    }

    using mult_frac_inner_tuple_type = std::vector<basic_tuple>;
    using mult_frac_tuple_type = std::tuple<bool, mult_frac_inner_tuple_type>;
    using mult_frac_token_type = std::vector<mult_frac_tuple_type>;

    inline void from_plus_minus_to_mult_frac(const plus_minus_token_type& plus_minus_tokens, mult_frac_token_type& mult_frac_tokens)
    {
        mult_frac_tokens.clear();
        for (unsigned int i = 0; i < plus_minus_tokens.size(); i++)
        {
            mult_frac_inner_tuple_type inner_tuple;
            inner_tuple.push_back(std::make_tuple(false, std::get<1>(plus_minus_tokens[i])));

            mult_frac_tuple_type tuple;
            tuple = std::make_tuple(std::get<0>(plus_minus_tokens[i]), inner_tuple);
            mult_frac_tokens.push_back(tuple);
        }
    }

    inline void get_mult_frac_tokens(mult_frac_token_type& mult_frac_tokens)
    {
        for (unsigned int i = 0; i < mult_frac_tokens.size(); i++)
        {
            std::string expression = std::get<1>(std::get<1>(mult_frac_tokens[i])[0]);
            std::vector<std::string> tokens_mult = parser_utils::split_by_string(expression, "*");
            mult_frac_inner_tuple_type tuple;
            for (unsigned int i = 0; i < tokens_mult.size(); i++)
            {
                std::vector<std::string> tokens_frac = parser_utils::split_by_string(tokens_mult[i], "/");
                if (tokens_frac.size() > 1 && tokens_frac.size() < 3)
                {
                    tuple.push_back(std::make_tuple(false, tokens_frac[0]));
                    tuple.push_back(std::make_tuple(true, tokens_frac[1]));
                }
                else if (tokens_frac.size() == 1)
                    tuple.push_back(std::make_tuple(false, tokens_frac[0]));
                else
                    throw std::exception();
            }

            auto it = std::remove_if(tuple.begin(), tuple.end(), [](basic_tuple& t)
                { return std::get<1>(t) == ""; });
            tuple.erase(it, tuple.end());
            std::get<1>(mult_frac_tokens[i]) = tuple;
        }
    }

    inline void simplify_mult_frac_tokens(mult_frac_token_type& mult_frac_tokens)
    {
        for (unsigned int i = 0; i < mult_frac_tokens.size(); i++)
        {
            mult_frac_inner_tuple_type current_tuple = std::get<1>(mult_frac_tokens[i]);

            //Simplify multiplication
            std::vector<std::string> expr;
            std::for_each(current_tuple.begin(), current_tuple.end(), [&expr](basic_tuple& t)
                { expr.push_back(std::get<1>(t)); });
            std::vector<std::string> unique_mult_frac_tokens = parser_utils::unique(expr);

            //This is the expression coming from simplifying multiplication
            std::string simplification = "";

            //If there is a 0, all multiplication is 0
            unsigned int count = std::count_if(unique_mult_frac_tokens.begin(), unique_mult_frac_tokens.end(), [](std::string& s)
                { return s == "0"; });
            if (count > 0)
                simplification = "0";
            else
            {
                for (unsigned int i = 0; i < unique_mult_frac_tokens.size(); i++)
                {
                    std::string token = unique_mult_frac_tokens[i];
                    unsigned int num_count = std::count_if(current_tuple.begin(), current_tuple.end(), [token](basic_tuple& s)
                        { return std::get<1>(s) == token && !std::get<0>(s); });
                    unsigned int den_count = std::count_if(current_tuple.begin(), current_tuple.end(), [token](basic_tuple& s)
                        { return std::get<1>(s) == token && std::get<0>(s); });
                    switch ((int)(num_count - den_count))
                    {
                    case 0:
                        break;
                        simplification += "1";
                        simplification += (i == unique_mult_frac_tokens.size() - 1 ? "" : "*");
                        break;
                    case 1:
                        simplification += token;
                        simplification += (i == unique_mult_frac_tokens.size() - 1 ? "" : "*");
                        break;
                    default:
                        simplification += token + "^" + std::to_string((int)(num_count - den_count));
                        simplification += (i == unique_mult_frac_tokens.size() - 1 ? "" : "*");
                    }
                }
            }

            std::get<1>(std::get<1>(mult_frac_tokens[i])[0]) = simplification;
        }
    }

    inline void from_mult_frac_to_plus_minus(const mult_frac_token_type& mult_frac_tokens, plus_minus_token_type& plus_minus_tokens)
    {
        plus_minus_tokens.clear();
        for (unsigned int i = 0; i < mult_frac_tokens.size(); i++)
        {
            plus_minus_tuple_type tuple;
            std::get<0>(tuple) = std::get<0>(mult_frac_tokens[i]);
            std::get<1>(tuple) = std::get<1>(std::get<1>(mult_frac_tokens[i])[0]);
            plus_minus_tokens.push_back(tuple);
        }
    }

    inline void simplify_plus_minus_tokens(plus_minus_token_type& plus_minus_tokens)
    {
    }

    inline std::string print_plus_minus_tokens(plus_minus_token_type& plus_minus_tokens)
    {
        std::string ret = "";
        for (unsigned int i = 0; i < plus_minus_tokens.size(); i++)
            if (std::get<1>(plus_minus_tokens[i]) != "0")
                ret += (std::get<0>(plus_minus_tokens[i]) ? "-" : "+") + std::get<1>(plus_minus_tokens[i]);

        return ret;
    }

    inline std::string parser(const std::string expression)
    {
        parser::plus_minus_token_type plus_minus_tokens;
        parser::get_plus_minus_tokens(expression, plus_minus_tokens);

        parser::mult_frac_token_type mult_frac_tokens;
        parser::from_plus_minus_to_mult_frac(plus_minus_tokens, mult_frac_tokens);
        parser::get_mult_frac_tokens(mult_frac_tokens);

        parser::simplify_mult_frac_tokens(mult_frac_tokens);
        parser::from_mult_frac_to_plus_minus(mult_frac_tokens, plus_minus_tokens);
        parser::simplify_plus_minus_tokens(plus_minus_tokens);

        return parser::print_plus_minus_tokens(plus_minus_tokens);
    }
}