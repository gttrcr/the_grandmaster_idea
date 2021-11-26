#pragma once

namespace test_2
{
    void save(const std::string &test_name, std::ofstream &of_match_link, std::vector<std::string> &matches, std::ostream_iterator<std::string> &of_match_link_it)
    {
        if ((unsigned long int)(of_match_link.tellp()) > MAX_FILE_SIZE_BYTE)
        {
            of_match_link.close();
            of_match_link.open(utils::get_filename(test_name), std::ios_base::app);
            of_match_link_it = std::ostream_iterator<std::string>(of_match_link, "\n");
        }

        std::copy(matches.begin(), matches.end(), of_match_link_it);
        matches.clear();
    }

    void test_2_matches(const unsigned int max_test)
    {
        const std::string test_name = "test_matches";
        std::cout << test_name << " start, max_test " << max_test << std::endl;
        if (max_test % global_threads != 0)
            std::cout << max_test << " is not a integer multiple of " << global_threads << " hence the test may be incorrect or incomplete" << std::endl;

        std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> th_work_distribution;
        for (unsigned int i = 0; i < global_threads; i++)
        {
            unsigned int start = i * max_test / global_threads;
            unsigned int stop = start + max_test / global_threads;
            th_work_distribution.push_back(std::make_tuple(i, start, stop));
        }

        std::for_each(
            std::execution::par_unseq,
            th_work_distribution.begin(),
            th_work_distribution.end(),
            [test_name](auto &&item)
            {
                unsigned int th_n = std::get<0>(item);
                unsigned int start = std::get<1>(item);
                unsigned int stop = std::get<2>(item);
                unsigned int num_of_tests = stop - start;

                std::ofstream of_match_link;
                std::ostream_iterator<std::string> of_match_link_it(of_match_link, "\n");
                of_match_link.open(utils::get_filename(test_name), std::ios_base::app);

                std::vector<unsigned int> match_duration;
                std::vector<std::string> matches;
                for (unsigned int i = start; i < stop; i++)
                {
                    unsigned int count = 0;
                    std::bitset<MAX_BITSET_MATCH_SIZE> match;
                    piece::color win = table::play(count, match);
                    match_duration.push_back(count);

                    //save match
                    //std::string tmp = match.to_string();
                    //unsigned int length = BITSET_SIZE * count;
                    //tmp = tmp.substr(tmp.size() - length, length);
                    //std::reverse(tmp.begin(), tmp.end());
                    //utils::chess::from_bitset_to_matches_string(match)
                    //matches.push_back(utils::bitstring_to_string(tmp));
                    matches.push_back(utils::chess::from_bitset_to_matches_string(match, count));

                    if (th_n == 0 && (i % 1000 == 0))
                        std::cout << "         \r" << (double)i * 100.0 / (double)num_of_tests << "%\r"
                                  << "\033[F" << std::endl;

                    if ((unsigned int)(matches.size()) % 100000 == 0)
                        save(test_name, of_match_link, matches, of_match_link_it);
                }

                if ((unsigned int)(matches.size()) > 0)
                    save(test_name, of_match_link, matches, of_match_link_it);

                of_match_link.close();

                std::ofstream of_match_duration(utils::get_filename(test_name + "_duration"), std::ios_base::app);
                std::ostream_iterator<unsigned int> of_match_duration_it(of_match_duration, "\n");
                std::copy(match_duration.begin(), match_duration.end(), of_match_duration_it);
                of_match_duration.close();
            });

        //merge all test_matches
        std::vector<std::string> files = utils::get_files(".", std::regex("^" + test_name + "_[0-9]+_[0-9]+." + std::string(FILE_EXT) + "$"));
        utils::merge_files(files, utils::get_filename(test_name));

        //merge all test_matches_duration
        files = utils::get_files(".", std::regex("^" + test_name + "_duration_[0-9]+_[0-9]+." + std::string(FILE_EXT) + "$"));
        utils::merge_files(files, utils::get_filename(test_name + "_duration"));

        std::cout << test_name << " end" << std::endl;
    }
}