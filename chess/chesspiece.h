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
}