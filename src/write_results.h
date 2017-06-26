//=============================================================================
// write_results.h
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    26 June 2017
//=============================================================================
#ifndef WRITE_RESULTS_H
#define WRITE_RESULTS_H

#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

//-----------------------------------------------------------------------------
class InvalidOutputFile : public std::runtime_error {
   public :
      InvalidOutputFile( const std::string& message ) : std::runtime_error(message) {
      }
};

//-----------------------------------------------------------------------------
void write_results( const std::string& outfilename, std::vector<DataRecord> obs, std::vector<Boomerang> results );


//=============================================================================
#endif  // WRITE_RESULTS_H
