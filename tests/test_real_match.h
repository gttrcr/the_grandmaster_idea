#pragma once

#include <ncurses.h>
#include <signal.h>

namespace test_4
{
    enum engine
    {
        stockfish,
        the_grandmaster_idea
    };

    std::string run_engine(const std::string &input, engine eng)
    {
        //TODO manage checkmate
        if (eng == engine::stockfish)
        {
            const std::string file = "send_cmd";
            std::ifstream i_file(file.c_str());
            std::stringstream buffer;
            buffer << i_file.rdbuf();
            i_file.close();
            std::string content = buffer.str();
            content = utils::replace(content, "##", input);

            const std::string out_file_name = "out_send_cmd";
            std::ofstream o_file(out_file_name.c_str());
            o_file << content << std::endl;
            o_file.close();
            std::string cmd = "chmod a+x " + out_file_name + ";./" + out_file_name;
            std::string new_position = utils::exec(cmd.c_str())[0];
            remove(out_file_name.c_str());

            return new_position;
        }
        else if (eng == engine::the_grandmaster_idea)
        {
            return "";
        }

        return "";
    }

    void handle_winch(int sig)
    {
        signal(SIGWINCH, SIG_IGN);
        endwin();
        initscr();
        refresh();
        clear();
        signal(SIGWINCH, handle_winch);
    }

    std::string pieces[8][8];
    void light(const unsigned int x, const unsigned int y, const bool on, const std::string piece = "")
    {
        unsigned int o_max = 5;

        if (on)
            attron(COLOR_PAIR(3));
        else
        {
            if ((x + y) % 2 == 0)
                attron(COLOR_PAIR(1));
            else
                attron(COLOR_PAIR(2));
        }

        for (unsigned int o = 0; o < o_max; o++)
        {
            if (o == o_max / 2 && piece != "")
            {
                std::string space = "";
                for (unsigned int i = 0; i < o_max - piece.length() / 2; i++)
                    space += " ";
                std::string text = space + piece + space;
                mvprintw(o_max * y + o, 2 * o_max * x, text.c_str());
                pieces[x][y] = piece;
            }
            else
                mvprintw(o_max * y + o, 2 * o_max * x, "          ");
        }

        if (on)
            attroff(COLOR_PAIR(3));
        else
        {
            if ((x + y) % 2 == 0)
                attroff(COLOR_PAIR(1));
            else
                attroff(COLOR_PAIR(2));
        }

        refresh();
    }

    void test_4_real_match(const std::string &input, engine eng = engine::stockfish)
    {
        setlocale(LC_ALL, "");
        WINDOW *ncurses_win = initscr();
        raw();
        keypad(ncurses_win, TRUE);
        noecho();
        start_color();
        curs_set(0);
        signal(SIGWINCH, handle_winch);
        handle_winch(SIGWINCH);

        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_GREEN);

        //create board and add pieces
        for (unsigned int y = 0; y < 8; y++)
            for (unsigned int x = 0; x < 8; x++)
            {
                if (y == 0)
                {
                    if (x == 0)
                        light(x, y, false, "B_ROOK");
                    else if (x == 1)
                        light(x, y, false, "B_KNIG");
                    else if (x == 2)
                        light(x, y, false, "B_BISH");
                    else if (x == 3)
                        light(x, y, false, "B_KING");
                    else if (x == 4)
                        light(x, y, false, "B_QUEE");
                    else if (x == 5)
                        light(x, y, false, "B_KNIG");
                    else if (x == 6)
                        light(x, y, false, "B_BISH");
                    else if (x == 7)
                        light(x, y, false, "B_ROOK");
                }
                else if (y == 1)
                    light(x, y, false, "B_PAWN");
                else if (y == 6)
                    light(x, y, false, "W_PAWN");
                else if (y == 7)
                {
                    if (x == 0)
                        light(x, y, false, "W_ROOK");
                    else if (x == 1)
                        light(x, y, false, "W_KNIG");
                    else if (x == 2)
                        light(x, y, false, "W_BISH");
                    else if (x == 3)
                        light(x, y, false, "W_KING");
                    else if (x == 4)
                        light(x, y, false, "W_QUEE");
                    else if (x == 5)
                        light(x, y, false, "W_KNIG");
                    else if (x == 6)
                        light(x, y, false, "W_BISH");
                    else if (x == 7)
                        light(x, y, false, "W_ROOK");
                }
                else
                    light(x, y, false);
            }

