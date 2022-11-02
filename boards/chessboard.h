#pragma once

#include "board.h"
#include "../rnd.h"

class chesspiece
{
public:
    enum value
    {
        empty_value = -1,
        pawn,
        rook,
        knight,
        bishop,
        king,
        queen
    };

    enum color
    {
        empty_color = -1,
        white,
        black
    };

private:
    value _value;
    color _color;

public:
    chesspiece(value value = value::empty_value, color color = color::empty_color)
    {
        _value = value;
        _color = color;
    }

    chesspiece::value get_value()
    {
        return _value;
    }

    chesspiece::color get_color()
    {
        return _color;
    }

    bool is_empty()
    {
        return _color == color::empty_color && _value == value::empty_value;
    }

    std::string to_string()
    {
        std::string ret = "";
        switch (_color)
        {
        case chesspiece::color::white:
            ret += "W";
            break;
        case chesspiece::color::black:
            ret += "B";
        default:
            break;
        }

        switch (_value)
        {
        case chesspiece::value::pawn:
            ret += "P";
            break;
        case chesspiece::value::rook:
            ret += "R";
            break;
        case chesspiece::value::knight:
            ret += "k";
            break;
        case chesspiece::value::bishop:
            ret += "B";
            break;
        case chesspiece::value::king:
            ret += "K";
            break;
        case chesspiece::value::queen:
            ret += "Q";
            break;
        default:
            break;
        }

        return ret == "" ? "--" : ret;
    }

    bool operator==(chesspiece &p)
    {
        return _color == p._color && _value == p._value;
    }

    bool operator!=(chesspiece &p)
    {
        return !(*this == p);
    }
};

class chessboardhistory
{
private:
    std::vector<unsigned int> _history;

public:
    chessboardhistory()
    {
    }

    void add()
    {
    }
};

struct _movement
{
public:
    std::initializer_list<unsigned int> from;
    std::initializer_list<unsigned int> to;
    bool capture;

    _movement(const std::initializer_list<unsigned int> &_from, const std::initializer_list<unsigned int> &_to, bool _capture)
    {
        from = _from;
        to = _to;
        capture = _capture;
    }
};

class chessboard : public virtual board<chesspiece>
{
private:
    // provides the list of positions where pieces can go
    void _available_positions(unsigned int x, unsigned int y, std::vector<_movement> &positions)
    {
        chesspiece *target = get({x, y});
        switch (target->get_value())
        {
        case chesspiece::value::pawn:
        {
            chesspiece *p;
            int dir = 2 * (get({x, y})->get_color() == chesspiece::color::white) - 1; // 1 for white, -1 for black
            if (get({x, y + dir})->is_empty())
                positions.push_back(_movement({x, y}, {x, y + dir}, false)); // if the cell is empty, can move but nothing to capture
            if ((p = get({x - 1, y + dir})) != nullptr && !p->is_empty() && target->get_color() != p->get_color())
                positions.push_back(_movement({x, y}, {x - 1, y + dir}, true)); // if the cell is not empty, can move and capture
            if ((p = get({x + 1, y + dir})) != nullptr && !p->is_empty() && target->get_color() != p->get_color())
                positions.push_back(_movement({x, y}, {x + 1, y + dir}, true)); // if the cell is not empty, can move and capture
            break;
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

public:
    chessboard() : board<chesspiece>({8, 8})
    {
    }

    void setup()
    {
        set({0, 0}, chesspiece(chesspiece::value::rook, chesspiece::color::white));
        set({1, 0}, chesspiece(chesspiece::value::knight, chesspiece::color::white));
        set({2, 0}, chesspiece(chesspiece::value::bishop, chesspiece::color::white));
        set({3, 0}, chesspiece(chesspiece::value::queen, chesspiece::color::white));
        set({4, 0}, chesspiece(chesspiece::value::king, chesspiece::color::white));
        set({5, 0}, chesspiece(chesspiece::value::bishop, chesspiece::color::white));
        set({6, 0}, chesspiece(chesspiece::value::knight, chesspiece::color::white));
        set({7, 0}, chesspiece(chesspiece::value::rook, chesspiece::color::white));
        set({0, 1}, chesspiece(chesspiece::value::pawn, chesspiece::color::white));
        set({1, 1}, chesspiece(chesspiece::value::pawn, chesspiece::color::white));
        set({2, 1}, chesspiece(chesspiece::value::pawn, chesspiece::color::white));
        set({3, 1}, chesspiece(chesspiece::value::pawn, chesspiece::color::white));
        set({4, 1}, chesspiece(chesspiece::value::pawn, chesspiece::color::white));
        set({5, 1}, chesspiece(chesspiece::value::pawn, chesspiece::color::white));
        set({6, 1}, chesspiece(chesspiece::value::pawn, chesspiece::color::white));
        set({7, 1}, chesspiece(chesspiece::value::pawn, chesspiece::color::white));
        set({0, 6}, chesspiece(chesspiece::value::pawn, chesspiece::color::black));
        set({1, 6}, chesspiece(chesspiece::value::pawn, chesspiece::color::black));
        set({2, 6}, chesspiece(chesspiece::value::pawn, chesspiece::color::black));
        set({3, 6}, chesspiece(chesspiece::value::pawn, chesspiece::color::black));
        set({4, 6}, chesspiece(chesspiece::value::pawn, chesspiece::color::black));
        set({5, 6}, chesspiece(chesspiece::value::pawn, chesspiece::color::black));
        set({6, 6}, chesspiece(chesspiece::value::pawn, chesspiece::color::black));
        set({7, 6}, chesspiece(chesspiece::value::pawn, chesspiece::color::black));
        set({0, 7}, chesspiece(chesspiece::value::rook, chesspiece::color::black));
        set({1, 7}, chesspiece(chesspiece::value::knight, chesspiece::color::black));
        set({2, 7}, chesspiece(chesspiece::value::bishop, chesspiece::color::black));
        set({3, 7}, chesspiece(chesspiece::value::queen, chesspiece::color::black));
        set({4, 7}, chesspiece(chesspiece::value::king, chesspiece::color::black));
        set({5, 7}, chesspiece(chesspiece::value::bishop, chesspiece::color::black));
        set({6, 7}, chesspiece(chesspiece::value::knight, chesspiece::color::black));
        set({7, 7}, chesspiece(chesspiece::value::rook, chesspiece::color::black));

        for (unsigned int y = 2; y < 6; y++)
        {
            set({0, y}, chesspiece());
            set({1, y}, chesspiece());
            set({2, y}, chesspiece());
            set({3, y}, chesspiece());
            set({4, y}, chesspiece());
            set({5, y}, chesspiece());
            set({6, y}, chesspiece());
            set({7, y}, chesspiece());
        }
    }

    void show()
    {
        std::cout << "START" << std::endl;
        for (unsigned int x = 0; x < *(get_board_sizes() + 1); x++)
        {
            for (unsigned int y = 0; y < *get_board_sizes(); y++)
                std::cout << get({y, *(get_board_sizes() + 1) - x - 1})->to_string() << " ";
            std::cout << std::endl;
        }
        std::cout << "STOP" << std::endl;
    }

    void move()
    {
        std::vector<_movement> positions;
        chesspiece *p;
        for (unsigned int x = 0; x < *get_board_sizes(); x++)
            for (unsigned int y = 0; y < *(get_board_sizes() + 1); y++)
                if ((p = get({x, y})) != nullptr && p->get_color() == chesspiece::color::white)
                    _available_positions(x, y, positions);

        _movement t = positions[rnd::get(0, positions.size() - 1)];
        set(t.to, *get(t.to));
        set(t.from, chesspiece());
    }
};