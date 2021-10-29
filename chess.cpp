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

#define THREADS 8

#include "tests/test_av_cap.h"
#include "tests/test_matches.h"
#include "tests/test_real_match.h"

int main()
{
    //test_av_cap(1000000);
    test_matches(10000000, true);
    //test_real_match();
}