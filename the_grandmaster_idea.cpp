#include <ctime>
#include <chrono>

#include "boards/chessboard.h"

#include <algorithm>
#include <thread>
#include <functional>
#include <vector>
#include <execution>
#include <tuple>

void simulation(const std::string &simulation_name, const std::vector<unsigned int> &elems)
{
	chessboard ch;
	chesspiece winner;
	for (unsigned int i = 0; i < elems.size(); i++)
	{
		ch.setup();
		ch.play_random(winner);
	}

	ch.save(simulation_name + ".tgi");
}

void simulations(unsigned int n_sims)
{
	const uint32_t thread_nums = std::thread::hardware_concurrency();
	std::vector<std::tuple<unsigned int, std::vector<unsigned int>>> foo;
	for (unsigned int t = 0; t < thread_nums; t++)
	{
		std::vector<unsigned int> th;
		for (unsigned int i = 0; i < n_sims / thread_nums; i++)
			th.push_back(i);
		foo.push_back(std::make_tuple(t, th));
	}

	std::string timestamp = std::to_string(std::time(nullptr));
	auto start = std::chrono::high_resolution_clock::now();
	std::for_each(
		std::execution::par,
		foo.begin(),
		foo.end(),
		[&timestamp](std::tuple<unsigned int, std::vector<unsigned int>> &item)
		{
			simulation(timestamp + "#" + std::to_string(std::get<0>(item)), std::get<1>(item));
		});

	std::cout << (float)(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count()) / n_sims << std::endl;
}

int main(int argc, char *argv[])
{
	simulations(100000);
}