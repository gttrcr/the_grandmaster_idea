#pragma once

#include <bitset>
#include <iostream>
#include <fstream>
#include <string>

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
    chesspiece(value value = chesspiece::value::empty_value, color color = chesspiece::color::empty_color)
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
        return _color == chesspiece::color::empty_color || _value == chesspiece::value::empty_value;
    }

    chesspiece::color get_opponent_color()
    {
        if (_color == chesspiece::color::white)
            return chesspiece::color::black;
        return chesspiece::color::white;
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

struct _movement
{
public:
    std::vector<unsigned int> from;
    std::vector<unsigned int> to;

    _movement() = default;

    _movement(const std::vector<unsigned int> &_from, const std::vector<unsigned int> &_to)
    {
        from = _from;
        to = _to;
    }
};

#define MAX_BITSET 10000

class chessboardhistory
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

    void add(const _movement &mov)
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
        _plays.push_back(to_string());
        _history.reset();
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

    static std::vector<_movement> from_history(const std::string &str_hist)
    {
        // the structure is 00xxxyyy|00xxxyyy|00xxxyyy|00xxxyyy
        std::vector<_movement> movs;
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
                movs.push_back(_movement(white_from, white_to));

            std::vector<unsigned int> black_from;
            black_from.push_back(move[11] + move[12] * 2 + move[13] * 4); // x
            black_from.push_back(move[8] + move[9] * 2 + move[10] * 4);   // y
            std::vector<unsigned int> black_to;
            black_to.push_back(move[3] + move[4] * 2 + move[5] * 4); // x
            black_to.push_back(move[0] + move[1] * 2 + move[2] * 4); // y

            if (black_from[0] == 0 && black_from[1] == 0 && black_to[0] == 0 && black_to[1] == 0)
                break;
            else
                movs.push_back(_movement(black_from, black_to));
        }

        return movs;
    }

    static std::vector<std::vector<_movement>> from_file(const std::string &file_name)
    {
        std::vector<std::vector<_movement>> movs;
        std::ifstream input(file_name);
        for (std::string line; getline(input, line);)
            if (line.size() > 0)
                movs.push_back(from_history(line));
        input.close();

        return movs;
    }
};

class chessboard : public virtual board<chesspiece>
{
private:
    chessboardhistory _history;

    // provides the list of positions of a single piece by coordinate
    bool _piece_available_positions(const unsigned int x, const unsigned int y, std::vector<_movement> &positions, const bool &first_random = false)
    {
        chesspiece *target = get({x, y});
        chesspiece *p;
        switch (target->get_value())
        {
        case chesspiece::value::pawn:
        {
            int dir = 2 * (target->get_color() == chesspiece::color::white) - 1; // 1 for white, -1 for black
            if ((p = get({x, y + dir})) != nullptr && p->is_empty())
            {
                positions.push_back(_movement({x, y}, {x, y + dir})); // if the cell is empty, can move but nothing to capture
                if (first_random)
                    return true;
            }
            if ((p = get({x - 1, y + dir})) != nullptr && !p->is_empty() && target->get_color() == p->get_opponent_color())
            {
                positions.push_back(_movement({x, y}, {x - 1, y + dir})); // if the cell is not empty, can move and capture
                if (first_random)
                    return true;
            }
            if ((p = get({x + 1, y + dir})) != nullptr && !p->is_empty() && target->get_color() != p->get_opponent_color())
            {
                positions.push_back(_movement({x, y}, {x + 1, y + dir})); // if the cell is not empty, can move and capture
                if (first_random)
                    return true;
            }
            break;
            // EN-PASSENT
        }
        case chesspiece::value::rook:
        {
            // move on x
            for (unsigned int i = x; i < *get_board_sizes(); i++)
                if ((p = get({i, y})) != nullptr && target->get_color())
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

        return false;
    }

    // provides the list of positions of pieces by color
    bool _available_positions(const chesspiece::color &color, std::vector<_movement> &movs, const bool &first_random = false)
    {
        movs.clear();
        chesspiece *p;
        for (unsigned int x = 0; x < *get_board_sizes(); x++)
            for (unsigned int y = 0; y < *(get_board_sizes() + 1); y++)
                if ((p = get({x, y})) != nullptr && p->get_color() == color)
                    if (_piece_available_positions(x, y, movs, first_random) && first_random)
                        return true;

        return false;
    }

    bool _random_available_position(const chesspiece::color &color, _movement &mov)
    {
        std::vector<_movement> movs;
        if (_available_positions(color, movs, true))
        {
            mov = movs[rnd::get(0, movs.size() - 1)];
            return true;
        }

        return false;
    }

    // execute a single move
    void _single_move(const _movement &mov)
    {
        set(mov.to, *get(mov.from));
        set(mov.from, chesspiece());
        _history.add(mov);
    }

    // get the winner's color
    bool _get_winner(chesspiece::color &color)
    {
        return false;
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
        std::cout << "---- START -----" << std::endl;
        for (unsigned int x = 0; x < *(get_board_sizes() + 1); x++)
        {
            for (unsigned int y = 0; y < *get_board_sizes(); y++)
                std::cout << get({y, *(get_board_sizes() + 1) - x - 1})->to_string() << " ";
            std::cout << std::endl;
        }
        std::cout << "---- STOP ----" << std::endl;
    }

    void random_play()
    {
        _history.start();
        chesspiece::color winner;
        _movement mov;
        do
        {
            _movement mov;
            if (_random_available_position(chesspiece::color::white, mov))
                _single_move(mov);
            else
                break;
            if (!_get_winner(winner))
            {
                if (_random_available_position(chesspiece::color::black, mov))
                    _single_move(mov);
                else
                    break;
            }
        } while (!_get_winner(winner));
        _history.stop();
    }

    void play(std::vector<_movement> &movs)
    {
        for (unsigned int i = 0; i < movs.size(); i++)
        {
            _movement t = movs[i];
            set(t.to, *get(t.from));
            set(t.from, chesspiece());
        }
    }

    void save(const std::string &file_name)
    {
        _history.save(file_name);
    }
};