#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include <gsl_statistics_double.h>
#include <gsl/gsl_spline.h>
#include <matplotlibcpp.h>

#include <my_processes.h>
#include <my_random.h>
#include <my_plotting.h>
#include <my_rates.h>
#include <my_helpers.h>


using std::cout, std::endl, std::vector,std::string, std::thread, std::getline;
using std::uniform_real_distribution, std::normal_distribution;


namespace plt = matplotlibcpp;

int main()
{

    initializeRNG(0); // seed 0 to get a random device seed
    
    int sims = 100;  // How many simulations

    
    auto startTime = std::chrono::high_resolution_clock::now();

    BrownianMotion R1(0.04, 0.01);
    R1.setLength(1100);
    thread t1(&BrownianMotion::simulate, &R1, sims); // Need to be passed by reference to threads
   

    GBrownianMotion S1(0.04, 0.01);
    S1.setLength(1100);
    S1.setStart(100);
    thread t2(&GBrownianMotion::simulate, &S1, sims);

    t1.join();
    t2.join();

    auto endTime = std::chrono::high_resolution_clock::now();

    plt::figure(1);
    for(const auto path : R1.getPaths())
    {
        plt::plot(path);
    }    
    plt::title("Brownian motion");


    plt::figure(2);
    for(const auto path : S1.getPaths())
    {
        plt::plot(path);
    }    
    plt::title("Geometric brownian motion");

    
    auto simTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    cout << "Simulation time: " << simTime.count() << " ms\n";

    YieldCurve y1("yields.csv", false);

    vector<double> interp, xs;

    xs = linspace(1.0/12 + 1e-4, 30 - 1e-4, 10000);
    interp = y1.interpolation(xs);

    plt::figure(3);
    plt::plot(xs, interp,"-k");
    plt::plot(y1.getMaturities(), y1.getYields(),"or");
    plt::xlabel("Years");
    plt::ylabel("Yield to maturity");
    plt::title("Yield curve as of " + y1.getDate());
    


    plt::show();
 

    return 0;
}