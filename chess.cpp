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

#define THREADS 1
#define BITSET_SIZE 14
#define MAX_BITSET_MATCH_SIZE 5000
//#define PERF 1 // PERFORMANCE (1)<-->SIZE (2)

#include "utils.h"
#include "position.h"
#include "piece.h"
#include "table.h"
#include "tests/test_av_cap.h"
#include "tests/test_matches.h"
#include "tests/test_real_match.h"

int main(int argc, char* argv[])
{
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