#ifndef my_processes_h
#define my_processes_h

#include <my_random.h>
#include <vector>

using std::vector;

class StochProcess
{
    protected:
        int length = 1000;
        float x0 = 0, dt = 0.01;
        vector<double> path = {};
        vector<vector<double>> paths = {};

    public:
        StochProcess(){};

        void setStep(float h) { dt = h; };

        void setStart(float start) { x0 = start; };

        void setLength(int N) { length = N; };

        vector<vector<double>> getPaths() { return paths; };
};

class BrownianMotion : public StochProcess
{
    private:
        float mean, stDev;
 
    public:
        BrownianMotion(float mu, float sigma) : mean(mu), stDev(sigma) {};

        vector<double> run()
        {
            path = {};
            path.push_back(x0);

            for(int i = 1; i < length; i++)
            {
                path.push_back(path[i-1] + mean * dt + stDev * randomN());
            }

            
            paths.push_back(path);

            return path;
        };

        void simulate(int N)
        {
            for(int i=0; i<N; i++)
            {
                run();
            }
        }

};

class GBrownianMotion : public StochProcess
{
    private:
        float mean, stDev;
    
    public:
        GBrownianMotion(float mu, float sigma) : mean(mu), stDev(sigma){ x0 = 1; };

        vector<double> run()
        {
            path = {};
            path.push_back(x0);

            for(int i = 1; i < length; i++)
            {
                path.push_back(path[i-1] + path[i-1] * mean * dt + path[i-1] * stDev * randomN());
            }

            
            paths.push_back(path);

            return path;
        };

        void simulate(int N)
        {
            for(int i=0; i<N; i++)
            {
                run();
            }
        }

};

#endif