//=============================================================================
// read_data.h
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    15 June 2017
//=============================================================================
#ifndef READ_DATA_H
#define READ_DATA_H

#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

//-----------------------------------------------------------------------------
class InvalidInputFile : public std::runtime_error {
   public :
      InvalidInputFile( const std::string& message ) : std::runtime_error(message) {
      }
};

class InvalidDataRecord : public std::runtime_error {
   public :
      InvalidDataRecord( const std::string& message ) : std::runtime_error(message) {
      }
};

//-----------------------------------------------------------------------------
struct DataRecord{
   std::string id;
   double x;
   double y;
   double z;
};

std::vector<DataRecord> read_data( const std::string& inpfilename );


//=============================================================================
#endif  // READ_DATA_H
