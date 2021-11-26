#pragma once

namespace test_1
{
    void test_1_av_cap(const unsigned int max_test)
    {
        const std::string test_name = "test_av_cap";
        std::cout << test_name << " start, max_test " << max_test << std::endl;

        table t;
        std::vector<std::tuple<
            piece,        //for each piece
            unsigned int, //number of this piece on table
            unsigned int, //number of available positions
            unsigned int, //number of available captures
            std::tuple<
                unsigned int, //number of pawn to capture
                unsigned int, //number of rook to capture
                unsigned int, //number of knight to capture
                unsigned int, //number of bishop to capture
                unsigned int, //number of king to capture
                unsigned int  //number of queen to capture
                >>>
            average;

        average.push_back(std::make_tuple(piece(piece::value::pawn, piece::color::white), 0, 0, 0, std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>()));
        average.push_back(std::make_tuple(piece(piece::value::rook, piece::color::white), 0, 0, 0, std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>()));
        average.push_back(std::make_tuple(piece(piece::value::knight, piece::color::white), 0, 0, 0, std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>()));
        average.push_back(std::make_tuple(piece(piece::value::bishop, piece::color::white), 0, 0, 0, std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>()));
        average.push_back(std::make_tuple(piece(piece::value::king, piece::color::white), 0, 0, 0, std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>()));
        average.push_back(std::make_tuple(piece(piece::value::queen, piece::color::white), 0, 0, 0, std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>()));

        std::string str = "";
        for (unsigned int i = 1; i <= max_test; i++)
        {
            t.random();
            std::vector<std::tuple<piece, position>> dist = t.pieces();
            for (unsigned int d = 0; d < dist.size(); d++)
            {
                unsigned int pos = (unsigned int)(t.available_positions(std::get<1>(dist[d]), TR_VALUE).size());
                std::vector<std::tuple<position, piece>> av_cap = t.available_captures(std::get<1>(dist[d]));
                unsigned int cap = (unsigned int)(av_cap.size());
                for (unsigned int r = 0; r < average.size(); r++)
                    if (std::get<0>(average[r]) == std::get<0>(dist[d]))
                    {
                        std::get<1>(average[r]) += 1;
                        std::get<2>(average[r]) += pos;
                        std::get<3>(average[r]) += cap;
                        for (unsigned int av = 0; av < av_cap.size(); av++)
                        {
                            piece::value v = std::get<1>(av_cap[av]).get_value();
                            if (v == piece::value::pawn)
                                std::get<0>(std::get<4>(average[r])) += 1;
                            else if (v == piece::value::rook)
                                std::get<1>(std::get<4>(average[r])) += 1;
                            else if (v == piece::value::knight)
                                std::get<2>(std::get<4>(average[r])) += 1;
                            else if (v == piece::value::bishop)
                                std::get<3>(std::get<4>(average[r])) += 1;
                            else if (v == piece::value::king)
                                std::get<4>(std::get<4>(average[r])) += 1;
                            else if (v == piece::value::queen)
                                std::get<5>(std::get<4>(average[r])) += 1;
                        }
                    }
            }

            if ((i % 1000) == 0)
            {
                std::string tmp = std::to_string(i) + " ";
                for (unsigned int r = 0; r < average.size(); r++)
                {
                    tmp += "(" + std::to_string(std::get<1>(average[r])) + " " + std::to_string(std::get<2>(average[r])) + " " + std::to_string(std::get<3>(average[r])) + " ";
                    tmp += "(" + std::to_string(std::get<0>(std::get<4>(average[r]))) + " " + std::to_string(std::get<1>(std::get<4>(average[r]))) + " " + std::to_string(std::get<2>(std::get<4>(average[r]))) + " " + std::to_string(std::get<3>(std::get<4>(average[r]))) + " " + std::to_string(std::get<4>(std::get<4>(average[r]))) + " " + std::to_string(std::get<5>(std::get<4>(average[r]))) + ")) ";
                }
                //std::cout << tmp << std::endl;
                str += tmp + "\n";
            }

            if (i % 10000 == 0 || i == max_test)
            {
                std::cout << "         \r" << (double)i * 100.0 / (double)max_test << "%\r"
                          << "\033[F" << std::endl;
                std::ofstream outfile(utils::get_filename(test_name), std::ios_base::app);
                outfile << str;
                outfile.close();
                str = "";
            }
        }

        std::cout << test_name << " end" << std::endl;
    }
}