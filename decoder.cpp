#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

// initializes variables and build solution path as a vector
// of points that corresponds to a sequence of pixels
// beginning at pathPts[0] == start, and ending with
// pathPts[pathPts.size() - 1] == end.
//
// BFS-based Algorithm
decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {

    int width = mapImg.width();
    int height = mapImg.height();

    vector<vector<bool>> visited(width, vector<bool>(height, false));
    vector<vector<int>> distances(width, vector<int>(height, 0));
    vector<vector<pair<int,int>>> prev(width, vector<pair<int,int>>(height, pair<int,int>(-1,-1)));
    Queue<pair<int,int>> q;

    visited[start.first][start.second] = true;
    distances[start.first][start.second] = 0;
    q.enqueue(start);

    while (!q.isEmpty()) {
        pair<int,int> curr = q.dequeue();
        vector<pair<int,int>> neighbors = decoder::neighbors(curr);

        for (int i = 0; i < neighbors.size(); i++) {
            pair<int,int> p = neighbors[i];
            if (good(visited, distances, curr, p)) {
                visited[p.first][p.second] = true;
                distances[p.first][p.second] = distances[curr.first][curr.second] + 1;
                prev[p.first][p.second] = curr;
                q.enqueue(p);
            }
        }
    }


    pair<int,int> curr = findSpot();
    while (curr != start) {
        pathPts.push_back(curr);
        curr = prev[curr.first][curr.second];
    }
    pathPts.push_back(start);

    for (int i = 0; i < pathPts.size()/2; i++) {
        pair<int,int> temp = pathPts[i];
        pathPts[i] = pathPts[pathPts.size()-1-i];
        pathPts[pathPts.size()-1-i] = temp;
    }
}

// Draws the solution path on top of the given image. The solution
// is the shortest path to the point which is farthest from the
// start in the embedded maze. Solution path is drawn in (255,0,0).
// This function assumes that the pathPts member variable has been
// populated by the constructor.
PNG decoder::renderSolution(){

    PNG treasureMap = mapImg;

    for (int i = 0; i < pathPts.size(); i++) {
        pair<int,int> curr = pathPts[i];
        RGBAPixel *pixelCurr = treasureMap.getPixel(curr.first, curr.second);
        pixelCurr->r = 255;
        pixelCurr->g = 0;
        pixelCurr->b = 0;
    }

    // STEP 3: Return image
    return treasureMap;

}

// discovers and draws the entire maze on top of the given image.
// The maze is revealed by darkening each of the maze pixels: change
// each colour channel to be 50% of its treasure map value. (use the
// setGrey function from the treasureMap class.)
//
// This function also draws a red 7px x 7px square at the start
// location.
PNG decoder::renderMaze(){


    PNG treasureMaze = mapImg;


    int width = mapImg.width();
    int height = mapImg.height();


    vector<vector<bool>> visited(width, vector<bool>(height, false)); //initialize visited vector
    vector<vector<int>> distances(width, vector<int>(height, 0)); //initialize distances vector

    Queue<pair<int,int>> q;

    visited[start.first][start.second] = true; // Mark as visited

    q.enqueue(start);
    while (!q.isEmpty()) {
        pair<int,int> curr = q.dequeue();
        vector<pair<int,int>> neighbors = decoder::neighbors(curr);
        for (int i = 0; i < neighbors.size(); i++) {
            pair<int,int> p = neighbors[i];
            if (good(visited, distances, curr, p)) {
                visited[p.first][p.second] = true;
                distances[p.first][p.second] = distances[curr.first][curr.second] + 1;
                setGrey(treasureMaze, p);
                q.enqueue(p);
            }
        }
    }

    int startX = start.first - 3;
    int startY = start.second - 3;
    int endX = start.first + 3;
    int endY = start.second + 3;

    startX = max(startX, 0);
    startY = max(startY, 0);
    endX = min(endX, width - 1);
    endY = min(endY, height - 1);

    for (int x = startX; x <= endX; x++) {
        for (int y = startY; y <= endY; y++) {
            treasureMaze.getPixel(x, y)->r = 255;
            treasureMaze.getPixel(x, y)->g = 0;
            treasureMaze.getPixel(x, y)->b = 0;
    }

    return treasureMaze;

}

void decoder::setGrey(PNG & im, pair<int,int> loc){

    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);

    pixel->r = (pixel->r / 4) * 2; // Red
    pixel->g = (pixel->g / 4) * 2; // Green
    pixel->b = (pixel->b / 4) * 2; // Blue

    // STEP 3: Return void
    return;

}

