//=============================================================================
// write_results.cpp
//
//    Read in the observation data from the user-specified file.
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    26 June 2017
//=============================================================================
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "engine.h"
#include "write_results.h"

//-----------------------------------------------------------------------------
void write_results( const std::string& outfilename, std::vector<DataRecord> obs, std::vector<Boomerang> results ) {

   // Open the specified output file.
   std::ofstream outfile( outfilename );
   if ( outfile.fail() ) {
      std::stringstream message;
      message << "Could not open <" << outfilename << "> for output.";
      throw InvalidOutputFile(message.str());
   }

   // Write out the header line to the output file.
   outfile << "ID,X,Y,Z,Count,Zhat,Kstd,Zeta,pValue" << std::endl;

   // Fill the output file with the observation-by-observation results using
   // a maximum precision .csv format.
   outfile << std::setprecision(std::numeric_limits<long double>::digits10 + 1);

   for ( unsigned n = 0; n < obs.size(); ++n ) {
      outfile << obs[n].id << ',';
      outfile << obs[n].x  << ',';
      outfile << obs[n].y  << ',';
      outfile << obs[n].z  << ',';
      outfile << results[n].cnt  << ',';
      outfile << results[n].zhat << ',';
      outfile << results[n].kstd << ',';
      outfile << results[n].zeta << ',';
      outfile << results[n].pvalue;
      outfile << std::endl;
   }
   outfile.close();
}
