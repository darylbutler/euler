
// Version 1, time: 2m11s, 2m11s, 2m11s  

#include <vector>
#include <iostream>

using namespace std;

typedef long long int lint;

#define SPDTST

// Function Prototypes
lint iterate(lint);
vector<lint> computeSequence(lint);
lint findLongestSequence(lint);
void printSequence(vector<lint>);


#ifndef SPDTST
int main()
{
    cout << "Euler: Problem 14, Longest Collatz Sequence." << endl;

    for (int i = 1; i <= 20; i++)
        printSequence(computeSequence(i));


    cout << "Longest sequence under 20...  == " << findLongestSequence(20) << endl;
    cout << "Longest sequence under 1m...  == " << findLongestSequence(1000000) << endl;
    
}
#else
int main()
{
    for (int i = 0; i < 10; i++)
        findLongestSequence(1000000);
}
#endif

lint iterate(lint num)
{
    if (num % 2 == 0)
        return num / 2;
    else
        return (num * 3) + 1;
}

vector<lint> computeSequence(lint num)
{
    auto path = vector<lint>();

    lint iter = num;

    while (iter > 0) 
    {
        if (iter == 1)
        {
            path.push_back(1);
            return path;
        }
        path.push_back(iter);
        iter = iterate(iter);
    }
    return path;
}

lint findLongestSequence(lint ceiling)
{
    lint longestNumber = -1;
    lint longestCount = -1;

    for (lint i = 2; i < ceiling; i++)
    {
        lint iterLength = computeSequence(i).size();

        if (iterLength >= longestCount)
        {
            longestNumber   = i;
            longestCount    = iterLength;
        }
    }
    return longestNumber;
}

void printSequence(vector<lint> seq)
{
    cout << "Seq #" << seq.front() << ", " << seq.size() << " => ";
    for (lint iter : seq)
        cout << iter << " > ";
    cout << endl;
}
