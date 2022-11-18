#pragma once

#include <bitset>
#include <iostream>
#include <string>

#include "../board/board2d.h"
#include "chesspiece.h"
#include "../rnd.h"

namespace board
{
    class chessboard : public virtual board2d<chesspiece>
    {
    private:
        // provides the list of positions of a single piece by coordinate
        void _piece_available_positions(const unsigned int x, const unsigned int y, std::vector<movement2d> &positions)
        {
            chesspiece target;
            chesspiece p;
            get(x, y, target);
            switch (target.get_value())
            {
            case chesspiece::value::pawn:
            {
                int dir = 2 * (target.get_color() == chesspiece::color::white) - 1; // 1 for white, -1 for black
                if (get(x, y + dir, p) && p.is_empty())
                    positions.push_back(movement2d(x, y, x, y + dir)); // if the cell is empty, can move but nothing to capture
                if (get(x - 1, y + dir, p) && !p.is_empty() && target.get_color() == p.get_opponent_color())
                    positions.push_back(movement2d(x, y, x - 1, y + dir)); // if the cell is not empty, can move and capture
                if (get(x + 1, y + dir, p) && !p.is_empty() && target.get_color() == p.get_opponent_color())
                    positions.push_back(movement2d(x, y, x + 1, y + dir)); // if the cell is not empty, can move and capture
                break;
                // EN-PASSENT
            }
            case chesspiece::value::rook:
            {
                break;
            }
            case chesspiece::value::knight:
            {
                break;
            }
            case chesspiece::value::bishop:
            {
                break;
            }
            case chesspiece::value::king:
            {
                break;
            }
            case chesspiece::value::queen:
            {
                break;
            }
            default:
                break;
            }
        }

        // provides the list of positions of pieces by color
        bool _available_positions(const chesspiece::color &color, std::vector<movement2d> &movs, const bool &first_random = false)
        {
            chesspiece p;
            for (unsigned int x = 0; x < *get_board_sizes(); x++)
                for (unsigned int y = 0; y < *(get_board_sizes() + 1); y++)
                    if (get(x, y, p) && p.get_color() == color)
                        _piece_available_positions(x, y, movs);

            return movs.size() > 0;
        }

        bool _random_available_position(const chesspiece::color &color, movement2d &mov)
        {
            std::vector<movement2d> movs;
            if (_available_positions(color, movs, true))
            {
                mov = movs[rnd::get(0, movs.size() - 1)];
                return true;
            }

            return false;
        }

    public:
        chessboard() : board2d<chesspiece>(8, 8)
        {
        }

        void setup_board()
        {
            set(0, 0, chesspiece(chesspiece::value::rook, chesspiece::color::white));
            set(1, 0, chesspiece(chesspiece::value::knight, chesspiece::color::white));
            set(2, 0, chesspiece(chesspiece::value::bishop, chesspiece::color::white));
            set(3, 0, chesspiece(chesspiece::value::queen, chesspiece::color::white));
            set(4, 0, chesspiece(chesspiece::value::king, chesspiece::color::white));
            set(5, 0, chesspiece(chesspiece::value::bishop, chesspiece::color::white));
            set(6, 0, chesspiece(chesspiece::value::knight, chesspiece::color::white));
            set(7, 0, chesspiece(chesspiece::value::rook, chesspiece::color::white));
            set(0, 1, chesspiece(chesspiece::value::pawn, chesspiece::color::white));
            set(1, 1, chesspiece(chesspiece::value::pawn, chesspiece::color::white));
            set(2, 1, chesspiece(chesspiece::value::pawn, chesspiece::color::white));
            set(3, 1, chesspiece(chesspiece::value::pawn, chesspiece::color::white));
            set(4, 1, chesspiece(chesspiece::value::pawn, chesspiece::color::white));
            set(5, 1, chesspiece(chesspiece::value::pawn, chesspiece::color::white));
            set(6, 1, chesspiece(chesspiece::value::pawn, chesspiece::color::white));
            set(7, 1, chesspiece(chesspiece::value::pawn, chesspiece::color::white));
            set(0, 6, chesspiece(chesspiece::value::pawn, chesspiece::color::black));
            set(1, 6, chesspiece(chesspiece::value::pawn, chesspiece::color::black));
            set(2, 6, chesspiece(chesspiece::value::pawn, chesspiece::color::black));
            set(3, 6, chesspiece(chesspiece::value::pawn, chesspiece::color::black));
            set(4, 6, chesspiece(chesspiece::value::pawn, chesspiece::color::black));
            set(5, 6, chesspiece(chesspiece::value::pawn, chesspiece::color::black));
            set(6, 6, chesspiece(chesspiece::value::pawn, chesspiece::color::black));
            set(7, 6, chesspiece(chesspiece::value::pawn, chesspiece::color::black));
            set(0, 7, chesspiece(chesspiece::value::rook, chesspiece::color::black));
            set(1, 7, chesspiece(chesspiece::value::knight, chesspiece::color::black));
            set(2, 7, chesspiece(chesspiece::value::bishop, chesspiece::color::black));
            set(3, 7, chesspiece(chesspiece::value::queen, chesspiece::color::black));
            set(4, 7, chesspiece(chesspiece::value::king, chesspiece::color::black));
            set(5, 7, chesspiece(chesspiece::value::bishop, chesspiece::color::black));
            set(6, 7, chesspiece(chesspiece::value::knight, chesspiece::color::black));
            set(7, 7, chesspiece(chesspiece::value::rook, chesspiece::color::black));

            for (unsigned int y = 2; y < 6; y++)
            {
                set(0, y, chesspiece());
                set(1, y, chesspiece());
                set(2, y, chesspiece());
                set(3, y, chesspiece());
                set(4, y, chesspiece());
                set(5, y, chesspiece());
                set(6, y, chesspiece());
                set(7, y, chesspiece());
            }
        }

#pragma region NORMAL_GAME

        // get the winner's color
        // TODO implement the winner search
        bool get_winner(chesspiece &winner)
        {
            return false;
        }

#pragma endregion NORMAL_GAME

#pragma region RANDOM_GAME

        // execute a complete random game_turn on the board
        bool game_turn_random(chesspiece &winner)
        {
            movement2d mov;
            if (_random_available_position(chesspiece::color::white, mov))
                single_move(mov);
            else
                return false;

            if (!get_winner(winner))
            {
                if (_random_available_position(chesspiece::color::black, mov))
                    single_move(mov);
                else
                    return false;
            }

            return true;
        }

        // execute a full game randomly
        void play_random(chesspiece &winner)
        {
            _history.start();
            while (game_turn_random(winner))
                ;
            _history.stop();
        }

#pragma endregion RANDOM_GAME
    };
}