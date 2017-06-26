# Webinan
Identify potential outliers using a geostatistical framework.

Webinan is a simple, pragmatic, heuristic algorithm. One-by-one we visit each observation. We temporarily remove this observation from the data set. We also temporarily remove all of the observations in the immediate vicinity of this observation. With this reduced data set we interpolate the value at this observation location using Ordinary Kriging.

We compare the interpolated value with the observed value. If they are significantly different, the observation is a potential outlier.

## Usage
   `Webinan <nugget> <sill> <range> <radius> <input file> <output file>` 
   `Webinan --help` 
   `Webinan --version` 

## Origin of the Project Name
The project name __Webinan__ is the Ojibwe word for the inanimate transitive verb "throw it away". See [http://ojibwe.lib.umn.edu](http://ojibwe.lib.umn.edu/search?utf8=%E2%9C%93&q=webinan&commit=Search&type=ojibwe). This name seems appropriate for a program used to identify potential outliers.
