/**
 * @file stringhelper.h
 * @brief Contains the definition and source code for the StringHelper class which provides extra string functionality.
 * @author Kristopher Bickmore
 * @date November 20, 2016
 */
#ifndef PROJECT1_STRINGHELPER_H_H
#define PROJECT1_STRINGHELPER_H_H
#pragma once

#include <algorithm>
#include <cctype>
#include <functional>
#include <list>
#include <locale>
#include <set>
#include <sstream>
#include <string>
#include <vector>
/**
 * @brief This class assists in handling std::string variables
 */
class StringHelper
{
public:

    /**
     * Splits a string into multiple strings at each location of the given delimiter.
     * @param source the string to be split
     * @param delimiter the character which will determine where to split the string
     * @return a list containing all the strings
     */
	static std::list<std::string> split(const std::string &source, char delimiter)
	{
		std::list<std::string> output1;
		std::istringstream ss(source);
		std::string nextItem;

		while (std::getline(ss, nextItem, delimiter))
		{
			output1.push_back(nextItem);
		}

		return output1;
	}

    /**
     * Returns a copy the string, with leading and trailing whitespace removed.
     * @param s The string to be trimmed
     * @return the copy of the string
     */
	static inline std::string trim(const std::string &s)
	{
		auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
		auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
		return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
	}

    template<typename T>

    /**
     * Converts a given value to a string
     * @param subject the value to be converted
     * @return the new string
     */
    static std::string toString(const T &subject)

    {
        std::ostringstream ss;
        ss << subject;
        return ss.str();
    }
    /**
     * Splits a string representing a file path based on delimiters.  Similar to split and it needs to be determined which is better.
     * @param str the string to be split
     * @param delimiters the set of delimiters
     * @return a vector containing the split path
     */
    static std::vector<std::string> splitpath(
        const std::string& str,
        const std::set<char> delimiters)
    {
        std::vector<std::string> result;

        char const* pch = str.c_str();
        char const* start = pch;
        for (; *pch; ++pch)
        {
            if (delimiters.find(*pch) != delimiters.end())
            {
                if (start != pch)
                {
                    std::string str(start, pch);
                    result.push_back(str);
                }
                else
                {
                    result.push_back("");
                }
                start = pch + 1;
            }
        }
        result.push_back(start);

        return result;
    }
};

#endif
