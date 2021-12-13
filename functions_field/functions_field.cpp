#include "exprtk.hpp"

#include <iostream>
#include <cmath>
#include <cctype>
#include <random>
#include <execution>

#include "intarray2bmp.h"

#define PROTO_FUNC "A(sin(Bt+C)+1)"
#define A_MIN 0.0
#define A_MAX 255.0/2
#define B_MIN 0.0
#define B_MAX 100.0
#define C_MIN 0.0
#define C_MAX 100.0
#define X_MAX 50
#define Y_MAX 50
std::string expr[X_MAX][X_MAX];
unsigned int frame[X_MAX][X_MAX];

std::string replace(const std::string& str, const std::string& from, const std::string& to)
{
    std::string ret(str);
    size_t start_pos = ret.find(from);
    ret.replace(start_pos, from.length(), to);
    return ret;
}

std::vector<unsigned int> x_vector;
void init_expr(unsigned int x_max, unsigned int y_max)
{
    std::random_device os_seed;
    const double seed = os_seed();
    std::mt19937 generator(seed);
    //std::uniform_real_distribution<double> a(A_MIN, A_MAX);
    //std::uniform_real_distribution<double> b(B_MIN, B_MAX);
    //std::uniform_real_distribution<double> c(C_MIN, C_MAX);
    std::normal_distribution<double> a(A_MAX, 10);
    std::normal_distribution<double> b(50, 1);
    std::normal_distribution<double> c(50, 1);

    x_vector.clear();
    for (unsigned int x = 0; x < x_max; x++)
    {
        x_vector.push_back(x);
        for (unsigned int y = 0; y < y_max; y++)
            expr[x][y] = replace(replace(replace(PROTO_FUNC, "A", std::to_string(a(generator))), "B", std::to_string(b(generator))), "C", std::to_string(c(generator)));
    }
}

unsigned char result(unsigned int x, unsigned int y, double t)
{
    exprtk::symbol_table<double> symbol_table;
    symbol_table.add_variable("t", t);

    exprtk::expression<double> expression;
    expression.register_symbol_table(symbol_table);

    exprtk::parser<double> parser;
    parser.compile(expr[x][y], expression);
    return (unsigned char)(expression.value());
}

void get_frame(unsigned int x_max, unsigned int y_max, double t)
{
    //for (unsigned int x = 0; x < x_max; x++)
    //    for (unsigned int y = 0; y < y_max; y++)
    //        frame[x][y] = result(x, y, t);
    
    std::for_each(
        std::execution::par_unseq,
        x_vector.begin(),
        x_vector.end(),
        [y_max, t](auto&& x)
        {
            for (unsigned int y = 0; y < y_max; y++)
                frame[x][y] = result(x, y, t);
        });
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "functions_field [t_max [t_delta]]" << std::endl;
        return 0;
    }

    double t_max = 10;
    double t_delta = 0.1;
    if (argc > 1)
        t_max = std::stod(argv[0]);
    if (argc > 2)
        t_delta = std::stod(argv[1]);

    unsigned int x_max = X_MAX;
    unsigned int y_max = Y_MAX;
    std::cout << "Init expr..." << X_MAX << "x" << Y_MAX << std::endl;
    init_expr(x_max, y_max);

    std::cout << "Creating simulation" << std::endl;
    for (double t = 0; t < t_max; t += t_delta)
    {
        std::cout << 100.0 * t / t_max << "%\r";
        get_frame(x_max, y_max, t);
        if (!intarray2bmp::intarray2bmp<unsigned int>(std::to_string(t) + ".bmp", *frame, x_max, y_max, 0, 255))
            std::cout << "Error" << std::endl;
    }

    //TODO prepare .mp4
	//cat *.bmp | ffmpeg  -framerate 25 -f image2pipe -i - output.mp4

    return 0;
}