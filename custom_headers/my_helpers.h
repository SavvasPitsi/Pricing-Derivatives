#ifndef my_helpers_h
#define my_helpers_h

#include <iostream>
#include <vector>


using std::vector;

vector<double> linspace(double start, double end, int n) {
    vector<double> result;
    if( n < 2) { return result; };

    double step = (end - start) / (n - 1);
    for (int i = 0; i < n; ++i) {
        result.push_back(start + i * step);
    }
    return result;
}


#endif