#ifndef my_random_h
#define my_random_h

#include <random>
//#include <thread>


thread_local std::mt19937 gen;

void initializeRNG(int seed = 0){

    if(seed != 0)
    {
        gen.seed(seed);
    }
    else
    {
        std::random_device rd;
        gen.seed(rd());
    }
};

double randomU()
{
    static thread_local std::uniform_real_distribution<double> distrUni(0, 1); // This is created only once
    return distrUni(gen);
};

double randomN()
{
    static thread_local std::normal_distribution<double> distrNorm(0, 1);
    return distrNorm(gen);
}


#endif