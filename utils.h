#pragma once
#include <algorithm>
#include <string>
#include <iostream>
#include <dirent.h>
#include <vector>
#include <regex>

namespace utils
{
    static std::string replace(std::string s, const std::string to_replace, const std::string &replacement)
    {
        s.replace(s.find(to_replace), sizeof(to_replace) - 1, replacement);
        return s;
    }

    std::vector<std::string> files(const std::string &record_dir_path, const std::regex regex = std::regex("."))
    {
        std::vector<std::string> files;
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

        return files;
    }
}