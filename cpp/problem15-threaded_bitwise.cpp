/*
 * Third attempt at Problem 15.
 * NOTE: See problem15_bitwise.cpp header comments for the description of the
 * bitwise algorithm, it is largely unchanged in this document.
 *
 * Solution 1 was bloated and slow (took forever and 100% ram).
 * Solution 2 was marginally faster and extremely more compact.
 * For comparison,
 *  -- solving for a 15x15 grid,
 *  Solution 1:     16.502s @ 100% cpu and 80% ram (16 gigs of ram....)
 *  Solution 2:      8.058s @ 100% cpu and  2% ram
 *  Solution 2.5:    1.876s @ 100% cpu and  2% ram (Significant Increase)
 *
 *  The success of solution 2 (problem15-bitwise.cpp) is that it is expontentially
 *      faster tha solution 1.  As the complexity increases, it gets better and better
 *      (Or, rather, solution 1 gets worse and worse).
 *  Note that solution 1.5 (problem15-testing.cpp) is solution 1 with the successful
 *      path collecting stripped out.  In other words, when it finds a successful path,
 *      it discards it and instead increaments a counter.
 *      Its performance is better than solution 2 (8.316 @ 100% cpu and 0% mem), though
 *      it is too easy of a solution for me to call this challenge defeated.
 *
 *  The biggest problem we run into is that there are just too many possible paths
 *      with a 20x20 grid that would take weeks to calculate (The math is scary).
 *      However, my machine has 8 cores, and this may be the best avenue.
 */

#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

const int GridWidth = 12;
const int GridLength = GridWidth;
const int GridBitLength = GridWidth * GridLength;
const int PathBitLength = GridWidth + GridLength - 2;

// -- Types
typedef unsigned long long int lint;
typedef unsigned int uint;
typedef bitset<GridBitLength> grid_t;
typedef bitset<PathBitLength> path_t;

// -- Class Definition (Using class to prevent static globals.  It is cleaner)
class PathSolver {
public:
    Grid bitset grid_t *grid;
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
    lint x = 1 << (GridWidth + GridLength - 3);

    PathSolver* solver = new PathSolver();

    //path_t path("100000");
    //cout << solver->isValidPath(path) << endl;

    //cout << endl << path << " minus 1 == ";
    //solver->bitsetMinusOne(path);
    //cout << path << endl;

    // Solver tests
    auto v = solver->GetAllPaths();
    cout << GridWidth << 'x' << GridLength << " grid, " << x  << " possible paths.  ";
    cout << v.size() << " of them are valid." << endl;

    //cout << "They are: " << endl;
    //for (auto iter = v.begin(); iter != v.end(); iter++)
    //    cout << *iter << endl;
}

// Constructor
PathSolver::PathSolver()
{
    int l = GridWidth * GridLength - 1;
    grid = new grid_t(1);
    *grid <<= l;
    grid_last = GridWidth * GridLength - 1;
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
            testGrid >>= GridWidth;
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

            // TEST: When approaching the # of possible permutation of 20x20,
            // this program crashes (core dump) because the vector is
            // denied any new allocation of memory.  I think we may be hitting
            // the limit the kernel imposes per process.  To test this, we will
            // trim the first element from the vector (So the last element is
            // left for comparison).
            //v.erase(v.begin());
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
