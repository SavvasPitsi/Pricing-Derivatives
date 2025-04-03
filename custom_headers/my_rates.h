#ifndef my_rates_h
#define my_rates_h

#include <vector>

#include <unordered_map>
#include <fstream>
#include <sstream>

#include <gsl/gsl_spline.h>


using std::vector, std::string; 
using std::ifstream, std::stringstream, std::getline;



class YieldCurve
{
    public:
        // member functions declarations

        vector<double> interpolation(vector<double> x);
        double interpolation(double x);
        double integrate(double a, double b);

        string getDate(void);
        vector<double> getYields(void);
        vector<double> getMaturities(void);


        YieldCurve(string path, bool e = true);
        ~YieldCurve(void);


    private:
        void bootstrapCurve(void);
        void readYields(void);
        double lineInt(double a, double b);
        std::unordered_map<string, double> translMap=
        {
            {
            {"0 Mo", 0.0}, 
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

        gsl_interp_accel *acc;
        gsl_spline *spline;

        bool extend;
        string path, date;
        vector<string> maturitiesStr;
        vector<vector<double>> yields;
        vector<double> maturitiesNum = {};
        double minX, maxX;
};

// member functions definitions
YieldCurve::YieldCurve(string p, bool e)
{
    path = p;
    extend = e;
    
    // get the yields into the object
    readYields();
    
    // interpolation accellerator, stores previous evals
    acc = gsl_interp_accel_alloc();
    
    // Allocate an spline object via pointer, using cubic spline interpolation
    spline = gsl_spline_alloc(gsl_interp_cspline, yields.back().size());

    // create the curve
    bootstrapCurve();

    minX = translMap[maturitiesStr.front()];
    maxX = translMap[maturitiesStr.back()];
};

YieldCurve::~YieldCurve(void)
{
    // free interpolation and accellerator
    gsl_spline_free(spline);
    gsl_interp_accel_free(acc);
};

// read file and get yields
void YieldCurve::readYields(void)
{
    ifstream file(path);
    
    if (!file.is_open()) 
    {
        std::cout << "ERROR: File is not open." << std::endl;
    }

    string line;
    vector<double> row = {};

    bool isDate = true;
    yields = {};
    maturitiesStr = {};



    stringstream ss;  // create a string-stream object, simulating an incoming input
    string value;

    if (getline(file, line))   // the first line of the file stream is read
    {
        ss.str(line);     // add the line to the string stream object to use "getline" again
        
        // explicitly set the 'end line' delimiter to "," so that it separates the values
        while (getline(ss, value, ','))  // keep on reading until the end of the stream
        {
            if(isDate) {isDate=false; continue;};
            maturitiesStr.push_back(value);
        }
        
        
    }

    while(getline(file, line))  // keep on reading lines from the file stream
    {
        // Both are needed to clear the string stream
        ss.str("");
        ss.clear();     
        
        ss.str(line);
        isDate = true;
        row = {};

        while(getline(ss, value, ','))
        {
            // keep the date separately
            if(isDate) {date = value; isDate=false; continue;};
            
            row.push_back(std::stof(value)); // convert to float
            
        };

        

        yields.push_back(row);
        // this goes over all lines but it actually only stores the last date
        // furthermore, only the last (most recent) yield data are used
    }

    file.close();

    double x = yields.size();

};


void YieldCurve::bootstrapCurve()
{
    
    vector<double> y = yields.back();
    
    for(string text : maturitiesStr)
    {
        maturitiesNum.push_back(translMap[text]);
    }

    if(y.size() != maturitiesNum.size()) {std::cout << "ERROR: incompatible array sizes" << std::endl;};

    // initialize the interpolation
    gsl_spline_init(spline, maturitiesNum.data(), y.data(), y.size());

    
};


double YieldCurve::interpolation(double x)
{    
    if( (x <= maxX) && (x >= minX) )
        return gsl_spline_eval(spline, x, acc);
    else if( (x >= 0) && (x < minX) && extend)
        // explicit linear interpolation to 0
        return x * ( yields.back().front() * 12 ); 
    else
        return 0;
};

vector<double> YieldCurve::interpolation(vector<double> x)
{
    vector<double> result;

    for(double xi : x)
    {
        result.push_back( interpolation(xi) );
    }

    return result;
};

double YieldCurve::lineInt(double x, double y)
{
    // explicit integral calculation
    return yields.back().front() * 12 * 0.5 * (y*y - x*x);
};

double YieldCurve::integrate(double a, double b)
{
    // return gsl_spline_eval_integ(spline, 1, 2, acc);

    // what if not extend?
    if(b == a) { return 0; };
    if(b < a) { return -integrate(b, a); };
    if(a < 0) { return integrate(0, b); };
    if(b > maxX) { return integrate(a, maxX); };
    
    if(extend)
    {
        if(a < minX && b <= minX) { return lineInt(a, b); };

        if(a < minX && b > minX) { return lineInt(a, minX) + integrate(minX, b); };

        if(a >= minX && b <= maxX) {  };
    }
    else
    {
        if(a < minX) { return integrate(minX, b); };
    }
    
    return gsl_spline_eval_integ(spline, a, b, acc);
};


string YieldCurve::getDate() { return date; };

vector<double> YieldCurve::getYields() { return yields.back(); };

vector<double> YieldCurve::getMaturities() { return maturitiesNum; };


#endif



    