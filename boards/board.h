#pragma once

#include <vector>
#include <iostream>
#include <exception>
#include <algorithm>

// TBoard is the type of elements on the board
template <class TBoard>
class board
{
private:
    unsigned int *_sizes; // sizes is the dimensionality of the board ( a line has 1 value, a square has 2 values, a cube has 3, ...)
    unsigned int _d;      // dimension of the board (1 for line, 2 for square, 3 for cube, ...)
    TBoard *_board;

    bool _check(const std::vector<unsigned int> &coords, unsigned int &pos)
    {
        bool ok = true;
        if (coords.size() != _d)
            throw new std::out_of_range("the board has " + std::to_string(_d) + " dimensions, coordinates are only " + std::to_string(coords.size()));
        for (unsigned int i = 0; i < _d; i++)
            if (*(coords.begin() + i) >= *(_sizes + i))
                ok = false;

        pos = *coords.begin();
        for (unsigned int i = 1; i < coords.size(); i++)
            pos += *(_sizes + i - 1) * *(coords.begin() + i);

        return ok;
    }

public:
    // it is a list because you can create:
    // 1-D game or 1-game (snakes and ladder, goose game, D&D)
    // 2-D game or 2-game (chess, checkers, backgammon, go)
    // ...
    // n-D game or d-game (let me know if you know one!)
    board(const std::initializer_list<unsigned int> sizes)
    {
        _d = sizes.size();
        _sizes = new unsigned int[_d];
        std::copy(sizes.begin(), sizes.end(), _sizes);
        std::rotate(_sizes, _sizes + _d - 1, _sizes + _d);
        unsigned int _cells = 1;
        for (unsigned int i = 0; i < _d; i++)
            _cells *= *(_sizes + i);
        _board = new TBoard[_cells];
    }

    unsigned int *get_board_sizes()
    {
        return _sizes;
    }

    unsigned int get_board_d()
    {
        return _d;
    }

    // set a TBoard piece on the n-board by coordinates
    void set(const std::vector<unsigned int> &coords, const TBoard &p)
    {
        unsigned int pos;
        if (_check(coords, pos))
            _board[pos] = p;
        else
            throw new std::out_of_range("out_of_range");
    }

    // get a TBoard piece on the n-board by coordinates
    TBoard *get(const std::vector<unsigned int> &coords)
    {
        unsigned int pos;
        if (_check(coords, pos))
            return &_board[pos];
        return nullptr;
    }

    // setup the board
    void setup()
    {
        throw std::logic_error("setup not implemented");
    }

    // show the board
    void show()
    {
        throw std::logic_error("show not implemented");
    }

    // execute a complete game_turn on the board
    void game_turn()
    {
        throw std::logic_error("game_turn not implemented");
    }

    // execute a full game
    void play()
    {
        throw std::logic_error("play not implemented");
    }
};