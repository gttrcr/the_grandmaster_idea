#pragma once

void test_real_match()
{
    std::string test_name = "test_real_match";
    std::cout << test_name << " start" << std::endl;
    std::vector<std::string> files = utils::get_files(".", std::regex("match_link_[0-9]+.bin"));

    std::for_each(
        std::execution::par_unseq,
        files.begin(),
        files.end(),
        [](auto&& item)
        {
            std::ifstream in_file(item);
            std::string line;
            while (std::getline(in_file, line))
            {
                std::bitset<MAX_BITSET_MATCH_SIZE> bit_set(line);
                //std::bitset<MAX_BITSET_MATCH_SIZE> bit_set = utils::string_to_bitset(line);
                //string_to_bitset_2(line);
                //std::cout << std::endl;
                unsigned int length = bit_set.size() % BITSET_SIZE;
                table t;
                for (unsigned int i = 0; i < bit_set.size(); i += BITSET_SIZE)
                {
                    char from = bit_set[i + 5] + 2 * bit_set[i + 4] + 4 * bit_set[i + 3] + 8 * bit_set[i + 2] + 16 * bit_set[i + 1] + 32 * bit_set[i + 0];
                    char to = bit_set[i + 11] + 2 * bit_set[i + 10] + 4 * bit_set[i + 9] + 8 * bit_set[i + 8] + 16 * bit_set[i + 7] + 32 * bit_set[i + 6];
                    t.move(position(from), position(to), false);
                }
            }
        });

    std::cout << test_name << " end" << std::endl;
}