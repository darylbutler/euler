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
 *
 * Revision 1:  implemented below todo
 * TODO: We should be able to add the opposite of any valid path.  Problem is
 *       how can we not iterate over it again when its natural permutation
 *       come around?  Will it be effecient to check our list of valid paths
 *       against each possible path?
 *
 * Revision 2:  Implement assumption #3, that in any valid path, the
 * count of 1's should == GridWidth, and the count of 0's should be == to
 * GridHeight, since the distance to travel across the grid is constant.
 * We make two adjustments:
 *  - The initial path generated in GetAllPaths will now start at the max legal path
 *  (Count(1s) = GridWdith, rest=Count(0s)) so we start iterating closer to our
 *  target
 *  - Quick check function added to isValidPath to prevent following the path
 *  out when we can tell by bit value count can quickly tell us no.
 *
 * Revision 3:  Fixed major bug.  When I shifted 1 << x, 1 is still an int since
 *  I had not cast it to a lint, resulting in buffer overflows when X got > 31.
 *  I. Am. An. Idiot...
 *
 *
 * 1 0 0
 * 0 0 0
 * 0 0 0
 *
 * 1111 iv  1110    iv
 * 1101 iv  1100    v
 * 1011 iv  1010    v
 * 1001 v   1000    iv
 * 0111 iv  0110    v
 * 0101 v   0100    iv
 * 0011 v   0010    iv
 * 0010 iv  0001    iv
 * 0000 iv
 */

#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

const int WIDTH = 20;
const int LENGTH = WIDTH;
const int GBITS = WIDTH * LENGTH;
const int PBITS = WIDTH + LENGTH - 2;

// -- Types
//typedef unsigned long long int lint;
typedef uint_fast64_t lint;
typedef unsigned int uint;
typedef bitset<GBITS> grid_t;
typedef bitset<PBITS> path_t;

// vector overflow?
static lint found = 0;

// -- Class Definition (Using class to prevent static globals.  It is cleaner)
class PathSolver
{
public:
    // Grid bitset
    grid_t *grid;

    // Private Methods
    bool isValidPath(const path_t&);
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
    lint x = (lint)1 << (WIDTH + LENGTH - 3);

    PathSolver* solver = new PathSolver();

    //path_t path("100000");
    //cout << solver->isValidPath(path) << endl;

    //cout << endl << path << " minus 1 == ";
    //solver->bitsetMinusOne(path);
    //cout << path << endl;

    // Solver tests
    auto v = solver->GetAllPaths();
    cout << WIDTH << 'x' << LENGTH << " grid, 2^" << (WIDTH + LENGTH - 3) << " (" <<  x  << ") possible paths.  ";
    //cout << v.size() << " of them are valid." << endl;
    cout << found << " of them are valid." << endl;

    //cout << "They are: " << endl;
    //for (auto iter = v.begin(); iter != v.end(); iter++)
        //cout << *iter << endl;
}

// Constructor
PathSolver::PathSolver()
{
    // Create the grid that is the length of the grid flattened, with the left most bit
    // set (So we start at the 'top left'
    int shiftLengthToMSB = WIDTH * LENGTH - 1ULL;
    grid = new grid_t(1);
    *grid <<= shiftLengthToMSB;
}

// Private Methods - Does this path result in grid == 1?  (See intro comments)
bool PathSolver::isValidPath(const path_t &path)
{
    //return false;
    // Revision 2:  Quick ruling out of paths by counting bits
    if (path.count() != (WIDTH - 1))
        return false;

    // Copy the grid so we can walk it with the test path
    grid_t testGrid = *grid;

    // Apply each bit in path to the test grid.
    for (lint i = 0; i < path.size(); i++) {
        if (path.test(i)) {
            // true = right, shift by 1
            testGrid >>= 1;
        } else {
            // false = down, shift by grid_width
            testGrid >>= WIDTH;
        }
    }
    // All path actions have been completed, so testGrid should equal 1 if
    // path is valid
    return (testGrid[0] && testGrid.count() == 1);
}

// Standard entry point.  Sets up and starts the loop to find all paths to the br corner
vector<path_t> PathSolver::GetAllPaths()
{
    // Try every combination of paths possible, checking each one with
    // isValidPath.  If it is, it is added to the return vector.
    vector<path_t> v;   // TODO Can we estimate the size needed here?
    path_t path;
    //path_t path, lastPathFlipped;

    // Set path to its maximum value
    // Revision 2: Max value has # of 1's == to GridWidth and similar with 0's
    path.set();
    //for (int i = 0; i < (LENGTH - 1); i++)
        //path.reset(i);
    //lastPathFlipped = path;     // Assgin a sane starting value
    //lastPathFlipped.flip();     // Has to be flipped or it will fail at start of do{}

    do {
        // If this path is equal to the opposite of the last path we evaluated,
        // we've reached the middle of the grid and have found all paths
        //if (lastPathFlipped == path)
            //return v;

        if (isValidPath(path)) {
            // Path was valid, add it to the return vector
            //v.push_back(path);
            // Also add its opposite
            //path.flip();
            //v.push_back(path);
            // reset path so iteration can continue
            //path.flip();
            //found += 2;
            found += 1;
        }

        // Subtract one and loop again
        //lastPathFlipped = path;
        //lastPathFlipped.flip();
        bitsetMinusOne(path);
    } while (path.count() > 0);

    return v;
}


void PathSolver::bitsetMinusOne(path_t &num)
{
    // test
    lint x = num.to_ullong();
    num = path_t(x - 1);
    return;

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
