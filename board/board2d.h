#pragma once

#include <iostream>
#include <exception>
#include <algorithm>

//#include "historyboard.h"
#include "chesspiece.h"

namespace board
{
    // board::chesspiece is the type of elements on the board
    class board2d
    {
    protected:
        // historyboard _history;

    private:
        unsigned int *_sizes;           // sizes is the dimensionality of the board (a line has 1 value, a square has 2 values, a cube has 3, ...)
        const unsigned int _dimensions; // dimension of the board (1 for line, 2 for square, 3 for cube, ...): it is the length of *_sizes
        unsigned int _cells;            // total number of cells
        // board::chesspiece _board[64];
        std::vector<board::chesspiece> _board;

        // check if the position is correct, if it is the correct value is pos
        bool _check_pos(const unsigned int &x, const unsigned int &y, unsigned int &pos)
        {
            if (x > *_sizes - 1 || y > *(_sizes + 1) - 1)
                return false;

            pos = x + y * *_sizes;
            if (pos > _cells - 1)
                return false;

            return true;
        }

    public:
#pragma region CONSTRUCTOR_GETTER_SETTER

        // it is a list because you can create:
        // 1-D game or 1-game (snakes and ladder, goose game, D&D)
        // 2-D game or 2-game (chess, checkers, backgammon, go)
        // ...
        // n-D game or d-game (let me know if you know one!)
        board2d(const unsigned int &x, const unsigned int &y) : _dimensions(2)
        {
            /*
            _dimensions = sizes.size();
            _sizes = new unsigned int[_dimensions];
            std::copy(sizes.begin(), sizes.end(), _sizes);
            std::rotate(_sizes, _sizes + _dimensions - 1, _sizes + _dimensions);
            _cells = 1;
            for (unsigned int i = 0; i < _dimensions; i++)
                _cells *= *(_sizes + i);
            _board = new board::chesspiece[_cells];
            */

            _sizes = new unsigned int[_dimensions];
            _sizes[0] = y;
            _sizes[1] = x;
            _cells = x * y;
            //_board = new board::chesspiece[_cells];
            _board = std::vector<board::chesspiece>(_cells);
        }

        ~board2d()
        {
            delete[] _sizes;
            // delete[] _board;
        }

        unsigned int *get_board_sizes()
        {
            return _sizes;
        }

        unsigned int get_board_dimensions()
        {
            return _dimensions;
        }

        // set a board::chesspiece piece on the n-board by coordinates
        void set(const unsigned int &x, const unsigned int &y, const board::chesspiece &p)
        {
            unsigned int pos;
            if (_check_pos(x, y, pos))
                _board[pos] = p;
            else
                throw std::logic_error("error on setting value");
        }

        // get a board::chesspiece piece on the n-board by coordinates
        // true is inside the board
        // false is outside the board
        // if true, then piece is nullptr if nothing found on that coords
        // if true and piece is not nullptr, then get return the piece of that coords
        bool get(const unsigned int &x, const unsigned int &y, board::chesspiece &piece)
        {
            unsigned int pos;
            if (_check_pos(x, y, pos))
            {
                // piece = *(_board + pos);
                piece = _board[pos];
                return true;
            }

            return false;
        }

        // historyboard &get_history()
        // {
        //     return _history;
        // }

#pragma endregion CONSTRUCTOR_GETTER_SETTER

#pragma region LOGIC

        // setup the board
        void setup_board()
        {
            throw std::logic_error("setup not implemented");
        }

        // show the board
        // TODO multidimensional show
        void show()
        {
            std::cout << "---- START -----" << std::endl;
            board::chesspiece p;
            for (unsigned int x = 0; x < *(get_board_sizes() + 1); x++)
            {
                for (unsigned int y = 0; y < *get_board_sizes(); y++)
                {
                    get(y, *(get_board_sizes() + 1) - x - 1, p);
                    std::cout << p.to_string() << " ";
                }
                std::cout << std::endl;
            }
            std::cout << "---- STOP ----" << std::endl;
        }

        // execute a single move
        void single_move(const movement2d &mov)
        {
            board::chesspiece p;
            get(mov.from_x, mov.from_y, p);
            set(mov.to_x, mov.to_y, p);
            set(mov.from_x, mov.from_y, board::chesspiece());
#ifdef OUTPUT
            show();
#endif
            _history.add(mov, _board);
        }

#pragma endregion LOGIC

#pragma region NORMAL_GAME

        // execute a complete game_turn on the board
        bool game_turn(board::chesspiece &winner)
        {
            throw std::logic_error("game_turn not implemented");
        }

        // execute a full game. play is a sequence of game_turn
        void play(board::chesspiece &winner)
        {
            throw std::logic_error("play not implemented");
        }

#pragma endregion NORMAL_GAME

#pragma region RANDOM_GAME

        // execute a complete random game_turn on the board
        bool game_turn_random(board::chesspiece &winner)
        {
            throw std::logic_error("game_turn_random not implemented");
        }

        // execute a full game randomly
        void play_random(board::chesspiece &winner)
        {
            throw std::logic_error("random_play not implemented");
        }

#pragma endregion RANDOM_GAME
    };
}