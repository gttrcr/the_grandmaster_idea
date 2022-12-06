#pragma once

//#define OUTPUT // show the board and the winner (very very slow!)

// define pieces you want to play with
//#define PAWN
//#define ROOK
#define KNIGHT
//#define BISHOP
//#define KING
//#define QUEEN

#include <bitset>
#include <iostream>
#include <string>

#include "board/board2d.h"
#include "check.h"
#include "rnd.h"

namespace chess
{
    class chessboard : public virtual board::board2d
    {
    private:
        // provides the list of positions of a single piece by coordinate
        void _piece_available_positions(const unsigned int x, const unsigned int y, std::vector<board::movement2d> &positions)
        {
            board::chesspiece target;
            board::chesspiece p;
            get(x, y, target);
            switch (target.get_value())
            {
#ifdef PAWN
            case board::chesspiece::value::pawn:
            {
                int dir = 2 * (target.get_color() == board::chesspiece::color::white) - 1; // 1 for white, -1 for black
                if (get(x, y + dir, p) && p.is_empty())
                    positions.push_back(board::movement2d(x, y, x, y + dir)); // if the cell is empty, can move but nothing to capture
                if (get(x - 1, y + dir, p) && !p.is_empty() && target.get_color() == p.get_opponent_color())
                    positions.push_back(board::movement2d(x, y, x - 1, y + dir)); // if the cell is not empty, can move and capture
                if (get(x + 1, y + dir, p) && !p.is_empty() && target.get_color() == p.get_opponent_color())
                    positions.push_back(board::movement2d(x, y, x + 1, y + dir)); // if the cell is not empty, can move and capture
                break;
                // EN-PASSENT
            }
#endif
#ifdef ROOK
            case board::chesspiece::value::rook:
            {
                break;
            }
#endif
#ifdef KNIGHT
            case board::chesspiece::value::knight:
            {
                if (get(x - 1, y + 2, p) && (p.is_empty() || (!p.is_empty() && target.get_color() == p.get_opponent_color())))
                    positions.push_back(board::movement2d(x, y, x - 1, y + 2));
                if (get(x + 1, y + 2, p) && (p.is_empty() || (!p.is_empty() && target.get_color() == p.get_opponent_color())))
                    positions.push_back(board::movement2d(x, y, x + 1, y + 2));
                if (get(x + 2, y + 1, p) && (p.is_empty() || (!p.is_empty() && target.get_color() == p.get_opponent_color())))
                    positions.push_back(board::movement2d(x, y, x + 2, y + 1));
                if (get(x + 2, y - 1, p) && (p.is_empty() || (!p.is_empty() && target.get_color() == p.get_opponent_color())))
                    positions.push_back(board::movement2d(x, y, x + 2, y - 1));
                if (get(x + 1, y - 2, p) && (p.is_empty() || (!p.is_empty() && target.get_color() == p.get_opponent_color())))
                    positions.push_back(board::movement2d(x, y, x + 1, y - 2));
                if (get(x - 1, y - 2, p) && (p.is_empty() || (!p.is_empty() && target.get_color() == p.get_opponent_color())))
                    positions.push_back(board::movement2d(x, y, x - 1, y - 2));
                if (get(x - 2, y - 1, p) && (p.is_empty() || (!p.is_empty() && target.get_color() == p.get_opponent_color())))
                    positions.push_back(board::movement2d(x, y, x - 2, y - 1));
                if (get(x - 2, y + 1, p) && (p.is_empty() || (!p.is_empty() && target.get_color() == p.get_opponent_color())))
                    positions.push_back(board::movement2d(x, y, x - 2, y + 1));
                break;
            }
#endif
#ifdef BISHOP
            case board::chesspiece::value::bishop:
            {
                break;
            }
#endif
#ifdef KING
            case board::chesspiece::value::king:
            {
                if (get(x + 1, y - 1, p) && (p.is_empty() || target.get_color() == p.get_opponent_color()))
                    positions.push_back(board::movement2d(x, y, x + 1, y - 1));
                if (get(x + 1, y, p) && (p.is_empty() || target.get_color() == p.get_opponent_color()))
                    positions.push_back(board::movement2d(x, y, x + 1, y));
                if (get(x + 1, y + 1, p) && (p.is_empty() || target.get_color() == p.get_opponent_color()))
                    positions.push_back(board::movement2d(x, y, x + 1, y + 1));
                if (get(x, y - 1, p) && (p.is_empty() || target.get_color() == p.get_opponent_color()))
                    positions.push_back(board::movement2d(x, y, x, y - 1));
                if (get(x, y + 1, p) && (p.is_empty() || target.get_color() == p.get_opponent_color()))
                    positions.push_back(board::movement2d(x, y, x, y + 1));
                if (get(x - 1, y - 1, p) && (p.is_empty() || target.get_color() == p.get_opponent_color()))
                    positions.push_back(board::movement2d(x, y, x - 1, y - 1));
                if (get(x - 1, y, p) && (p.is_empty() || target.get_color() == p.get_opponent_color()))
                    positions.push_back(board::movement2d(x, y, x - 1, y));
                if (get(x - 1, y + 1, p) && (p.is_empty() || target.get_color() == p.get_opponent_color()))
                    positions.push_back(board::movement2d(x, y, x - 1, y + 1));
                break;
            }
#endif
#ifdef QUEEN
            case board::chesspiece::value::queen:
            {
                break;
            }
#endif
            default:
                break;
            }
        }

