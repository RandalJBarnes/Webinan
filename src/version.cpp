//=============================================================================
// version.cpp
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    26 June 2017
//=============================================================================
#include <iostream>

#include "version.h"

namespace
{
   const char* VERSION = "26 June 2017 [Beta2]";
}

//-----------------------------------------------------------------------------
void Banner( std::ostream& ost)
{
   ost <<
      "-------------------------------------------- \n"
      "Webinan (" << VERSION << ") \n"
      "\n"
      "R. Barnes, University of Minnesota \n"
      "R. Soule,  Minnesota Department of Health \n"
      "-------------------------------------------- \n"
   << std::endl;
}

//-----------------------------------------------------------------------------
void Help()
{
   Version();

   std::cout <<
      "   Identify potential outliers using a geostatistical framework. \n"
   << std::endl;

   std::cout <<
      "   Webinan is a simple, pragmatic, heuristic algorithm. One-by-one we \n"
      "   visit each observation. We temporarily remove this observation from \n"
      "   the data set. We also temporarily remove all of the observations in \n"
      "   the immediate vicinity of this observation. With this reduced data \n"
      "   set we interpolate the value at this observation location using \n"
      "   Ordinary Kriging. \n"
      "\n"
      "   We compare the interpolated value with the observed value. If they \n"
      "   are significantly different, the observation is a potential outlier. \n"
   << std::endl;

   Usage();

   std::cout <<
      "Arguments: \n"
      "   <nugget>        The <nugget>, or 'nugget effect', is the discontinuity \n"
      "                   in the variogram at a lag of 0. The <nugget> quantifies \n"
      "                   the variance of the sampling and measurement errors and \n"
      "                   the hyper-local spatial variation. The units of the \n"
      "                   <nugget> are the units of the observed values squared. \n"
      "                   The <nugget> must be a strictly positive value. \n"
      "\n"
      "   <sill>          The <sill> is the value (height) at which variogram \n"
      "                   levels out. With the exponential model used in Webinan, \n"
      "                   the variogram approaches the <sill> asymptotically. \n"
      "                   In the common geostatistical framework (i.e. a second \n"
      "                   order stationary model), the sill equals the variance \n"
      "                   of the underlying population. The units of the <sill> \n"
      "                   are the units of the observed values squared. The <sill> \n"
      "                   must be a strictly positive value. \n"
      "\n"
      "   <range>         The <range> is the separation distance at which we \n"
      "                   model two observations as essentially uncorrelated. \n"
      "                   With the exponential model used in Webinan, this is \n"
      "                   the separation distance at which the variogram reaches \n"
      "                   95% of the <sill>. The units of the <range> are the \n"
      "                   units of the observations locations. The <range> must \n"
      "                   be a strictly positive value. \n"
      "\n"
      "   <radius>        The <radius> is the radius of the circle defining the \n"
      "                   'immediate vicinity' of each observation. When we \n"
      "                   interpolate the value at an observation location we \n"
      "                   exclude all of the observations that fall within a \n"
      "                   circle centered on the observation location with a \n"
      "                   radius equal to <radius>. The units of the <radius> are \n"
      "                   the units of the observations locations. The <radius> \n"
      "                   must be a non-negative value. \n"
      "\n"
      "   <obs filename>  The <obs filename> is the name of the file (including any \n"
      "                   necessary path information and the .csv file extension) \n"
      "                   containing the observation data. \n"
      "\n"
      "   <out filename>  The <out filename> is the name of the file (including \n"
      "                   any necessary path information and the file extension) \n"
      "                   where Webinan will write all of the program results. \n"
      "                   If the specified <output file> already exists, it will \n"
      "                   be overwritten. (See below.) \n"
   << std::endl;

   std::cout <<
      "Example: \n"
      "   Webinan 3 25 3500 50 input.csv output.csv \n"
   << std::endl;

   std::cout <<
      "Observation File: \n"
      "   All of the observation head data are supplied by this .csv file. \n"
      "\n"
      "   The observation file contains no header line. The observation file may \n"
      "   include blank lines, which are ignored. The observation may include \n"
      "   comment lines, which are identified by an octothorpe (#) in the first \n"
      "   column of the line. \n"
      "\n"
      "   The observation file contains one line for each head observation. Each \n"
      "   line in the observation file has four fields. \n"
      "\n"
      "   <ID>            The observation identification string. The ID string can \n"
      "                   contain numbers, letters, underscores, and internal spaces. \n"
      "                   The ID may not contain commas. \n"
      "\n"
      "   <x>             The x-coordinate [L] of observation location. \n"
      "\n"
      "   <y>             The y-coordinate [L] of observation location. \n"
      "\n"
      "   <z>             The observation value. at location (x,y). \n"
      "\n"
      "   Each of the four fields must separated by a single comma. Spaces and tabs \n"
      "   at the start and end of fields are trimmed. \n"
   << std::endl;

   std::cout <<
      "Output File: \n"
      "   All of the program results are written to the output .csv file. \n"
      "\n"
      "   The output file contains one header line with nine comma separated \n"
      "   text fields containing the field titles. \n"
      "\n"
      "   The rest of the output file comprises one line for each observation. \n"
      "   Each line has nine fields. \n"
      "\n"
      "   <ID>            The observation identification string. \n"
      "\n"
      "   <X>             The x-coordinate for the location of the observation. \n"
      "\n"
      "   <Y>             The y-coordinate for the location of the observation. \n"
      "\n"
      "   <Z>             The observed value at location (X,Y). \n"
      "\n"
      "   <Count>         The number of observations used in the interpolation. \n"
      "                   The <Count> is the number of observations remaining \n"
      "                   after excluding the observations within the immediate \n"
      "                   vicinity of the observation location. \n"
      "\n"
      "   <Zhat>          The interpolated value at the observation location. \n"
      "\n"
      "   <Kstd>          The 'standard error' of the interpolated value at the \n"
      "                   observation location. The <Kstd> is the square root \n"
      "                   Ordinary Kriging variance. \n"
      "\n"
      "   <Zeta>          The normalized interpolation error. \n"
      "                      <Zeta> = (<Z> - <Zhat>)/<Kstd>. \n"
      "\n"
      "   <pValue>        A small <pValue> indicates a potential outlier; e.g. \n"
      "                   a <pValue> less than 0.01. The <pValue> is defined as \n"
      "                   the probability that a standard normal random variable \n"
      "                   is more extreme than <Zeta>. \n"
      "                     if <Zeta> < 0 then \n"
      "                        <pValue> = Pr( standard normal < <Zeta> ) \n"
      "                     else if <Zeta> > 0 then \n"
      "                        <pValue> = Pr( standard normal > <Zeta> ) \n"
   << std::endl;

   std::cout <<
      "Notes: \n"
      "   o  An exponential semivariogram model is used. \n"
      "         gamma(h) = <nugget> + (<sill>-<nugget>)*(1 - exp(-3h/<range>)) \n"
      "\n"
      "   o  The set of observations may include 'duplicates', i.e. multiple \n"
      "      observation values at the same location. \n"
      "\n"
      "   o  The project name 'Webinan' is the Ojibwe word for the inanimate \n"
      "      transitive verb 'throw it away'. See [http://ojibwe.lib.umn.edu]. \n"
      "      This name seems appropriate for a program used to identify potential \n"
      "      outliers. \n"
   << std::endl;

   std::cout <<
      "Authors: \n"
      "   R. Barnes, University of Minnesota \n"
      "   R. Soule,  Minnesota Department of Health \n"
   << std::endl;
}

//-----------------------------------------------------------------------------
void Usage()
{
   std::cout <<
      "Usage: \n"
      "   Webinan <nugget> <sill> <range> <radius> <input file> <output file> \n"
      "   Webinan --help \n"
      "   Webinan --version \n"
   << std::endl;
}

//-----------------------------------------------------------------------------
void Version()
{
   std::cout << std::endl;
   std::cout << "Webinan (" << VERSION << ")" << std::endl;
   std::cout << std::endl;
}
