#include <gtest/gtest.h>
#include <unordered_map>
#include <string>
#include <algorithm>
#include "../src/4th_round/fourthround.h"

typedef enum HandResult { LOSS, TIE, WIN };

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

    void expectHandComparison(
        std::string p0, std::string p1,
        std::string e0, std::string e1,
        std::string c0, std::string c1, std::string c2,
        std::string c3, std::string c4, int expected) {
        std::array<int, 7> pCards = {
            this->deckMap[p0], this->deckMap[p1], this->deckMap[c0],
            this->deckMap[c1], this->deckMap[c2], this->deckMap[c3],
            this->deckMap[c4]
        };
        std::array<int, 7> eCards = {
            this->deckMap[e0], this->deckMap[e1], this->deckMap[c0],
            this->deckMap[c1], this->deckMap[c2], this->deckMap[c3],
            this->deckMap[c4]
        };
        std::sort(pCards.begin(), pCards.end());
        std::sort(eCards.begin(), eCards.end());
        int result = getValue(
            &pCards, &eCards,
            &handTypeCache, &sFlush, &sFlushSize, &straightMask);
        EXPECT_EQ(expected, result) << "pCards:(" << p0 << "," << p1 << ") eCards:(" << e0 << "," << e1 << ") cCards:(" << c0 << "," << c1 << "," << c2 << "," << c3 << "," << c4 << ")\n";
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
    expectHandType(
        "2H", "3D", "4C", "5H", "8H", "9D", "AD",
        STRAIGHT, 3, 2, 1, 0, 6, handType);
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
    expectHandType(
        "2H", "3H", "4H", "5H", "8C", "9D", "AH",
        STRAIGHT_FLUSH, 3, 2, 1, 0, 6, handType);
}

TEST_F(HandEvalTests, RoyalFlush) {
    expectHandType(
        "9C", "9H", "TD", "JD", "QD", "KD", "AD",
        ROYAL_FLUSH, 6, 5, 4, 3, 2, handType);
}

