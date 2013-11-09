// Find the first triangle number with 5 factors

#include <iostream>
#include <vector>

using namespace std;

// function prototypes
vector<int> find_factors(int);
int         find_first_trinumber(int);

int main()
{
    cout << "Hello, world!" << endl;

    cout << "find_factors() test:  28: ";
    auto x = find_factors(28);
    for (int i = 0; i < x.size(); i++)
        cout << x.at(i) << ", ";
    cout << endl;

    cout << "find_first_trinumber() test:  5 factors: " << find_first_trinumber(5) << endl;


    cout << "--- The Main Event ----------" << endl;
    cout << "500 factors: " << find_first_trinumber(500) << endl;
}

// find the first triangle number with n factors
int find_first_trinumber(int n)
{
    const int MAX = 1 << 30;
    // iterate up, checking each number for the required number of factors
    int i = 2;  
    int sum = 1;

    while (i < MAX)
    {
        sum += i;
        auto factors = find_factors(sum);

        if (factors.size() >= n)
            return sum;
        else
            i++;
    }
    // i iterated past MAX
    cerr << "Error in function: find_first_trinumber(int).  Iterated past";
    cerr << " MAX(" << MAX << ") before finding " << n << " factors!";
    return -1;
}

// Finds all of the factors of num
vector<int> find_factors(int num)
{
    auto factors = vector<int>();

    // Add 1 and num, since 1 and a number are always factors of itself
    factors.push_back(1);
    factors.push_back(num);

    // keep track of the lowest divisor of factors, so we can quit when
    // we loop past it
    int last_factor = num;  // We technically added num last, since 1/num == num
    int i = 2;  // first factor to try
    
    // Loop to find the factors
    while (i < last_factor)
    {
        if (num % i == 0)
        {
            last_factor = num / i;
            factors.push_back(i);
            factors.push_back(last_factor);
        }
        i++;
    }
    return factors;
}
