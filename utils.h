#pragma once

#include <iostream>
#include <vector>
#include <regex>
#ifdef __linux__
#include <dirent.h>
#elif _WIN32
#include <filesystem>
#endif
#include <random>

namespace utils
{
    std::string replace(std::string s, const std::string to_replace, const std::string &replacement)
    {
        s.replace(s.find(to_replace), sizeof(to_replace) - 1, replacement);
        return s;
    }

    std::string get_filename(const std::string &principal_name)
    {
        std::string timestamp = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> rnd(0, 10000);
        std::string rand = std::to_string(rnd(rng));
        return principal_name + "_" + timestamp + "_" + rand + FILE_EXT;
    }

    std::vector<std::string> get_files(const std::string &record_dir_path, const std::regex regex = std::regex("."))
    {
        std::vector<std::string> files;
#ifdef __linux__
        struct dirent *entry;
        DIR *dir = opendir(record_dir_path.c_str());

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
        for (const auto &entry : std::filesystem::directory_iterator(record_dir_path))
        {
            std::string name = entry.path().filename().string();
            if (std::regex_match(name, regex))
                files.push_back(name);
        }
#endif

        return files;
    }

    template <size_t N1, size_t N2>
    void bitset_merge(std::bitset<N1> &b1, const std::bitset<N2> &b2)
    {
        b1 <<= b2.size();
        for (unsigned int i = 0; i < b2.size(); i++)
            b1.set(i, b2[i]);
    }

    std::string bitstring_to_string(const std::string &bitstring)
    {
        std::string ret = "";
        for (unsigned int i = 0; i < bitstring.size(); i += COMPRESSION_OFFSET)
        {
            unsigned char ch = 0;
            for (unsigned int c = 0; c < COMPRESSION_OFFSET; c++)
            {
                ch += (bitstring[i + c] - '0');
                ch <<= 1;
            }
            ch >>= 1;
            ret += (ch + 32);
        }

        return ret;
    }

    std::bitset<MAX_BITSET_MATCH_SIZE> string_to_bitset(const std::string &string)
    {
        std::bitset<MAX_BITSET_MATCH_SIZE> ret;
        for (unsigned int i = 0; i < string.size(); i++)
            bitset_merge(ret, std::bitset<COMPRESSION_OFFSET>(string[i] - 32));

        return ret;
    }

    void prepare_string_view(const std::string &file, std::fstream &is, size_t &data_size, std::unique_ptr<char[]> &data)
    {
        is = std::fstream(file, std::ios::in | std::ios::binary);
        is.seekg(0, std::ios::end);
        data_size = is.tellg();
        is.seekg(0, std::ios::beg);
        data = std::unique_ptr<char[]>(new char[data_size]);
        is.read(data.get(), data_size);
    }

    template <typename T>
    void remove_duplicates(std::vector<T> &vec)
    {
        std::sort(vec.begin(), vec.end());
        vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
    }
}