#include <iostream>
// #include <memory>
// #include <random>
#include <vector>
#include <gsl_statistics_double.h>

#include <my_processes.h>
#include <my_random.h>
#include <my_plotting.h>


using std::cout;
using std::endl;
using std::uniform_real_distribution;
using std::normal_distribution;
using std::vector;


int main()
{

    initializeRNG(0); // seed 0 to get a random seed every time

    vector<double> sim1, sim2;

    BrownianMotion R1(0.04, 0.01);
    sim1 = R1.run(1000);

    GBrownianMotion S1(0.04, 0.01);
    S1.setStart(100);
    sim2 = S1.run(1000);

    plt::figure(1);
    plt::plot(sim1);
    plt::title("Brownian motion");

    plt::figure(2);
    plt::plot(sim2);
    plt::title("Geometric brownian motion");

    plt::show();
 

    return 0;
}