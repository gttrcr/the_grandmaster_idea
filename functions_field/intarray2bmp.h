#pragma once

#include <fstream>
#include <iostream>
#include <string>

namespace intarray2bmp
{
    struct lwrite
    {
        unsigned long value;
        unsigned size;
        lwrite(unsigned long value, unsigned size) : value(value), size(size) {}
    };

    inline std::ostream &operator<<(std::ostream &outs, const lwrite &v)
    {
        unsigned long value = v.value;
        for (unsigned cntr = 0; cntr < v.size; cntr++, value >>= 8)
            outs.put(static_cast<char>(value & 0xFF));
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
    bool intarray2bmp(const std::string &filename, IntType **intarray, unsigned rows, unsigned columns, IntType min_value, IntType max_value)
    {
        // This is the difference between each color based upon
        // the number of distinct values in the input array.
        double granularity = 360.0 / ((double)(max_value - min_value) + 1);

        // Open the output BMP file
        std::ofstream f(filename.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
        if (!f)
            return false;

        // Some basic
        unsigned long headers_size = 14    // sizeof( BITMAPFILEHEADER )
                                     + 40; // sizeof( BITMAPINFOHEADER )
        unsigned long padding_size = (4 - ((columns * 3) % 4)) % 4;
        unsigned long pixel_data_size = rows * ((columns * 3) + padding_size);

        // Write the BITMAPFILEHEADER
        f.put('B').put('M');                            // bfType
        f << lwrite(headers_size + pixel_data_size, 4); // bfSize
        f << lwrite(0, 2);                              // bfReserved1
        f << lwrite(0, 2);                              // bfReserved2
        f << lwrite(headers_size, 4);                   // bfOffBits

        // Write the BITMAPINFOHEADER
        f << lwrite(40, 4);              // biSize
        f << lwrite(columns, 4);         // biWidth
        f << lwrite(rows, 4);            // biHeight
        f << lwrite(1, 2);               // biPlanes
        f << lwrite(24, 2);              // biBitCount
        f << lwrite(0, 4);               // biCompression=BI_RGB
        f << lwrite(pixel_data_size, 4); // biSizeImage
        f << lwrite(0, 4);               // biXPelsPerMeter
        f << lwrite(0, 4);               // biYPelsPerMeter
        f << lwrite(0, 4);               // biClrUsed
        f << lwrite(0, 4);               // biClrImportant

        // Write the pixel data
        for (unsigned row = rows; row; row--) // bottom-to-top
        {
            for (unsigned col = 0; col < columns; col++) // left-to-right
            {
#ifdef GREY_SCALE
                IntType t = intarray[row - 1][col];
                f.put(static_cast<char>(t)).put(static_cast<char>(t)).put(static_cast<char>(t));
#elif
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
                int H = (int)(hue / 60) % 6;
                double F = (hue / 60) - H;
                double Q = 1.0 - F;

#define c(x) (255 * x)
                switch (H)
                {
                case 0:
                    red = c(1);
                    green = c(F);
                    blue = c(0);
                    break;
                case 1:
                    red = c(Q);
                    green = c(1);
                    blue = c(0);
                    break;
                case 2:
                    red = c(0);
                    green = c(1);
                    blue = c(F);
                    break;
                case 3:
                    red = c(0);
                    green = c(Q);
                    blue = c(1);
                    break;
                case 4:
                    red = c(F);
                    green = c(0);
                    blue = c(1);
                    break;
                default:
                    red = c(1);
                    green = c(0);
                    blue = c(Q);
                }
#undef c

                f.put(static_cast<char>(blue)).put(static_cast<char>(green)).put(static_cast<char>(red));
#endif
            }

            if (padding_size)
                f << lwrite(0, padding_size);
        }

        return f.good();
    }

    template <typename IntType>
    bool intarray2bmp(const std::string &filename, IntType *intarray, unsigned rows, unsigned columns, IntType min_value, IntType max_value)
    {
        IntType **ia = new (std::nothrow) IntType *[rows];
        for (unsigned row = 0; row < rows; row++)
            ia[row] = intarray + (row * columns);
        bool result = intarray2bmp(filename, ia, rows, columns, min_value, max_value);
        delete[] ia;
        return result;
    }
}
