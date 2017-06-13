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
//    13 June 2017
//=============================================================================
#include <iostream>

#include "version.h"

namespace
{
   const char* VERSION = "13 June 2017 [beta1]";
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
   std::cerr << "   Identify potential outliers using a geostatistical framework."              << std::endl;
   std::cerr << std::endl;
   std::cerr << "   Webinan is a simple, pragmatic, heuristic algorithm. One-by-one we "        << std::endl;
   std::cerr << "   visit each observation. We temporarily remove this observation from "       << std::endl;
   std::cerr << "   the data set. We also temporarily remove all of the observations in "       << std::endl;
   std::cerr << "   the immediate vicinity of this observation. With this reduced data "        << std::endl;
   std::cerr << "   set we interpolate the value at this observation location using "           << std::endl;
   std::cerr << "   Ordinary Kriging. "                                                         << std::endl;
   std::cerr << std::endl;
   std::cerr << "   We compare the interpolated value with the observed value. If they "        << std::endl;
   std::cerr << "   are significantly different, the observation is a potential outlier. "      << std::endl;
   std::cerr << std::endl;

   Usage();
   std::cerr << "Arguments:" << std::endl;
   std::cerr << "   <nugget>        The <nugget>, or 'nugget effect', is the discontinuity "    << std::endl;
   std::cerr << "                   in the variogram at a lag of 0. The <nugget> quantifies "   << std::endl;
   std::cerr << "                   the variance of the sampling and measurement errors and "   << std::endl;
   std::cerr << "                   the hyper-local spatial variation. The units of the "       << std::endl;
   std::cerr << "                   <nugget> are the units of the observed values squared. "    << std::endl;
   std::cerr << "                   The <nugget> must be a strictly positive value. "           << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <sill>          The <sill> is the value (height) at which variogram "       << std::endl;
   std::cerr << "                   levels out. With the exponential model used in Webinan, "   << std::endl;
   std::cerr << "                   the variogram approaches the <sill> asymptotically. "       << std::endl;
   std::cerr << "                   In the common geostatistical framework (i.e. a second "     << std::endl;
   std::cerr << "                   order stationary model), the sill equals the variance "     << std::endl;
   std::cerr << "                   of the underlying population. The units of the <sill>"      << std::endl;
   std::cerr << "                   are the units of the observed values squared. The <sill>"   << std::endl;
   std::cerr << "                   must be a strictly positive value. "                        << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <range>         The <range> is the separation distance at which we "        << std::endl;
   std::cerr << "                   model two observations as essentially uncorrelated. "       << std::endl;
   std::cerr << "                   With the exponential model used in Webinan, this is "       << std::endl;
   std::cerr << "                   the separation distance at which the variogram reaches "    << std::endl;
   std::cerr << "                   95% of the <sill>. The units of the <range> are the "       << std::endl;
   std::cerr << "                   units of the observations locations. The <range> must "     << std::endl;
   std::cerr << "                   be a strictly positive value. "                             << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <radius>        The <radius> is the radius of the circle defining the "     << std::endl;
   std::cerr << "                   'immediate vicinity' of each observation. When we "         << std::endl;
   std::cerr << "                   interpolate the value at an observation location we "       << std::endl;
   std::cerr << "                   exclude all of the observations that fall within a "        << std::endl;
   std::cerr << "                   circle centered on the observation location with a "        << std::endl;
   std::cerr << "                   radius equal to <radius>. The units of the <radius> are "   << std::endl;
   std::cerr << "                   the units of the observations locations. The <radius> "     << std::endl;
   std::cerr << "                   must be a non-negative value. "                             << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <input file>    The <input file> is the name of the file (including any "   << std::endl;
   std::cerr << "                   necessary path information and the file extension) "        << std::endl;
   std::cerr << "                   containing the observation data. "                          << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <output file>   The <output file> is the name of the file (including "      << std::endl;
   std::cerr << "                   any necessary path information and the extension) where "   << std::endl;
   std::cerr << "                   Webinan will write all of the program results. If the "     << std::endl;
   std::cerr << "                   specified file already exists, it will be overwritten."     << std::endl;
   std::cerr << std::endl;

   std::cerr << "Example:" << std::endl;
   std::cerr << "   Webinan 3 25 3500 50 .\\data\\Input.csv Output.txt" << std::endl;
   std::cerr << std::endl;

   std::cerr << "Input:" << std::endl;
   std::cerr << "   All of the observation data is supplied to Webinan via the <input file>."   << std::endl;
   std::cerr << "   The <input file> contains no header lines. The <input file> contains "      << std::endl;
   std::cerr << "   one line for each observation. Each observation line has four fields. "     << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <Well_ID>       The specified observation unique identification number."    << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <X>             The specified observation location x-coordinate."           << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <Y>             The specified observation location y-coordinate."           << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <Z>             The specified observation value."                           << std::endl;
   std::cerr << std::endl;
   std::cerr << "   The four fields may delimited by a single comma, a single comma and "       << std::endl;
   std::cerr << "   one or more spaces, or by one or more spaces without a comma. "             << std::endl;
   std::cerr << std::endl;

   std::cerr << "Output:" << std::endl;
   std::cerr << "   All of the program results go to the <output file>. The <output file> "     << std::endl;
   std::cerr << "   contains ten header lines. Following the header lines there is one "      << std::endl;
   std::cerr << "   line for each observation. Each observation line has nine fields. "         << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <Well_ID>       The specified observation unique identification number."    << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <X>             The specified observation location x-coordinate."           << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <Y>             The specified observation location y-coordinate."           << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <Z>             The specified observation value."                           << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <Count>         The number of observations used in the interpolation."      << std::endl;
   std::cerr << "                   The <Count> is the number of observations remaining "       << std::endl;
   std::cerr << "                   after excluding the observations within the immediate "     << std::endl;
   std::cerr << "                   vicinity of the observation location. "                     << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <Zhat>          The interpolated value at the observation location."        << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <Kstd>          The 'standard error' of the interpolated value at the "     << std::endl;
   std::cerr <<"                    observation location. The <Kstd> is the square root "       << std::endl;
   std::cerr <<"                    Ordinary Kriging variance."                                 << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <Zeta>          The <Zeta> is the normalized interpolation error. "         << std::endl;
   std::cerr << "                      <Zeta> = (<Z> - <Zhat>)/<Kstd>."                         << std::endl;
   std::cerr << std::endl;
   std::cerr << "   <pValue>        A small <pValue> indicates a potential outlier; e.g. "      << std::endl;
   std::cerr << "                   a <pValue> less than 0.01. The <pValue> is defined as "     << std::endl;
   std::cerr << "                   the probability that a standard normal random variable "    << std::endl;
   std::cerr << "                   is more extreme than <Zeta>. "                              << std::endl;
   std::cerr << "                     if <Zeta> < 0 then "                                      << std::endl;
   std::cerr << "                        <pValue> = Pr( standard normal < <Zeta> ) "            << std::endl;
   std::cerr << "                     else if <Zeta> > 0 then "                                 << std::endl;
   std::cerr << "                        <pValue> = Pr( standard normal > <Zeta> ) "            << std::endl;
   std::cerr << std::endl;

   std::cerr << "Notes:" << std::endl;
   std::cerr << "   o  An exponential semivariogram model is used. "                            << std::endl;
   std::cerr << "         gamma(h) = <nugget> + (<sill>-<nugget>)*(1 - exp(-3h/<range>)) "      << std::endl;
   std::cerr << std::endl;
   std::cerr << "   o  The set of observations may include 'duplicates', i.e. multiple "        << std::endl;
   std::cerr << "      observation values at the same location. "                               << std::endl;
   std::cerr << std::endl;
   std::cerr << "   o  The project name 'Webinan' is the Ojibwe word for the inanimate "        << std::endl;
   std::cerr << "      transitive verb 'throw it away'. See [http://ojibwe.lib.umn.edu]. "      << std::endl;
   std::cerr << "      This name seems appropriate for a program used to identify potential "   << std::endl;
   std::cerr << "      outliers."                                                               << std::endl;
   std::cerr << std::endl;

   std::cerr << "Authors:" << std::endl;
   std::cerr << "   R. Barnes, University of Minnesota          "  << std::endl;
   std::cerr << "   R. Soule,  Minnesota Department of Health   "  << std::endl;
   std::cerr << std::endl;
}

//-----------------------------------------------------------------------------
void Usage()
{
   std::cerr << "Usage:" << std::endl;
   std::cerr << "   Webinan <nugget> <sill> <range> <radius> <input file> <output file>" << std::endl;
   std::cerr << "   Webinan --help" << std::endl;
   std::cerr << "   Webinan --version" << std::endl;
   std::cerr << std::endl;
}

//-----------------------------------------------------------------------------
void Version()
{
   std::cerr << std::endl;
   std::cerr << "Webinan (" << VERSION << ")" << std::endl;
   std::cerr << std::endl;
}
