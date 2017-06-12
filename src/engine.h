//=============================================================================
// aakozi_engine.h
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    12 June 2017
//=============================================================================
#ifndef ENGINE_H
#define ENGINE_H

#include <vector>

//=============================================================================
// Boomerang
//=============================================================================
struct Boomerang
{
   double zhat;
   double kstd;
   double zeta;
   double pvalue;
   int    cnt;
};

//=============================================================================
std::vector<Boomerang> Engine(
   const std::vector<double>x,
   const std::vector<double>y,
   const std::vector<double>z,
   double nugget,
   double sill,
   double range,
   double radius );


//=============================================================================
#endif  // ENGINE_H
