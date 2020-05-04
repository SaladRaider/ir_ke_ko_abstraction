#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <algorithm>
#include "../src/2nd_round/secondround.h"

class SecondRoundTests : public ::testing::Test {
protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    SecondRoundTests() {
        // You can do set-up work for each test here.
    }

    virtual ~SecondRoundTests() {
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

    // call dummy constructor to prevent full allocation of buckets
    SecondRoundGenerator gen{0};
};

TEST_F(SecondRoundTests, HashOrder) {
    size_t p0, p1, c0, c1, c2, expectedHash;
    expectedHash = 0;
    for (c0 = 0; c0 < 50; c0++)
    for (c1 = c0 + 1; c1 < 51; c1++)
    for (c2 = c1 + 1; c2 < 52; c2++) {
        gen.c_inDeck[c0] = false;
        gen.c_inDeck[c1] = false;
        gen.c_inDeck[c2] = false;
        for (p0 = 0; p0 < 51; p0++)
        if (gen.c_inDeck[p0])
        for (p1 = p0 + 1; p1 < 52; p1++) {
            if (gen.c_inDeck[p1]) {
                ASSERT_EQ(expectedHash, gen.getHash(p0, p1, c0, c1, c2))
                    << '(' << p0 << ',' << p1 << ") (" << c0 << ',' << c1
                    << ',' << c2 << ")\n";
                expectedHash += 1;
            }
        }
        gen.c_inDeck[c0] = true;
        gen.c_inDeck[c1] = true;
        gen.c_inDeck[c2] = true;
    }
}

TEST_F(SecondRoundTests, BucketDistribution) {
    // Create dummy buckets file
    std::array<std::array<int, 50>, 1> dummyBuckets;
    std::array<std::array<int, 50>, 1> dummySavedBuckets;
    for (size_t i = 0; i < 50; i++) {
        dummyBuckets[0][i] = rand() % 100;
    }
    dummySavedBuckets[0].fill(0);
    std::ofstream f;
    f.open("test_dummyBuckets.txt");
    for (size_t i = 0; i < dummyBuckets.size(); i++) {
        for (size_t j = 0; j < 49; j++) {
            f << int(dummyBuckets[i][j]) << ' ';
        }
        f << int(dummyBuckets[i][49]) << '\n';
    }
    f.close();

    size_t p0, p1, c0, c1, c2, c3, h0, h1, h2, h3, thirdRoundHash, count;
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
        for (p1 = p0 + 1; p1 < 52; p1++)
        if (gen.c_inDeck[p1]) {
            if (count % 7913173 != 0) {
                count += 1;
                continue;
            }
            SecondRoundGenerator *genSecond = new SecondRoundGenerator{0};
            genSecond->compute2ndRound(count, count + 1,
                                       "test_dummyBuckets.txt");
            h0 = genSecond->getHash(p0,p1,c0,c1,c2);
            h1 = genSecond->getHash(p0,p1,c0,c1,c3);
            h2 = genSecond->getHash(p0,p1,c0,c2,c3);
            h3 = genSecond->getHash(p0,p1,c1,c2,c3);
            std::ifstream infile;
            std::string fileBuffer;
            genSecond->saveBuckets(h0, h0 + 1, "test_dummySavedBuckets.txt");
            infile.open("test_dummySavedBuckets.txt");
            std::getline(infile, fileBuffer);
            std::istringstream iss(fileBuffer);
            for (size_t i = 0; i < 50; i++) {
                iss >> dummySavedBuckets[0][i];
            }
            infile.close();
            for (size_t i = 0; i < 50; i++) {
                EXPECT_EQ(dummyBuckets[0][i], dummySavedBuckets[0][i])
                    << "h0 -> i:" << i << ";\n";
            }
            genSecond->saveBuckets(h1, h1 + 1, "test_dummySavedBuckets.txt");
            infile.open("test_dummySavedBuckets.txt");
            std::getline(infile, fileBuffer);
            std::istringstream iss2(fileBuffer);
            for (size_t i = 0; i < 50; i++) {
                iss2 >> dummySavedBuckets[0][i];
            }
            infile.close();
            for (size_t i = 0; i < 50; i++) {
                EXPECT_EQ(dummyBuckets[0][i], dummySavedBuckets[0][i])
                    << "h1 -> i:" << i << ";\n";
            }
            genSecond->saveBuckets(h2, h2 + 1, "test_dummySavedBuckets.txt");
            infile.open("test_dummySavedBuckets.txt");
            std::getline(infile, fileBuffer);
            std::istringstream iss3(fileBuffer);
            for (size_t i = 0; i < 50; i++) {
                iss3 >> dummySavedBuckets[0][i];
            }
            infile.close();
            for (size_t i = 0; i < 50; i++) {
                EXPECT_EQ(dummyBuckets[0][i], dummySavedBuckets[0][i])
                    << "h2 -> i:" << i << ";\n";
            }
            genSecond->saveBuckets(h3, h3 + 1, "test_dummySavedBuckets.txt");
            infile.open("test_dummySavedBuckets.txt");
            std::getline(infile, fileBuffer);
            std::istringstream iss4(fileBuffer);
            for (size_t i = 0; i < 50; i++) {
                iss4 >> dummySavedBuckets[0][i];
            }
            infile.close();
            for (size_t i = 0; i < 50; i++) {
                EXPECT_EQ(dummyBuckets[0][i], dummySavedBuckets[0][i])
                    << "h3 -> i:" << i << ";\n";
            }
            delete genSecond;
            count += 1;
        }
        gen.c_inDeck[c0] = true;
        gen.c_inDeck[c1] = true;
        gen.c_inDeck[c2] = true;
        gen.c_inDeck[c3] = true;
    }

    std::remove("test_dummyBuckets.txt");
}
