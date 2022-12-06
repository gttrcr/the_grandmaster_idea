#include "simulation.h"

#include <chrono>
#include <vector>

int main(int argc, char *argv[])
{
	simulation::simulation("ciao", 1, true);

	// unsigned int i = 1000000;
	// auto start = std::chrono::high_resolution_clock::now();
	// simulation::simulations(i, true);
	// std::cout << (float)(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count()) / i << std::endl;
}
