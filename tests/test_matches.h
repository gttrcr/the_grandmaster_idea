#pragma once

void test_matches(const unsigned int max_test, const bool save_all_match = false)
{
    std::string test_name = "test_matches";
    std::cout << test_name << " start" << std::endl;

    std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> th_work_distribution;
    for (unsigned int i = 0; i < THREADS; i++)
    {
        unsigned int start = i * max_test / THREADS;
        unsigned int stop = start + max_test / THREADS;
        th_work_distribution.push_back(std::make_tuple(i, start, stop));
    }

    std::for_each(
        std::execution::par_unseq,
        th_work_distribution.begin(),
        th_work_distribution.end(),
        [save_all_match](auto&& item)
        {
            unsigned int th_n = std::get<0>(item);
            unsigned int start = std::get<1>(item);
            unsigned int stop = std::get<2>(item);
            unsigned int num_of_tests = stop - start;
            unsigned int giga_file = 1000 * th_n;

            std::ofstream of_match_link;
            std::string match_link_file = "match_link_#.bin";
            std::ostream_iterator<std::string> of_match_link_it(of_match_link, "\n");

            if (save_all_match)
            {
                of_match_link.open(utils::replace(match_link_file, "#", std::to_string(giga_file)) + ".bin", std::ios_base::out | std::ios_base::trunc);
                of_match_link.close();
                of_match_link.open(utils::replace(match_link_file, "#", std::to_string(giga_file)) + ".bin", std::ios_base::app);
            }

            std::vector<unsigned int> match_duration;
            std::vector<std::string> matches;
            for (unsigned int i = start; i <= stop; i++)
            {
                unsigned int count = 0;
                std::bitset<MAX_BITSET_MATCH_SIZE> match;
                piece::color win = table::play(count, match);
                match_duration.push_back(count);
                if (save_all_match)
                {
                    std::string tmp = match.to_string();
                    tmp = tmp.substr(tmp.size() - 14 * count, 14 * count);
                    std::reverse(tmp.begin(), tmp.end());
                    //matches.push_back(utils::bitstring_to_string(tmp));
                    matches.push_back(tmp);
                }

                if (th_n == 0 && (i % 1000 == 0))
                    std::cout << "         \r" << (double)i * 100.0 / (double)num_of_tests << "%\r"
#ifdef __linux__
                    << "\033[F" << std::endl;
#elif _WIN32
                    ;
#endif

                unsigned int s = (unsigned int)(matches.size());
                if (save_all_match && (s > 100000 || s % num_of_tests == 0))
                {
                    unsigned long int size = (unsigned long int)(of_match_link.tellp());
                    if (size > 1000000000)
                    {
                        of_match_link.close();
                        giga_file++;
                        of_match_link.open(utils::replace(match_link_file, "#", std::to_string(giga_file)) + ".bin", std::ios_base::out | std::ios_base::trunc);
                        of_match_link.close();
                        of_match_link.open(utils::replace(match_link_file, "#", std::to_string(giga_file)) + ".bin", std::ios_base::app);
                        of_match_link_it = std::ostream_iterator<std::string>(of_match_link, "\n");
                    }

                    std::copy(matches.begin(), matches.end(), of_match_link_it);
                    matches.clear();
                }
            }

            of_match_link.close();

            std::string match_duration_file = "match_duration_#.bin";
            match_duration_file = utils::replace(match_duration_file, "#", std::to_string(th_n));
            std::ofstream of_match_duration;
            of_match_duration.open(match_duration_file, std::ios_base::out | std::ios_base::trunc);
            of_match_duration.close();
            of_match_duration.open(match_duration_file, std::ios_base::app);
            std::ostream_iterator<unsigned int> of_match_duration_it(of_match_duration, "\n");
            std::copy(match_duration.begin(), match_duration.end(), of_match_duration_it);
            of_match_duration.close();
        });

    std::cout << test_name << " end" << std::endl;
}