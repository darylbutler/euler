
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

typedef unsigned long long int lint;

class bignum
{
    string value;   // internal value

public:
    bignum(unsigned long long int v)    { value = to_string(v); }
    bignum()                            { value= "0"; }

    add(string toAdd) {
        string *sh, *lg;
        stringstream ss;

        if (toAdd.size() > value.size()) {
            sh = &value;
            lg = &toAdd;
        }
        else {
            sh = &toAdd;
            lg = &value;
        }

        int remainder = 0;
        int sum = 0;
        for (int i = lg->size(); i >= 0; i--) {
            if (i >= sh->size()) 
                sum = (lg->at(i) - '0') + remainder;
            else 
                sum = (lg->at{i} - '0') + (sh->at(i) - '0');
            
            sum += remainder;
            remainder = 0;

            if (sum % 10 != 0) {
                remainder = sum % 10;
                sum /= 10;
            }
            ss << sum;
        }




// functions prototypes
lint exponent(lint base, lint exp);
lint sumDigits(lint num);

int main()
{
    cout << "Hello World" << endl;

    cout << "2^1 == " << exponent(2, 1) << endl;
    cout << "2^2 == " << exponent(2, 2) << endl;
    cout << "2^5 == " << exponent(2, 5) << endl;

    cout << endl;
    cout << "Sum of digits:     123 == " << sumDigits(123) << endl;
    cout << "Sum of digits:   12345 == " << sumDigits(12345) << endl;

    cout << " -------- " << endl;
    lint i = exponent(2, 69);
    cout << "2^1000 == " << i << endl;
    cout << "Sum of digits == " << sumDigits(i) << endl;

    cout << sizeof(i) << endl;
}

lint exponent(lint base, lint exp)
{
    lint num = 1;

    for (lint i = 1; i <= exp; i++)
        num *= base;

    return num;
}

lint sumDigits(lint num)
{
    // convert num to string
    string i = to_string(num);

    // Sum each digit
    lint sum = 0;
    for (auto iter = i.begin(); iter != i.end(); iter++)
        sum += *iter - '0';

    return sum;
}