        // provides the list of positions of pieces by color
        void _available_positions(const board::chesspiece::color &color, std::vector<board::movement2d> &movs, const board::chesspiece::value &filter_by_value = board::chesspiece::value::empty_value)
        {
            board::chesspiece p;
            for (unsigned int x = 0; x < *get_board_sizes(); x++)
                for (unsigned int y = 0; y < *(get_board_sizes() + 1); y++)
                    if (get(x, y, p) && p.get_color() == color)
                        if (filter_by_value == board::chesspiece::value::empty_value || filter_by_value == p.get_value())
                            _piece_available_positions(x, y, movs);
        }

        bool _test_ok_for_king(const board::chesspiece::color &make_check, const board::chesspiece::color &receive_check, const unsigned int king_from_x, const unsigned int king_from_y, const unsigned int &king_to_x, const unsigned int &king_to_y)
        {
            unsigned int unused_king_x;
            unsigned int unused_king_y;
            board::chesspiece p;
            get(king_to_x, king_to_y, p);
            single_move(board::movement2d(king_from_x, king_from_y, king_to_x, king_to_y));
            bool ok = !_check(make_check, receive_check, unused_king_x, unused_king_y);
            single_move(board::movement2d(king_to_x, king_to_y, king_from_x, king_from_y));
            set(king_to_x, king_to_y, p);

            return ok;
        }

        bool _random_available_position(const board::chesspiece::color &color, board::movement2d &mov)
        {
            std::vector<board::movement2d> movs;
            _available_positions(color, movs);
            if (movs.size() > 0)
            {
                do
                {
                    mov = movs[rnd::get(0, movs.size() - 1)];
                    board::chesspiece p;
                    if (get(mov.from_x, mov.from_y, p) && p.get_value() == board::chesspiece::value::king)
                    {
                        if (p.get_color() == color && _test_ok_for_king(p.get_opponent_color(), color, mov.from_x, mov.from_y, mov.to_x, mov.to_y))
                            return true;
                    }
                    else
                        return true;
                } while (true);
            }

            return false;
        }

        bool _check(const board::chesspiece::color &make_check, const board::chesspiece::color &receive_check, unsigned int &king_x, unsigned int &king_y)
        {
            board::chesspiece p;
            for (king_x = 0; king_x < *get_board_sizes(); king_x++)
                for (king_y = 0; king_y < *(get_board_sizes() + 1); king_y++)
                    if (get(king_x, king_y, p) && p.get_color() == receive_check && board::chesspiece::value::king == p.get_value())
                        goto cont;

        cont:
            std::vector<board::movement2d> make_check_positions;
            _available_positions(make_check, make_check_positions);
            for (unsigned int m = 0; m < make_check_positions.size(); m++)
                if (make_check_positions[m].to_x == king_x && make_check_positions[m].to_y == king_y)
                    return true;

            return false;
        }

        check _check(const board::chesspiece::color &make_check, const board::chesspiece::color &receive_check, std::vector<board::movement2d> &compulsory_movements)
        {
            unsigned int king_from_x;
            unsigned int king_from_y;
            if (_check(make_check, receive_check, king_from_x, king_from_y))
            {
                std::vector<board::movement2d> receive_check_king_positions;
                _available_positions(receive_check, receive_check_king_positions, board::chesspiece::value::king);
                if (receive_check_king_positions.size() == 0)
                    return check::mate;

                for (unsigned int i = 0; i < receive_check_king_positions.size(); i++)
                    if (_test_ok_for_king(make_check, receive_check, king_from_x, king_from_y, receive_check_king_positions[i].to_x, receive_check_king_positions[i].to_y))
                        compulsory_movements.push_back(receive_check_king_positions[i]);

                if (compulsory_movements.size() == 0)
                    return check::mate;
                return check::no_mate;
            }

            return check::no_check;
        }

    public:
        chessboard() : board2d(8, 8)
        {
        }

