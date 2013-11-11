/*
 * Second attempt at problem 15.  Original method ('problem15.cpp') was too 
 * slow and used too much memory to get to a 20x20 grid.
 *
 * New approach uses a few assumptions made:
 *  > All paths through the grid will be the same length.
 *  > For any valid path found, its opposite will also be a valid path.
 *
 * We represent the grid as a sequence of bits.  So a 3x3 grid becomes a
 * stream of 9 bits.  The left-most bit (msb?) being the starting square and 
 * the right-most bit (lsb?) being the goal position.  
 *
 * We represent the moves as a similiar stream of bits, with each bit 
 * representing the direction we move (Remember, all paths should be the same
 * length).  We will represent a right direction with true (1) and a down
 * direction with false (0).
 * A right directional move will be accomplished by shifting the grid bits
 * right once (x >> 1), while a down motion will be accomplished by shifting
 * the grid bits right the length of the grid (So x >> 3, in the case of a 
 * 3x3 grid).
 *
 * This means that, after a path has been applied, the affected grid should 
 * be equal to 1.  If it is not, it is not a valid path.
 *
 * So, if a path is always the same length, then the length of every path
 * is computable.  It is always (l + w - 2), where l and w are the mesure of
 * the sides (So, if w = 2 and l = 4, the grid is 2x4 or grid[2][4]).
 *
 * Finally, to find all the valid paths, we can try every permutation of 
 * paths, since the length of the path is known.
 * TODO: We should be able to add the opposite of any valid path.  Problem is
 *       how can we not iterate over it again when its natural permutation
 *       come around?  Will it be effecient to check our list of valid paths
 *       against each possible path?
 */

#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

const int WIDTH = 16;
const int LENGTH = WIDTH;
const int GBITS = WIDTH * LENGTH;
const int PBITS = WIDTH + LENGTH - 2;

// -- Types
typedef long long int lint;
typedef unsigned int uint;
typedef bitset<GBITS> grid_t;
typedef bitset<PBITS> path_t;

// -- Class Definition (Using class to prevent static globals.  It is cleaner)
class PathSolver
{
public:
    // Grid bitset
    grid_t *grid;
    // Convenience
    uint grid_last;     // Index of last element in grid bitarrays

    // Private Methods
    bool isValidPath(path_t&);
    void bitsetMinusOne(path_t&);
    bool vectorContains(vector<path_t>&, path_t&);
    
public:
    // Constructor
    PathSolver();

    // Magic Method
    vector<path_t> GetAllPaths();
};

// -- Main()!
int main()
{
    //cout << "Hello, World!" << endl;

    PathSolver* solver = new PathSolver();

    //path_t path("100000");
    //cout << solver->isValidPath(path) << endl;

    //cout << endl << path << " minus 1 == ";
    //solver->bitsetMinusOne(path);
    //cout << path << endl;

    // Solver tests
    auto v = solver->GetAllPaths();
    cout << "Found " << v.size() << " paths.  They are:" << endl;
    //for (auto iter = v.begin(); iter != v.end(); iter++)
    //    cout << *iter << endl;
}

// Constructor
PathSolver::PathSolver()
{
    int l = WIDTH * LENGTH - 1;
    grid = new grid_t(1);
    *grid <<= l;
    grid_last = WIDTH * LENGTH - 1;
}

// Private Methods - Does this path result in grid == 1?  (See intro comments)
bool PathSolver::isValidPath(path_t &path)
{
    grid_t testGrid = *grid;
    for (uint i = 0; i < path.size(); i++) {
        if (path.test(i)) {
            // true = right, shift by 1
            testGrid >>= 1;
        } else {
            // false = down, shift by grid_width
            testGrid >>= WIDTH;
        }
    }
    // All path actions have been completed, so testGrid should equal 1 if
    // it is valid
    return (testGrid[0] && testGrid.count() == 1);
}


vector<path_t> PathSolver::GetAllPaths()
{
    // Try every combination of paths possible, checking each one with
    // isValidPath.  If it is, it is added to the return vector.
    vector<path_t> v;   // TODO Can we estimate the size needed here?
    path_t path, lastPath;

    // Set path to its maximum value
    path.set();
    lastPath = path;

    do {
        // If this path is equal to the opposite of the last path we evaluated,
        // we've reached the middle of the grid and have found all paths
        lastPath.flip();
        if (lastPath == path)
            return v;

        // Check path and, if it leads to end, add to the return vector
        if (isValidPath(path)) {
            // Add the current path
            v.push_back(path);
            // Also add its opposite
            path.flip();
            v.push_back(path);
            // reset path so iteration can continue
            path.flip();
        }

        // Subtract one and loop again
        lastPath = path;
        bitsetMinusOne(path);
    } while (path.count() > 0);

    return v;
}


void PathSolver::bitsetMinusOne(path_t &num)
{  
    if (num[0])
    {
        num.flip(0);
        return;
    }
    else
    {
        // If the last bit in 0, then we need to flip the bits until we arrive
        // at 1 (And we'll borrow from it so it will turn into zero)
        for (int i = 0; i < num.size(); i++) {
            num.flip(i);    // bit gets flipped no matter what
            if (!num[i]) {
                // We've reached our 1 so we're done here
                return;
            } 
        }
    }
}

bool PathSolver::vectorContains(vector<path_t> &v, path_t &path)
{
    for (auto iter = v.begin(); iter != v.end(); iter++)
        if (*iter == path) return true;
    return false;
}
