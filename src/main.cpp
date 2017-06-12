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
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

#include "engine.h"
#include "now.h"
#include "numerical_constants.h"
#include "version.h"


namespace{
   //--------------------------------------------------------------------------
   //
   //--------------------------------------------------------------------------
   void Usage()
   {
      std::cerr << std::endl;
      std::cerr << "Usage:" << std::endl;
      std::cerr << "   Webinan <nugget> <sill> <range> <radius> <filename>" << std::endl;
      std::cerr << std::endl;
   }

   //--------------------------------------------------------------------------
   //
   //--------------------------------------------------------------------------
   void PrintVersion()
   {
      std::cerr << std::endl;
      std::cerr << "Webinan (" << Version() << ")" << std::endl;
      std::cerr << std::endl;
   }

   //--------------------------------------------------------------------------
   //
   //--------------------------------------------------------------------------
   void Help()
   {
      Usage();
      std::cerr << std::endl;
      std::cerr << "Arguments" << std::endl;
      std::cerr << "   <nugget>        " << std::endl;
      std::cerr << "   <sill>          semivariogram sill [z^2]" << std::endl;
      std::cerr << "   <range>         " << std::endl;
      std::cerr << "   <radius>        " << std::endl;
      std::cerr << "   <filename>     " << std::endl;
      std::cerr << std::endl;
      std::cerr << "Output" << std::endl;
      std::cerr << "   <Well_ID>       " << std::endl;
      std::cerr << "   <X>             " << std::endl;
      std::cerr << "   <Y>             " << std::endl;
      std::cerr << "   <Z>             " << std::endl;
      std::cerr << "   <Count>         " << std::endl;
      std::cerr << "   <Zhat>          " << std::endl;
      std::cerr << "   <Kstd>          " << std::endl;
      std::cerr << "   <Zeta>          " << std::endl;
      std::cerr << "   <pValue>        " << std::endl;
      std::cerr << std::endl;
      std::cerr << "Example" << std::endl;
      std::cerr << "   Webinan 2 15 2500 100 Input.txt" << std::endl;
      std::cerr << std::endl;
      std::cerr << "Authors" << std::endl;
      std::cerr << std::endl;
      std::cerr << "Copyright" << std::endl;
      std::cerr << std::endl;
   }

   //--------------------------------------------------------------------------
   //
   //--------------------------------------------------------------------------
   void Banner( std::ostream& ost)
   {
      ost << "================================================="  << std::endl;
      ost << "Webinan                      (" << Version() << ')' << std::endl;
      ost                                                         << std::endl;
      ost << "R. Barnes, University of Minnesota               "  << std::endl;
      ost << "R. Soule,  Minnesota Department of Health        "  << std::endl;
      ost << "================================================="  << std::endl;
   }
}


//-----------------------------------------------------------------------------
//
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
         PrintVersion();
      else
         Usage();

      return 0;
   }
   else if( argc == 6 )
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
   if( radius <= EPS )
   {
      std::cerr << "ERROR: buffer radius = " << argv[4] << " is not valid;  0 < radius." << std::endl;
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
   std::string outfilename = "Webinan.out";
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
   std::cout << std::endl << N << " data records read from <" << argv[1] << ">. \n";

   // Fill the output file with the results.
   std::vector<Boomerang> results = Engine(x, y, z, nugget, sill, range, radius);

   outfile << "     Well_ID           X           Y           Z       Count        Zhat        Kstd        Zeta      pValue" << std::endl;
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

   // Successful termination.
   double elapsed = static_cast<double>(clock())/CLOCKS_PER_SEC;
   std::cout << std::endl << "elapsed time: " << std::fixed << elapsed << " seconds." << std::endl;

   // Terminate execution.
	return 0;
}
