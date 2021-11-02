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
#include "tests/test_real_match.h"

void show_help()
{

}

int main(unsigned int argc, char* argv[])
{
    if (argc == 1)
        show_help();

    //set global execution variable
    for (unsigned int i = 1; i < argc; i++)
    {
        std::string cmd = argv[i];
        //set the number of threads
        if (cmd == "-t")
            global_threads = std::stoi(cmd);
        if (cmd == "-h")
            show_help();
    }

    std::cout << "Threads " << global_threads << std::endl;

    //run tests
    for (unsigned int i = 1; i < argc; i++)
    {
        std::string cmd = argv[i];
        if (cmd == "-1")
        {
            unsigned int max_test = std::stoi(argv[++i]);
            test_1_av_cap(max_test);
        }
        if (cmd == "-2")
        {
            unsigned int max_test = std::stoi(argv[++i]);
            bool save_all_match = false;
            if (++i < argc)
                save_all_match = (argv[i][0] == '1');
            test_2_matches(max_test, save_all_match);
        }
        if (cmd == "-3")
        {
            test_3_real_match();
        }
    }
}