//=============================================================================
// main.cpp
//
//    The driver for the command line version of the user interface.
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    13 June 2017
//=============================================================================
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "engine.h"
#include "now.h"
#include "numerical_constants.h"
#include "version.h"


//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
   // Check the command line.
   if( argc == 1 )
   {
      Usage();
      return 0;
   }
   else if( argc == 2 )
   {
      if( strcmp(argv[1], "--help") == 0 )
         Help();
      else if( strcmp(argv[1], "--version") == 0 )
         Version();
      else
         Usage();

      return 0;
   }
   else if( argc == 7 )
   {
      Banner( std::cout );
   }
   else
   {
      Usage();
      return 1;
   }

   // Get and check the semi-variogram nugget effect.
   double nugget = atof( argv[1] );
   if( nugget <= EPS )
   {
      std::cerr << "ERROR: nugget = " << argv[1] << " is not valid;  0 < nugget." << std::endl;
      std::cerr << std::endl;
      Usage();
      return 2;
   }

   // Get and check the semi-variogram sill.
   double sill = atof( argv[2] );
   if( sill <= EPS )
   {
      std::cerr << "ERROR: sill = " << argv[2] << " is not valid;  0 < sill." << std::endl;
      std::cerr << std::endl;
      Usage();
      return 2;
   }

   // Get and check the semi-variogram range.
   double range = atof( argv[3] );
   if( range <= EPS )
   {
      std::cerr << "ERROR: range = " << argv[3] << " is not valid;  0 < range." << std::endl;
      std::cerr << std::endl;
      Usage();
      return 2;
   }

   // Get and check the buffer radius.
   double radius = atof( argv[4] );
   if( radius < 0 )
   {
      std::cerr << "ERROR: buffer radius = " << argv[4] << " is not valid;  0 <= radius." << std::endl;
      std::cerr << std::endl;
      Usage();
      return 2;
   }

   // Open the specified data file.
   std::string inpfilename = argv[5];
   std::ifstream inpfile( argv[5] );
   if( inpfile.fail() )
   {
      std::cerr << "ERROR: could not open the specified input file <" << argv[5] << "> for input." << std::endl;
      Usage();
      return 3;
   }

   // Open the specified output file.
   std::string outfilename = argv[6];
   std::ofstream outfile( outfilename );
   if( outfile.fail() )
   {
      std::cerr << "ERROR: could not open the output file <" << outfilename << "> for output." << std::endl;
      Usage();
      return 4;
   }

   // Read in the observation data from the specified data file.
   std::vector<int>   id;
   std::vector<double> x;
   std::vector<double> y;
   std::vector<double> z;

   std::string line;
   int ii;
   double xx, yy, zz;

   while( std::getline(inpfile, line) )
   {
      std::replace( line.begin(), line.end(), ',', ' ' );

      std::istringstream is(line);
      if( is >> ii >> xx >> yy >> zz )
      {
         id.push_back(ii);
         x.push_back(xx);
         y.push_back(yy);
         z.push_back(zz);
      }
      else
      {
         std::cerr << "ERROR -- reading the observation data failed on line "
            << z.size()+1 << " of file " << inpfilename << "." << std::endl;
         inpfile.close();
         outfile.close();
         return 5;
      }
   }
   inpfile.close();

   int N = x.size();
   std::cout << N << " data records read from <" << inpfilename << ">." << std::endl;

   // Execute all of the computations.
   std::vector<Boomerang> results;
   try
   {
       results = Engine(x, y, z, nugget, sill, range, radius);
   }
   catch(...)
   {
      std::cerr << "ERROR -- the Webinan Engine failed for an unknown reason." << std::endl;
      outfile.close();
      throw;
   }

   // Write out the header lines to the <output file>.
   outfile << Now() << std::endl;
   outfile << "<nugget>       " << nugget       << std::endl;
   outfile << "<sill>         " << sill         << std::endl;
   outfile << "<range>        " << range        << std::endl;
   outfile << "<input file>   " << inpfilename  << std::endl;
   outfile << "<output file>  " << outfilename  << std::endl;
   outfile << std::endl;
   outfile << "------------------------------------------------------------------------------------------------------------" << std::endl;
   outfile << "     Well_ID           X           Y           Z       Count        Zhat        Kstd        Zeta      pValue" << std::endl;
   outfile << "------------------------------------------------------------------------------------------------------------" << std::endl;
   // Fill the <output file> with the observation-by-observation results.
   for( int n=1; n<N; ++n )
   {
      outfile << std::fixed << std::setw(12)                         << id[n];
      outfile << std::fixed << std::setw(12) << std::setprecision(0) << x[n];
      outfile << std::fixed << std::setw(12) << std::setprecision(0) << y[n];
      outfile << std::fixed << std::setw(12) << std::setprecision(2) << z[n];
      outfile << std::fixed << std::setw(12)                         << results[n].cnt;
      outfile << std::fixed << std::setw(12) << std::setprecision(2) << results[n].zhat;
      outfile << std::fixed << std::setw(12) << std::setprecision(2) << results[n].kstd;
      outfile << std::fixed << std::setw(12) << std::setprecision(2) << results[n].zeta;
      outfile << std::fixed << std::setw(12) << std::setprecision(3) << results[n].pvalue;
      outfile << std::endl;
   }
   outfile.close();
   std::cout << "Output file <" << outfilename << "> created. " << std::endl;

   // Successful termination.
   double elapsed = static_cast<double>(clock())/CLOCKS_PER_SEC;
   std::cout << "elapsed time: " << std::fixed << elapsed << " seconds." << std::endl;
   std::cout << std::endl;

   // Terminate execution.
	return 0;
}
