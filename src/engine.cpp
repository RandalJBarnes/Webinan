//=============================================================================
// engine.cpp
//
//    Compute the boomerang statistic for each measured location using the
//    user-specified semi-variogram model.
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    11 June 2017
//=============================================================================
#include <cassert>
#include <iomanip>
#include <math.h>
#include <numeric>

#include "engine.h"
#include "matrix.h"
#include "linear_systems.h"
#include "special_functions.h"

//=============================================================================
//
//=============================================================================
std::vector<Boomerang> Engine(
   const std::vector<double>x,
   const std::vector<double>y,
   const std::vector<double>z,
   double nugget,
   double sill,
   double range,
   double radius )
{
   // Manifest constants.
   const int MINIMUM_COUNT = 10;
   const int N = x.size();     // number of observations.
   assert(N > 1);

   // Pre-compute the separation distance matrix for all of the observations.
   Matrix D(N, N);
   for( int i=0; i<N-1; ++i )
   {
      for( int j=i+1; j<N; ++j )
      {
         D(i,j) = _hypot( x[i]-x[j], y[i]-y[j] );
         D(j,i) = D(i,j);
      }
   }

   // Pre-compute the covariance matrix for all of the observations.
   Matrix C(N, N, sill);
   for( int i=0; i<N-1; ++i )
   {
      for( int j=i+1; j<N; ++j )
      {
         C(i,j) = (sill-nugget)*exp(-3.0*D(i,j)/range);
         C(j,i) = C(i,j);
      }
   }

   // Create the matrix of observed values.
   Matrix ZZ(z);

   // Pass through the set of observations one at a time.
   std::vector<Boomerang> results(N);

   for( int k=0; k<N; ++k )
   {
      // Determine the active subset of the observations for the location of
      // observation [k]; i.e. those observations outside of the buffer radius.
      std::vector<int> current(N, 0);
      current[k] = 1;

      std::vector<int> first(1, 0);
      first[0] = 1;

      std::vector<int> active(N);
      for( int j=0; j<N; ++j)
      {
         if( D(k,j) > radius )
            active[j] = 1;
         else
            active[j] = 0;
      }
      int M = std::accumulate(active.begin(), active.end(), int(0));

      if( M < MINIMUM_COUNT )
      {
         results[k].zhat   = NAN;
         results[k].kstd   = NAN;
         results[k].zeta   = NAN;
         results[k].pvalue = NAN;
         results[k].cnt    = 0;
         continue;
      }

      // Setup the Ordinary Kriging system for the location of observation [k]
      // using only the active data.
      Matrix A;
      Slice(C, active, active, A);

      Matrix b;
      Slice(C, active, current, b);

      Matrix Z;
      Slice(ZZ, active, first, Z);

      // Solve the Ordinary Kriging system.
      Matrix L, u, v, lv, w;
      Matrix ones(M, 1, 1.0);

      if( CholeskyDecomposition(A,L) )
      {
         CholeskySolve(L,b,u);
         CholeskySolve(L,ones,v);

         double lambda = ( Sum(u) - 1 ) / Sum(v);

         Multiply_aM( lambda, v, lv );
         Subtract_MM( u, lv, w );

         double zhat = DotProduct(w,Z);
         double kstd = sqrt( sill - DotProduct(b,w) - lambda );

         results[k].zhat = zhat;
         results[k].kstd = kstd;
         results[k].zeta = (z[k]-zhat) / kstd;

         if( results[k].zeta < 0 )
            results[k].pvalue = GaussianCDF(results[k].zeta);
         else
            results[k].pvalue = 1 - GaussianCDF(results[k].zeta);

         results[k].cnt  = M;
      }
      else
      {
         std::cerr << "WARNING: Cholesky Decompositon failed " << k << std::endl;
         exit(5);
      }
   }
   return results;
}
