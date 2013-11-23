
#include <iostream>
#include <bitset>
#include <cmath>
using namespace std;

// Types --
typedef unsigned long int lint;

// Sizes --
static const int GridWidth      = 17;
static const int GridHeight     = GridWidth;
static const int GridBitWidth   = GridWidth * GridHeight;       // bit length of grid
static const int PathBitWidth   = GridWidth + GridHeight - 2;   // bit length of path
static const lint PathMax       = pow(2, PathBitWidth) - 1;     // bit of path length of 1s
static lint GetAllPaths_LoopCount = 0;
// Path Class --
class Path
{
    lint value; // internal value

public:
    // Constructor
    Path(lint initialValue)
        :value(initialValue) {}

    // Tests if a path is valid
    bool isValid() {
        bitset<PathBitWidth> path(value);
        return path.count() == (GridWidth - 1);
    }
    const Path& operator--() {
        this->value -= 1;
        return *this;
    }
    inline bool operator==(const Path& rhs) {
        return this->value == rhs.value;
    }
    inline bool operator==(const lint rhs) {
        return this->value == rhs;
    }
    inline bool operator!=(const Path& rhs) { return !(*this == rhs); }
    inline bool operator>(const Path& rhs) { return this->value > rhs.value; }
    inline bool operator>(const lint rhs) { return this->value > rhs; }
    inline bool operator<(const Path& rhs) { return this->value < rhs.value; }
    inline bool operator<(const lint rhs) { return this->value < rhs; }
    inline lint Value() { return value; }
    lint Flipped() {
        bitset<PathBitWidth> path(value);
        path.flip();
        return path.to_ullong();
    }
};

// Function Prototypes --
lint GetAllPaths();

// Entry (Main) --
int main() {
    cout << "Hello" << endl;

    cout << "Paths: " << GetAllPaths() << endl;
    cout << "Took " << GetAllPaths_LoopCount << " loops." << endl;
}

// Get All Paths --
lint GetAllPaths() {
    // Paths Found
    lint pathCnt = 0;

    // Last found path (Used for fast exit)
    lint lastVPath = PathMax;

    // Start with the initial path
    Path path(PathMax);

    // DEBUG: Reset the Global Loop Count
    GetAllPaths_LoopCount = 0;
    while (path > 0) {
        // DEBUG: Update the loop counter
        ++GetAllPaths_LoopCount;

        // Fast Exit: Check if the last path we found is the path we are
        // currently checking.  If it is, we've passed the median of the
        // bit and can safely exit
        if (path.Flipped() == lastVPath) {
            //cout << "Able to Fast Exit.  Last Path == " << lastVPath << ", Path == " << path.Value() << endl;
            break;
        }

        if (path.isValid()) {
            pathCnt += 2;
            lastVPath = path.Value();
        }

        // Decrement the path and try again
        --path;
    }
    return pathCnt;
}
