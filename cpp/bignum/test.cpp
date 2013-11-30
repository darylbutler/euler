
#include <iostream>
#include "bignum.cpp"
using namespace std;

int main()
{
    unsigned long long int x = 0;
    x = ~x;
    BigNum num(x);
    cout << num.AsString() << endl;
    cout << num.dump() << endl;
    num -= 5;
    cout << num.AsString() << endl;
    cout << num.dump() << endl;
}
