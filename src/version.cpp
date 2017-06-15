//=============================================================================
// version.cpp
//
//    A simple version reporting class.
//
// author:
//    Dr. Randal J. Barnes
//    Department of Civil, Environmental, and Geo- Engineering
//    University of Minnesota
//
// version:
//    15 June 2017
//=============================================================================
#include <iostream>

#include "version.h"

namespace
{
   const char* VERSION = "15 June 2017 [beta1]";
}

//-----------------------------------------------------------------------------
void Banner( std::ostream& ost)
{
   ost                                                    << std::endl;
   ost << "--------------------------------------------"  << std::endl;
   ost << "Webinan (" << VERSION << ")"                   << std::endl;
   ost                                                    << std::endl;
   ost << "R. Barnes, University of Minnesota          "  << std::endl;
   ost << "R. Soule,  Minnesota Department of Health   "  << std::endl;
   ost << "--------------------------------------------"  << std::endl;
}

//-----------------------------------------------------------------------------
void Help()
{
   Version();
   std::cout << "   Identify potential outliers using a geostatistical framework."              << std::endl;
   std::cout << std::endl;
   std::cout << "   Webinan is a simple, pragmatic, heuristic algorithm. One-by-one we "        << std::endl;
   std::cout << "   visit each observation. We temporarily remove this observation from "       << std::endl;
   std::cout << "   the data set. We also temporarily remove all of the observations in "       << std::endl;
   std::cout << "   the immediate vicinity of this observation. With this reduced data "        << std::endl;
   std::cout << "   set we interpolate the value at this observation location using "           << std::endl;
   std::cout << "   Ordinary Kriging. "                                                         << std::endl;
   std::cout << std::endl;
   std::cout << "   We compare the interpolated value with the observed value. If they "        << std::endl;
   std::cout << "   are significantly different, the observation is a potential outlier. "      << std::endl;
   std::cout << std::endl;

   Usage();
   std::cout << "Arguments:" << std::endl;
   std::cout << "   <nugget>        The <nugget>, or 'nugget effect', is the discontinuity "    << std::endl;
   std::cout << "                   in the variogram at a lag of 0. The <nugget> quantifies "   << std::endl;
   std::cout << "                   the variance of the sampling and measurement errors and "   << std::endl;
   std::cout << "                   the hyper-local spatial variation. The units of the "       << std::endl;
   std::cout << "                   <nugget> are the units of the observed values squared. "    << std::endl;
   std::cout << "                   The <nugget> must be a strictly positive value. "           << std::endl;
   std::cout << std::endl;
   std::cout << "   <sill>          The <sill> is the value (height) at which variogram "       << std::endl;
   std::cout << "                   levels out. With the exponential model used in Webinan, "   << std::endl;
   std::cout << "                   the variogram approaches the <sill> asymptotically. "       << std::endl;
   std::cout << "                   In the common geostatistical framework (i.e. a second "     << std::endl;
   std::cout << "                   order stationary model), the sill equals the variance "     << std::endl;
   std::cout << "                   of the underlying population. The units of the <sill>"      << std::endl;
   std::cout << "                   are the units of the observed values squared. The <sill>"   << std::endl;
   std::cout << "                   must be a strictly positive value. "                        << std::endl;
   std::cout << std::endl;
   std::cout << "   <range>         The <range> is the separation distance at which we "        << std::endl;
   std::cout << "                   model two observations as essentially uncorrelated. "       << std::endl;
   std::cout << "                   With the exponential model used in Webinan, this is "       << std::endl;
   std::cout << "                   the separation distance at which the variogram reaches "    << std::endl;
   std::cout << "                   95% of the <sill>. The units of the <range> are the "       << std::endl;
   std::cout << "                   units of the observations locations. The <range> must "     << std::endl;
   std::cout << "                   be a strictly positive value. "                             << std::endl;
   std::cout << std::endl;
   std::cout << "   <radius>        The <radius> is the radius of the circle defining the "     << std::endl;
   std::cout << "                   'immediate vicinity' of each observation. When we "         << std::endl;
   std::cout << "                   interpolate the value at an observation location we "       << std::endl;
   std::cout << "                   exclude all of the observations that fall within a "        << std::endl;
   std::cout << "                   circle centered on the observation location with a "        << std::endl;
   std::cout << "                   radius equal to <radius>. The units of the <radius> are "   << std::endl;
   std::cout << "                   the units of the observations locations. The <radius> "     << std::endl;
   std::cout << "                   must be a non-negative value. "                             << std::endl;
   std::cout << std::endl;
   std::cout << "   <input file>    The <input file> is the name of the file (including any "   << std::endl;
   std::cout << "                   necessary path information and the file extension) "        << std::endl;
   std::cout << "                   containing the observation data. "                          << std::endl;
   std::cout << std::endl;
   std::cout << "   <output file>   The <output file> is the name of the file (including "      << std::endl;
   std::cout << "                   any necessary path information and the extension) where "   << std::endl;
   std::cout << "                   Webinan will write all of the program results. If the "     << std::endl;
   std::cout << "                   specified file already exists, it will be overwritten."     << std::endl;
   std::cout << std::endl;

   std::cout << "Example:" << std::endl;
   std::cout << "   Webinan 3 25 3500 50 .\\data\\Input.csv Output.txt" << std::endl;
   std::cout << std::endl;

   std::cout << "Input:" << std::endl;
   std::cout << "   All of the observation data is supplied to Webinan via the <input file>."   << std::endl;
   std::cout << "   The <input file> contains no header lines. The <input file> contains "      << std::endl;
   std::cout << "   one line for each observation. Each observation line has four fields. "     << std::endl;
   std::cout << std::endl;
   std::cout << "   <Well_ID>       The specified observation unique identification number."    << std::endl;
   std::cout << std::endl;
   std::cout << "   <X>             The specified observation location x-coordinate."           << std::endl;
   std::cout << std::endl;
   std::cout << "   <Y>             The specified observation location y-coordinate."           << std::endl;
   std::cout << std::endl;
   std::cout << "   <Z>             The specified observation value."                           << std::endl;
   std::cout << std::endl;
   std::cout << "   The four fields may delimited by a single comma, a single comma and "       << std::endl;
   std::cout << "   one or more spaces, or by one or more spaces without a comma. "             << std::endl;
   std::cout << std::endl;

   std::cout << "Output:" << std::endl;
   std::cout << "   All of the program results go to the <output file>. The <output file> "     << std::endl;
   std::cout << "   contains ten header lines. Following the header lines there is one "      << std::endl;
   std::cout << "   line for each observation. Each observation line has nine fields. "         << std::endl;
   std::cout << std::endl;
   std::cout << "   <Well_ID>       The specified observation unique identification number."    << std::endl;
   std::cout << std::endl;
   std::cout << "   <X>             The specified observation location x-coordinate."           << std::endl;
   std::cout << std::endl;
   std::cout << "   <Y>             The specified observation location y-coordinate."           << std::endl;
   std::cout << std::endl;
   std::cout << "   <Z>             The specified observation value."                           << std::endl;
   std::cout << std::endl;
   std::cout << "   <Count>         The number of observations used in the interpolation."      << std::endl;
   std::cout << "                   The <Count> is the number of observations remaining "       << std::endl;
   std::cout << "                   after excluding the observations within the immediate "     << std::endl;
   std::cout << "                   vicinity of the observation location. "                     << std::endl;
   std::cout << std::endl;
   std::cout << "   <Zhat>          The interpolated value at the observation location."        << std::endl;
   std::cout << std::endl;
   std::cout << "   <Kstd>          The 'standard error' of the interpolated value at the "     << std::endl;
   std::cout <<"                    observation location. The <Kstd> is the square root "       << std::endl;
   std::cout <<"                    Ordinary Kriging variance."                                 << std::endl;
   std::cout << std::endl;
   std::cout << "   <Zeta>          The <Zeta> is the normalized interpolation error. "         << std::endl;
   std::cout << "                      <Zeta> = (<Z> - <Zhat>)/<Kstd>."                         << std::endl;
   std::cout << std::endl;
   std::cout << "   <pValue>        A small <pValue> indicates a potential outlier; e.g. "      << std::endl;
   std::cout << "                   a <pValue> less than 0.01. The <pValue> is defined as "     << std::endl;
   std::cout << "                   the probability that a standard normal random variable "    << std::endl;
   std::cout << "                   is more extreme than <Zeta>. "                              << std::endl;
   std::cout << "                     if <Zeta> < 0 then "                                      << std::endl;
   std::cout << "                        <pValue> = Pr( standard normal < <Zeta> ) "            << std::endl;
   std::cout << "                     else if <Zeta> > 0 then "                                 << std::endl;
   std::cout << "                        <pValue> = Pr( standard normal > <Zeta> ) "            << std::endl;
   std::cout << std::endl;

   std::cout << "Notes:" << std::endl;
   std::cout << "   o  An exponential semivariogram model is used. "                            << std::endl;
   std::cout << "         gamma(h) = <nugget> + (<sill>-<nugget>)*(1 - exp(-3h/<range>)) "      << std::endl;
   std::cout << std::endl;
   std::cout << "   o  The set of observations may include 'duplicates', i.e. multiple "        << std::endl;
   std::cout << "      observation values at the same location. "                               << std::endl;
   std::cout << std::endl;
   std::cout << "   o  The project name 'Webinan' is the Ojibwe word for the inanimate "        << std::endl;
   std::cout << "      transitive verb 'throw it away'. See [http://ojibwe.lib.umn.edu]. "      << std::endl;
   std::cout << "      This name seems appropriate for a program used to identify potential "   << std::endl;
   std::cout << "      outliers."                                                               << std::endl;
   std::cout << std::endl;

   std::cout << "Authors:" << std::endl;
   std::cout << "   R. Barnes, University of Minnesota          "  << std::endl;
   std::cout << "   R. Soule,  Minnesota Department of Health   "  << std::endl;
   std::cout << std::endl;
}

//-----------------------------------------------------------------------------
void Usage()
{
   std::cout << "Usage:" << std::endl;
   std::cout << "   Webinan <nugget> <sill> <range> <radius> <input file> <output file>" << std::endl;
   std::cout << "   Webinan --help" << std::endl;
   std::cout << "   Webinan --version" << std::endl;
   std::cout << std::endl;
}

//-----------------------------------------------------------------------------
void Version()
{
   std::cout << std::endl;
   std::cout << "Webinan (" << VERSION << ")" << std::endl;
   std::cout << std::endl;
}
