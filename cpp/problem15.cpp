

#include <iostream>
#include <vector>
using namespace std;


// -- Structs
struct point {
    int x, y;

    point(int a, int b) { x = a; y = b; }
};

// -- Convience
typedef vector<point> path;

// -- Function Prototypes
long long int getPaths(int, int);
void buildPath(int, int, path&, long long int&);


// ---- Function Body Start
int main(int argc, char** argv)
{

    int s;

    if (argc > 1)   s = argv[1][0] - '0'; 
    else            s = 17;
    auto paths = getPaths(s, s);

    cout << "Links in a " << s << 'x' << s << " grid." << endl;
    cout << "Found " << paths << " links." << endl;
/*
    for (auto iter = paths.begin(); iter != paths.end(); iter++)
    {
        cout << ">> Begin new path >>" << endl;
        for (auto innerIter = iter->begin(); innerIter != iter->end(); innerIter++)
            cout << '\t' << innerIter->x << ", " << innerIter->y << endl;
        cout << "<< End path" << endl;
    }
*/
}

// getPaths()
//      Returns all the paths from (0,0) to the bottom right corner 
//  (Width-1, Height-1) possible using only the up and right directions.
long long int getPaths(int height, int width)
{
    // Return vector
    long long int paths = 0;
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
void buildPath(int h, int w, path &current, long long int &validPaths)
{
    //point at;   // Point we are at now, before we look for another
    bool canGoDown  = true, 
         canGoRight = true;    // true if x or y + 1 isn't out of h or w

    // Set at to the last point in the path
    point at = *(current.end() - 1);
    
    // Check if we're out of bounds
    if (at.x >= w || at.y >= h) {
        // If so, don't return anything and abandon this invalid path
        return;
    }

    // Can we go up on the next iteration?
    if (at.x == (w - 1))   canGoRight  = false;
    if (at.y == (h - 1))   canGoDown   = false;

    // Check if we're at the goal position
    if (!canGoRight && !canGoDown) {
        //validPaths.push_back(current);
        validPaths++;
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

