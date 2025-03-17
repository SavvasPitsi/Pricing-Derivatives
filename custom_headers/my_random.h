#ifndef my_random_h
#define my_random_h

#include <random>


std::mt19937 gen;

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
    static std::uniform_real_distribution<double> distrUni(0, 1); // This is created only once
    return distrUni(gen);
};

double randomN()
{
    static std::normal_distribution<double> distrNorm(0, 1);
    return distrNorm(gen);
}


#endif