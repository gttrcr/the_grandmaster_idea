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
            utils::merge_files(files, merge_file);
            std::cout << "\tmerge end" << std::endl;
        }
        else
            merge_file = files[0];

        //sort and process file
        unsigned int lines_number = std::stoi(utils::exec(std::string("wc -l " + merge_file + " | cut -d \" \" -f1").c_str())[0]);
		std::string distinct_file = utils::get_filename("test_matches_distinct");
        std::cout << "\tsort start" << std::endl;
		utils::sort(merge_file);
		std::cout << "\tsort end" << std::endl;
		std::cout << "\tdistinct start" << std::endl;
        utils::distinct(merge_file, distinct_file);
		std::cout << "\tdistinct end" << std::endl;
		std::cout << "\tdistinct once and count start" << std::endl;
		unsigned int lines_exactly_once = utils::once_and_count(merge_file);
        unsigned int distinct_file_lines_number = std::stoi(utils::exec(std::string("wc -l " + distinct_file + " | cut -d \" \" -f1").c_str())[0]);
		std::cout << "\tdistinct once and count end" << std::endl;

        std::cout << "\t" << distinct_file_lines_number << " distinct games on " << lines_number << std::endl;
		std::cout << "\t" << lines_exactly_once << " games appear exactly once on " << lines_number << std::endl;
    }
    else
        std::cout << "\tno files" << std::endl;

    std::cout << test_name << " end" << std::endl;
}
