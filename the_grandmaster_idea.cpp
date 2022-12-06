#include "simulation.h"
#include "board/historyboard.h"

#include <chrono>
#include <vector>
#include "board/movement2d.h"

int main(int argc, char *argv[])
{
	// simulation::simulation("ciao", 1, true);

	unsigned int i = 1000000;
	simulation::simulations(i, true);
	// auto start = std::chrono::high_resolution_clock::now();
	// std::cout << (float)(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count()) / i << std::endl;
}
