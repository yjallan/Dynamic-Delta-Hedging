#include <iostream>
#include <fstream>
#include<random>
#include "string"
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <sstream>
#include <algorithm>
#include "StdNormalCDF.h"
#include "options.h"
#include "all_funcs.h"


//BOX MULLER METHOD TO GET STD NORMAL Random Variable as taught by professor in lecture
double DrawNormal(){

    double x;
    double y;
    double size_squared;

    do
    {
        x = 2.0*rand()/(double)(RAND_MAX)-1;
        y = 2.0*rand()/(double)(RAND_MAX)-1;
        size_squared = x*x + y*y;
    }
    while (size_squared >= 1.0);

    return x*sqrt(-2*log(size_squared)/size_squared);

}

//USING BISECTION METHOD FOR CALCULATING IMPLIED VOLATILITY

double interval_bisection(double y_target,  // Target y value
	double m,         // Left interval value
	double n,         // Right interval value
	double epsilon,   // Tolerance
	string type, //option type
	double K, //strike price
	double T, //expiration time
	double s, //stock price
	double r, //risk free
	double q //dividend
	) {            // Function object of type T, named g

					  // Create the initial x mid-point value
					  // Find the mapped y value of g(x)
	
	Options op; //Defining an Instance of the Black Scholes option pricing class
	double x = 0.5 * (m + n);
	//cout << type << " " << x << " " << K << " " << " " << T << " " << s << " " << r << " " << q << endl;
	double y =op(type, x, K, T, s,r,q);
	//cout <<"First BS Price: "<< y << endl;
	int count = 0;

	// While the difference between y and the y_target
	// value is greater than epsilon, keep subdividing
	// the interval into successively smaller halves
	// and re-evaluate the new y.
	do {
		if (y < y_target) {
			m = x;
		}

		if (y > y_target) {
			n = x;
			//cout << "N is: " << n << endl;
		}

		x = 0.5 * (m + n);
		//cout << "Volatility is: " << x << endl;
		
		y = op(type, x, K, T, s, r, q);
		//cout << "BS Price: " << y << endl;
		count += 1;
		
		if (count == 500)
		{
			break;
		}
		
	} while (fabs(y - y_target) > epsilon);
	//cout << "COUNT: "<<count << endl;

	return x;
}