pair<int,int> decoder::findSpot(){
    int width = mapImg.width();
    int height = mapImg.height();


    vector<vector<bool>> visited(width, vector<bool>(height, false));
    vector<vector<int>> tempDistances(width, vector<int>(height, 0));
    Queue<pair<int,int>> q;


    visited[start.first][start.second] = true;
    tempDistances[start.first][start.second] = 0;
    q.enqueue(start);

    while (!q.isEmpty()) {
        pair<int,int> curr = q.dequeue();
        vector<pair<int,int>> neighbors = this->neighbors(curr);

        for (pair<int,int> p : neighbors) {
            if (good(visited, tempDistances, curr, p)) {
                visited[p.first][p.second] = true;
                tempDistances[p.first][p.second] = tempDistances[curr.first][curr.second] + 1;
                q.enqueue(p);
            }
        }
    }

    pair<int,int> furthestPoint = start;
    int maxDistance = 0;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (tempDistances[x][y] > maxDistance) {
                maxDistance = tempDistances[x][y];
                furthestPoint = make_pair(x, y);
            }
        }
    }

    return furthestPoint;
}


int decoder::pathLength(){

    int length = 0;

    for (size_t i = 0; i < pathPts.size() - 1; i++) {
        length += abs(pathPts[i+1].first - pathPts[i].first) + abs(pathPts[i+1].second - pathPts[i].second);
    }

    return length + 1;

}

// tests a neighbor (adjacent vertex) to see if it is
// 1. within the image, 2. unvisited, and 3. coloured so that
// lower order bits fit the pattern prescribed by the encoder.
// An entry in table v is true if a cell has previously been
// visited. table d contains the shortest distance from each location
// back to the start. the (r,g,b) colour of position next must have lower order
// bits (XX,XX,XX) that, when interpreted as an integer between 0 and 63,
// inclusive, is d[curr.second][curr.first] + 1 (mod 64).
bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){

    if (next.first < 0 || next.first >= mapImg.width() || next.second < 0 || next.second >= mapImg.height()) {
        return false;
    }

    if (v[next.first][next.second]) {
        return false;
    }

    RGBAPixel* currPixel = mapImg.getPixel(curr.first, curr.second);
    RGBAPixel* nextPixel = mapImg.getPixel(next.first, next.second);

    int currMazeValue = ((currPixel->r & 3) << 4) + ((currPixel->g & 3) << 2) + (currPixel->b & 3);
    int nextMazeValue = ((nextPixel->r & 3) << 4) + ((nextPixel->g & 3) << 2) + (nextPixel->b & 3);

    int mazeDistance = (nextMazeValue - currMazeValue + 64) % 64;
    if (mazeDistance != 1) {
        return false;
    }

    return true;

}

// builds a vector containing the locations of the
// four vertices adjacent to curr:
// left, below, right, above.
// does not pay any attention to whether or not the neighbors are
// valid (in the image, previously visited, or the right colour).
vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {

    vector<pair<int,int>> n;
    // left
    n.push_back(make_pair(curr.first - 1, curr.second));
    // below
    n.push_back(make_pair(curr.first, curr.second + 1));
    // right
    n.push_back(make_pair(curr.first + 1, curr.second));
    // above
    n.push_back(make_pair(curr.first, curr.second - 1));
    return n;

}


bool decoder::compare(RGBAPixel p, int d){

    int r = p.r % 4;
    int g = p.g % 4;
    int b = p.b % 4;

    int num = (r << 4) + (g << 2) + b;

    return num == (d + 1) % 64;

}
