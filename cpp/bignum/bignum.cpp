/*
 * Class to represent huge numbers.
 * Value is tracked using a vector of maximum widths integars
 * Value can only be retrived as a string, where the total is computed digit by digit
 * and stored in the returned string.
 *
 * Currently does not support negative numbers
 * Probably will never support anything up integars
 */

#include <vector>
#include <stdint.h> // Do we need this?
#include <sstream>  // for Value output

#include <iostream> // for debug only

#include "bignum.h"

// -- Public Functions --




// -- Private Functions --

// willFit(lint a, lint into)
// checks if 'a' will fit inside 'into' without overflowing.
bool BigNum::willFit(lint a, lint into) {
    // Find out how much room we have in 'into'
    // and see if a will fit into the difference.
    return a < (BigNum::MAX - into);
}

// add(lint)
// We add by checking if we can fit inside our current value.
// if so, we perform simple addition.  Otherwise, we add the difference
// to the value and increment the maxes count.
void BigNum::add(lint toAdd) {
    if (willFit(toAdd, value)) {
        value += toAdd;
    } else {
        lint difference = toAdd - (BigNum::MAX - value);
        value = difference;
        ++maxes;
    }
}

// subtract(lint)
// Same as add, but with subtraction!
// If the value given to subtract is larger than the actual value of this
// instance, the value will be set to zero.
void BigNum::subtract(lint toMinus) {
    if (value >= toMinus) {
        value -= toMinus;
    } else if (maxes == 0) {
        // we could see if value == toMinus, but it doesn't matter.  we
        // have already figured out toMinus >= our total value, so we
        // become zero no matter what.
        value = 0;
    } else {
        //lint difference = toMinus - value;
        value -= toMinus + 1;
        --maxes;
    }
}

// multiply(lint)
// Since the values in the maxes count are just parts of the total sum,
// we can multiply each value.  Multiplcation is just addition.
// 2 x 3 = 6, which is 2 + 2 + 2.  If our max were 2,
// and our initial value was 2 (values.size() == 1), then we only have to
// increment the maxes count twice.
void BigNum::multiply(lint multiple) {
    // Every max value should be duplicated 'multiple' times
    maxes *= multiple;  // TODO possible overflow

    // Now we have to ensure that when we multiply against 'value', it
    // does not overflow (so we can increment the maxes count.
    //
    // I think the easiest way to do this is with addition.
    lint oldVal = value;
    for (int i = 0; i < (multiple - 1); ++i) {
        add(oldVal); // This is some clever shit, eh?  DRY, BABY!
    }
}

// divide(lint)
// Similiar to multiply, we will divide the 'value' and then, for each 'maxes',
// we will determine the difference between MAX and MAX/divisor, and then
// call subtract(difference) for each 'maxes'
void BigNum::divide(lint divisor) {
    // start with the value, since dealing with the 'maxes' will change
    // its value (via subtract() calls)
    value /= divisor;

    // determine the value the divisor will remove from MAX
    lint diff = BigNum::MAX / divisor;
    for (int i = 0; i < maxes; ++i)
        subtract(diff);
}


// -- Output of value as string
// To do this, we have to turn value and MAX into strings.
// Then, we can iterate through them, summing a column
// and carrying any remainders.
// Note that the stringstream requires the sum to be assembled
// backwards.  It should be reversed before returned.
std::string BigNum::AsString() {
    if (maxes == 0) return std::to_string(value);

    const std::string   val = std::to_string(value),
                        max = std::to_string(BigNum::MAX);

    std::stringstream ss;
    uint carry = 0;

    // Iterate for each character.  MAX will definitely always be longest.
    int val_pos = val.size() - 1;
    for (auto iter = max.rbegin(); iter != max.rend(); ++iter) {
        uint sum = 0;

        // -- Calculate Sum
        // If our we're adding digits our smaller value has...
        //int value_pos = max.size() - i - 1;
        if (val_pos >= 0) {
            sum = std::stoi(val.substr(val_pos, 1));
            --val_pos;
        }
        // All the digits in all maxes are the same, so just add it
        sum += (/*char to int*/*iter - '0') * maxes;
        // Add any carry
        sum += carry;

        // -- Split rightmost digit from the carry and insert into string
        carry = sum / 10;
        ss << sum % 10;
    }

    // reverse string
    const std::string flipped = ss.str();
    ss.str(""); // Clear the stream

    // Add in any leftover carry (Add it before the reversed number)
    if (carry > 0)
        ss << carry;

    for (auto iter = flipped.rbegin(); iter != flipped.rend(); ++iter)
        ss << *iter;

    return ss.str();
}

// DEBUG
// Dump the internal value to a string
std::string BigNum::dump()
{
    return "DUMP: Maxes: " + std::to_string(maxes) + ", Value: " + std::to_string(value);
}
