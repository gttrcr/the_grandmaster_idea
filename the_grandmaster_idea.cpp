#include "simulation/chesssimulation.h"

#include <chrono>

int main(int argc, char *argv[])
{
	unsigned int i = 1000000;
	auto start = std::chrono::high_resolution_clock::now();
	simulation::chess::simulations(i, true);
	std::cout << (float)(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count()) / i << std::endl;
}