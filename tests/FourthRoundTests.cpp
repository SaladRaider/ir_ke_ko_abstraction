#include <gtest/gtest.h>
#include <unordered_map>
#include <string>
#include <algorithm>
#include "../src/4th_round/fourthround.h"

class HandEvalTests : public ::testing::Test {
protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    HandEvalTests() {
        // You can do set-up work for each test here.
    }

    virtual ~HandEvalTests() {
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

    void expectHandType(std::string s0, std::string s1, std::string s2,
                        std::string s3, std::string s4, std::string s5,
                        std::string s6, HandIndex handIndex,
                        int c0, int c1, int c2, int c3, int c4,
                        std::unique_ptr<HandType> &handType) {
        std::array<int, 7> cards = {
            this->deckMap[s0], this->deckMap[s1], this->deckMap[s2],
            this->deckMap[s3], this->deckMap[s4], this->deckMap[s5],
            this->deckMap[s6]
        };
        std::sort(cards.begin(), cards.end());
        findHandType(handType, &cards, &sFlush, &sFlushSize, &straightMask);
        EXPECT_EQ(handIndex, handType->handIndex);
        EXPECT_EQ(c0, handType->keyCardIndex[0]);
        EXPECT_EQ(c1, handType->keyCardIndex[1]);
        EXPECT_EQ(c2, handType->keyCardIndex[2]);
        EXPECT_EQ(c3, handType->keyCardIndex[3]);
        EXPECT_EQ(c4, handType->keyCardIndex[4]);
    }

    // Objects declared here can be used by all tests in the test case for Project1.
    std::unique_ptr<HandType> handType = std::unique_ptr<HandType> (new HandType);
    std::unordered_map<int, std::unique_ptr<HandType>> handTypeCache;
    std::array<std::array<int, 5>, 4> sFlush;
    std::array<int, 4> sFlushSize;
    std::array<int, 15> straightMask;
    std::unordered_map<std::string, int> deckMap = {
        {"2H",0}, {"2D",1}, {"2C",2}, {"2S",3},
        {"3H",4}, {"3D",5}, {"3C",6}, {"3S",7},
        {"4H",8}, {"4D",9}, {"4C",10}, {"4S",11},
        {"5H",12}, {"5D",13}, {"5C",14}, {"5S",15},
        {"6H",16}, {"6D",17}, {"6C",18}, {"6S",19},
        {"7H",20}, {"7D",21}, {"7C",22}, {"7S",23},
        {"8H",24}, {"8D",25}, {"8C",26}, {"8S",27},
        {"9H",28}, {"9D",29}, {"9C",30}, {"9S",31},
        {"TH",32}, {"TD",33}, {"TC",34}, {"TS",35},
        {"JH",36}, {"JD",37}, {"JC",38}, {"JS",39},
        {"QH",40}, {"QD",41}, {"QC",42}, {"QS",43},
        {"KH",44}, {"KD",45}, {"KC",46}, {"KS",47},
        {"AH",48}, {"AD",49}, {"AC",50}, {"AS",51}
    };
};

TEST_F(HandEvalTests, HighCard) {
    expectHandType(
        "2H", "4D", "5C", "6H", "7H", "9H", "TD",
        HIGH_CARD, 6, 5, 4, 3, 2, handType);
}

TEST_F(HandEvalTests, OnePair) {
    expectHandType(
        "2H", "2D", "5C", "6H", "7H", "9H", "TD",
        ONE_PAIR, 1, 0, 6, 5, 4, handType);
}

TEST_F(HandEvalTests, TwoPair) {
    expectHandType(
        "2H", "2D", "5C", "6H", "7H", "7D", "TD",
        TWO_PAIR, 5, 4, 1, 0, 6, handType);
}

TEST_F(HandEvalTests, ThreeOfAKind) {
    expectHandType(
        "2H", "2D", "2C", "6H", "7H", "9D", "TD",
        THREE_OF_A_KIND, 2, 1, 0, 6, 5, handType);
}

TEST_F(HandEvalTests, Straight) {
    expectHandType(
        "2H", "3D", "4C", "5H", "6H", "9D", "TD",
        STRAIGHT, 4, 3, 2, 1, 0, handType);
}

TEST_F(HandEvalTests, Flush) {
    expectHandType(
        "2H", "3H", "4H", "5H", "7H", "9D", "TD",
        FLUSH, 4, 3, 2, 1, 0, handType);
}

TEST_F(HandEvalTests, FullHouse) {
    expectHandType(
        "2H", "2D", "2C", "5H", "9H", "9D", "TD",
        FULL_HOUSE, 2, 1, 0, 5, 4, handType);
}

TEST_F(HandEvalTests, FourOfAKind) {
    expectHandType(
        "2H", "2D", "2C", "2S", "9H", "9D", "TD",
        FOUR_OF_A_KIND, 3, 2, 1, 0, 6, handType);
}

TEST_F(HandEvalTests, StraightFlush) {
    expectHandType(
        "2H", "3H", "4H", "5H", "6H", "9D", "TD",
        STRAIGHT_FLUSH, 4, 3, 2, 1, 0, handType);
}

TEST_F(HandEvalTests, RoyalFlush) {
    expectHandType(
        "9C", "9H", "TD", "JD", "QD", "KD", "AD",
        ROYAL_FLUSH, 6, 5, 4, 3, 2, handType);
}
