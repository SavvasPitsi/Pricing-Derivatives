#ifndef my_rates_h
#define my_rates_h

#include <vector>
#include <tuple>
#include <unordered_map>
#include <fstream>
#include <sstream>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>


using std::vector, std::string; 
using std::ifstream, std::stringstream, std::getline;
using std::tuple;



std::unordered_map<string, float> maturities=
{
    {
    {"1 Mo", 1.0/12},
    {"1.5 Month", 1.5/12},
    {"2 Mo", 1.0/6},
    {"3 Mo", 1.0/4},
    {"4 Mo", 1.0/3},
    {"6 Mo", 1.0/2},
    {"1 Yr", 1.0},
    {"2 Yr", 2.0},
    {"3 Yr", 3.0},
    {"5 Yr", 5.0},
    {"7 Yr", 7.0},
    {"10 Yr", 10.0},
    {"20 Yr", 20.0},
    {"30 Yr", 30.0}
    }
};



tuple< string, vector<vector<float>> > getYields(string path)
{
    ifstream file(path);

    string line, date;
    vector<string> titles;
    vector<vector<float>> data;
    vector<float> row;
    int counter = 0;


    stringstream ss;  // create a string-stream object, simulating an incoming input
    string value;

    if (getline(file, line))   // the first line of the file stream is read
    {
        ss.str(line);     // add the line to the string stream object to use "getline" again

        // explicitly set the 'end line' delimiter to "," so that it separates the values
        while (getline(ss, value, ','))  // keep on reading until the end of the stream
        {
            titles.push_back(value);
        }
    }

    while(getline(file, line))  // keep on reading lines from the file stream
    {
        
        ss.str(line);
        counter = 0;

        while(getline(ss, value, ','))
        {
            if(counter == 0) {date = value; continue;};
            row.push_back(std::stof(value));
        };

        data.push_back(row);

    }

    file.close();

    return {date, data};    // assign returned to "auto [a, b]"
}


// void bootstrapCurve(vector<float> yields)
// {
//     {
//         gsl_interp_accel *acc = gsl_interp_accel_alloc();
//         gsl_spline *spline = gsl_spline_alloc(gsl_interp_cspline, 10);
    
//         gsl_spline_init(spline, x, y, 10);
    
//         for (xi = x[0]; xi < x[9]; xi += 0.01)
//           {
//             yi = gsl_spline_eval (spline, xi, acc);
//             printf ("%g %g\n", xi, yi);
//           }
//         gsl_spline_free (spline);
//         gsl_interp_accel_free (acc);
//       }
// }

#endif



    