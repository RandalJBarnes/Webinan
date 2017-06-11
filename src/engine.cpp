//=============================================================================
// engine.cpp
//
//    Compute the boomerang statistic for each measured location using a
//    simple linear variogram model.
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    11 June 2017
//=============================================================================
#include "engine.h"
#include "special_functions.h"
#include "matrix.h"
#include "linear_systems.h"

#include <numeric>
#include <math.h>
#include <iomanip>
#include <cassert>

//=============================================================================
//
//=============================================================================
std::vector<Boomerang> Engine(
   const std::vector<double>x,
   const std::vector<double>y,
   const std::vector<double>z,
   double radius )
{
   // Manifest constants.
   const int MINIMUM_COUNT = 10;
   const int N = x.size();     // number of observations.
   assert( N>1 );

   std::vector<Boomerang> results(N);

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

   Matrix Z(N, 1);
   Matrix Xi(N, 1);

   for( int k=0; k<N; ++k )
      Z(k,0) = z[k];

   double lambda = MaxAbs(D);

   // Pass through the set of observations one at a time.
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
         results[k].zeta   = NAN;
         results[k].pvalue = NAN;
         results[k].cnt    = 0;
         continue;
      }

      // Setup the Ordinary Kriging system for the location of observation [k].
      Matrix A, B;
      Slice(D, active, active, A);
      Subtract_aM(lambda, A, B);

      Matrix b, c;
      Slice(D, active, current, b);
      Subtract_aM(lambda, b, c);

      Matrix zactive;
      Slice(Z, active, first, zactive);

      // Solve the Ordinary Kriging system.
      Matrix L, u, v, bv, w;
      Matrix ones(M, 1, 1.0);

      if( CholeskyDecomposition(B,L) )
      {
         CholeskySolve(L,c,u);
         CholeskySolve(L,ones,v);

         double beta = ( Sum(u) - 1 ) / Sum(v);

         Multiply_aM( beta, v, bv );
         Subtract_MM( u, bv, w );

         double zhat = DotProduct(w,zactive);
         double tau2 = lambda - DotProduct(c,w) - beta;
         Xi(k,0) = ( z[k]-zhat ) / sqrt(tau2);

         results[k].zhat = zhat;
         results[k].cnt  = M;
      }
      else
      {
         std::cerr << "WARNING: Cholesky Decompositon failed " << k << std::endl;
      }
   }

   // Normalize the xi to account for the unknown variogram slope.
   double stdXi = sqrt( DotProduct(Xi, Xi) / N );
   for( int k=0; k<N; ++k)
   {
      results[k].zeta = Xi(k,0)/stdXi;

      if( results[k].zeta < 0 )
         results[k].pvalue = GaussianCDF(results[k].zeta);
      else
         results[k].pvalue = 1 - GaussianCDF(results[k].zeta);
   }
   return results;
}
