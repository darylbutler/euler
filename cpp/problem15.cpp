/*
 * Testing for faster solving, allowing for deeper iteration.
 *
 */

#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;

// -- Structs
struct point {
    int x, y;

    point(int a, int b) { x = a; y = b; }
};

// -- Convience
typedef vector<point> path;
typedef long long int lint;

// -- Debug Global Vars
static lint BuildPathCallCount = 0;
static lint BuildPath_GoodPathCount = 0;
static lint BuildPath_BadPathCount = 0;

// -- Function Prototypes
vector<path> getPaths(int, int);
void buildPath(int, int, path&, vector<path>&);


// ---- Function Body Start
int main(int argc, char** argv)
{

    int s = 14;

    if (argc > 1)   s = atoi(argv[1]);
    else            s = 20;
    auto paths = getPaths(s, s);

    //cout << "Links in a " << s << 'x' << s << " grid." << endl;
    cout << "Found " << paths.size() << " links." << endl;
    //cout << "buildPath() called " << BuildPathCallCount << " times." << endl;
    //cout << "Good Paths: " << BuildPath_GoodPathCount << " ";
    //cout << "Bad Paths : " << BuildPath_BadPathCount << endl;

    //cout << "---- Paths Found:" << endl;

    //for (auto iter = paths.begin(); iter != paths.end(); iter++)
    //{
    //    cout << ">> Begin new path >>" << endl;
    //    for (auto innerIter = iter->begin(); innerIter != iter->end(); innerIter++)
    //        cout << '\t' << innerIter->x << ", " << innerIter->y << endl;
    //    cout << "<< End path" << endl;
    //}

}

// getPaths()
//      Returns all the paths from (0,0) to the bottom right corner
//  (Width-1, Height-1) possible using only the up and right directions.
vector<path> getPaths(int height, int width)
{
    // Return vector
    vector<path> paths;

    // Starting path
    path newPath;
    newPath.push_back(point(0, 0));

    // build the paths vector
    buildPath(height, width, newPath, paths);

    // Done
    return paths;
}

// buildPath()
//      Recursively called to build a path to the destination
void buildPath(int h, int w, path &current, vector<path> &validPaths)
{
    // Debug -- BuildPath was called, so increment the call counter
    //BuildPathCallCount++;

    //point at;   // Point we are at now, before we look for another
    bool canGoDown  = true,
         canGoRight = true;    // true if x or y + 1 isn't out of h or w

    // Set at to the last point in the path
    point at = *(current.end() - 1);

    // Check if we're out of bounds
    if (at.x >= w || at.y >= h) {
        // If so, don't return anything and abandon this invalid path
        // -- Debug -- This was a wasted call, increment the BadPath count
        //BuildPath_BadPathCount++;
        return;
    }

    // Can we go up on the next iteration?
    if (at.x == (w - 1))   canGoRight  = false;
    if (at.y == (h - 1))   canGoDown   = false;

    // Check if we're at the goal position
    if (!canGoRight && !canGoDown) {
        // -- Debug -- This was a good path, so increment the good path count
        //BuildPath_GoodPathCount++;
        validPaths.push_back(current);
        return;
    }

    // We're at a point in between the start and the goal, and there are
    // only two moves we can make at each point.  Do them.
    if (canGoDown) {
        // -- We can go Down
        auto downV = current;
        downV.push_back(point(at.x, at.y + 1));
        buildPath(h, w, downV, validPaths);
    }
    if (canGoRight) {
        // -- We can go Right
        current.push_back(point(at.x + 1, at.y));
        buildPath(h, w, current, validPaths);
    }
}

