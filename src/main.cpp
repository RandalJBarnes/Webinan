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
//    11 June 2017
//=============================================================================
#include <vector>
#include <set>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <time.h>

#include "engine.h"
#include "version.h"
#include "now.h"


namespace{
   //--------------------------------------------------------------------------
   //
   //--------------------------------------------------------------------------
   void Usage()
   {
      std::cerr << std::endl;
      std::cerr << "Webinan (" << Version() << ')'      << std::endl;
      std::cerr << "Usage: Webinan <filename> <Co> <C> <A> <radius>" << std::endl;
      std::cerr << std::endl;
   }

   //--------------------------------------------------------------------------
   //
   //--------------------------------------------------------------------------
   void Banner( std::ostream& ost)
   {
      ost << "================================================="  << std::endl;
      ost << "Webinan                      (" << Version() << ')' << std::endl;
      ost << "================================================="  << std::endl;
   }
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
   // Check the command line.
   if( argc != 3 )
   {
      Usage();
      return 1;
   }
   else
   {
      Banner( std::cout );
   }

   // Get and check the buffer radius.
   double radius = atof( argv[2] );
   if( radius <= 0.0 )
   {
      std::cerr << "ERROR: buffer radius = " << argv[2] << " is not valid;  0 < radius." << std::endl;
      std::cerr << std::endl;
      Usage();
      return 2;
   }

   // Open the specified data file.
   std::string inpfilename = argv[1];
   std::ifstream inpfile( argv[1] );
   if( inpfile.fail() )
   {
      std::cerr << "ERROR: could not open the specified input file <" << argv[1] << "> for input." << std::endl;
      Usage();
      return 3;
   }

   // Open the specified output file.
   std::string outfilename = "Webinan.out";
   std::ofstream outfile( outfilename );
   if( outfile.fail() )
   {
      std::cerr << "ERROR: could not open the output file <" << outfilename << "> for output." << std::endl;
      Usage();
      return 4;
   }

   // Read in the observation data from the specified data file.
   std::vector<double> x;
   std::vector<double> y;
   std::vector<double> z;
   std::vector<int>   id;

   std::string line;

   double xx, yy, zz;
   int ii;
   while( std::getline(inpfile, line) )
   {
      std::istringstream is(line);
      if( is >> ii >> xx >> yy >> zz )
      {
         id.push_back(ii);
         x.push_back(xx);
         y.push_back(yy);
         z.push_back(zz);
      }
   }
   inpfile.close();

   int N = x.size();
   std::cout << std::endl << N << " data read from <" << argv[1] << ">. \n";

   // Fill the output file with the results.
   std::vector<Boomerang> results = Engine(x,y,z,radius);

   for( int n=1; n<N; ++n )
   {
      outfile << std::fixed << std::setw(12)                         << id[n];
      outfile << std::fixed << std::setw(12) << std::setprecision(2) << x[n];
      outfile << std::fixed << std::setw(12) << std::setprecision(2) << y[n];
      outfile << std::fixed << std::setw(12) << std::setprecision(2) << z[n];
      outfile << std::fixed << std::setw(12) << std::setprecision(2) << results[n].zhat;
      outfile << std::fixed << std::setw(12) << std::setprecision(2) << results[n].zeta;
      outfile << std::fixed << std::setw(12) << std::setprecision(3) << results[n].pvalue;
      outfile << std::fixed << std::setw(12)                         << results[n].cnt;
      outfile << std::endl;
   }
   inpfile.close();

   // Successful termination.
   double elapsed = static_cast<double>(clock())/CLOCKS_PER_SEC;
   std::cout << std::endl << "elapsed time: " << std::fixed << elapsed << " seconds." << std::endl;

   // Terminate execution.
	return 0;
}
