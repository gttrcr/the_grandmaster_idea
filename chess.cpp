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

#define DEFAULT_THREADS 4
#define BITSET_SIZE 14
#define COMPRESSION_OFFSET 7
#define MAX_BITSET_MATCH_SIZE 300 * BITSET_SIZE
#define MAX_FILE_SIZE_BYTE 1000000000
#define FILE_EXT ".chess"

unsigned int global_threads = DEFAULT_THREADS;

#include "utils.h"
#include "position.h"
#include "piece.h"
#include "table.h"
#include "tests/test_av_cap.h"
#include "tests/test_matches.h"
#include "tests/test_uniqueness.h"
#include "tests/test_real_match.h"

void show_help()
{
    std::cout << "[-1 max_test][-2 max_test [0-1]][-3][-4 configuration][-t num_th]" << std::endl;
    std::cout << "-1 execute max_test tests on random board to estimate the number of pieces that are captured and the number of possible positions for each piece" << std::endl;
    std::cout << "-2 execute max_test games saving them if [0-1] is 1 or not if [0-1] is 0 on.This allows to simulate an entire game" << std::endl;
    std::cout << "-3 check the uniqueness of each game generated by test -2" << std::endl;
    std::cout << "-4 make a move from a configuration" << std::endl;
    std::cout << "-t set the number of threads to use, default is " << DEFAULT_THREADS << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
        show_help();

    //set global execution variable
    for (unsigned int i = 1; i < argc; i++)
    {
        std::string cmd = argv[i];
        if (cmd == "-t")
            global_threads = std::stoi(argv[++i]);
        if (cmd == "-h")
            show_help();
    }

    //run tests
    for (unsigned int i = 1; i < argc; i++)
    {
        std::string cmd = argv[i];
        if (cmd == "-1")
            test_1_av_cap(std::stoi(argv[++i]));
        if (cmd == "-2")
        {
            unsigned int max_test = std::stoi(argv[++i]);
            bool save_all_match = false;
            if (++i < argc)
                save_all_match = (argv[i][0] == '1');
            test_2_matches(max_test, save_all_match);
        }
        if (cmd == "-3")
            test_3_uniqueness();
        if (cmd == "-4")
            test_4_real_match("Q }0");
    }
}