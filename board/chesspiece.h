#pragma once

#include <iostream>

namespace board
{
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
        chesspiece(value value = board::chesspiece::value::empty_value, color color = board::chesspiece::color::empty_color)
        {
            _value = value;
            _color = color;
        }

        board::chesspiece::value get_value()
        {
            return _value;
        }

        board::chesspiece::color get_color()
        {
            return _color;
        }

        bool is_empty()
        {
            return _color == board::chesspiece::color::empty_color || _value == board::chesspiece::value::empty_value;
        }

        board::chesspiece::color get_opponent_color()
        {
            if (_color == board::chesspiece::color::white)
                return board::chesspiece::color::black;
            return board::chesspiece::color::white;
        }

        const std::string to_string()
        {
            std::string ret = "";
            switch (_color)
            {
            case board::chesspiece::color::white:
                ret += 'W';
                break;
            case board::chesspiece::color::black:
                ret += 'B';
                break;
            default:
                ret += '-';
                break;
            }

            switch (_value)
            {
            case board::chesspiece::value::pawn:
                ret += 'P';
                break;
            case board::chesspiece::value::rook:
                ret += 'R';
                break;
            case board::chesspiece::value::knight:
                ret += 'k';
                break;
            case board::chesspiece::value::bishop:
                ret += 'B';
                break;
            case board::chesspiece::value::king:
                ret += 'K';
                break;
            case board::chesspiece::value::queen:
                ret += 'Q';
                break;
            default:
                ret += '-';
                break;
            }

            return ret;
        }

        bool operator==(const board::chesspiece &p)
        {
            return _color == p._color && _value == p._value;
        }

        bool operator!=(const board::chesspiece &p)
        {
            return !(*this == p);
        }
    };
}