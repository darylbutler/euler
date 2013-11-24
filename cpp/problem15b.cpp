/*
 * Rethinking this approach.  Instead of >finding< the correct paths, let's
 * >build< valid paths.  We have all the information we need.
 *
 */

#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

// -- Types
typedef unsigned long long int lint;

// -- Constants
static const int PathSize = 04;

// -- Globals
static lint Iterations = 0;

// -- Path Class
struct Path
{
    static const int NumOfOnes = PathSize / 2;
    bitset<PathSize> bits;
    vector<int> ones;       // Indexes of 1's in bits

    void stepOnes() {
        // Advance the one's positions leftward in the bit (Just update the indices vector)
        // If a bit has reached the end, then we'll increment the next bit and reset the
        // last bits

        // Simplest situation: 0011 -> 0101 (There is room for the last 1 to move forard)
        if (ones.back() < PathSize - 1) {
            ++ones.back();
            return;
        }
        else {
            // Search backwards in ones until we find a bit that isn't next to a 1
            // i.e., has room to move left.
            int max = PathSize - 1;
            for (int iter = ones.size() - 1; iter >= 0; --iter) {
                if (ones[iter] < max) {
                    // Move this and the other indexes next to it
                    int new_pos = ++(ones[iter]);
                    // Now traverse the opposite direction...
                    ++iter; // First, move off of the last bit we just moved before loop
                    for ( ; iter < ones.size(); ++iter) {
                        // set the other 1's next to the incremented 1 index
                        ones[iter] = ++new_pos;
                    }
                    // We're done
                    return;
                }
                else {
                    --max;  // Max has to decrement because the bit we just checked is
                    // occuping that space (We can't shove it into the 1 beside it
                }
            }
        }
    }
//public:
    // The function that does all the work
    const lint CountPaths() {
        lint count = 0;
        Iterations = 0;
        bits.reset();
        ones.clear();
        // setup the indices of the 1's
        for (int x = 0; x < NumOfOnes; x++)
            ones.push_back(x);
        // generate the finished test (All the 1's are to the right)
        lint isFinishedTest = 0;
        for (int x = 0; x < PathSize; x++) {
            // Add a zero...
            isFinishedTest <<= 1;
            // we we're below NumOfOnes, we're insterting 1's
            if (x < NumOfOnes)
                ++isFinishedTest;
        }

/*
        // Start the loop
        while (isFinishedTest != bits.to_ullong()) {
            // This path is valid, so count it
            ++count;
            // We're iterating
            ++Iterations;
            // Set the 1's according to their positions in the 'ones' vector
            bits.reset();
            for (auto iter = ones.begin(); iter != ones.end(); ++iter) {
                bits.set(*iter);
            }
            // Increment the 1's positions
            stepOnes();
        }
*/
        // Store the last found bitset for fast exit
        bitset<PathSize> lastBits(bits);
        // Start the loop
        while (isFinishedTest != bits.to_ullong()) {
                        // We're iterating
            ++Iterations;
            // Set the 1's according to their positions in the 'ones' vector
            bits.reset();
            for (auto iter = ones.begin(); iter != ones.end(); ++iter) {
                bits.set(*iter);
            }
            // Check if this bitset is the opposite of the last one tried.  If so, exit
            lastBits.flip();
            if (lastBits == bits) return count;
            // Reset the last bit
            lastBits = bits;

            // Increment the 1's positions
            stepOnes();
            // This path is valid, so count it
            // Edit: Moved this to the end, so the last bit found on fast exit isn't counted
            count += 2; // +2 since we also found its opposite (And we have fast exit)

        }


        return count;
    }
};

void print(vector<int>& v);

int main()
{
    Path path;
    cout << "Path Cnt: " << path.CountPaths() << endl;
    cout << "Took " << Iterations << " loops" << endl;
}

void print(vector<int>& v) {
    for (auto iter = v.begin(); iter != v.end(); ++iter)
        cout << *iter << endl;
}
