//=============================================================================
// read_data.cpp
//
//    Read in the observation data from the user-specified file.
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    15 June 2017
//=============================================================================
#include <algorithm>
#include <fstream>
#include <sstream>

#include "../include/csv.h"

#include "read_data.h"

//-----------------------------------------------------------------------------
InputData read_data( const std::string& inpfilename ) {
   // Open the specified input data file.
   std::ifstream inpfile( inpfilename );
   if (inpfile.fail()) {
      std::stringstream message;
      message << "Could not open <" << inpfilename << "> for input.";
      throw InvalidInputFile(message.str());
   }

   // Get the input data.
   std::vector<std::string> headers = {"A", "B", "C", "D"};
   std::vector<DataRecord> obs;

   std::string line;
   while( std::getline(inpfile, line) ) {
      std::replace( line.begin(), line.end(), ',', ' ' );
      std::istringstream is(line);

      std::string ii;
      double xx, yy, zz;

      if ( is >> ii >> xx >> yy >> zz ) {
         DataRecord s = { ii, xx, yy, zz };
         obs.push_back(s);
      }
      else {
         inpfile.close();
         std::stringstream message;
         message << "Reading the observation data failed on line " << obs.size()+1 << " of file " << inpfilename << ".";
         throw InvalidDataRecord(message.str());
      }
   }
   inpfile.close();

   return std::make_tuple( headers, obs );
}
