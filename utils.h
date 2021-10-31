#pragma once

#include<iostream>
#include<vector>
#include<regex>
#ifdef __linux__
#include <dirent>
#elif _WIN32
#include<filesystem>
#endif

namespace utils
{
    std::string replace(std::string s, const std::string to_replace, const std::string& replacement)
    {
        s.replace(s.find(to_replace), sizeof(to_replace) - 1, replacement);
        return s;
    }

    std::vector<std::string> get_files(const std::string& record_dir_path, const std::regex regex = std::regex("."))
    {
        std::vector<std::string> files;
#ifdef __linux__
        struct dirent* entry;
        DIR* dir = opendir(record_dir_path.c_str());

        if (dir == NULL)
            return files;
        while ((entry = readdir(dir)) != NULL)
        {
            std::string name = entry->d_name;
            if (std::regex_match(name, regex))
                files.push_back(name);
        }
        closedir(dir);
#elif _WIN32
        for (const auto& entry : std::filesystem::directory_iterator(record_dir_path))
        {
            std::string name = entry.path().filename().string();
            if (std::regex_match(name, regex))
                files.push_back(name);
        }
#endif

        return files;
    }

    template <size_t N1, size_t N2>
    void bitset_merge(std::bitset<N1>& b1, const std::bitset<N2>& b2)
    {
        b1 <<= b2.size();
        for (unsigned int i = 0; i < b2.size(); i++)
            b1.set(i, b2[i]);
    }

    std::string bitstring_to_string(const std::string& data)
    {
        std::stringstream sstream(data);
        std::string output;
        while (sstream.good())
        {
            std::bitset<8> bits;
            sstream >> bits;
            char c = char(bits.to_ulong());
            output += c;
        }

        return output;
    }

    std::bitset<MAX_BITSET_MATCH_SIZE> string_to_bitset(std::string& string)
    {
        std::bitset<MAX_BITSET_MATCH_SIZE> ret;
        for (char& _char : string)
            bitset_merge(ret, std::bitset<8>(_char));
        return ret;
    }
}