#ifndef my_processes_h
#define my_processes_h

#include <my_random.h>
#include <vector>

using std::vector;

class StochProcess
{
    protected:
        int length = 0;
        float x0 = 0, dt = 0.01;
        vector<double> path = {};

    public:
        StochProcess(){};

        void setStep(float h) { dt = h; };

        void setStart(float start) { x0 = start; };

        int getLength() { return length; };

        vector<double> getPath() { return path; };
};

class BrownianMotion : public StochProcess
{
    private:
        float mean, stDev;
 
    public:
        BrownianMotion(float mu, float sigma) : mean(mu), stDev(sigma) {};

        vector<double> run(int N)
        {
            path.push_back(x0);

            for(int i = 1; i < N; i++)
            {
                path.push_back(path[i-1] + mean * dt + stDev * randomN());
            }

            length = path.size();

            return path;
        };

};

class GBrownianMotion : public StochProcess
{
    private:
        float mean, stDev;
    
    public:
        GBrownianMotion(float mu, float sigma) : mean(mu), stDev(sigma){ x0 = 1; };

        vector<double> run(int N)
        {
            path.push_back(x0);

            for(int i = 1; i < N; i++)
            {
                path.push_back(path[i-1] + path[i-1] * mean * dt + path[i-1] * stDev * randomN());
            }

            length = path.size();

            return path;
        };

};

#endif