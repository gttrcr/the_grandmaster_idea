#include "../chess/chessboard.h"

#include <thread>
#include <tbb/parallel_for.h>

namespace simulation
{
    class chess
    {
    public:
        static void simulation(const std::string &simulation_name, const unsigned int &n_sims, const bool &save)
        {
            board::chessboard ch;
            board::chesspiece winner;
            for (unsigned int i = 0; i < n_sims; i++)
            {
                ch.setup_board();
                ch.play_random(winner);
            }

            if (save)
                ch.get_history().save(simulation_name + ".tgi");
        }

        static void simulations(const unsigned int n_sims, const bool &save)
        {
            const uint32_t thread_nums = std::thread::hardware_concurrency();
            std::string timestamp = std::to_string(std::time(nullptr));
            tbb::parallel_for(tbb::blocked_range<unsigned int>(0, n_sims, n_sims / thread_nums),
                              [&timestamp, &save](tbb::blocked_range<unsigned int> r)
                              {
                                  simulation(timestamp + "#" + std::to_string(r.begin()), r.end() - r.begin(), save);
                              });
        }
    };
}