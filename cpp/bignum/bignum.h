#ifndef BIGNUM_H
#define BIGNUM_H

class BigNum
{
    // -- Private types
    typedef unsigned long long int lint;
    typedef unsigned int uint;
    static const lint MAX = ~lint(0); // Maximum possible value lint can hold
    // -- Private Members
    lint maxes; // How many max values (MAX) we have
    lint value; // the remainder (So the actual value of this class=(MAX*maxes + value)
    // -- Private Functions
    // utility
    bool willFit(lint, lint);
    // modifiers
    void add(lint);
    void subtract(lint);
    void multiply(lint);
    void divide(lint);

public:

    // -- Constructors
    BigNum(lint v)              { value = v; maxes = 0; }
    BigNum()                    { value = maxes = 0; }
    BigNum(const BigNum& other)
        :maxes(other.maxes), value(other.value) {}
    BigNum& operator=(const BigNum& rhs) {
        value = rhs.value;
        maxes = rhs.maxes;
        return *this;
    }

    // -- Public Math Accessors
    inline void Add(lint x)          { add(x); }
    inline void Subtract(lint x)     { subtract(x); }
    inline void Multiply(lint x)     { multiply(x); }
    inline void Divide(lint x)       { divide(x); }
    BigNum& operator+=(const lint x) { add(x);      return *this; }
    BigNum& operator-=(const lint x) { subtract(x); return *this; }
    BigNum& operator*=(const lint x) { multiply(x); return *this; }
    BigNum& operator/=(const lint x) { divide(x);   return *this; }
    // TODO
    //BigNum& operator+=(const BigNum&);
    //BigNum& operator-=(const BigNum&);
    //BigNum& operator*=(const BigNum&);
    //BigNum& operator/=(const BigNum&);

    // Get Value
    std::string AsString();
};

#endif
