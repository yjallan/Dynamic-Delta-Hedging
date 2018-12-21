#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <algorithm>
#include "StdNormalCDF.h"
#define PI 3.14159265358979323846
using namespace std;


StdNormalCDF::StdNormalCDF()
{
    A1=0.319381530;
    A2=-0.356563782;
    A3=1.781477937;
    A4=-1.821255978;
    A5=1.330274429;
}

double StdNormalCDF::operator()(double x)
{
        double z,R;
        B=x;
        if (B>0){
        z=1/(1+0.2316419*B);
        R = A1*z + A2*pow(z,2) + A3*pow(z,3) + A4*pow(z,4) + A5*pow(z,5);
        NORMALIZER= 1-(1/sqrt(2*PI))*exp(-B*B/2)*R;
        return NORMALIZER;
        }
        else {
        B=-1*B;
        z=1/(1+0.2316419*B);
        R = A1*z + A2*pow(z,2) + A3*pow(z,3) + A4*pow(z,4) + A5*pow(z,5);
        NORMALIZER= (1/sqrt(2*PI))*exp(-B*B/2)*R;
        return NORMALIZER;
        }
}
