
#include "gtest/gtest.h"
#include "bignum.cpp"

typedef unsigned long long lint;
static const lint max = ~(lint(0));
static const std::string max_str = "18446744073709551615";

TEST(ArithmaticTest, Addition) {
    BigNum num(0);

    // Add 1
    num += 1;
    EXPECT_EQ("1", num.AsString());
    // ++
    ++num;
    EXPECT_EQ("2", num.AsString());
    // Add Max
    num += max;
    EXPECT_EQ("18446744073709551617", num.AsString());
    num += max;
    EXPECT_EQ("36893488147419103232", num.AsString());
}

TEST(ArithmaticTest, Subtraction) {
    BigNum num(20);

    EXPECT_EQ("20", num.AsString());

    // Subtract 1
    --num;
    EXPECT_EQ("19", num.AsString());

    // Bounds test
    num -= 100000;
    EXPECT_EQ("0", num.AsString());

    // Addition around max
    num += max;
    ++num;
    --num;
    EXPECT_EQ(max_str, num.AsString());

    // Let's make sure that works... (Hit the last condition in subtract()
    num -= 5;
    EXPECT_EQ("18446744073709551610", num.AsString());
}

TEST(ArithmaticTest, Multiplication) {
    BigNum num(5);

    num *= 5;
    EXPECT_EQ("25", num.AsString());

    num *= 20;
    EXPECT_EQ("500", num.AsString());

    num *= max;
    EXPECT_EQ("9223372036854775807500", num.AsString());
}

TEST(ArithmaticTest, Division) {
    BigNum num(10);

    num /= 5;
    EXPECT_EQ("2", num.AsString());

    num.Reset();
    num += (max);
    num *= 500;
    num /= 25;
    EXPECT_EQ("368934881474191032300", num.AsString());
}























int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
