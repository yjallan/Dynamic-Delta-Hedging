#include "math.h"
#include "string"
#include "options.h"
#include "cmath"
#include "StdNormalCDF.h"
#include "iostream"

using namespace std;

Options::Options(){}
Options::~Options(){}

double Options::operator() (string type, double sigma,double K,double T,double s,double r,double q){
	//double Options::operator() (string type, double s, double q, double T, double sigma, double K, double r) {

    StdNormalCDF N;
    double d1=((log(s/K))+(r-q+(0.5*pow(sigma,2)))*T)/(sigma*sqrt(T));
    double d2=d1-sigma*sqrt(T);
    //cout<<d1<<" "<<d2<<endl;

    //cout<<"\n"<<s<<"\n"<<q<<"\n"<<T<<"\n"<<sigma<<"\n"<<K<<"\n"<<r<<"\n"<<d1<<"\n"<<d2;

    if (type=="call")
    {
        return s*exp(-q*T)*N(d1)-K*exp(-r*T)*N(d2);
    }
    else if (type=="put")
    {
        return -s*exp(-q*T)*N(-d1)+K*exp(-r*T)*N(-d2);
    }
    else
    {
        return -1;
    }
}

double Options::call_delta(string type, double sigma, double K, double T, double s, double r, double q) {
	StdNormalCDF N;
	double d1 = ((log(s / K)) + (r - q + (0.5*pow(sigma, 2)))*T) / (sigma*sqrt(T));
	return N(d1);
}