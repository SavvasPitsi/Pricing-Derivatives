#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <gsl_statistics_double.h>

#include <my_processes.h>
#include <my_random.h>
#include <my_plotting.h>


using std::cout;
using std::endl;
using std::uniform_real_distribution;
using std::normal_distribution;
using std::vector;
using std::thread;


int main()
{

    initializeRNG(0); // seed 0 to get a random device seed

    int sims = 1000;

    
    auto startTime = std::chrono::high_resolution_clock::now();

    BrownianMotion R1(0.04, 0.01);
    R1.setLength(1100);
    thread t1(&BrownianMotion::simulate, &R1, sims);
   

    GBrownianMotion S1(0.04, 0.01);
    S1.setLength(1100);
    S1.setStart(100);
    thread t2(&GBrownianMotion::simulate, &S1, sims);

    t1.join();
    t2.join();

    auto endTime = std::chrono::high_resolution_clock::now();

    plt::figure(1);
    for(auto path : R1.getPaths())
    {
        plt::plot(path);
    }    
    plt::title("Brownian motion");


    plt::figure(2);
    for(auto path : S1.getPaths())
    {
        plt::plot(path);
    }    
    plt::title("Geometric brownian motion");

    
    auto simTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    cout << "Simulation time: " << simTime.count() << " ms\n";

    plt::show();
 

    return 0;
}