#include "exprtk.hpp"

#include <iostream>
#include <cmath>
#include <cctype>
#include <random>
#include <execution>

#include "intarray2bmp.h"

#define PROTO_FUNC "A(sin(Bt+C)+1)"
#define A_MIN 0.0
#define A_MAX 255.0 / 2
#define B_MIN 0.0
#define B_MAX 100.0
#define C_MIN 0.0
#define C_MAX 100.0
std::string **expr;
unsigned int **frame;

std::string replace(const std::string &str, const std::string &from, const std::string &to)
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
    
    std::uniform_real_distribution<double> a(A_MIN, A_MAX);
    std::uniform_real_distribution<double> b(B_MIN, B_MAX);
    std::uniform_real_distribution<double> c(C_MIN, C_MAX);
    //std::normal_distribution<double> a(A_MAX, 10);
    //std::normal_distribution<double> b(50, 1);
    //std::normal_distribution<double> c(50, 1);

    x_vector.clear();
    expr = new std::string *[x_max];
    frame = new unsigned int *[x_max];
    for (unsigned int x = 0; x < x_max; x++)
    {
        expr[x] = new std::string[y_max];
        frame[x] = new unsigned int[y_max];
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
    std::for_each(
        std::execution::par_unseq,
        x_vector.begin(),
        x_vector.end(),
        [y_max, t](auto &&x)
        {
            for (unsigned int y = 0; y < y_max; y++)
                frame[x][y] = result(x, y, t);
        });
}

std::string create_tmp_file()
{
    char tmpname[L_tmpnam];
    std::string t(std::tmpnam(tmpname));
    //mkstemp(tmpname);

    return std::string(tmpname);
}

std::vector<std::string> exec(const char *command, bool check = false)
{
    std::string tmpname = create_tmp_file();
    std::string scommand = command;
    std::string cmd = scommand + " >> " + tmpname;
    int s = std::system(cmd.c_str());

    bool ok = true;
    if (!check)
        ok = exec(std::string("if [ -s " + std::string(tmpname) + " ]; then echo 1; else echo 0; fi").c_str(), true)[0] == "1" ? true : false;

    if (!ok)
        return std::vector<std::string>();

    std::ifstream file(tmpname, std::ios::in | std::ios::binary);
    std::string result;
    if (file)
    {
        while (!file.eof())
            result.push_back(file.get());
        file.close();
    }
    remove(tmpname.c_str());

    std::vector<std::string> ret;
    std::istringstream iss(result);
    std::string line;
    while (std::getline(iss, line))
        ret.push_back(line);

    ret.pop_back();

    return ret;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        std::cout << "functions_field file_name.mp4 [t_max [t_delta=0.1]] [x_size=50 y_size=50]" << std::endl;
        return 0;
    }

    std::string file_name = "";
    double t_max = 10;
    double t_delta = 0.1;
    unsigned int x_max = 50;
    unsigned int y_max = 50;

    if (argc > 1)
        file_name = argv[1];
    if (argc > 2)
        t_max = std::stod(argv[2]);
    if (argc > 3)
        t_delta = std::stod(argv[3]);
    if (argc > 5)
    {
        x_max = std::stoi(argv[4]);
        y_max = std::stoi(argv[5]);
    }

    std::cout << "Init expr..." << x_max << "x" << y_max << std::endl;
    init_expr(x_max, y_max);

    std::cout << "Creating simulation..." << std::endl;
    for (double t = 0; t < t_max; t += t_delta)
    {
        std::cout << "\t\t\r" << t * 100.0 / t_max << "%\r"
                  << "\033[F" << std::endl;
        get_frame(x_max, y_max, t);
        if (!intarray2bmp::intarray2bmp<unsigned int>(std::to_string(t) + ".bmp", frame, x_max, y_max, 0, 255))
            std::cout << "Error" << std::endl;
    }

    std::cout << std::endl
              << "Saving..." << std::endl;
    exec(std::string("cat *.bmp | ffmpeg  -framerate 25 -f image2pipe -i - " + file_name + ".mp4").c_str());
    exec("rm -rf *.bmp");

    std::cout << "Clearing memory..." << std::endl;
    for (unsigned x = 0; x < x_max; x++)
    {
        delete[] expr[x];
        delete[] frame[x];
    }
    delete[] expr;
    delete[] frame;

    return 0;
}
