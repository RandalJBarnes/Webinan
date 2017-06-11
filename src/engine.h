//=============================================================================
// aakozi_engine.h
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    11 June 2017
//=============================================================================
#ifndef AAKOZI_ENGINE_H
#define AAKOZI_ENGINE_H

#include <vector>

//=============================================================================
// Boomerang
//=============================================================================
struct Boomerang
{
   double   zhat;
   double   zeta;
   double   pvalue;
   int      cnt;
};

//=============================================================================
std::vector<Boomerang> Engine( const std::vector<double>x, const std::vector<double>y, const std::vector<double>z, double radius );


//=============================================================================
#endif  // AAKOZI_ENGINE_H
