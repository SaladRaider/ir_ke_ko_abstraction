#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <algorithm>
#include "../src/3rd_round/thirdround.h"

class ThirdRoundGeneratorTests : public ::testing::Test {
protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    ThirdRoundGeneratorTests() {
        // You can do set-up work for each test here.
    }

    virtual ~ThirdRoundGeneratorTests() {
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
    ThirdRoundGenerator gen{0};
};

TEST_F(ThirdRoundGeneratorTests, HashOrder1st) {
    size_t p0, p1, expectedHash;
    expectedHash = 0;
    for (p0 = 0; p0 < 51; p0++)
    for (p1 = p0 + 1; p1 < 52; p1++) {
        ASSERT_EQ(expectedHash, gen.getHash1st(p0, p1))
            << '(' << p0 << ',' << p1 << ")\n";
        expectedHash += 1;
    }
}

TEST_F(ThirdRoundGeneratorTests, HashOrder2nd) {
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
                ASSERT_EQ(expectedHash, gen.getHash2nd(p0, p1, c0, c1, c2))
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

TEST_F(ThirdRoundGeneratorTests, HashOrder3rd) {
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
                    << '(' << p0 << ',' << p1 << ") (" << c0 << ',' << c1
                    << ',' << c2 << ',' << c3 << ")\n";
                expectedHash += 1;
            }
        }
        gen.c_inDeck[c0] = true;
        gen.c_inDeck[c1] = true;
        gen.c_inDeck[c2] = true;
        gen.c_inDeck[c3] = true;
    }
}

