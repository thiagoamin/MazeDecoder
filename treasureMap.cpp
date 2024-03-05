#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{
    base = baseim;
    maze = mazeim;
    start = s;
}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){

    // STEP 1: Get the current pixel
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);

    // STEP 2: Set the RGB values of the pixel
    pixel->r = (pixel->r / 4) * 2; // Red
    pixel->g = (pixel->g / 4) * 2; // Green
    pixel->b = (pixel->b / 4) * 2; // Blue

    // STEP 3: Return void
    return;
}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){

    // STEP 1: Get the current pixel
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);

    // STEP 2: Get the lower order bits of d
    int dLOB = d % 64;

    // STEP 3: Set the RGB values of the pixel
    pixel->r = (pixel->r & ~0b11) | ((dLOB >> 4) & 0b11); // Most significant 2 bits
    pixel->g = (pixel->g & ~0b11) | ((dLOB >> 2) & 0b11); // Middle 2 bits
    pixel->b = (pixel->b & ~0b11) | (dLOB & 0b11);        // Least significant 2 bits
}

PNG treasureMap::renderMap(){

    // STEP 0: Make a copy of the base image:
    PNG treasureMap = base;

    // STEP 1: Initialize working vectors
    int width = maze.width(); //get width of maze (== width baseim)
    int height = maze.height(); //get height of maze (== height baseim)

    // image[x][y] (x then y --> width then height)
    vector<vector<bool>> visited(width, vector<bool>(height, false)); //initialize visited vector
    vector<vector<int>> distances(width, vector<int>(height, 0)); //initialize distances vector

    // STEP 2: Initialize queue:
    Queue<pair<int,int>> q; //initialize queue

    // Step 3: Mark the start location
    visited[start.first][start.second] = true; // Mark as visited
    distances[start.first][start.second] = 0; // Set distance to 0 (start point)
    setLOB(treasureMap, start, 0); // Encode distance in lower order bits
    q.enqueue(start); // Enqueue start location

    // STEP 4: Transversal
    while (!q.isEmpty()) { // While queue is not empty
        pair<int,int> curr = q.dequeue(); // Set current location to front of queue
        vector<pair<int,int>> neighbors = treasureMap::neighbors(curr); // Get neighbors of current location

        for (int i = 0; i < neighbors.size(); i++) { // For each neighbor
            pair<int,int> p = neighbors[i]; // Set p to neighbor
            if (good(visited, curr, p)) { // If p is good
                visited[p.first][p.second] = true; // Mark as visited
                distances[p.first][p.second] = distances[curr.first][curr.second] + 1; // Set distance to curr distance + 1
                setLOB(treasureMap, p, distances[p.first][p.second]); // Encode distance in lower order bits
                q.enqueue(p); // Enqueue p
            }
        }
    }

    // STEP 5: Return image
    return treasureMap;
}


PNG treasureMap::renderMaze(){

    // STEP 0: Make a copy of the base image:
    PNG treasureMaze = base;

    // STEP 1: Initialize working vectors
    int width = maze.width(); //get width of maze (== width baseim)
    int height = maze.height(); //get height of maze (== height baseim)

    // image[x][y] (x then y --> width then height)
    vector<vector<bool>> visited(width, vector<bool>(height, false)); //initialize visited vector
    vector<vector<int>> distances(width, vector<int>(height, 0)); //initialize distances vector

    // STEP 2: Initialize queue:
    Queue<pair<int,int>> q; //initialize queue

    // Step 3: Mark the start location
    visited[start.first][start.second] = true; // Mark as visited
    distances[start.first][start.second] = 0; // Set distance to 0 (start point)

    // STEP 4: Transversal
    q.enqueue(start); // Enqueue start location
    while (!q.isEmpty()) { // While queue is not empty
        pair<int,int> curr = q.dequeue(); // Set current location to front of queue
        vector<pair<int,int>> neighbors = treasureMap::neighbors(curr); // Get neighbors of current location

        for (int i = 0; i < neighbors.size(); i++) { // For each neighbor
            pair<int,int> p = neighbors[i]; // Set p to neighbor
            if (good(visited, curr, p)) { // If p is good
                visited[p.first][p.second] = true; // Mark as visited
                distances[p.first][p.second] = distances[curr.first][curr.second] + 1; // Set distance to curr distance + 1
                setGrey(treasureMaze, p); // Set grey
                q.enqueue(p); // Enqueue p
            }
        }
    }

    // STEP 5: Draw Red Square
    // Calculate the coordinates for the top-left and bottom-right corners of the red square
        int startX = start.first - 3; // 7px square centered around the start
        int startY = start.second - 3;
        int endX = start.first + 3;
        int endY = start.second + 3;

    // Ensure that the square does not go outside the image boundaries
        startX = max(startX, 0);
        startY = max(startY, 0);
        endX = min(endX, width - 1);
        endY = min(endY, height - 1);

    // Draw the red square by setting the color of pixels within the square to red
        for (int x = startX; x <= endX; x++) {
            for (int y = startY; y <= endY; y++) {
                treasureMaze.getPixel(x, y)->r = 255; // Set red channel to 255 (full red)
                treasureMaze.getPixel(x, y)->g = 0;   // Set green channel to 0 (no green)
                treasureMaze.getPixel(x, y)->b = 0;   // Set blue channel to 0 (no blue)
            }
        }

    // STEP 5.2: Return image
    return treasureMaze;
}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){

    // STEP 1: Check if next is within the image
    if (next.first < 0 || next.first >= maze.width() || next.second < 0 || next.second >= maze.height()) {
        return false;
    }

    // STEP 2: Check if next is unvisited
    if (v[next.first][next.second]) {
        return false;
    }

    // STEP 3: Check if next is the same colour as curr in the maze image
    RGBAPixel *currPixel = maze.getPixel(curr.first, curr.second);
    RGBAPixel *nextPixel = maze.getPixel(next.first, next.second);
    if (*currPixel != *nextPixel) {
        return false;
    }

    // STEP 4: If all passes, return true
    return true;

}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {

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

