#include <gtest/gtest.h>
#include <unordered_map>
#include <string>
#include <algorithm>
#include "../src/3rd_round/thirdround.h"

class HashTests : public ::testing::Test {
protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    HashTests() {
        // You can do set-up work for each test here.
    }

    virtual ~HashTests() {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:
    virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    ThirdRoundGenerator gen{0};
};

TEST_F(HashTests, HashOrder) {
    size_t p0, p1, c0, c1, c2, c3, expectedHash;
    expectedHash = 0;
    for (c0 = 0; c0 < 49; c0++)
    for (c1 = c0 + 1; c1 < 50; c1++)
    for (c2 = c1 + 1; c2 < 51; c2++)
    for (c3 = c2 + 1; c3 < 52; c3++) {
        gen.c_inDeck[c0] = false;
        gen.c_inDeck[c1] = false;
        gen.c_inDeck[c2] = false;
        gen.c_inDeck[c3] = false;
        for (p0 = 0; p0 < 51; p0++)
        if (gen.c_inDeck[p0])
        for (p1 = p0 + 1; p1 < 52; p1++) {
            if (gen.c_inDeck[p1]) {
                ASSERT_EQ(expectedHash, gen.getHash(p0, p1, c0, c1, c2, c3))
                    << '(' << p0 << ',' << p1 << ") (" << c0 << ',' << c1 << ',' << c2 << ',' << c3 << ')';
                expectedHash += 1;
            }
        }
        gen.c_inDeck[c0] = true;
        gen.c_inDeck[c1] = true;
        gen.c_inDeck[c2] = true;
        gen.c_inDeck[c3] = true;
    }
}