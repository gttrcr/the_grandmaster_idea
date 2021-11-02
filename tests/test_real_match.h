#pragma once

void test_4_real_match(const std::string& input)
{
    const std::string test_name = "test_real_match";
    std::cout << test_name << " start" << std::endl;
    std::vector<std::string> files = utils::get_files(".", std::regex("test_matches_[0-9]+_[0-9]+" + std::string(FILE_EXT)));
    std::vector<std::vector<std::string>> files_th(global_threads);
    for (unsigned int i = 0; i < files.size(); i++)
        files_th[i % global_threads].push_back(files[i]);

    std::for_each(
        std::execution::par_unseq,
        files_th.begin(),
        files_th.end(),
        [](auto&& item)
        {
            for (unsigned int i = 0; i < item.size(); i++)
            {
                std::ifstream in_file(item[i]);
                std::string line;
                while (std::getline(in_file, line))
                {
                    std::bitset<MAX_BITSET_MATCH_SIZE> bit_set = utils::string_to_bitset(line);
                    unsigned int length = bit_set.size() % BITSET_SIZE;
                    table t;
                    for (unsigned int i = 0; i < bit_set.size(); i += BITSET_SIZE)
                    {
                        //unsigned char from = bit_set[i + 5] + 2 * bit_set[i + 4] + 4 * bit_set[i + 3] + 8 * bit_set[i + 2] + 16 * bit_set[i + 1] + 32 * bit_set[i + 0];
                        //unsigned char to = bit_set[i + 11] + 2 * bit_set[i + 10] + 4 * bit_set[i + 9] + 8 * bit_set[i + 8] + 16 * bit_set[i + 7] + 32 * bit_set[i + 6];
                        
                        unsigned char from = 0;
                        unsigned char to = 0;
                        for (unsigned int c = 0; c < COMPRESSION_OFFSET; c++)
                        {
                            from += (bit_set[i + c] << (COMPRESSION_OFFSET - c - 1));
                            to += (bit_set[i + c + COMPRESSION_OFFSET - 1] << (COMPRESSION_OFFSET - c - 1));
                        }
                        from >>= 1;
                        to >>= 1;
                        
                        t.move(position(from), position(to), false);
                    }
                }
            }
        });

    std::cout << test_name << " end" << std::endl;
}