TEST_F(ThirdRoundGeneratorTests, BucketDistribution) {
    std::array<int, 50> buckets;
    buckets.fill(0);
    size_t p0, p1, c0, c1, c2, c3, c4, h0, h1, h2, h3, h4, thirdRoundHash, count;
    count = 0;
    for (c0 = 0; c0 < 48; c0++)
    for (c1 = c0 + 1; c1 < 49; c1++)
    for (c2 = c1 + 1; c2 < 50; c2++)
    for (c3 = c2 + 1; c3 < 51; c3++)
    for (c4 = c3 + 1; c4 < 52; c4++) {
        gen.c_inDeck[c0] = false;
        gen.c_inDeck[c1] = false;
        gen.c_inDeck[c2] = false;
        gen.c_inDeck[c3] = false;
        gen.c_inDeck[c4] = false;
        for (p0 = 0; p0 < 51; p0++)
        if (gen.c_inDeck[p0])
        for (p1 = p0 + 1; p1 < 52; p1++)
        if (gen.c_inDeck[p1]) {
            if (count % 791317311 != 0) {
                count += 1;
                continue;
            }
            // Create dummy buckets file
            int bucketIdx = rand() % 50;
            float mean = float(bucketIdx) / float(50);
            int bucketVal = 0;
            std::ofstream f;
            f.open("test_dummyBuckets.txt");
            f << mean << '\n';
            f.close();
            printf("bucketIdx: %d; mean: %f;\n", bucketIdx, mean);
            printf("(%zu,%zu) (%zu,%zu,%zu,%zu,%zu)\n",
                    p0,p1,c0,c1,c2,c3,c4);

            ThirdRoundGenerator *genThird = new ThirdRoundGenerator{0};
            genThird->compute3rdRound(count, count + 1,
                                      "test_dummyBuckets.txt");
            h0 = genThird->getHash(p0,p1,c0,c1,c2,c3);
            h1 = genThird->getHash(p0,p1,c0,c1,c2,c4);
            h2 = genThird->getHash(p0,p1,c0,c1,c3,c4);
            h3 = genThird->getHash(p0,p1,c0,c2,c3,c4);
            h4 = genThird->getHash(p0,p1,c1,c2,c3,c4);
            std::ifstream infile;
            std::string fileBuffer;

            genThird->saveBuckets(h0, h0 + 1, "test_dummySavedBuckets.txt");
            infile.open("test_dummySavedBuckets.txt");
            std::getline(infile, fileBuffer);
            std::istringstream iss(fileBuffer);
            for (int i = 0; i < bucketIdx + 1; i++) {
                iss >> bucketVal;
                buckets[i] = bucketVal;
            }
            infile.close();
            std::string bucketsStr(std::begin(buckets), std::end(buckets));
            ASSERT_EQ(1, buckets[bucketIdx]) << "h0;  3rd round\n";

            genThird->saveBuckets(h1, h1 + 1, "test_dummySavedBuckets.txt");
            infile.open("test_dummySavedBuckets.txt");
            std::getline(infile, fileBuffer);
            std::istringstream iss2(fileBuffer);
            for (int i = 0; i < bucketIdx + 1; i++) {
                iss2 >> bucketVal;
            }
            infile.close();
            ASSERT_EQ(1, bucketVal) << "h1; 3rd round\n";

            genThird->saveBuckets(h2, h2 + 1, "test_dummySavedBuckets.txt");
            infile.open("test_dummySavedBuckets.txt");
            std::getline(infile, fileBuffer);
            std::istringstream iss3(fileBuffer);
            for (int i = 0; i < bucketIdx + 1; i++) {
                iss3 >> bucketVal;
            }
            infile.close();
            ASSERT_EQ(1, bucketVal) << "h2; 3rd round\n";

            genThird->saveBuckets(h3, h3 + 1, "test_dummySavedBuckets.txt");
            infile.open("test_dummySavedBuckets.txt");
            std::getline(infile, fileBuffer);
            std::istringstream iss4(fileBuffer);
            for (int i = 0; i < bucketIdx + 1; i++) {
                iss4 >> bucketVal;
            }
            infile.close();
            ASSERT_EQ(1, bucketVal) << "h3; 3rd round\n";

            genThird->saveBuckets(h4, h4 + 1, "test_dummySavedBuckets.txt");
            infile.open("test_dummySavedBuckets.txt");
            std::getline(infile, fileBuffer);
            std::istringstream iss5(fileBuffer);
            for (int i = 0; i < bucketIdx + 1; i++) {
                iss5 >> bucketVal;
            }
            infile.close();
            ASSERT_EQ(1, bucketVal) << "h4; 3rd round\n";

            genThird->compute2ndRound(h0, h0 + 1);
            h0 = genThird->getHash2nd(p0,p1,c0,c1,c2);
            h1 = genThird->getHash2nd(p0,p1,c0,c1,c3);
            h2 = genThird->getHash2nd(p0,p1,c0,c2,c3);
            h3 = genThird->getHash2nd(p0,p1,c1,c2,c3);

            {
                genThird->saveBuckets2nd(h0, h0 + 1, "test_dummySavedBuckets.txt");
                infile.open("test_dummySavedBuckets.txt");
                std::getline(infile, fileBuffer);
                std::istringstream iss(fileBuffer);
                for (int i = 0; i < bucketIdx + 1; i++) {
                    iss >> bucketVal;
                }
                infile.close();
                ASSERT_EQ(1, bucketVal) << "h0; 2nd round\n";
            }
            {
                genThird->saveBuckets2nd(h1, h1 + 1, "test_dummySavedBuckets.txt");
                infile.open("test_dummySavedBuckets.txt");
                std::getline(infile, fileBuffer);
                std::istringstream iss(fileBuffer);
                for (int i = 0; i < bucketIdx + 1; i++) {
                    iss >> bucketVal;
                }
                infile.close();
                ASSERT_EQ(1, bucketVal) << "h1; 2nd round\n";
            }
            {
                genThird->saveBuckets2nd(h2, h2 + 1, "test_dummySavedBuckets.txt");
                infile.open("test_dummySavedBuckets.txt");
                std::getline(infile, fileBuffer);
                std::istringstream iss(fileBuffer);
                for (int i = 0; i < bucketIdx + 1; i++) {
                    iss >> bucketVal;
                }
                infile.close();
                ASSERT_EQ(1, bucketVal) << "h2; 2nd round\n";
            }
            {
                genThird->saveBuckets2nd(h2, h2 + 1, "test_dummySavedBuckets.txt");
                infile.open("test_dummySavedBuckets.txt");
                std::getline(infile, fileBuffer);
                std::istringstream iss(fileBuffer);
                for (int i = 0; i < bucketIdx + 1; i++) {
                    iss >> bucketVal;
                }
                infile.close();
                ASSERT_EQ(1, bucketVal) << "h3; 2nd round\n";
            }

            genThird->compute1stRound(h0, h0 + 1);
            genThird->compute1stRound(h1, h1 + 1);
            genThird->compute1stRound(h2, h2 + 1);
            genThird->compute1stRound(h3, h3 + 1);
            h0 = genThird->getHash1st(p0,p1);
            {
                genThird->saveBuckets1st(h0, h0 + 1, "test_dummySavedBuckets.txt");
                infile.open("test_dummySavedBuckets.txt");
                std::getline(infile, fileBuffer);
                std::istringstream iss(fileBuffer);
                for (int i = 0; i < bucketIdx + 1; i++) {
                    iss >> bucketVal;
                }
                infile.close();
                ASSERT_EQ(4, bucketVal) << "h0=" << h0 << "; 1st round\n";
            }

            delete genThird;
            count += 1;
        }
        gen.c_inDeck[c0] = true;
        gen.c_inDeck[c1] = true;
        gen.c_inDeck[c2] = true;
        gen.c_inDeck[c3] = true;
        gen.c_inDeck[c4] = true;
    }

    std::remove("test_dummyBuckets.txt");
}