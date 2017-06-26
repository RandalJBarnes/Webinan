//=============================================================================
// engine.h
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    26 June 2017
//=============================================================================
#ifndef ENGINE_H
#define ENGINE_H

#include <stdexcept>
#include <vector>

#include "read_data.h"


//-----------------------------------------------------------------------------
struct Boomerang {
   double zhat;
   double kstd;
   double zeta;
   double pvalue;
   int    cnt;
};

//-----------------------------------------------------------------------------
std::vector<Boomerang> Engine(
   double nugget,
   double sill,
   double range,
   double radius,
   std::vector<DataRecord> obs
);


//=============================================================================
#endif  // ENGINE_H
