#ifndef STDNORMALCDF_H_INCLUDED
#define STDNORMALCDF_H_INCLUDED

class StdNormalCDF
{
private:
    double A1;
    double A2;
    double A3;
    double A4;
    double A5;
    double B;
    double NORMALIZER;
public:
    StdNormalCDF();
    double operator() (double x);
};

#endif // STDNORMALCDF_H_INCLUDED
