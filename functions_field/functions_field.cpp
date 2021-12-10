#include "exprtk.hpp"

#include <iostream>
#include <cmath>
#include <cctype>
#include <random>

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

void init_expr(unsigned int x_max, unsigned int y_max)
{
    std::random_device os_seed;
    const double seed = os_seed();
    std::mt19937 generator(seed);
    std::uniform_real_distribution<double> a(A_MIN, A_MAX);
    std::uniform_real_distribution<double> b(B_MIN, B_MAX);
    std::uniform_real_distribution<double> c(C_MIN, C_MAX);

    for (unsigned int x = 0; x < x_max; x++)
        for (unsigned int y = 0; y < y_max; y++)
            expr[x][y] = replace(replace(replace(PROTO_FUNC, "A", std::to_string(a(generator))), "B", std::to_string(b(generator))), "C", std::to_string(c(generator)));
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
    for (unsigned int x = 0; x < x_max; x++)
        for (unsigned int y = 0; y < y_max; y++)
            frame[x][y] = result(x, y, t);
}

#ifndef INTARRAY2BMP_HPP
#define INTARRAY2BMP_HPP

#include <fstream>
#include <iostream>
#include <string>

namespace intarray2bmp
{
    //-------------------------------------------------------------------------- 
    // This little helper is to write little-endian values to file.
    //
    struct lwrite
    {
        unsigned long value;
        unsigned      size;
        lwrite(unsigned long value, unsigned size) :
            value(value), size(size)
        { }
    };

    //--------------------------------------------------------------------------
    inline std::ostream& operator << (std::ostream& outs, const lwrite& v)
    {
        unsigned long value = v.value;
        for (unsigned cntr = 0; cntr < v.size; cntr++, value >>= 8)
            outs.put(static_cast <char> (value & 0xFF));
        return outs;
    }

    //--------------------------------------------------------------------------
    // Take an integer array and convert it into a color image.
    //
    // This first version takes an array of array style of array:
    //   int* a[ 10 ]
    //
    // The second, overloaded version takes a flat C-style array:
    //   int a[ 10 ][ 10 ]
    //
    template <typename IntType>
    bool intarray2bmp(
        const std::string& filename,
        IntType** intarray,
        unsigned           rows,
        unsigned           columns,
        IntType            min_value,
        IntType            max_value
    ) {
        // This is the difference between each color based upon
        // the number of distinct values in the input array.
        double granularity = 360.0 / ((double)(max_value - min_value) + 1);

        // Open the output BMP file
        std::ofstream f(filename.c_str(),
            std::ios::out | std::ios::trunc | std::ios::binary);
        if (!f) return false;

        // Some basic
        unsigned long headers_size = 14  // sizeof( BITMAPFILEHEADER )
            + 40; // sizeof( BITMAPINFOHEADER )
        unsigned long padding_size = (4 - ((columns * 3) % 4)) % 4;
        unsigned long pixel_data_size = rows * ((columns * 3) + padding_size);

        // Write the BITMAPFILEHEADER
        f.put('B').put('M');                           // bfType
        f << lwrite(headers_size + pixel_data_size, 4);  // bfSize
        f << lwrite(0, 2);  // bfReserved1
        f << lwrite(0, 2);  // bfReserved2
        f << lwrite(headers_size, 4);  // bfOffBits

        // Write the BITMAPINFOHEADER
        f << lwrite(40, 4);  // biSize
        f << lwrite(columns, 4);  // biWidth
        f << lwrite(rows, 4);  // biHeight
        f << lwrite(1, 2);  // biPlanes
        f << lwrite(24, 2);  // biBitCount
        f << lwrite(0, 4);  // biCompression=BI_RGB
        f << lwrite(pixel_data_size, 4);  // biSizeImage
        f << lwrite(0, 4);  // biXPelsPerMeter
        f << lwrite(0, 4);  // biYPelsPerMeter
        f << lwrite(0, 4);  // biClrUsed
        f << lwrite(0, 4);  // biClrImportant

        // Write the pixel data
        for (unsigned row = rows; row; row--)           // bottom-to-top
        {
            for (unsigned col = 0; col < columns; col++)  // left-to-right
            {
                unsigned char red, green, blue;
                //
                // This is how we convert an integer value to a color:
                // by mapping it evenly along the CIECAM02 hue color domain.
                //
                // http://en.wikipedia.org/wiki/Hue
                // http://en.wikipedia.org/wiki/hsl_and_hsv#conversion_from_hsv_to_rgb
                //
                // The following algorithm takes a few shortcuts since
                // both 'value' and 'saturation' are always 1.0.
                //
                double hue = (intarray[row - 1][col] - min_value) * granularity;
                int    H = (int)(hue / 60) % 6;
                double F = (hue / 60) - H;
                double Q = 1.0 - F;

#define c( x ) (255 * x)
                switch (H)
                {
                case 0:  red = c(1);  green = c(F);  blue = c(0);  break;
                case 1:  red = c(Q);  green = c(1);  blue = c(0);  break;
                case 2:  red = c(0);  green = c(1);  blue = c(F);  break;
                case 3:  red = c(0);  green = c(Q);  blue = c(1);  break;
                case 4:  red = c(F);  green = c(0);  blue = c(1);  break;
                default: red = c(1);  green = c(0);  blue = c(Q);
                }
#undef c

                f.put(static_cast <char> (blue))
                    .put(static_cast <char> (green))
                    .put(static_cast <char> (red));
            }

            if (padding_size) f << lwrite(0, padding_size);
        }

        // All done!
        return f.good();
    }

    //--------------------------------------------------------------------------
    template <typename IntType>
    bool intarray2bmp(
        const std::string& filename,
        IntType* intarray,
        unsigned           rows,
        unsigned           columns,
        IntType            min_value,
        IntType            max_value
    ) {
        IntType** ia = new(std::nothrow) IntType * [rows];
        for (unsigned row = 0; row < rows; row++)
        {
            ia[row] = intarray + (row * columns);
        }
        bool result = intarray2bmp(
            filename, ia, rows, columns, min_value, max_value
        );
        delete[] ia;
        return result;
    }

} // namespace intarray2bmp

#endif

// end intarray2bmp.hpp 

int main()
{
    unsigned int x_max = X_MAX;
    unsigned int y_max = Y_MAX;
    init_expr(x_max, y_max);

    for (double t = 0; t < 10; t += 0.001)
    {
        std::cout << "Getting frame...for t=" << t << std::endl;
        get_frame(x_max, y_max, t);
        std::string f_name = std::to_string(t) + ".bmp";
        std::cout << "Drawing frame..." << f_name << std::endl;
        if (!intarray2bmp::intarray2bmp<unsigned int>(f_name, *frame, X_MAX, Y_MAX, 0, 255))
            std::cout << "Error" << std::endl;
    }

    return 0;
}