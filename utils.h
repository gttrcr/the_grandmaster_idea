#pragma once

#include <iostream>
#include <vector>
#include <regex>
#include <dirent.h>

namespace utils
{
    inline std::string replace(std::string s, const std::string to_replace, const std::string &replacement)
    {
        s.replace(s.find(to_replace), sizeof(to_replace) - 1, replacement);
        return s;
    }

    inline std::string get_filename(const std::string &principal_name)
    {
        std::string timestamp = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
        rnd rnd_file(0, 10000);
        std::string rand = std::to_string(rnd_file.get());
        return principal_name + "_" + timestamp + "_" + rand + FILE_EXT;
    }

    inline std::vector<std::string> get_files(const std::string &record_dir_path, const std::regex regex = std::regex("."))
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

    template <size_t N1, size_t N2>
    inline void bitset_merge(std::bitset<N1> &b1, const std::bitset<N2> &b2)
    {
        b1 <<= b2.size();
        for (unsigned int i = 0; i < b2.size(); i++)
            b1.set(i, b2[i]);
    }

    inline std::string bitstring_to_string(const std::string &bitstring)
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

    inline std::bitset<MAX_BITSET_MATCH_SIZE> string_to_bitset(const std::string &string)
    {
        std::bitset<MAX_BITSET_MATCH_SIZE> ret;
        for (unsigned int i = 0; i < string.size(); i++)
            bitset_merge(ret, std::bitset<COMPRESSION_OFFSET>(string[i] - 32));

        return ret;
    }

    inline std::vector<std::string> exec(const char *command)
    {
        char tmpname[L_tmpnam];
        std::tmpnam(tmpname);
        std::string scommand = command;
        std::string cmd = scommand + " >> " + tmpname;
        std::system(cmd.c_str());
        std::ifstream file(tmpname, std::ios::in | std::ios::binary);
        std::string result;
        if (file)
        {
            while (!file.eof())
                result.push_back(file.get());
            file.close();
        }
        remove(tmpname);

        std::vector<std::string> ret;
        std::istringstream iss(result);
        std::string line;
        while (std::getline(iss, line))
            ret.push_back(line);

        return ret;
    }

    inline void merge_files(const std::vector<std::string> &file_list, const std::string &output_file)
    {
        std::string cmd = "cat ";
        for (unsigned int i = 0; i < file_list.size(); i++)
            cmd += file_list[i] + " ";
        cmd += "> " + output_file;
        std::system(cmd.c_str());

        //remove all files except for merged file
        for (unsigned int i = 0; i < file_list.size(); i++)
            remove(file_list[i].c_str());
    }
	
	inline void sort(const std::string& file)
	{
		std::string sort = "sort " + file + " -o " + file;
		std::system(sort.c_str());
	}

	//print duplicated and non duplicated lines only once
    inline void distinct(const std::string &file, const std::string& new_file)
    {
        std::string distinct = "uniq " + file + " " + new_file;
        std::system(distinct.c_str());
    }
	
	//print only non duplicated line
	unsigned int once_and_count(const std::string& file)
	{
        std::string unique = "uniq -u " + file + " | wc -l";
        std::vector<std::string> vec = exec(unique.c_str());
		return std::stoi(vec[0]);
	}

    inline bool is_number(const std::string &s)
    {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && std::isdigit(*it))
            ++it;
        return !s.empty() && it == s.end();
    }
}
