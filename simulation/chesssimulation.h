#include "../board/chessboard.h"

#include <thread>
#include <vector>
#include <tbb/parallel_for.h>

namespace simulation
{
    class chess
    {
    public:
        static void simulation(const std::string &simulation_name, const unsigned int &n_sims)
        {
            board::chessboard ch;
            board::chesspiece winner;
            for (unsigned int i = 0; i < n_sims; i++)
            {
                ch.setup_board();
                ch.play_random(winner);
            }
        }

        static void simulations(unsigned int n_sims)
        {
            const uint32_t thread_nums = std::thread::hardware_concurrency();
            std::string timestamp = std::to_string(std::time(nullptr));

            auto start = std::chrono::high_resolution_clock::now();
            tbb::parallel_for(tbb::blocked_range<unsigned int>(0, n_sims, n_sims / thread_nums),
                              [&timestamp](tbb::blocked_range<unsigned int> r)
                              {
                                  simulation(timestamp + "#" + std::to_string(r.begin()), r.end() - r.begin());
                              });

            std::cout << (float)(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count()) / n_sims << std::endl;
        }
    };
}