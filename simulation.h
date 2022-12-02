#include "chess/chessboard.h"

#include <thread>
//#include <tbb/parallel_for_each.h>

#include <execution>
#include <algorithm>
#include <functional>

namespace simulation
{
    static inline void simulation(const std::string &simulation_name, const unsigned int &n_sims, const bool &save)
    {
        chess::chessboard ch;
        chess::chesspiece::color winner;
        for (unsigned int i = 0; i < n_sims; i++)
        {
            ch.setup_board();
            ch.play_random(winner);
#ifdef OUTPUT
            std::cout << winner << std::endl;
#endif
        }

        if (save)
            ch.get_history().save(simulation_name + ".tgi");
    }

    static inline void simulations(const unsigned int n_sims, const bool &save)
    {
        const uint32_t thread_nums = std::thread::hardware_concurrency();
        std::vector<std::tuple<unsigned int, unsigned int>> blocks;
        for (unsigned int i = 0; i < thread_nums; i++)
            blocks.push_back(std::make_tuple(i * n_sims / thread_nums, (i + 1) * n_sims / thread_nums));

        std::string timestamp = std::to_string(std::time(nullptr));
        std::for_each(std::execution::par, blocks.begin(), blocks.end(), [&timestamp, &save](std::tuple<unsigned int, unsigned int> &block)
                      { simulation(timestamp + "#" + std::to_string(std::get<0>(block)), std::get<1>(block) - std::get<0>(block), save); });
    }
}
