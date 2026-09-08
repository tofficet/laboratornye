#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "primitive_roots.h"

using namespace std;

TEST(PhiTest, SmallNumbers) {
    EXPECT_EQ(phi(1), 1);
    EXPECT_EQ(phi(2), 1);
    EXPECT_EQ(phi(3), 2);
    EXPECT_EQ(phi(4), 2);
    EXPECT_EQ(phi(5), 4);
    EXPECT_EQ(phi(6), 2);
    EXPECT_EQ(phi(7), 6);
    EXPECT_EQ(phi(8), 4);
    EXPECT_EQ(phi(9), 6);
    EXPECT_EQ(phi(10), 4);
}

TEST(PhiTest, LargeNumbers) {
    EXPECT_EQ(phi(30), 8);
    EXPECT_EQ(phi(36), 12);
    EXPECT_EQ(phi(100), 40);
}

TEST(PrimitiveRootExistsTest, TrueCases) {
    EXPECT_TRUE(primitiveRootExists(2));
    EXPECT_TRUE(primitiveRootExists(3));
    EXPECT_TRUE(primitiveRootExists(4));
    EXPECT_TRUE(primitiveRootExists(5));
    EXPECT_TRUE(primitiveRootExists(7));
    EXPECT_TRUE(primitiveRootExists(9));
    EXPECT_TRUE(primitiveRootExists(11));
    EXPECT_TRUE(primitiveRootExists(13));
    EXPECT_TRUE(primitiveRootExists(17));
    EXPECT_TRUE(primitiveRootExists(18));
    EXPECT_TRUE(primitiveRootExists(25));
    EXPECT_TRUE(primitiveRootExists(27));
}

TEST(PrimitiveRootExistsTest, FalseCases) {
    EXPECT_FALSE(primitiveRootExists(8));
    EXPECT_FALSE(primitiveRootExists(12));
    EXPECT_FALSE(primitiveRootExists(15));
    EXPECT_FALSE(primitiveRootExists(16));
    EXPECT_FALSE(primitiveRootExists(20));
    EXPECT_FALSE(primitiveRootExists(21));
    EXPECT_FALSE(primitiveRootExists(24));
    EXPECT_FALSE(primitiveRootExists(28));
    EXPECT_FALSE(primitiveRootExists(30));
}

void checkRoots(int n, const vector<int>& expected) {
    vector<int> roots = primitiveRoots(n);
    EXPECT_EQ(roots.size(), expected.size());
    
    sort(roots.begin(), roots.end());
    vector<int> sortedExpected = expected;
    sort(sortedExpected.begin(), sortedExpected.end());
    
    EXPECT_EQ(roots, sortedExpected);
}

TEST(PrimitiveRootsTest, SmallNumbers) {
    checkRoots(2, {1});
    checkRoots(3, {2});
    checkRoots(4, {3});
    checkRoots(5, {2, 3});
    checkRoots(6, {5});
    checkRoots(7, {3, 5});
    checkRoots(9, {2, 5});
    checkRoots(10, {3, 7});
    checkRoots(11, {2, 6, 7, 8});
    checkRoots(13, {2, 6, 7, 11});
    checkRoots(14, {3, 5});
    checkRoots(17, {3, 5, 6, 7, 10, 11, 12, 14});
    checkRoots(18, {5, 11});
    checkRoots(19, {2, 3, 10, 13, 14, 15});
    checkRoots(25, {2, 3, 8, 12, 13, 17, 22, 23});
    checkRoots(27, {2, 5, 11, 14, 20, 23});
}

TEST(PrimitiveRootsTest, NoRoots) {
    EXPECT_TRUE(primitiveRoots(8).empty());
    EXPECT_TRUE(primitiveRoots(12).empty());
    EXPECT_TRUE(primitiveRoots(15).empty());
    EXPECT_TRUE(primitiveRoots(16).empty());
    EXPECT_TRUE(primitiveRoots(20).empty());
    EXPECT_TRUE(primitiveRoots(21).empty());
}

TEST(GcdTest, Basic) {
    EXPECT_EQ(gcd(12, 8), 4);
    EXPECT_EQ(gcd(17, 19), 1);
    EXPECT_EQ(gcd(0, 5), 5);
    EXPECT_EQ(gcd(5, 0), 5);
}

TEST(PowmodTest, Basic) {
    EXPECT_EQ(powmod(2, 3, 10), 8);
    EXPECT_EQ(powmod(5, 0, 100), 1);
    EXPECT_EQ(powmod(3, 4, 5), 1);
    EXPECT_EQ(powmod(2, 10, 1000), 24);
}

TEST(PrimitiveRootsTest, Boundary) {
    EXPECT_TRUE(primitiveRoots(1).empty());
    
    EXPECT_TRUE(primitiveRoots(0).empty());
}

TEST(PrimitiveRootsTest, VeryLarge) {
    vector<int> roots = primitiveRoots(289);

    // φ(φ(289)) = φ(272) = 128 корней
    EXPECT_EQ(roots.size(), 128);
    
    for (int r : roots) {
        EXPECT_EQ(gcd(r, 289), 1);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}