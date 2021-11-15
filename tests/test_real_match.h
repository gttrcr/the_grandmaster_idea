#pragma once

#include <future>
#include <mutex>

std::mutex mtx;
std::vector<char>::size_type BLOCK_SIZE = 1000000000;

bool expensive_operation(std::vector<char> const &data)
{
    bool r = false;
    return r;
}

std::vector<std::future<bool>> partition(std::ifstream &in)
{
    std::vector<std::future<bool>> tasks;

    while (!in.eof() && !in.fail())
    {
        std::vector<char> c(BLOCK_SIZE);
        in.read(c.data(), BLOCK_SIZE);
        c.resize(in.gcount());
        tasks.push_back(std::async([](std::vector<char> data)
                                   { return expensive_operation(data); },
                                   std::move(c)));
    }
    return tasks;
}

void test_4_real_match(const std::string &input)
{
    const std::string test_name = "test_real_match";
    std::cout << test_name << " start" << std::endl;
    std::vector<std::string> files = utils::get_files(".", std::regex("^test_matches_[0-9]+_[0-9]+" + std::string(FILE_EXT) + "$"));

    if (files.size() > 0)
    {
        if (files.size() == 1)
        {
            unsigned int index = 0;
            std::vector<unsigned int> th(2);
            std::iota(th.begin(), th.end(), 0);
            std::ifstream is(files[0], std::ios::binary);
            std::for_each(
                std::execution::par_unseq,
                th.begin(),
                th.end(),
                [&index, &is](auto &&item)
                {
                    while (is.good())
                    {
                        mtx.lock();
                        std::string sLine;
                        getline(is, sLine);
                        mtx.unlock();
                        index++;
                    }
                });

            std::cout << index << std::endl;
        }
        else
            std::cout << "more than 1 file" << std::endl;
    }
    else
        std::cout << "no files" << std ::endl;

    //std::vector<std::vector<std::string>> files_th(global_threads);
    //for (unsigned int i = 0; i < files.size(); i++)
    //    files_th[i % global_threads].push_back(files[i]);
    //
    //std::for_each(
    //    std::execution::par_unseq,
    //    files_th.begin(),
    //    files_th.end(),
    //    [](auto &&item)
    //    {
    //        for (unsigned int i = 0; i < item.size(); i++)
    //        {
    //            std::ifstream in_file(item[i]);
    //            std::string line;
    //            while (std::getline(in_file, line))
    //            {
    //                std::bitset<MAX_BITSET_MATCH_SIZE> bit_set = utils::string_to_bitset(line);
    //                unsigned int length = bit_set.size() % BITSET_SIZE;
    //                table t;
    //                for (unsigned int i = 0; i < bit_set.size(); i += BITSET_SIZE)
    //                {
    //                    //unsigned char from = bit_set[i + 5] + 2 * bit_set[i + 4] + 4 * bit_set[i + 3] + 8 * bit_set[i + 2] + 16 * bit_set[i + 1] + 32 * bit_set[i + 0];
    //                    //unsigned char to = bit_set[i + 11] + 2 * bit_set[i + 10] + 4 * bit_set[i + 9] + 8 * bit_set[i + 8] + 16 * bit_set[i + 7] + 32 * bit_set[i + 6];
    //
    //                    unsigned char from = 0;
    //                    unsigned char to = 0;
    //                    for (unsigned int c = 0; c < COMPRESSION_OFFSET; c++)
    //                    {
    //                        from += (bit_set[i + c] << (COMPRESSION_OFFSET - c - 1));
    //                        to += (bit_set[i + c + COMPRESSION_OFFSET - 1] << (COMPRESSION_OFFSET - c - 1));
    //                    }
    //                    from >>= 1;
    //                    to >>= 1;
    //
    //                    t.move(position(from), position(to), false);
    //                    t.draw();
    //                }
    //            }
    //        }
    //    });

    std::cout << test_name << " end" << std::endl;
}