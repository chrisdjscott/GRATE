#ifndef RIVERPROFILE_H
#define RIVERPROFILE_H

#include <vector>
#include <QVector>
#include <QDateTime>
#include <cmath>
using namespace std;

double gammln2(double xx);

class NodeGSDObject
{

    // Object intended to hold grain size info at each node

public:

    NodeGSDObject();

    vector <double> abrasion;                        // abrasion value for each lithology type (3)
    vector < double > psi;                            // psi (base 2) grain size categories
    vector < vector < double > > pct;                         // Grain-size  (ngsz x nlith)
    double dsg;                                 // Geometric mean grain size
    double d84;
    double d90;
    double stdv;                                // Standard deviation in GSD
    double sand_pct;                            // Percentage of sand (< 2 mm) in GSD

    void norm_frac();

    void dg_and_std();                         // Calculate D50, sand%, geometric (log2)
};

class NodeXSObject
{

public:

    NodeXSObject();                            // Grain size in psi; 2^psi results in mm value

    unsigned int node;
    unsigned int noChannels;                            // In the case of channel bifurcation; keep track of # of (identical) channels
    double depth;                              // Flow depth (m)
    double wsl;                                // Water surface level (m above sea level)
    double width;                              // Nominal channel width (m) at bottom of trapezoid for each node (e.g. P_bed)
    double b2b;                                // Bank-to-bank width (top of in-channel flow section)
    double velocity;                           // Mean velocity (m/s) at each node
    double ustar;                              // Shear velocity
    double theta;                              // Bank sideslope angle (degrees)
    double Hmax;                               // Bank strength as a vertical upper bank section (m)
    double mu;                                 // Bank strength, relative to bed (afer Millar, 2005)
    double fpSlope;                            // Lateral floodplain slope, from valley wall to channel (m/m)
    double valleyWallSlp;                      // Valley wall slope (m/m)
    double fpWidth;                            // Floodplain width (m)
    double bankHeight;                         // Characteristic bank height (m)
    double chSinu;                             // Sinuosity (>1, channel length/valley length)
    double topW;                               // Top width of water surface
    double flow_area[3];                       // [1] Channel [2] Floodplain [3] Total area
    double flow_perim[3];                      // [1] Channel [2] Floodplain [3] Total perimeter
    double hydRadius;                          // Hydraulic radius
    double centr;                              // Vertical centroid of flow
    double k_mean;                             // Conveyance coefficient
    double eci;                                // Energy coefficient related to channel form drag
    double critdepth;                          // Critical depth
    double rough;                              // Grain roughness height
    double omega;                              // Reciprocal of Dingman's Omega (~prop u*/U), Eqn. 6.17
    double Tbed;                               // Shear stress acting on the channel bed (Pa)
    double Tbank;                              // Shear stress acting on the channel banks (Pa)
    double Qb_cap;                             // Transport capacity (m3/s)
    double comp_D;                             // The largest grain that the flow can move
    double K;                                  // Estimated division between key stones and bed material load
    double deltaW;                             // Magnitude of change in width, given regime criteria

    void xsArea();                             // Calculate x-sec area for a given depth

    void xsPerim();                            // Channel/fp perimeter

    void xsCentr();                            // Elevation of xsec centre of mass

    void xsECI(NodeGSDObject F);               // Energy coefficient

    void xsStressTerms(NodeGSDObject F, double bedSlope);   // Bed and banks shear stress partition

    void xsWilcockTransport(NodeGSDObject F);  // Compute bedload transport, based on previous elements

};

class TS_Object
{
            // Generic Time Series Object for either water or sediment inputs
public:

    TS_Object();

    QDateTime date_time;       // Date and time of inputs (see QDateTime doc)
    double Q;                  // m3/s (Qw) for water, m3/s (Qs) for sediment
    int Coord;                  // Stream-wise coordinate of input (m)
    int GRP;                       // For sediment : sed group
};

class RiverProfile
{

public:

    RiverProfile();                            // Constructor
    // Profile Elements

    int nnodes;                       // No. of points in the computational grid
    unsigned int npts;                         // No. of points in the long-profile supplied (later interpolated to nnodes, if necessary)
    QDateTime cTime;                           // Current model time
    QDateTime startTime;
    QDateTime endTime;
    unsigned int counter;
    unsigned int yearCounter;
    int dt;                           // Delta t in seconds
    double dx;                                 // Delta x - distance between cross-sections
    vector<double> xx;                         // Chainage (m) at each node (ordered, increasing)
    vector<double> eta;                        // Elevation (m) at each node (high z to low)

    // Sedimentary Elements

    unsigned int ngsz;                         // 18
    unsigned int nlith;                        // 3
    unsigned int ngrp;                         // 31
    unsigned int nlayer;                       // 20; No. of sublayers
    double poro;                               // 0.6
    double default_la;                         // Active layer default thickness at each node (0.5)
    double layer;                              // Storage layer default thickness (5 m)

    vector< vector < NodeGSDObject > > storedf;// Array of subsurface GSD elements [nnodes][#store layers]
    vector<NodeGSDObject> grp;                 // 'Library' of grain size distributions
    vector<NodeGSDObject> F;                   // Array of surface GSD elements [nnodes]
    vector<double> la;                         // Thickness of the active layer (~2 D90)
    vector<unsigned int> algrp;                         // Active layer group #
    vector<unsigned int> ntop;                          // Top storage layer number (indicates remaining layers beneath current one, '0' means bedrock);
    vector<unsigned int> stgrp;                         // Storage layer group #
    vector<double> toplayer;                    // Thickness of top storage layer
    vector<double> bedrock;                     // Elevation of bedrock at each node (m)
    vector<double> rand_nums;                   // 10 random nums for Monte-Carlo run. Uses rand1().

    vector<NodeXSObject> RiverXS;              // Array of river cross-sectional details

    // Randomizers
    vector<double> tweakArray;
    double qsTweak;                           // Augment the rate of tributary Qs, Qw inputs
    double qwTweak;
    double substrDial;
    double feedQw;
    double feedQs;
    double HmaxTweak;
    double randAbr;

    vector<float> N;                           // Transition matrix for coarsening or fining mixtures

    void initData();
    
    vector<double> hydroGraph();

    void readData();

    const char *getNextParam(ifstream &openFile, const char *nextParam);

    void getLongProfile(std::ifstream &openFile);

    void getGrainSizeLibrary(std::ifstream &openFile);

    void getLibraryLith(std::ifstream &openFile);

    void getStratigraphy(std::ifstream &openFile);

};


#endif // RIVERPROFILE_H
