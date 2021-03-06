// midterm.cpp : Defines the entry point for the console application.
#include <ql/quantlib.hpp>
#include <iostream>
#include <fstream>
#include <random>
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
using namespace std;
using namespace QuantLib;

int main()
{	
	//PART 1

	double S = 100.0;  // Option price spot price
	double K = 105.0;  // Strike price
	double r = 0.025;   // Risk-free rate (2.5%)
	double v = 0.24;    // Volatility of the underlying (24%)
	double T = 0.4;    // in years until expiry
	double q = 0.0;    // Dividend Yield
	double mew = 0.05;
	double N = 100.0;
	double delta_time = (T / N);	
	double d_t = 1.0/252.0; // 1/252;		
	
	vector <double> stockPrices;
	vector <double> optionPrice;
	vector <double> delta;
	vector <double> beta;
	vector <double> H_error;
	vector <double> D_H_error;
	vector <double> PNL_withoutHedge;	

	double stockTemp;
	double optionTemp;	
	double deltaTemp;
	double betaTemp;
	double H_errorTemp;	
	double D_H_errorTemp;
	double Normal_RV;

	//CREATING AN INSTANCE OF THE OPTION CLASS
	Options Opt;

	ofstream results_partI("results_partI.csv"); //Writing Output to File	
	results_partI << "Generated Stock Price" << "," << "Option Price" << "," << "Delta" << "," << "Cumulative Hedging Error" << "," << "Daily Hedging Error" << "," << "PNL without Hedge" << endl;
	
	for (unsigned int i = 0; i < N; i++) {		

		//Stock Values
		if (i == 0)
		{
			stockTemp = S;
		}
		else {
			Normal_RV = DrawNormal(); // Calling the Standard Normal RV generator function
			//cout << Normal_RV << endl;
			stockTemp= stockPrices[i-1] + (mew * stockPrices[i-1] * delta_time) + (v * stockPrices[i-1] * sqrt(delta_time) * Normal_RV);			
		}		
		stockPrices.push_back(stockTemp);		
		results_partI << stockPrices[i] << ",";
		

		//Calculate Option Price using our Black Scholes price calculator class
		optionTemp = Opt("call", v, K, T, stockPrices[i], r, q);
		optionPrice.push_back(optionTemp);		
		results_partI << optionPrice[i] << ",";

		//Delta Value
		deltaTemp = Opt.call_delta("call", v, K, T, stockPrices[i], r, q);
		delta.push_back(deltaTemp);		
		results_partI << delta[i] << ",";
		
		
		//Calculate B Value
		if (i == 0)
		{
			betaTemp = optionPrice[i]- delta[i]* stockPrices[i];
		}
		else {
			betaTemp = (delta[i - 1] * stockPrices[i]) + (beta[i - 1] * exp(r*d_t)) - (delta[i] * stockPrices[i]);
		}		
		beta.push_back(betaTemp);		
		
		//Calculate Cumulative Hedge Error
		if (i == 0)
		{
			H_errorTemp = 0.0;
		}
		else {
			H_errorTemp = (delta[i - 1] * stockPrices[i]) + (beta[i - 1] * exp(r*d_t)) - optionPrice[i];
		}		
		H_error.push_back(H_errorTemp);		
		results_partI << H_error[i] << ",";

		//Calculate Daily Hedge Error		
		if (i == 0)
		{
			D_H_errorTemp = H_error[i];
		}
		else {
			D_H_errorTemp = H_error[i] - H_error[i - 1];
		}		
		D_H_error.push_back(D_H_errorTemp);
		results_partI << D_H_error[i] << ",";

		//PNL without Hedge calculation
		PNL_withoutHedge.push_back(optionPrice[0] - optionPrice[i]);
		results_partI << PNL_withoutHedge[i] << endl;
				
		//Changing Time_to_expiration for the next loop
		T = T - (delta_time);		
	}
	results_partI.close();

	//END OF PART I

	//PART II BEGINS

	cout << "DYNAMIC DELTA HEDGING OF A CALL OPTION:" << endl;
	cout << "**************************************" << endl;
	cout << endl;
	cout << "RESULTS OF PART 1 AVAILABLE IN GENERATED CSV FILE NAMED 'results_partI.CSV'" << endl;
	cout << endl;
	cout << "PART II BEGINS HERE: "<<endl;
	cout << "Please enter the values of T0, Tn and Texp and Strike (K) here in the format requested:" << endl;
	cout << "Kindly make sure that the values entered are within the time range given in the CSV files:" << endl;
	cout << endl;

	string t0;
	cout << "Please Enter the Start date T0 in format 'YYYY-MM-DD'" << endl;
	cin >> t0;
	cout << endl;

	//string t0 = "2011-07-05";

	string tn;
	cout << "Please Enter the  date Tn in format 'YYYY-MM-DD'" << endl;
	cin >> tn;
	cout << endl;

	//string tn = "2011-07-29";

	string texp;
	cout << "Please Enter the Start date Texp in format 'YYYY-MM-DD'" << endl;
	cin >> texp;
	//string texp = "2011-09-17";
	cout << endl;

	string strike_str;
	cout << "Please Enter the Strike Price (ex: '500'): " << endl;
	cin >> strike_str;
	cout << endl;
	cout << "The code is running. Pls wait for a brief moment: " << endl;

	//double strike = 500.0;
	double strike = (stod(strike_str));  //strike price in string format for comparison when reading in the file
										 //string strike_str = "500";
										 //cout << strike << endl;

	//converting the date strings to date format and counting the business days using Quantlib calender functionalities
	Date t0_parse, tn_parse, texp_parse;
	Calendar usCal = UnitedStates();
	t0_parse = DateParser::parseFormatted(t0, "%Y-%m-%d");
	tn_parse = DateParser::parseFormatted(tn, "%Y-%m-%d");
	texp_parse = DateParser::parseFormatted(texp, "%Y-%m-%d");
	double businessDays = usCal.businessDaysBetween(t0_parse, texp_parse, true, true);
	cout << endl;
	//cout << "The number of Business Days from start time to expiration time using QuantLib is: " << businessDays << endl;
	
	//Reading Files
	vector<double> rate;
	vector<string> date;
	ifstream infile("Data/interest.csv");
	if (infile.fail())
	{
		cout << "File did not read correctly" << endl;
		exit(1);
	}

	string strDate;
	string strRate;
	while (!infile.eof()) //Looping through the CSV file
	{
		getline(infile, strDate, ',');
		getline(infile, strRate, '\n');
		if (isdigit(strDate[0]))
		{
			date.push_back(strDate);
			rate.push_back(stod(strRate)*0.01); //Changing percentages to actual values
		}
	}
	infile.close();


	//PRINTING THE CONTENTS OF THE INTEREST FILE TO SEE IT READ CORRECTLY
	/*
	for (int i = 0; i < date.size(); i++)
	{
	cout << date[i] << "  "<<rate[i] << endl;
	}
	*/

	//CALCULATE NUM OF BUSINESS DAYS BETWEEN T0 AND TEXP
	//This is another way to count business days but Quantlib way is better, easier and convenient
	/*
	double businessDays;
	int first;
	int last = 0;

	for (int i = 0; date[i] < texp; i++)
	{
	if (date[i] == t0)
	{
	first = i;
	}
	last += 1;
	}
	//cout << first << endl;
	//cout << last << endl;
	businessDays = last - first;
	//cout << businessDays << endl;
	*/

	//ONLY KEEPING REQUIRED DATES AND RATES that is Between T0 and TN
	vector<double> rate_new;
	vector<string> date_new;
	int flag_var = 0;
	for (unsigned int i = 0; i < date.size(); i++)
	{

		if (date[i] == t0)
		{
			flag_var = 1;
		}
		if (flag_var == 1)
		{
			rate_new.push_back(rate[i]);
			date_new.push_back(date[i]);
		}
		if (date[i] == tn)
		{
			flag_var = 0;
		}
	}

	//Checking this part by taking a print
	/*
	for (int i = 0; i < date_new.size(); i++)
	{
	cout << date_new[i] << "  "<<rate_new[i] << endl;
	}
	*/
	
	//Read 2nd File
	vector<double> close_rate;
	vector<string> date_Goog;
	ifstream file_goog("Data/sec_GOOG.csv");

	string strDate_;
	string strRate_;

	if (file_goog.fail())
	{
		cout << "File did not read correctly" << endl;
		exit(1);
	}


	while (!file_goog.eof()) //Looping through the CSV file
	{
		getline(file_goog, strDate_, ',');
		getline(file_goog, strRate_, '\n');
		if (isdigit(strDate_[0]))
		{
			date_Goog.push_back(strDate_);
			close_rate.push_back(stod(strRate_));
		}
	}
	file_goog.close();


	//PRINTING THE CONTENTS OF THE GOOGLE Stock Price FILE TO SEE IT READ CORRECTLY
	/*
	for (int i = 0; i < close_rate.size(); i++)
	{
	cout << date_Goog[i] << "  "<<close_rate[i] << endl;
	}
	*/

	//ONLY KEEPING REQUIRED DATES AND RATES that is Between T0 and Tn
	vector<double> close_rate_new;
	vector<string> date_Goog_new;
	int flag_varr = 0;
	for (unsigned int i = 0; i < close_rate.size(); i++)
	{
		if (date_Goog[i] == t0)
		{
			flag_varr = 1;
		}
		if (flag_varr == 1)
		{
			close_rate_new.push_back(close_rate[i]);
			date_Goog_new.push_back(date_Goog[i]);
		}
		if (date_Goog[i] == tn)
		{
			flag_varr = 0;
		}
	}

	//Printing this part to check if correct
	/*
	for (unsigned int i = 0; i < date_Goog_new.size(); i++)
	{
	cout << date_Goog_new[i] << "  "<<close_rate_new[i] << endl;
	}
	*/

	//Read 3rd File based on the User Input
	vector<string> buy_date;
	vector<string> ex_date;
	vector<string> cp_flag;
	vector<double> strike_price;
	vector<double> best_bid;
	vector<double> best_offer;
	vector<double> goog_option_price;

	ifstream file_opt("Data/op_GOOG.csv");

	string s1;
	string s2;
	string s3;
	string s4;
	string s5;
	string s6;

	if (file_opt.fail())
	{
		cout << "File did not read correctly" << endl;
		exit(1);
	}
	int flag_varrr = 0;
	while (!file_opt.eof()) //Looping through the CSV file
	{
		getline(file_opt, s1, ',');
		getline(file_opt, s2, ',');
		getline(file_opt, s3, ',');
		getline(file_opt, s4, ',');
		getline(file_opt, s5, ',');
		getline(file_opt, s6, '\n');

		if (s1 == t0)
		{
			flag_varrr = 1;
		}

		if (flag_varrr == 1)
		{
			if (s2 == texp)
			{
				if (s3 == "C")
				{
					if (s4 == strike_str)
					{
						buy_date.push_back(s1);
						ex_date.push_back(s2);
						cp_flag.push_back(s3);
						strike_price.push_back(stod(s4));
						best_bid.push_back(stod(s5));
						best_offer.push_back(stod(s6));
						goog_option_price.push_back((stod(s5) + stod(s6)) / 2);

						if (s1 == tn)
						{
							flag_varrr = 0;
						}
					}
				}
			}
		}

	}
	file_opt.close();

	/*
	for (unsigned int i = 0; i < buy_date.size(); i++)
	{
	cout << buy_date[i] << "  " << ex_date[i] << " " << cp_flag[i] << "  " << strike_price[i] << " " << best_bid[i] << "  " << best_offer[i] << " " << goog_option_price[i]<< endl;
	}
	*/

	//CREATING AN INSTANCE OF THE OPTION CLASS
	Options Opt_class;

	ofstream PartII("result_partII.csv"); //Writing output to CSV File	
	PartII << "Date" << "," << "Stock Price" << "," << "Option Price" << "," << "Implied Volatility" << "," << "Delta Value" << "," << "Cumulative Hedging Error" << "," << "Daily Hedging Error" << "," << "PNL without Hedging" << endl;

	double low_vol = 0.0001;
	double high_vol = 0.99;
	double epsilon = 0.001;
	vector<double> implied_vols;
	vector<double> deltas;
	vector<double> betas;
	vector<double> H_errors;
	vector<double> D_H_errors;
	vector<double> pnl;
	double D_H_errors_temp = 0;
	double implied_vol_temp;
	double time = (businessDays / 252);
	//cout << "TIME IS: " << time << endl;
	double dividend = 0.0;
	double dt = 1.0 / 252.0; // 1/252;
								   //exit(1);
	for (unsigned int i = 0; i < date_new.size(); i++)
	{
		//DATE
		//cout << "DATE : " << date_new[i] << endl;
		PartII << date_new[i] << ",";

		//Stock Price
		//cout << "Stock Price : " << close_rate_new[i] << endl;
		PartII << close_rate_new[i] << ",";

		//Option Price
		//cout << "Option Price: " << goog_option_price[i] << endl;
		PartII << goog_option_price[i] << ",";
		//PartII << goog_option_price[i] << endl;

		//Implied Volatility
		//cout << goog_option_price[i] << '\n' << low_vol << '\n' << high_vol << '\n' << epsilon << '\n' << "call" << '\n' << strike << '\n' << time << '\n' << close_rate_new[i] << '\n' << rate_new[i] << '\n' << dividend << endl;
		implied_vol_temp = interval_bisection(goog_option_price[i], low_vol, high_vol, epsilon, "call", strike, time, close_rate_new[i], rate_new[i], dividend);
		implied_vols.push_back(implied_vol_temp);
		//cout << "Implied Volatility: " << implied_vols[i] << endl;
		PartII << implied_vols[i] << ",";

		//Delta	
		deltas.push_back(Opt_class.call_delta("call", implied_vols[i], strike, time, close_rate_new[i], rate_new[i], dividend));
		//cout << "Delta Value : " << deltas[i] << endl;
		PartII << deltas[i] << ",";

		//Beta
		if (i == 0)
		{
			betas.push_back(goog_option_price[i] - (deltas[i] * close_rate_new[i]));
		}
		else {
			betas.push_back((deltas[i - 1] * close_rate_new[i]) + (betas[i - 1] * exp(rate_new[i - 1] * dt)) - (deltas[i] * close_rate_new[i]));
		}
		//cout << "Beta Value : " << betas[i] << endl;	

		//Calculate Cumulative Hedge Error
		if (i == 0)
		{
			H_errors.push_back(0.0);
		}
		else {
			H_errors.push_back((deltas[i - 1] * close_rate_new[i]) + (betas[i - 1] * exp(rate_new[i - 1] * dt)) - (goog_option_price[i]));
		}
		//cout << "Cumulative Hedging Error: " << H_errors[i] << endl;
		PartII << H_errors[i] << ",";

		//Calculate Daily Hedge Error
		if (i == 0)
		{
			D_H_errors_temp = H_errors[i];
		}
		else {
			D_H_errors_temp = H_errors[i] - H_errors[i - 1];
		}
		D_H_errors.push_back(D_H_errors_temp);
		//cout << "Daily Hedge Error: " << D_H_errors[i] << endl;
		PartII << D_H_errors[i] << ",";

		//PNL without Hedge calculation
		pnl.push_back((goog_option_price[0] - goog_option_price[i]));
		//cout << "PNL without hedge" << pnl[i] << endl;
		PartII << pnl[i] << endl;

		//cout << endl;
		time = time - dt; //time = time - 1/252;
		 //cout << time << endl;
	}
	PartII.close();	
	cout << "RESULTS OF PART II AVAILABLE IN GENERATED CSV FILE NAMED 'results_partII.CSV'" << endl;
	cout << "THANKS !!" << endl;
return 0;
}