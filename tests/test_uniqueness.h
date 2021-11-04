#pragma once

void test_3_uniqueness()
{
    const std::string test_name = "test_uniqueness";
    std::cout << test_name << " start" << std::endl;

    //get all available files
    std::string merge_file = utils::get_filename("test_matches");
    std::vector<std::string> files = utils::get_files(".", std::regex("^test_matches_[0-9]+_[0-9]+." + std::string(FILE_EXT) + "$"));

    if (files.size() > 0)
    {
        if (files.size() > 1)
        {
            //merge all files
            std::cout << "\tmerge start" << std::endl;
            std::string cat = "cat ";
            for (unsigned int i = 0; i < files.size(); i++)
                cat += files[i] + " ";
            cat += "> " + merge_file;
            std::system(cat.c_str());
            std::cout << "\tmerge end" << std::endl;

            //remove all files except for merged file
            for (unsigned int i = 0; i < files.size(); i++)
                remove(files[i].c_str());
        }
        else
            merge_file = files[0];

        //sort merged file
        unsigned int lines_number = std::stoi(utils::exec(std::string("wc -l " + merge_file + " | cut -d \" \" -f1").c_str())[0]);
        std::cout << "\tsort start" << std::endl;
        std::string sort = "sort -u " + merge_file + " -o " + merge_file;
        std::system(sort.c_str());
        std::cout << "\tsort end" << std::endl;
        unsigned int unique_lines_number = std::stoi(utils::exec(std::string("wc -l " + merge_file + " | cut -d \" \" -f1").c_str())[0]);

        //split merge_file, remove merge_file, rename new files
        std::ifstream if_merge_file(merge_file);
        if_merge_file.seekg(0, std::ios::end);
        unsigned int n_files = ((unsigned long int)(if_merge_file.tellg()) / MAX_FILE_SIZE_BYTE) + 1;
        if_merge_file.close();
        if (n_files > 1)
        {
            std::cout << "\tsplit merge file start" << std::endl;
            std::string split = "split -n " + std::to_string(n_files) + " " + merge_file + " " + merge_file;
            std::system(split.c_str());
            remove(merge_file.c_str());
            files = utils::get_files(".", std::regex("^test_matches_[0-9]+_[0-9]+." + std::string(FILE_EXT) + "[a-z]+$"));
            for (unsigned int i = 0; i < files.size(); i++)
                std::system(std::string("mv " + files[i] + " " + utils::get_filename("test_matches")).c_str());
        }
        else
            std::cout << "\tno need of split merge file" << std::endl;

        std::cout << unique_lines_number << " unique games on " << lines_number << " games (" << lines_number - unique_lines_number << " duplicated games)" << std::endl;
    }
    else
        std::cout << "\tno files" << std::endl;

    std::cout << test_name << " end" << std::endl;
}