#include <thread>
#include <string>
#include <chrono>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <execution>
#include <regex>
#include <bitset>

#define BITSET_SIZE 14
#define MAX_BITSET_MATCH_SIZE 5000

#include "utils.h"
#include "position.h"
#include "piece.h"
#include "table.h"

#define THREADS 1

void test_av_cap(const unsigned int max_test)
{
    std::string test_name = "test_av_cap";
    std::cout << test_name << " start" << std::endl;

    std::ofstream outfile;
    outfile.open(test_name + ".txt", std::ios_base::out | std::ios_base::trunc);
    outfile.close();

    table t;
    std::vector<std::tuple<
        piece,        //for each piece
        unsigned int, //number of this piece on table
        unsigned int, //number of available positions
        unsigned int, //number of available captures
        std::tuple<
            unsigned int, //number of pawn to capture
            unsigned int, //number of rook to capture
            unsigned int, //number of knight to capture
            unsigned int, //number of bishop to capture
            unsigned int, //number of king to capture
            unsigned int  //number of queen to capture
            >>>
        average;

    average.push_back(std::make_tuple(piece(piece::value::pawn, piece::color::white), 0, 0, 0, std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>()));
    average.push_back(std::make_tuple(piece(piece::value::rook, piece::color::white), 0, 0, 0, std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>()));
    average.push_back(std::make_tuple(piece(piece::value::knight, piece::color::white), 0, 0, 0, std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>()));
    average.push_back(std::make_tuple(piece(piece::value::bishop, piece::color::white), 0, 0, 0, std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>()));
    average.push_back(std::make_tuple(piece(piece::value::king, piece::color::white), 0, 0, 0, std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>()));
    average.push_back(std::make_tuple(piece(piece::value::queen, piece::color::white), 0, 0, 0, std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>()));

    std::string str = "";
    for (unsigned int i = 1; i <= max_test; i++)
    {
        t.random();
        std::vector<std::tuple<piece, position>> dist = t.pieces();
        for (unsigned int d = 0; d < dist.size(); d++)
        {
            unsigned int pos = (unsigned int)(t.available_positions(std::get<1>(dist[d]), TR_VALUE).size());
            std::vector<std::tuple<position, piece>> av_cap = t.available_captures(std::get<1>(dist[d]));
            unsigned int cap = (unsigned int)(av_cap.size());
            for (unsigned int r = 0; r < average.size(); r++)
                if (std::get<0>(average[r]) == std::get<0>(dist[d]))
                {
                    std::get<1>(average[r]) += 1;
                    std::get<2>(average[r]) += pos;
                    std::get<3>(average[r]) += cap;
                    for (unsigned int av = 0; av < av_cap.size(); av++)
                    {
                        piece::value v = std::get<1>(av_cap[av]).get_value();
                        if (v == piece::value::pawn)
                            std::get<0>(std::get<4>(average[r])) += 1;
                        else if (v == piece::value::rook)
                            std::get<1>(std::get<4>(average[r])) += 1;
                        else if (v == piece::value::knight)
                            std::get<2>(std::get<4>(average[r])) += 1;
                        else if (v == piece::value::bishop)
                            std::get<3>(std::get<4>(average[r])) += 1;
                        else if (v == piece::value::king)
                            std::get<4>(std::get<4>(average[r])) += 1;
                        else if (v == piece::value::queen)
                            std::get<5>(std::get<4>(average[r])) += 1;
                    }
                }
        }

        if ((i % 1000) == 0)
        {
            std::string tmp = std::to_string(i) + " ";
            for (unsigned int r = 0; r < average.size(); r++)
            {
                tmp += "(" + std::to_string(std::get<1>(average[r])) + " " + std::to_string(std::get<2>(average[r])) + " " + std::to_string(std::get<3>(average[r])) + " ";
                tmp += "(" + std::to_string(std::get<0>(std::get<4>(average[r]))) + " " + std::to_string(std::get<1>(std::get<4>(average[r]))) + " " + std::to_string(std::get<2>(std::get<4>(average[r]))) + " " + std::to_string(std::get<3>(std::get<4>(average[r]))) + " " + std::to_string(std::get<4>(std::get<4>(average[r]))) + " " + std::to_string(std::get<5>(std::get<4>(average[r]))) + ")) ";
            }
            //std::cout << tmp << std::endl;
            str += tmp + "\n";
        }

        if ((i % 10000) == 0)
        {
            std::cout << "         \r" << (double)i * 100.0 / (double)max_test << "%\r"
                      << "\033[F" << std::endl;
            outfile.open(test_name + ".txt", std::ios_base::app);
            outfile << str;
            outfile.close();
            str = "";
        }
    }

    std::cout << test_name << " end" << std::endl;
}

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
        [save_all_match](auto &&item)
        {
            unsigned int th_n = std::get<0>(item);
            unsigned int start = std::get<1>(item);
            unsigned int stop = std::get<2>(item);
            unsigned int num_of_tests = stop - start;
            unsigned int giga_file = 1000 * th_n;

            std::ofstream of_match_link;
            std::string match_link_file = "match_link_#.txt";
            std::ostream_iterator<std::string> of_match_link_it(of_match_link, "\n");

            if (save_all_match)
            {
                of_match_link.open(utils::replace(match_link_file, "#", std::to_string(giga_file)) + ".txt", std::ios_base::out | std::ios_base::trunc);
                of_match_link.close();
                of_match_link.open(utils::replace(match_link_file, "#", std::to_string(giga_file)) + ".txt", std::ios_base::app);
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

                if (th_n == 0 && (i % 10000 == 0))
                    std::cout
                        << "         \r" << (double)i * 100.0 / (double)num_of_tests << "%\r"
                        << "\033[F" << std::endl;

                unsigned int s = (unsigned int)(matches.size());
                if (save_all_match && (s > 100000 || s % num_of_tests == 0))
                {
                    unsigned long int size = (unsigned long int)(of_match_link.tellp());
                    if (size > 1000000000)
                    {
                        of_match_link.close();
                        giga_file++;
                        of_match_link.open(utils::replace(match_link_file, "#", std::to_string(giga_file)) + ".txt", std::ios_base::out | std::ios_base::trunc);
                        of_match_link.close();
                        of_match_link.open(utils::replace(match_link_file, "#", std::to_string(giga_file)) + ".txt", std::ios_base::app);
                        of_match_link_it = std::ostream_iterator<std::string>(of_match_link, "\n");
                    }

                    std::copy(matches.begin(), matches.end(), of_match_link_it);
                    matches.clear();
                }
            }

            of_match_link.close();

            std::string match_duration_file = utils::replace("match_duration_#.txt", "#", std::to_string(th_n));
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

void test_real_match()
{
    std::string test_name = "test_real_match";
    std::cout << test_name << " start" << std::endl;
    std::vector<std::string> files = utils::get_files(".", std::regex("match_link_[0-9]+.txt"));

    std::for_each(
        std::execution::seq,
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
                    t.draw();
                }
            }
        });

    std::cout << test_name << " end" << std::endl;
}

int main()
{
    //test_av_cap(1000000);
    test_matches(100, true);
    test_real_match();
}