        int x_pos = -1;
        int y_pos = -1;
        unsigned int ch;
        std::string str = "";
        std::string moving_piece = "";
        while ((ch = getch()) != 'q')
        {
            if (x_pos == -1 && y_pos == -1 && ch != ' ')
            {
                x_pos = 0;
                y_pos = 0;
                light(x_pos, y_pos, true, pieces[x_pos][y_pos]);
                continue;
            }

            light(x_pos, y_pos, false, pieces[x_pos][y_pos]);
            switch (ch)
            {
            case KEY_DOWN:
                y_pos++;
                if (y_pos > 7)
                    y_pos = 7;
                light(x_pos, y_pos, true, pieces[x_pos][y_pos]);
                break;
            case KEY_UP:
                if (y_pos != 0)
                    y_pos--;
                light(x_pos, y_pos, true, pieces[x_pos][y_pos]);
                break;
            case KEY_RIGHT:
                x_pos++;
                if (x_pos > 7)
                    x_pos = 7;
                light(x_pos, y_pos, true, pieces[x_pos][y_pos]);
                break;
            case KEY_LEFT:
                if (x_pos != 0)
                    x_pos--;
                light(x_pos, y_pos, true, pieces[x_pos][y_pos]);
                break;
            case (unsigned int)(' '):
                if (x_pos != -1 && y_pos != -1)
                {
                    char c = (char)(x_pos + 'a');
                    std::string tmp = c + std::to_string(8 - y_pos);
                    if (moving_piece == "")
                    {
                        str += tmp;
                        moving_piece = pieces[x_pos][y_pos];
                        pieces[x_pos][y_pos] = "";
                    }
                    else
                    {
                        str += tmp + " ";
                        pieces[x_pos][y_pos] = moving_piece;
                        moving_piece = "";
                        light(x_pos, y_pos, false, pieces[x_pos][y_pos]);
                        std::string result = run_engine(str, eng);
                        str += result + " ";
                        unsigned int eng_x_from = result[0] - 'a';
                        unsigned int eng_y_from = 8 - (result[1] - '0');
                        unsigned int eng_x_to = result[2] - 'a';
                        unsigned int eng_y_to = 8 - (result[3] - '0');
                        std::string eng_piece = pieces[eng_x_from][eng_y_from];
                        pieces[eng_x_from][eng_y_from] = "";
                        pieces[eng_x_to][eng_y_to] = eng_piece;
                        light(eng_x_from, eng_y_from, false);
                        light(eng_x_to, eng_y_to, false, eng_piece);
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));

                    break;
                }
            }
        }

        refresh();
        endwin();
    }
}

//std::vector<std::vector<std::string>> files_th(global_threads);
//for (unsigned int i = 0; i < files.size(); i++)
//    files_th[i % global_threads].push_back(files[i]);
//
//std::for_each(
//    std::execution::par_unseq,
//    files_th.begin(),
//    files_th.end(),
//    [](auto &&item)
//    {
//        for (unsigned int i = 0; i < item.size(); i++)
//        {
//            std::ifstream in_file(item[i]);
//            std::string line;
//            while (std::getline(in_file, line))
//            {
//                std::bitset<MAX_BITSET_MATCH_SIZE> bit_set = utils::string_to_bitset(line);
//                unsigned int length = bit_set.size() % BITSET_SIZE;
//                table t;
//                for (unsigned int i = 0; i < bit_set.size(); i += BITSET_SIZE)
//                {
//                    //unsigned char from = bit_set[i + 5] + 2 * bit_set[i + 4] + 4 * bit_set[i + 3] + 8 * bit_set[i + 2] + 16 * bit_set[i + 1] + 32 * bit_set[i + 0];
//                    //unsigned char to = bit_set[i + 11] + 2 * bit_set[i + 10] + 4 * bit_set[i + 9] + 8 * bit_set[i + 8] + 16 * bit_set[i + 7] + 32 * bit_set[i + 6];
//
//                    unsigned char from = 0;
//                    unsigned char to = 0;
//                    for (unsigned int c = 0; c < COMPRESSION_OFFSET; c++)
//                    {
//                        from += (bit_set[i + c] << (COMPRESSION_OFFSET - c - 1));
//                        to += (bit_set[i + c + COMPRESSION_OFFSET - 1] << (COMPRESSION_OFFSET - c - 1));
//                    }
//                    from >>= 1;
//                    to >>= 1;
//
//                    t.move(position(from), position(to), false);
//                    t.draw();
//                }
//            }
//        }
//    });

//std::cout << test_name << " end" << std::endl;
//}
