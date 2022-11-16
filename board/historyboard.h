#pragma once

#include <bitset>
#include <vector>
#include <fstream>
#include <iostream>

#include "movement.h"

#define MAX_BITSET 10000

namespace board
{
    class historyboard
    {
    private:
        unsigned int _size = 0;
        std::bitset<MAX_BITSET> _history;
        std::vector<std::string> _plays;

    public:
        void start()
        {
            _size = 0;
            _history.reset();
        }

        void add(const movement &mov)
        {
            for (unsigned int i = 0; i < mov.from.size(); i++)
            {
                _size += 3;
                _history <<= 3;
                _history |= std::bitset<MAX_BITSET>(mov.from[i]);
            }

            for (unsigned int i = 0; i < mov.to.size(); i++)
            {
                _size += 3;
                _history <<= 3;
                _history |= std::bitset<MAX_BITSET>(mov.to[i]);
            }
        }

        std::string to_string()
        {
            std::string str = "";
            std::bitset<6> single_char;
            for (unsigned int h = 0; h < _size; h += single_char.size())
            {
                for (unsigned int i = 0; i < single_char.size(); i++)
                {
                    if ((int)_size - 1 - (int)h - (int)i < 0)
                        single_char.set(single_char.size() - 1 - i, false);
                    else
                        single_char.set(single_char.size() - 1 - i, _history[_size - 1 - h - i]);
                }

                str += (unsigned char)(32 + single_char.to_ullong());
            }

            return str;
        }

        void stop()
        {
            _plays.push_back(to_string()); // TODO - improve performances
        }

        void save(const std::string &file_name)
        {
            std::ofstream output;
            output.open(file_name, std::ios_base::out | std::ios_base::binary);
            if (output.is_open())
            {
                for (unsigned int i = 0; i < _plays.size(); i++)
                    output << _plays[i] << std::endl;
                output.close();
            }
        }

        static std::vector<movement> from_history(const std::string &str_hist)
        {
            // the structure is 00xxxyyy|00xxxyyy|00xxxyyy|00xxxyyy
            std::vector<movement> movs;
            for (unsigned int i = 0; i < str_hist.size(); i += 4)
            {
                std::bitset<32> move((unsigned char)str_hist[i]);
                move <<= 8;
                move |= std::bitset<32>((unsigned char)str_hist[i + 1]);
                move <<= 8;
                move |= std::bitset<32>((unsigned char)str_hist[i + 2]);
                move <<= 8;
                move |= std::bitset<32>((unsigned char)str_hist[i + 3]);

                std::cout << move.to_string() << std::endl;

                std::vector<unsigned int> white_from;
                white_from.push_back(move[27] + move[28] * 2 + move[29] * 4); // x
                white_from.push_back(move[24] + move[25] * 2 + move[26] * 4); // y
                std::vector<unsigned int> white_to;
                white_to.push_back(move[19] + move[20] * 2 + move[21] * 4); // x
                white_to.push_back(move[16] + move[17] * 2 + move[18] * 4); // y
                if (white_from[0] == 0 && white_from[1] == 0 && white_to[0] == 0 && white_to[1] == 0)
                    break;
                else
                    movs.push_back(movement(white_from, white_to));

                std::vector<unsigned int> black_from;
                black_from.push_back(move[11] + move[12] * 2 + move[13] * 4); // x
                black_from.push_back(move[8] + move[9] * 2 + move[10] * 4);   // y
                std::vector<unsigned int> black_to;
                black_to.push_back(move[3] + move[4] * 2 + move[5] * 4); // x
                black_to.push_back(move[0] + move[1] * 2 + move[2] * 4); // y

                if (black_from[0] == 0 && black_from[1] == 0 && black_to[0] == 0 && black_to[1] == 0)
                    break;
                else
                    movs.push_back(movement(black_from, black_to));
            }

            return movs;
        }

        static std::vector<std::vector<movement>> from_file(const std::string &file_name)
        {
            std::vector<std::vector<movement>> movs;
            std::ifstream input(file_name);
            for (std::string line; getline(input, line);)
                if (line.size() > 0)
                    movs.push_back(from_history(line));
            input.close();

            return movs;
        }
    };
}