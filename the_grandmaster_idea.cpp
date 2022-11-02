#define _CRT_SECURE_NO_WARNINGS

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
#include <map>

#define BITSET_SIZE 14
#define COMPRESSION_OFFSET 7
#define MAX_BITSET_MATCH_SIZE 300 * BITSET_SIZE
#define MAX_FILE_SIZE_BYTE 1000000000
#define FILE_EXT ".chess"

unsigned int global_threads = std::thread::hardware_concurrency();

//#include "rnd.h"
//#include "utils.h"
//#include "position.h"
//#include "table.h"
//#include "tests/test_av_cap.h"
//#include "tests/test_matches.h"
//#include "tests/test_uniqueness.h"
//#include "tests/test_real_match.h"
//#include "tests/test_total_illegal_positions.h"
//#include "tests/NxN_super_game.h"

#include "boards/chessboard.h"

void show_help()
{
	std::cout << "example" << std::endl
			  << "[-1 max_test][-2 max_test [0-1]][-3][-4 configuration]" << std::endl
			  << "[-5 'default' n_pw n_rw n_nw n_bw n_kw n_qw n_pb n_rb n_nb n_bb n_kb n_qb][-t num_th]" << std::endl
			  << "[-6 N]" << std::endl;
	std::cout << "-1 execute max_test tests on random board to estimate the number of pieces that are captured and the number of possible positions for each piece" << std::endl;
	std::cout << "-2 execute max_test games saving them" << std::endl;
	std::cout << "-3 check the uniqueness of games generated by test -2" << std::endl;
	std::cout << "-4 make a move from a given configuration" << std::endl;
	std::cout << "-5 calculate the total number of positions (legal + illegal)" << std::endl;
	std::cout << "-6 create a board made of NxN chessboards and simulate the all games, then save all matrices as .mp4 video";
	std::cout << "-t set the number of threads to use, default is " << global_threads << std::endl;
}

using cmd_map_type = std::map<std::string, std::vector<std::string>>;
cmd_map_type produce_commands(int argc, char *argv[])
{
	cmd_map_type ret;
	for (unsigned int i = 1; i < (unsigned int)argc; i++)
	{
		std::string cmd = argv[i];
		std::vector<std::string> list;
		for (; i < (unsigned int)argc; i++)
			if (argv[i][0] != '-')
				list.push_back(argv[i]);

		ret[cmd] = list;
	}

	return ret;
}

int main(int argc, char *argv[])
{
	chessboard ch;
	ch.setup();
	unsigned int i = 0;
	while (true)
	{
		ch.show();
		ch.move();
	}

	/*
		if (argc == 1)
			show_help();

		// create a map of commands
		cmd_map_type commands = produce_commands(argc, argv);

		// set global execution variable
		for (cmd_map_type::iterator it = commands.begin(); it != commands.end(); ++it)
		{
			std::string cmd = it->first;
			std::vector<std::string> args = it->second;
			if (cmd == "-t" && args.size() == 1)
				global_threads = std::stoi(args[0]);
			if (cmd == "-h" && args.size() == 0)
				show_help();
		}

		// tests
		for (cmd_map_type::iterator it = commands.begin(); it != commands.end(); ++it)
		{
			std::string cmd = it->first;
			std::vector<std::string> args = it->second;

			if (cmd == "-1")
			{
				if (args.size() == 1 && utils::is_number(args[0]))
					test_1::test_1_av_cap(std::stoi(args[0]));
				else
					std::cout << cmd << " malformed command" << std::endl;
			}

			if (cmd == "-2")
			{
				if (args.size() == 1 && utils::is_number(args[0]))
					test_2::test_2_matches(std::stoi(args[0]));
				else
					std::cout << cmd << " malformed command" << std::endl;
			}

			if (cmd == "-3")
			{
				if (args.size() == 0)
					test_3::test_3_uniqueness();
				else
					std::cout << cmd << " malformed command" << std::endl;
			}

			if (cmd == "-4")
			{
	#ifdef __linux__
				if (args.size() == 1 && utils::is_number(args[0]))
				{
					if (std::stoi(args[0]) == 1)
						test_4::test_4_real_match(args[0], test_4::engine::stockfish);
					else if (std::stoi(args[0]) == 0)
						test_4::test_4_real_match(args[0], test_4::engine::the_grandmaster_idea);
					else
						std::cout << cmd << " malformed command" << std::endl;
				}
				else
					std::cout << cmd << " malformed command" << std::endl;
	#elif _WIN32
				std::cout << cmd << " this option is available only for linux" << std::endl;
	#endif
			}

			if (cmd == "-5")
			{
				if (args.size() == 1 && args[0] == "default")
					test_5::test_5_total_illegal_positions_integral();
				else if (args.size() == 14 && std::all_of(args.begin(), args.end(), [](std::string &s)
														  { return utils::is_number(s); }))
				{
					if (args[0] == "1" && test_5::check_input(std::vector<std::string>(args.begin() + 1, args.begin() + 14)))
						test_5::test_5_total_illegal_positions(std::stoi(args[1]), std::stoi(args[2]), std::stoi(args[3]),
															   std::stoi(args[4]), std::stoi(args[5]), std::stoi(args[6]),
															   std::stoi(args[7]), std::stoi(args[8]), std::stoi(args[9]),
															   std::stoi(args[10]), std::stoi(args[11]), std::stoi(args[12]),
															   std::stoi(args[13]));
					else if (args[0] == "2" && test_5::check_input(std::vector<std::string>(args.begin() + 1, args.begin() + 14)))
						test_5::test_5_total_illegal_positions_integral(std::stoi(args[1]), std::stoi(args[2]), std::stoi(args[3]),
																		std::stoi(args[4]), std::stoi(args[5]), std::stoi(args[6]),
																		std::stoi(args[7]), std::stoi(args[8]), std::stoi(args[9]),
																		std::stoi(args[10]), std::stoi(args[11]), std::stoi(args[12]),
																		std::stoi(args[13]));
					else
						std::cout << cmd << " malformed command" << std::endl;
				}
				else
					std::cout << cmd << " malformed command" << std::endl;
			}

			if (cmd == "-6")
			{
				if (args.size() == 1 && utils::is_number(args[0]))
					test_6::test_6_NxN_super_game(std::stoi(args[0]));
				else
					std::cout << cmd << " malformed command" << std::endl;
			}
		}
		*/
}