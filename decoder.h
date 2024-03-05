
#ifndef _DECODER_H
#define _DECODER_H

#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include <utility>
#include <vector>
using namespace std;
using namespace cs221util;

class decoder {

public:

    // initializes variables and build solution path as a vector
    // of points that corresponds to a sequence of pixels 
    // beginning at pathPts[0] == start, and ending with
    // pathPts[pathPts.size() - 1] == end. 
    //

	decoder(const PNG & tm, pair<int,int> s);

	// draws the solution path on top of the given image. The solution
    // is the shortest path to the point which is farthest from the
    // start in the embedded maze. Solution path is drawn in (255,0,0).
    // This function assumes that the pathPts member variable has been
    // populated by the constructor.
	PNG renderSolution();

	// discovers and draws the entire maze on top of the given image. 
    // The maze is revealed by darkening each of the maze pixels: change 
    // each colour channel to be 50% of its treasure map value. (use the
    // setGrey function from the treasureMap class.)
    //
    //
    // This function also draws a red 7px x 7px square at the start 
    // location.

	PNG renderMaze();

    // returns the location of the treasure
    pair<int,int> findSpot();

    // returns the length of the path to the treasure, in pixels, using
    // manhattan distance. (no diagonal steps)
    int pathLength();

private:

    // tests a neighbor (adjacent vertex) to see if it is 
    // 1. within the image, 2. unvisited, and 3. coloured so that
    // lower order bits fit the pattern prescribed by the encoder.
    // An entry in table v is true if a cell has previously been 
    // visited. table d contains the shortest distance from each location
    // back to the start. the (r,g,b) colour of position next must have lower order
    // bits (XX,XX,XX) that, when interpreted as an integer between 0 and 63,
    // inclusive, is d[curr.second][curr.first] + 1 (mod 64).
	bool good(vector<vector<bool> > & v, vector<vector<int> > & d,pair<int,int> curr, pair<int,int> next);

    // builds a vector containing the locations of the 
    // four vertices adjacent to curr:
    // left, below, right, above. 
    // does not pay any attention to whether or not the neighbors are 
    // valid (in the image, previously visited, or the right colour).
	vector<pair<int,int> > neighbors(pair<int,int> curr) ;

    // tests whether p's lower order bits, when interpreted as a 
    // number, is d+1 (mod 64).
    bool compare(RGBAPixel p, int d);

    // changes the pixel at position loc to consist of colour channel
    // values which are 50% of their original. Note that because of 
    // our encoding scheme which essentially invalidates the lower
    // order bits, you should compute each value as 2*(x/4), where
    // x is the value of a colour channel.
    void setGrey(PNG & im, pair<int,int> loc);


// ========= private member variables ================

	pair<int,int> start;  // col,row
	PNG mapImg; // this image has a maze encoded in it 
    vector<pair<int,int> > pathPts; // vector of points on the solution path

};

#endif
