#ifndef OPTIONS_H_INCLUDED
#define OPTIONS_H_INCLUDED
#include "StdNormalCDF.h"
#include <string>


using namespace std;

class Options
{
    public:
        Options();
        ~Options();
        double operator() (string, double,double,double,double,double,double);
		double call_delta(string, double, double, double, double, double, double);
};

#endif // OPTIONS_H_INCLUDED
