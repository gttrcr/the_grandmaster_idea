#pragma once

#include <bitset>
#include <vector>
#include <fstream>
#include <iostream>
#include <bits/stdc++.h>

#include "movement2d.h"
#include "chesspiece.h"

#define MAX_BITSET 10000

namespace board
{
    class historyboard
    {
    private:
        unsigned int _size = 0;
        std::bitset<MAX_BITSET> _history;
        std::vector<std::string> _plays;
        std::map<std::string, unsigned int> _boards;
        const std::bitset<MAX_BITSET> _filter = std::bitset<MAX_BITSET>(63);
        bool _triple;

        std::string _to_string()
        {
            std::string str = "";
            std::bitset<MAX_BITSET> single_char;
            for (unsigned int i = _size; i > 0; i--)
            {
                single_char = _history >> 6 * (i - 1);
                str += (unsigned char)(32 + single_char.to_ullong());
                _history = _history & (_filter << 6 * (i - 1)).flip();
            }

            return str;
        }

        std::string _hash(std::vector<board::chesspiece> &board)
        {
            std::string hash = "";
            for (unsigned int i = 0; i < board.size(); i++)
                hash += board[i].to_string();

            return hash;
        }

    public:
        void start()
        {
            _size = 0;
            _triple = false;
            _history.reset();
            _boards.clear();
        }

        void add(const movement2d &mov, std::vector<board::chesspiece> board)
        {
            // for (unsigned int i = 0; i < mov.from.size(); i++)
            //{
            //     _size += 3;
            //     _history <<= 3;
            //     _history |= std::bitset<MAX_BITSET>(mov.from[i]);
            // }
            //
            // for (unsigned int i = 0; i < mov.to.size(); i++)
            //{
            //    _size += 3;
            //    _history <<= 3;
            //    _history |= std::bitset<MAX_BITSET>(mov.to[i]);
            //}

            std::string hash = _hash(board);
            _boards[hash]++;
            if (_boards[hash] == 3)
                _triple = true;

            //_size += 3;
            _history <<= 3;
            _history |= std::bitset<MAX_BITSET>(mov.from_x);
            //_size += 3;
            _history <<= 3;
            _history |= std::bitset<MAX_BITSET>(mov.from_y);
            //_size += 3;
            _history <<= 3;
            _history |= std::bitset<MAX_BITSET>(mov.to_x);
            _size += 2;
            _history <<= 3;
            _history |= std::bitset<MAX_BITSET>(mov.to_y);
        }

        void stop()
        {
            _plays.push_back(_to_string());
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

        bool triple()
        {
            return _triple;
        }

        static inline std::vector<movement2d> from_history(const std::string &str_hist)
        {
            // the structure is 00xxxyyy|00xxxyyy|00xxxyyy|00xxxyyy
            std::vector<movement2d> movs;
            for (unsigned int i = 0; i < str_hist.size(); i += 4)
            {
                std::bitset<24> move((unsigned char)str_hist[i] - 32);
                move <<= 6;
                move |= std::bitset<24>((unsigned char)str_hist[i + 1] - 32);
                move <<= 6;
                move |= std::bitset<24>((unsigned char)str_hist[i + 2] - 32);
                move <<= 6;
                move |= std::bitset<24>((unsigned char)str_hist[i + 3] - 32);

                unsigned int white_from_x = move[21] + move[22] * 2 + move[23] * 4; // x
                unsigned int white_from_y = move[18] + move[19] * 2 + move[20] * 4; // y
                unsigned int white_to_x = move[15] + move[16] * 2 + move[17] * 4;   // x
                unsigned int white_to_y = move[12] + move[13] * 2 + move[14] * 4;   // y

                // if (white_from[0] == 0 && white_from[1] == 0 && white_to[0] == 0 && white_to[1] == 0)
                //     throw std::exception();
                // else
                movs.push_back(movement2d(white_from_x, white_from_y, white_to_x, white_to_y));

                unsigned int black_from_x = move[9] + move[10] * 2 + move[11] * 4; // x
                unsigned int black_from_y = move[6] + move[7] * 2 + move[8] * 4;   // y
                unsigned int black_to_x = move[3] + move[4] * 2 + move[5] * 4;     // x
                unsigned int black_to_y = move[0] + move[1] * 2 + move[2] * 4;     // y

                // if (black_from[0] == 0 && black_from[1] == 0 && black_to[0] == 0 && black_to[1] == 0)
                //     throw std::exception();
                // else
                movs.push_back(movement2d(black_from_x, black_from_y, black_to_x, black_to_y));
            }

            return movs;
        }

        static inline std::vector<std::vector<movement2d>> from_file(const std::string &file_name)
        {
            std::vector<std::vector<movement2d>> movs;
            std::ifstream input(file_name);
            for (std::string line; getline(input, line);)
                if (line.size() > 0)
                    movs.push_back(from_history(line));
            input.close();

            return movs;
        }
    };
}