TEST_F(HandEvalTests, HighCardVSOnePair) {
    expectHandComparison(
        "AC", "2H",                   // hero cards
        "3D", "5S",                   // villan cards
        "3C", "8H", "TD", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3D", "5S",                   // hero cards
        "AC", "2H",                   // villan cards
        "3C", "8H", "TD", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, HighCardVSTwoPair) {
    expectHandComparison(
        "AC", "2H",                   // hero cards
        "3D", "TS",                   // villan cards
        "3C", "8H", "TD", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3D", "TS",                   // hero cards
        "AC", "2H",                   // villan cards
        "3C", "8H", "TD", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, HighCardVSThreeOfAKind) {
    expectHandComparison(
        "AC", "2H",                   // hero cards
        "3D", "3H",                   // villan cards
        "3C", "8H", "TD", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3D", "3H",                   // hero cards
        "AC", "2H",                   // villan cards
        "3C", "8H", "TD", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, HighCardVSStraight) {
    expectHandComparison(
        "AC", "2H",                   // hero cards
        "9D", "KH",                   // villan cards
        "3C", "8H", "TD", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "9D", "KH",                   // hero cards
        "AC", "2H",                   // villan cards
        "3C", "8H", "TD", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, HighCardVSFlush) {
    expectHandComparison(
        "AC", "2H",                   // hero cards
        "4H", "3H",                   // villan cards
        "3C", "8H", "TD", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "4H", "3H",                   // hero cards
        "AC", "2H",                   // villan cards
        "3C", "8H", "TD", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, HighCardVSStraightFlush) {
    expectHandComparison(
        "AC", "2H",                   // hero cards
        "9H", "KH",                   // villan cards
        "3C", "8D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "9H", "KH",                   // hero cards
        "AC", "2H",                   // villan cards
        "3C", "8D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, HighCardVSRoyalFlush) {
    expectHandComparison(
        "AC", "2H",                   // hero cards
        "KH", "AH",                   // villan cards
        "3C", "8D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "KH", "AH",                   // hero cards
        "AC", "2H",                   // villan cards
        "3C", "8D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, OnePairVSTwoPair) {
    expectHandComparison(
        "AC", "3S",                   // hero cards
        "3H", "8C",                   // villan cards
        "3C", "8D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3H", "8C",                   // hero cards
        "AC", "3S",                   // villan cards
        "3C", "8D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, OnePairVSThreeOfAKind) {
    expectHandComparison(
        "AC", "3S",                   // hero cards
        "3H", "3D",                   // villan cards
        "3C", "8D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3H", "3D",                   // hero cards
        "AC", "3S",                   // villan cards
        "3C", "8D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, OnePairVSStraight) {
    expectHandComparison(
        "AC", "3S",                   // hero cards
        "9H", "KD",                   // villan cards
        "3C", "8D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "9H", "KD",                   // hero cards
        "AC", "3S",                   // villan cards
        "3C", "8D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, OnePairVSFlush) {
    expectHandComparison(
        "AC", "QS",                   // hero cards
        "2H", "3H",                   // villan cards
        "3C", "8D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "2H", "3H",                   // hero cards
        "AC", "QS",                   // villan cards
        "3C", "8D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, OnePairVSFullHouse) {
    expectHandComparison(
        "AC", "KS",                   // hero cards
        "2H", "3H",                   // villan cards
        "3C", "3D", "2H", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "2H", "3H",                   // hero cards
        "AC", "KS",                   // villan cards
        "3C", "3D", "2H", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, OnePairVSFourOfAKind) {
    expectHandComparison(
        "AC", "KS",                   // hero cards
        "3S", "3H",                   // villan cards
        "3C", "3D", "2H", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3S", "3H",                   // hero cards
        "AC", "KS",                   // villan cards
        "3C", "3D", "2H", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, OnePairVSStraightFlush) {
    expectHandComparison(
        "AC", "QS",                   // hero cards
        "8H", "9H",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "8H", "9H",                   // hero cards
        "AC", "QS",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, OnePairVSRoyalFlush) {
    expectHandComparison(
        "AC", "QS",                   // hero cards
        "KH", "AH",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "KH", "AH",                   // hero cards
        "AC", "QS",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, TwoPairVThreeOfAKind) {
    expectHandComparison(
        "AC", "QS",                   // hero cards
        "2H", "3S",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "2H", "3S",                   // hero cards
        "AC", "QS",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, TwoPairVSStraight) {
    expectHandComparison(
        "AC", "QS",                   // hero cards
        "8H", "9S",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "8H", "9S",                   // hero cards
        "AC", "QS",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, TwoPairVSFlush) {
    expectHandComparison(
        "AC", "QS",                   // hero cards
        "8H", "2H",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "8H", "2H",                   // hero cards
        "AC", "QS",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, TwoPairVSFullHouse) {
    expectHandComparison(
        "AC", "QS",                   // hero cards
        "3H", "TC",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3H", "TC",                   // hero cards
        "AC", "QS",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, TwoPairVSFourOfAKind) {
    expectHandComparison(
        "AC", "QS",                   // hero cards
        "3H", "3S",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3H", "3S",                   // hero cards
        "AC", "QS",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, TwoPairVSStraightFlush) {
    expectHandComparison(
        "AC", "QS",                   // hero cards
        "8H", "9H",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "8H", "9H",                   // hero cards
        "AC", "QS",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, TwoPairVSRoyalFlush) {
    expectHandComparison(
        "AC", "QS",                   // hero cards
        "KH", "AH",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "KH", "AH",                   // hero cards
        "AC", "QS",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, ThreeOfAKindVSStraight) {
    expectHandComparison(
        "AC", "3S",                   // hero cards
        "8H", "9S",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "8H", "9S",                   // hero cards
        "AC", "3S",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, ThreeOfAKindVSFlush) {
    expectHandComparison(
        "AC", "3S",                   // hero cards
        "8H", "4H",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "8H", "4H",                   // hero cards
        "AC", "3S",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, ThreeOfAKindVSFullHouse) {
    expectHandComparison(
        "AC", "3S",                   // hero cards
        "3H", "TD",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3H", "TD",                   // hero cards
        "AC", "3S",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, ThreeOfAKindVSFourOfAKind) {
    expectHandComparison(
        "QD", "QS",                   // hero cards
        "3H", "3S",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3H", "3S",                   // hero cards
        "QD", "QS",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, ThreeOfAKindVSStraightFlush) {
    expectHandComparison(
        "AC", "3S",                   // hero cards
        "8H", "9H",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "8H", "9H",                   // hero cards
        "AC", "3S",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, ThreeOfAKindVSRoyalFlush) {
    expectHandComparison(
        "AC", "3S",                   // hero cards
        "KH", "AH",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "KH", "AH",                   // hero cards
        "AC", "3S",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, StraightVSFlush) {
    expectHandComparison(
        "KC", "AC",                   // hero cards
        "2H", "3H",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "2H", "3H",                   // hero cards
        "KC", "AC",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, StraightVSFullHouse) {
    expectHandComparison(
        "KC", "AC",                   // hero cards
        "3H", "TD",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3H", "TD",                   // hero cards
        "KC", "AC",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, StraightVSFourOfAKind) {
    expectHandComparison(
        "KC", "AC",                   // hero cards
        "3H", "3S",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3H", "3S",                   // hero cards
        "KC", "AC",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, StraightVSStraightFlush) {
    expectHandComparison(
        "KC", "AC",                   // hero cards
        "8H", "9H",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "8H", "9H",                   // hero cards
        "KC", "AC",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, StraightVSRoyalFlush) {
    expectHandComparison(
        "KC", "AC",                   // hero cards
        "KH", "AH",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "KH", "AH",                   // hero cards
        "KC", "AC",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, FlushVSFullHouse) {
    expectHandComparison(
        "6H", "7H",                   // hero cards
        "3S", "TD",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3S", "TD",                   // hero cards
        "6H", "7H",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, FlushVSFourOfAKind) {
    expectHandComparison(
        "6H", "7H",                   // hero cards
        "3S", "3H",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3S", "3H",                   // hero cards
        "6H", "7H",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, FlushVSStraightFlush) {
    expectHandComparison(
        "6H", "AH",                   // hero cards
        "8H", "9H",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "8H", "9H",                   // hero cards
        "6H", "AH",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, FlushVSRoyalFlush) {
    expectHandComparison(
        "6H", "7H",                   // hero cards
        "KH", "AH",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "KH", "AH",                   // hero cards
        "6H", "7H",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, FullHouseVSFourOfAKind) {
    expectHandComparison(
        "QC", "QS",                   // hero cards
        "3H", "3S",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3H", "3S",                   // hero cards
        "QC", "QS",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, FullHouseVSStraightFlush) {
    expectHandComparison(
        "QC", "QS",                   // hero cards
        "8H", "9H",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "8H", "9H",                   // hero cards
        "QC", "QS",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, FullHouseVSRoyalFlush) {
    expectHandComparison(
        "QC", "QS",                   // hero cards
        "KH", "AH",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "KH", "AH",                   // hero cards
        "QC", "QS",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, FourOfAKindVSStraightFlush) {
    expectHandComparison(
        "3H", "3S",                   // hero cards
        "8H", "9H",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "8H", "9H",                   // hero cards
        "3H", "3S",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, FourOfAKindVSRoyalFlush) {
    expectHandComparison(
        "3H", "3S",                   // hero cards
        "KH", "AH",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "KH", "AH",                   // hero cards
        "3H", "3S",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, StraightFlushVSRoyalFlush) {
    expectHandComparison(
        "8H", "9H",                   // hero cards
        "KH", "AH",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "KH", "AH",                   // hero cards
        "8H", "9H",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);
}

TEST_F(HandEvalTests, HighCardVSHighCard) {
    expectHandComparison(
        "4S", "5C",                   // hero cards
        "6C", "4C",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "6C", "4C",                   // hero cards
        "4S", "5C",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        WIN);

    expectHandComparison(
        "4C", "AS",                   // hero cards
        "5C", "AC",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "5C", "AC",                   // hero cards
        "4C", "AS",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        WIN);

    expectHandComparison(
        "7S", "8S",                   // hero cards
        "7D", "8D",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        TIE);
    expectHandComparison(
        "7D", "8D",                   // hero cards
        "7S", "8S",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        TIE);

    expectHandComparison(
        "3H", "5C",                   // hero cards
        "2D", "5S",                   // villan cards
        "4S", "AC", "TH", "JH", "QH", // community cards
        TIE);
    expectHandComparison(
        "2D", "5S",                   // hero cards
        "3H", "5C",                   // villan cards
        "4S", "AC", "TH", "JH", "QH", // community cards
        TIE);

    expectHandComparison(
        "6S", "7S",                   // hero cards
        "2D", "3D",                   // villan cards
        "9S", "AC", "TH", "JH", "QH", // community cards
        TIE);
    expectHandComparison(
        "2D", "3D",                   // hero cards
        "6S", "7S",                   // villan cards
        "9S", "AC", "TH", "JH", "QH", // community cards
        TIE);
}

TEST_F(HandEvalTests, OnePairVSOnePair) {
    expectHandComparison(
        "2C", "5C",                   // hero cards
        "3D", "4C",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3D", "4C",                   // hero cards
        "2C", "5C",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        WIN);

    expectHandComparison(
        "2S", "KC",                   // hero cards
        "2H", "AC",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "2H", "AC",                   // hero cards
        "2S", "KC",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        WIN);

    expectHandComparison(
        "2H", "9C",                   // hero cards
        "2S", "8C",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        TIE);
    expectHandComparison(
        "2S", "8C",                   // hero cards
        "2H", "9C",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        TIE);
}

TEST_F(HandEvalTests, TwoPairVSTwoPair) {
    expectHandComparison(
        "TD", "3S",                   // hero cards
        "QD", "JS",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "QD", "JS",                   // hero cards
        "TD", "3S",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        WIN);

    expectHandComparison(
        "JD", "TC",                   // hero cards
        "QD", "TD",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "QD", "TD",                   // hero cards
        "JD", "TC",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        WIN);

    expectHandComparison(
        "QS", "TC",                   // hero cards
        "QD", "JC",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "QD", "JC",                   // hero cards
        "QS", "TC",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        WIN);

    expectHandComparison(
        "QS", "KC",                   // hero cards
        "QD", "AC",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "QD", "AC",                   // hero cards
        "QS", "KC",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);

    expectHandComparison(
        "QS", "TC",                   // hero cards
        "QD", "JC",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "QD", "JC",                   // hero cards
        "QS", "TC",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        WIN);

    expectHandComparison(
        "QS", "4C",                   // hero cards
        "QD", "5C",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        TIE);
    expectHandComparison(
        "QD", "5C",                   // hero cards
        "QS", "4C",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        TIE);

    expectHandComparison(
        "QS", "TC",                   // hero cards
        "QD", "TD",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        TIE);
    expectHandComparison(
        "QD", "TD",                   // hero cards
        "QS", "TC",                   // villan cards
        "3C", "3D", "TH", "JH", "QH", // community cards
        TIE);
}

TEST_F(HandEvalTests, ThreeOfAKindVSThreeOfAKind) {
    expectHandComparison(
        "2H", "2C",                   // hero cards
        "3D", "3H",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3D", "3H",                   // hero cards
        "2H", "2C",                   // villan cards
        "3C", "2D", "TH", "JH", "QH", // community cards
        WIN);

    expectHandComparison(
        "2H", "2C",                   // hero cards
        "AD", "AC",                   // villan cards
        "3C", "2D", "TH", "JH", "AH", // community cards
        LOSS);
    expectHandComparison(
        "AD", "AC",                   // hero cards
        "2H", "2C",                   // villan cards
        "3C", "2D", "TH", "JH", "AH", // community cards
        WIN);

    expectHandComparison(
        "3S", "9C",                   // hero cards
        "3H", "TH",                   // villan cards
        "3C", "3D", "6H", "8H", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3H", "TH",                   // hero cards
        "3S", "9C",                   // villan cards
        "3C", "3D", "6H", "8H", "QH", // community cards
        WIN);

    expectHandComparison(
        "3S", "KH",                   // hero cards
        "3H", "AH",                   // villan cards
        "3C", "3D", "6H", "8H", "QH", // community cards
        LOSS);
    expectHandComparison(
        "3H", "AH",                   // hero cards
        "3S", "KH",                   // villan cards
        "3C", "3D", "6H", "8H", "QH", // community cards
        WIN);

    expectHandComparison(
        "3S", "TH",                   // hero cards
        "3H", "9C",                   // villan cards
        "3C", "3D", "6H", "JH", "QH", // community cards
        TIE);
    expectHandComparison(
        "3H", "9C",                   // hero cards
        "3S", "TH",                   // villan cards
        "3C", "3D", "6H", "JH", "QH", // community cards
        TIE);
}

TEST_F(HandEvalTests, StraightVSStraight) {
    expectHandComparison(
        "TS", "8S",                   // hero cards
        "TC", "KC",                   // villan cards
        "5D", "6D", "9H", "JH", "QC", // community cards
        LOSS);
    expectHandComparison(
        "TC", "KC",                   // hero cards
        "TS", "8S",                   // villan cards
        "5D", "6D", "9H", "JH", "QC", // community cards
        WIN);

    expectHandComparison(
        "AS", "8S",                   // hero cards
        "6S", "8D",                   // villan cards
        "5D", "4D", "3H", "2H", "QC", // community cards
        LOSS);
    expectHandComparison(
        "6S", "8D",                   // hero cards
        "AS", "8S",                   // villan cards
        "5D", "4D", "3H", "2H", "QC", // community cards
        WIN);

    expectHandComparison(
        "6D", "AC",                   // hero cards
        "6S", "2D",                   // villan cards
        "5D", "4D", "3H", "2H", "QC", // community cards
        TIE);
    expectHandComparison(
        "6S", "2D",                   // hero cards
        "6D", "AC",                   // villan cards
        "5D", "4D", "3H", "2H", "QC", // community cards
        TIE);
}

TEST_F(HandEvalTests, FlushVSFlush) {
    expectHandComparison(
        "9H", "QH",                   // hero cards
        "KH", "AH",                   // villan cards
        "5D", "4H", "6H", "8H", "TH", // community cards
        LOSS);
    expectHandComparison(
        "KH", "AH",                   // hero cards
        "9H", "QH",                   // villan cards
        "5D", "4H", "6H", "8H", "TH", // community cards
        WIN);

    expectHandComparison(
        "7H", "AD",                   // hero cards
        "9H", "2D",                   // villan cards
        "5D", "4H", "6H", "8H", "TH", // community cards
        LOSS);
    expectHandComparison(
        "9H", "2D",                   // hero cards
        "7H", "AD",                   // villan cards
        "5D", "4H", "6H", "8H", "TH", // community cards
        WIN);

    expectHandComparison(
        "5H", "AD",                   // hero cards
        "7H", "2D",                   // villan cards
        "5D", "4H", "6H", "8H", "TH", // community cards
        LOSS);
    expectHandComparison(
        "7H", "2D",                   // hero cards
        "5H", "AD",                   // villan cards
        "5D", "4H", "6H", "8H", "TH", // community cards
        WIN);

    expectHandComparison(
        "3H", "AD",                   // hero cards
        "5H", "2D",                   // villan cards
        "5D", "4H", "6H", "8H", "TH", // community cards
        LOSS);
    expectHandComparison(
        "5H", "2D",                   // hero cards
        "3H", "AD",                   // villan cards
        "5D", "4H", "6H", "8H", "TH", // community cards
        WIN);

    expectHandComparison(
        "7H", "2H",                   // hero cards
        "9H", "2D",                   // villan cards
        "5D", "4H", "6H", "8H", "TH", // community cards
        LOSS);
    expectHandComparison(
        "9H", "2D",                   // hero cards
        "7H", "2H",                   // villan cards
        "5D", "4H", "6H", "8H", "TH", // community cards
        WIN);

    expectHandComparison(
        "AS", "3H",                   // hero cards
        "3D", "2D",                   // villan cards
        "5H", "4H", "6H", "8H", "TH", // community cards
        TIE);
    expectHandComparison(
        "3D", "2D",                   // hero cards
        "AS", "3H",                   // villan cards
        "5H", "4H", "6H", "8H", "TH", // community cards
        TIE);
}

TEST_F(HandEvalTests, FullHouseVSFullHouse) {
    expectHandComparison(
        "8D", "8C",                   // hero cards
        "TD", "TC",                   // villan cards
        "4D", "4H", "6H", "8H", "TH", // community cards
        LOSS);
    expectHandComparison(
        "TD", "TC",                   // hero cards
        "8D", "8C",                   // villan cards
        "4D", "4H", "6H", "8H", "TH", // community cards
        WIN);

    expectHandComparison(
        "8D", "4S",                   // hero cards
        "TD", "4C",                   // villan cards
        "4D", "4H", "6H", "8H", "TH", // community cards
        LOSS);
    expectHandComparison(
        "TD", "4C",                   // hero cards
        "8D", "4S",                   // villan cards
        "4D", "4H", "6H", "8H", "TH", // community cards
        WIN);

    expectHandComparison(
        "TS", "4S",                   // hero cards
        "6D", "6C",                   // villan cards
        "4D", "4H", "6H", "8H", "TH", // community cards
        LOSS);
    expectHandComparison(
        "6D", "6C",                   // hero cards
        "TS", "4S",                   // villan cards
        "4D", "4H", "6H", "8H", "TH", // community cards
        WIN);

    expectHandComparison(
        "6S", "4S",                   // hero cards
        "6D", "4C",                   // villan cards
        "4D", "4H", "6H", "8H", "TH", // community cards
        TIE);
    expectHandComparison(
        "6D", "4C",                   // hero cards
        "6S", "4S",                   // villan cards
        "4D", "4H", "6H", "8H", "TH", // community cards
        TIE);
}

TEST_F(HandEvalTests, FourOfAKindVSFourOfAKind) {
    expectHandComparison(
        "4C", "4S",                   // hero cards
        "8D", "8C",                   // villan cards
        "4D", "4H", "8S", "8H", "TH", // community cards
        LOSS);
    expectHandComparison(
        "8D", "8C",                   // hero cards
        "4C", "4S",                   // villan cards
        "4D", "4H", "8S", "8H", "TH", // community cards
        WIN);

    expectHandComparison(
        "TD", "TS",                   // hero cards
        "QD", "2D",                   // villan cards
        "8D", "8C", "8S", "8H", "TH", // community cards
        LOSS);
    expectHandComparison(
        "QD", "2D",                   // hero cards
        "TD", "TS",                   // villan cards
        "8D", "8C", "8S", "8H", "TH", // community cards
        WIN);

    expectHandComparison(
        "9H", "7H",                   // hero cards
        "3D", "2D",                   // villan cards
        "8D", "8C", "8S", "8H", "TH", // community cards
        TIE);
    expectHandComparison(
        "3D", "2D",                   // hero cards
        "9H", "7H",                   // villan cards
        "8D", "8C", "8S", "8H", "TH", // community cards
        TIE);

    expectHandComparison(
        "9H", "JH",                   // hero cards
        "JC", "2D",                   // villan cards
        "8D", "8C", "8S", "8H", "TH", // community cards
        TIE);
    expectHandComparison(
        "JC", "2D",                   // hero cards
        "9H", "JH",                   // villan cards
        "8D", "8C", "8S", "8H", "TH", // community cards
        TIE);
}

TEST_F(HandEvalTests, StraightFlushVSStraightFlush) {
    expectHandComparison(
        "AH", "2H",                   // hero cards
        "6H", "7H",                   // villan cards
        "5H", "3H", "4H", "TC", "TH", // community cards
        LOSS);
    expectHandComparison(
        "6H", "7H",                   // hero cards
        "AH", "2H",                   // villan cards
        "5H", "3H", "4H", "TC", "TH", // community cards
        WIN);

    expectHandComparison(
        "3H", "2H",                   // hero cards
        "7H", "8H",                   // villan cards
        "5H", "6H", "4H", "TC", "TH", // community cards
        LOSS);
    expectHandComparison(
        "7H", "8H",                   // hero cards
        "3H", "2H",                   // villan cards
        "5H", "6H", "4H", "TC", "TH", // community cards
        WIN);

    expectHandComparison(
        "6H", "AH",                   // hero cards
        "JH", "2D",                   // villan cards
        "9H", "8H", "7H", "TC", "TH", // community cards
        LOSS);
    expectHandComparison(
        "JH", "2D",                   // hero cards
        "6H", "AH",                   // villan cards
        "9H", "8H", "7H", "TC", "TH", // community cards
        WIN);

    expectHandComparison(
        "AS", "AH",                   // hero cards
        "TS", "TC",                   // villan cards
        "9H", "8H", "7H", "JH", "TH", // community cards
        TIE);
    expectHandComparison(
        "TS", "TC",                   // hero cards
        "AS", "AH",                   // villan cards
        "9H", "8H", "7H", "JH", "TH", // community cards
        TIE);
}

