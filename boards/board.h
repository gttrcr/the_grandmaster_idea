#pragma once

#include <iostream>
#include <exception>

// TBoard is the type of elements on the board
template <class TBoard>
class board
{
private:
    unsigned int *_sizes;
    unsigned int _cells;
    TBoard *_board;

public:
    board(const std::initializer_list<unsigned int> sizes)
    {
        _sizes = new unsigned int[sizes.size()];
        std::copy(sizes.begin(), sizes.end(), _sizes);
        std::rotate(_sizes, _sizes + sizes.size() - 1, _sizes + sizes.size());
        _cells = 1;
        for (unsigned int i = 0; i < sizes.size(); i++)
            _cells *= *(_sizes + i);
        _board = new TBoard[_cells];
    }

    unsigned int *get_board_dims()
    {
        return _sizes;
    }

    void set(unsigned int x, unsigned int y, TBoard p)
    {
        if (x >= *_sizes || y >= *(_sizes + 1))
            throw new std::out_of_range("out_of_range position x:" + std::to_string(x) + " y:" + std::to_string(y));
        _board[*_sizes * y + x] = p;
    }

    TBoard *get(unsigned int x, unsigned int y)
    {
        if (x >= *_sizes || y >= *(_sizes + 1))
            return nullptr;
        return &_board[*_sizes * y + x];
    }

    void setup()
    {
        throw std::logic_error("setup not implemented");
    }

    void show()
    {
        throw std::logic_error("show not implemented");
    }

    void move()
    {
        throw std::logic_error("mode not implemented");
    }
};