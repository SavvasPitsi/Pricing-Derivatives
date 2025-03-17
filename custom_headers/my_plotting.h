#ifndef my_plotting_h
#define my_plotting_h

#include <matplotlibcpp.h>

namespace plt = matplotlibcpp;

void hist(std::vector<double>& vec, int figNum = 1, std::string s = "Title" )
{
    plt::figure(figNum);
    plt::clf();
    plt::hist(vec);
    plt::title(s);
    plt::show(false);
    plt::pause(0.1);
}




#endif