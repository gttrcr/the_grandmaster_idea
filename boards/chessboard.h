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

struct _movement
{
public:
    std::vector<unsigned int> from;
    std::vector<unsigned int> to;

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
    void start_play()
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

    void end_play()
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
    void _piece_available_positions(unsigned int x, unsigned int y, std::vector<_movement> &positions)
    {
        chesspiece *target = get({x, y});
        switch (target->get_value())
        {
        case chesspiece::value::pawn:
        {
            chesspiece *p;
            int dir = 2 * (get({x, y})->get_color() == chesspiece::color::white) - 1; // 1 for white, -1 for black
            if ((p = get({x, y + dir})) != nullptr && p->is_empty())
                positions.push_back(_movement({x, y}, {x, y + dir})); // if the cell is empty, can move but nothing to capture
            if ((p = get({x - 1, y + dir})) != nullptr && !p->is_empty() && target->get_color() != p->get_color())
                positions.push_back(_movement({x, y}, {x - 1, y + dir})); // if the cell is not empty, can move and capture
            if ((p = get({x + 1, y + dir})) != nullptr && !p->is_empty() && target->get_color() != p->get_color())
                positions.push_back(_movement({x, y}, {x + 1, y + dir})); // if the cell is not empty, can move and capture
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

    // provides the list of positions of pieces by color
    std::vector<_movement> _available_positions(const chesspiece::color &color)
    {
        std::vector<_movement> positions;
        chesspiece *p;
        for (unsigned int x = 0; x < *get_board_sizes(); x++)
            for (unsigned int y = 0; y < *(get_board_sizes() + 1); y++)
                if ((p = get({x, y})) != nullptr && p->get_color() == color)
                    _piece_available_positions(x, y, positions);

        return positions;
    }

    // execute a single move (white or black)
    void _single_move(const chesspiece::color &color)
    {
        std::vector<_movement> positions = _available_positions(color);
        if (positions.size() > 0)
        {
            _movement t = positions.at(rnd::get(0, positions.size() - 1));
            set(t.to, *get(t.from));
            set(t.from, chesspiece());
            _history.add(t);
        }
    }

    // get the winner's color
    bool _get_winner(chesspiece::color &color)
    {
        std::vector<_movement> positions = _available_positions(chesspiece::color::white);
        std::vector<_movement> black_positions = _available_positions(chesspiece::color::black);
        positions.insert(positions.end(), black_positions.begin(), black_positions.end());
        if (positions.size() == 0)
        {
            color = chesspiece::color::empty_color;
            return true;
        }

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

    void game_turn()
    {
        _single_move(chesspiece::color::white);
        _single_move(chesspiece::color::black);
    }

    void play()
    {
        _history.start_play();
        chesspiece::color winner;
        do
        {
            _single_move(chesspiece::color::white);
            if (!_get_winner(winner))
                _single_move(chesspiece::color::black);
        } while (!_get_winner(winner));
        _history.end_play();
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