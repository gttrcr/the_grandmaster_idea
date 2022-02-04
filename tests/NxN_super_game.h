#pragma once

namespace test_6
{
	void test_6_NxN_super_game(const unsigned int n)
	{
		unsigned int number_of_games = 10;
		std::vector<std::vector<std::string>> frames;
		for (unsigned int game = 0; game < number_of_games; game++)
		{
			std::vector<std::string> frame;
			for (unsigned int x_chess = 0; x_chess < n; x_chess++)
				for (unsigned int y_chess = 0; y_chess < n; y_chess++)
				{
					unsigned int count = 0;
					std::bitset<MAX_BITSET_MATCH_SIZE> match;
					piece::color win = table::play(count, match);
					frame.push_back(utils::from_bitset_to_matches_string(match, count));
				}
			frames.push_back(frame);
		}

		//append every game at the end of the previous game for the corresponding chessboard
		std::vector<std::vector<std::string>> story(pow(n, 2));
		for (unsigned int game = 0; game < number_of_games; game++)
		{
			for (unsigned int x_chess = 0; x_chess < n; x_chess++)
				for (unsigned int y_chess = 0; y_chess < n; y_chess++)
					story[x_chess + n * y_chess].push_back(frames[game][x_chess + n * y_chess]);
			frames[game].clear();
		}
		frames.clear();

		//create the story, frame by frame
		for (unsigned int x_chess = 0; x_chess < n; x_chess++)
			for (unsigned int y_chess = 0; y_chess < n; y_chess++)
			{
				std::bitset<MAX_BITSET_MATCH_SIZE> bit_set = utils::string_to_bitset(story[x_chess + n * y_chess][0]);
				table t;
				for (unsigned int i = 0; i < bit_set.size(); i += BITSET_SIZE)
				{
					unsigned char from = 0;
					unsigned char to = 0;
					for (unsigned int c = 0; c < COMPRESSION_OFFSET; c++)
					{
						from += (bit_set[i + c] << (COMPRESSION_OFFSET - c - 1));
						to += (bit_set[i + c + COMPRESSION_OFFSET - 1] << (COMPRESSION_OFFSET - c - 1));
					}
					from >>= 1;
					to >>= 1;

					t.move(position(from), position(to), false);
					t.draw();
				}
			}
	}
}