// Same idea as problem14.cpp, but with an implementation of a lookup table
// plus a pointer-linked-list.
//
// Speed results:   1) 0m36.7s
//                  2) 0m36.6s
//                  3) 0m36.6s
//          72% faster

#include <iostream>
#include <vector>
#include <map>

using namespace std;

// Use big numbers
typedef long long int lint;

// structure to build the sequence from
struct seq_node 
{
    lint value;
    seq_node* next;
};

// -- Function Prototypes --
vector<lint>    buildSequence(lint);    // Walk the sequence to assemble the values into a vector
lint            countSequence(lint);    // Like buildSequence(), but just count the nodes
void            addToSequence(lint);    // Actually builds the Tree and LoopupLists
lint            iterate(lint);          // Computes the next number in the sequence 
lint            findLongestSequence(lint);  // Finds the number with the longest path to 1 that is under ceiling

// Helpers
seq_node*       _create_node(lint);     // Creates a new node in Tree and updates LoopupList
bool            _sequenced(lint);       // Returns true if LoopupList has a pointer node mapped to the value
void            _print_seq(vector<lint>);   // Debug.  Pretty prints a vector to stdout
void            init();                 
void            cleanup();

// -- Global Vars --
vector<seq_node*>       Tree;       // The node storage (Stores pointers, ensure they're all released)
map<lint, seq_node*>    LookupList; // The index of value to node (by means of a pointer)

#define SPDTST

// -- Main --
#ifndef SPDTST
int main() 
{
    init();

    addToSequence(5);
    auto list = buildSequence(3);
    _print_seq(list);
    cout << "Count Test: of 3 is : " << countSequence(3) << endl;

    cout << "Test longest seq under 1m..." << endl << "Found: " << findLongestSequence(1000000) << endl;

    cleanup();
}
#else
int main()
{
    init();

    for (int i = 0; i < 10; i++)
        findLongestSequence(1000000);

    cleanup();
}
#endif
void init() 
{
    _create_node(1);    // Always prime the Tree array with 1
}
void cleanup()
{
    for (auto node : Tree)
        delete node;
}

// iterate() -- takes a value and returns the next value in the sequence
lint iterate(lint num)
{
    if (num % 2 == 0)
        return num / 2;
    else
        return (num * 3) + 1;
}

// addToSequence()
//   This function should be called when the supplied number isn't in the loopup table,
//   indicating that we've yet to compute that far yet.  So, add the number to the 
//   loopup table and the tree, and compute / add each value until the value already
//   exists in the tree.
void addToSequence(lint num)
{
    lint curr = num;
    seq_node *node, *lastNode;

    if (_sequenced(num)) return;

    lastNode = node = _create_node(curr);

    while (curr > 1)
    {
        curr = iterate(curr);

        if (_sequenced(curr))
        {
            node->next = LookupList.at(curr);
            return;
        }

        lastNode = node;
        node = _create_node(curr);
        lastNode->next = node;
    }
    // Curr should = 1, and 1 is added manually in main().
    if (curr != 1)
        cerr << "Panic at addToSequence().  Danger Will Robertson!" << endl;
}

// buildSequence()
//  This function is called to create and return an array of values, detailing the path the 
//  supplied number follows to 1.  It does this by walking the pointers from LookupList.
vector<lint> buildSequence(lint num)
{
    vector<lint> list;

    if (!_sequenced(num))
        addToSequence(num);

    seq_node* node = LookupList.at(num);
    lint loopCount = 0;

    while (node->value != 1 && loopCount < (1 << 30))
    {
        list.push_back(node->value);
        node = node->next;
        loopCount++;
    }

    if (node->value == 1)
        list.push_back(1);

    return list;
}

// countSequence()
//  This function is exactly like buildSequence above, except we simply count
//  the nodes in the sequence instead of making a list of them.
lint countSequence(lint num)
{
    lint count = 0;

    if (!_sequenced(num))
        addToSequence(num);

    seq_node* node = LookupList.at(num);
    lint loopCount = 0;

    while (node->value != 1 && loopCount < (1 << 30))
    {
        count++;
        node = node->next;
        loopCount++;
    }

    if (node->value == 1)
        count++;

    return count;
}

// findLongestSequence()
//  This functions starts at 2 and increments to ceiling, iterately building
//  the sequence (through countSequence()'s addToSequence() call), counting
//  each path so it can return the number that resulted in the longest chain.
lint findLongestSequence(lint ceiling)
{
    lint longestNum = -1;
    lint longestCnt = -1;

    for (lint i = 2; i < ceiling; i++)
    {
        lint iterCnt = countSequence(i);
        if (iterCnt > longestCnt)
        {
            longestCnt = iterCnt;
            longestNum = i;
        }
    }
    return longestNum;
}

// _create_node()
seq_node* _create_node(lint num)
{
    // Create new node
    seq_node* node = new seq_node();
    node->value = num;
    // Store the pointer
    Tree.push_back(node);
    LookupList[num] = node;
    // Return pointer
    return node;
}

// _sequenced()
bool _sequenced(lint num)
{
    auto iter = LookupList.find(num);

    if (iter == LookupList.end())
        return false;
    return true;
}

// _print_seq
void _print_seq(vector<lint> list)
{
    cout << "Seq #" << list.front() << ", " << list.size() << " => ";
    for (lint iter : list)
        cout << iter << " > ";
    cout << endl;
}