        void setup_board()
        {
            set(0, 0, board::chesspiece(board::chesspiece::value::rook, board::chesspiece::color::white));
            set(1, 0, board::chesspiece(board::chesspiece::value::knight, board::chesspiece::color::white));
            set(2, 0, board::chesspiece(board::chesspiece::value::bishop, board::chesspiece::color::white));
            set(3, 0, board::chesspiece(board::chesspiece::value::queen, board::chesspiece::color::white));
            set(4, 0, board::chesspiece(board::chesspiece::value::king, board::chesspiece::color::white));
            set(5, 0, board::chesspiece(board::chesspiece::value::bishop, board::chesspiece::color::white));
            set(6, 0, board::chesspiece(board::chesspiece::value::knight, board::chesspiece::color::white));
            set(7, 0, board::chesspiece(board::chesspiece::value::rook, board::chesspiece::color::white));
            set(0, 1, board::chesspiece(board::chesspiece::value::pawn, board::chesspiece::color::white));
            set(1, 1, board::chesspiece(board::chesspiece::value::pawn, board::chesspiece::color::white));
            set(2, 1, board::chesspiece(board::chesspiece::value::pawn, board::chesspiece::color::white));
            set(3, 1, board::chesspiece(board::chesspiece::value::pawn, board::chesspiece::color::white));
            set(4, 1, board::chesspiece(board::chesspiece::value::pawn, board::chesspiece::color::white));
            set(5, 1, board::chesspiece(board::chesspiece::value::pawn, board::chesspiece::color::white));
            set(6, 1, board::chesspiece(board::chesspiece::value::pawn, board::chesspiece::color::white));
            set(7, 1, board::chesspiece(board::chesspiece::value::pawn, board::chesspiece::color::white));
            set(0, 6, board::chesspiece(board::chesspiece::value::pawn, board::chesspiece::color::black));
            set(1, 6, board::chesspiece(board::chesspiece::value::pawn, board::chesspiece::color::black));
            set(2, 6, board::chesspiece(board::chesspiece::value::pawn, board::chesspiece::color::black));
            set(3, 6, board::chesspiece(board::chesspiece::value::pawn, board::chesspiece::color::black));
            set(4, 6, board::chesspiece(board::chesspiece::value::pawn, board::chesspiece::color::black));
            set(5, 6, board::chesspiece(board::chesspiece::value::pawn, board::chesspiece::color::black));
            set(6, 6, board::chesspiece(board::chesspiece::value::pawn, board::chesspiece::color::black));
            set(7, 6, board::chesspiece(board::chesspiece::value::pawn, board::chesspiece::color::black));
            set(0, 7, board::chesspiece(board::chesspiece::value::rook, board::chesspiece::color::black));
            set(1, 7, board::chesspiece(board::chesspiece::value::knight, board::chesspiece::color::black));
            set(2, 7, board::chesspiece(board::chesspiece::value::bishop, board::chesspiece::color::black));
            set(3, 7, board::chesspiece(board::chesspiece::value::queen, board::chesspiece::color::black));
            set(4, 7, board::chesspiece(board::chesspiece::value::king, board::chesspiece::color::black));
            set(5, 7, board::chesspiece(board::chesspiece::value::bishop, board::chesspiece::color::black));
            set(6, 7, board::chesspiece(board::chesspiece::value::knight, board::chesspiece::color::black));
            set(7, 7, board::chesspiece(board::chesspiece::value::rook, board::chesspiece::color::black));

            for (unsigned int y = 2; y < 6; y++)
            {
                set(0, y, board::chesspiece());
                set(1, y, board::chesspiece());
                set(2, y, board::chesspiece());
                set(3, y, board::chesspiece());
                set(4, y, board::chesspiece());
                set(5, y, board::chesspiece());
                set(6, y, board::chesspiece());
                set(7, y, board::chesspiece());
            }
        }

#pragma region NORMAL_GAME

#pragma endregion NORMAL_GAME

#pragma region RANDOM_GAME

        // execute a complete random game_turn on the board
        bool game_turn_random(board::chesspiece::color &winner)
        {
            /*
            there's a check?
                YES: is it a mate?
                    YES: black wins
                    NO: white must move queen
                NO: white move random
            there's a check?
                YES: is it a mate?
                    YES: white wins
                    NO: black must move queen
                NO: black move random
            */

            board::movement2d mov;
            std::vector<board::movement2d> compulsory_movements;
            switch (_check(board::chesspiece::color::black, board::chesspiece::color::white, compulsory_movements))
            {
            case check::no_check:
                if (_random_available_position(board::chesspiece::color::white, mov) && !_history.triple())
                    single_move(mov);
                else
                {
                    winner = board::chesspiece::color::empty_color;
                    return true;
                }
                break;
            case check::no_mate:
                single_move(compulsory_movements[rnd::get(0, compulsory_movements.size() - 1)]);
                break;
            case check::mate:
                winner = board::chesspiece::color::black;
                return true;
            }

            switch (_check(board::chesspiece::color::white, board::chesspiece::color::black, compulsory_movements))
            {
            case check::no_check:
                if (_random_available_position(board::chesspiece::color::black, mov) && !_history.triple())
                    single_move(mov);
                else
                {
                    winner = board::chesspiece::color::empty_color;
                    return true;
                }
                break;
            case check::no_mate:
                single_move(compulsory_movements[rnd::get(0, compulsory_movements.size() - 1)]);
                break;
            case check::mate:
                winner = board::chesspiece::color::white;
                return true;
            }

            return false;
        }

        // execute a full game randomly
        void play_random(board::chesspiece::color &winner)
        {
            winner = board::chesspiece::color::empty_color;
            _history.start();
            while (!game_turn_random(winner))
                ;

            _history.stop();
        }

#pragma endregion RANDOM_GAME
    };
}