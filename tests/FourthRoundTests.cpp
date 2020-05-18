#include <gtest/gtest.h>

#include <algorithm>
#include <string>
#include <unordered_map>

#include "../src/4th_round/fourthround.h"

enum HandResult { LOSS, TIE, WIN };

class FourthRoundGeneratorTests : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  FourthRoundGeneratorTests() : handType{new HandType} {
    // You can do set-up work for each test here.
  }

  virtual ~FourthRoundGeneratorTests() {
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

  float firstRoundMean(std::string p0Str, std::string p1Str) {
    int p0Target = this->deckMap[p0Str];
    int p1Target = this->deckMap[p1Str];
    int p0, p1, c0, c1, c2, c3, c4;
    p0 = p0Target;
    p1 = p1Target;
    gen.c_inDeck[p0] = false;
    gen.c_inDeck[p1] = false;
    double mean = 0;
    double meanSum = 0;
    int numMeans = 0;
    for (c0 = 0; c0 < 48; c0++)
      if (gen.c_inDeck[c0])
        for (c1 = c0 + 1; c1 < 49; c1++)
          if (gen.c_inDeck[c1])
            for (c2 = c1 + 1; c2 < 50; c2++)
              if (gen.c_inDeck[c2])
                for (c3 = c2 + 1; c3 < 51; c3++)
                  if (gen.c_inDeck[c3])
                    for (c4 = c3 + 1; c4 < 52; c4++) {
                      if (gen.c_inDeck[c4]) {
                        gen.c_inDeck[c0] = false;
                        gen.c_inDeck[c1] = false;
                        gen.c_inDeck[c2] = false;
                        gen.c_inDeck[c3] = false;
                        gen.c_inDeck[c4] = false;

                        gen.pCards[0] = p0;
                        gen.pCards[1] = p1;
                        gen.pCards[2] = c0;
                        gen.pCards[3] = c1;
                        gen.pCards[4] = c2;
                        gen.pCards[5] = c3;
                        gen.pCards[6] = c4;
                        std::sort(gen.pCards.begin(), gen.pCards.end());
                        gen.eCards[0] = 0;
                        gen.eCards[1] = 0;
                        gen.eCards[2] = c0;
                        gen.eCards[3] = c1;
                        gen.eCards[4] = c2;
                        gen.eCards[5] = c3;
                        gen.eCards[6] = c4;
                        mean = gen.getMean();
                        meanSum += mean;
                        numMeans += 1;
                        gen.c_inDeck[c0] = true;
                        gen.c_inDeck[c1] = true;
                        gen.c_inDeck[c2] = true;
                        gen.c_inDeck[c3] = true;
                        gen.c_inDeck[c4] = true;
                      }
                    }
    gen.c_inDeck[c0] = true;
    gen.c_inDeck[c1] = true;
    gen.c_inDeck[c2] = true;
    gen.c_inDeck[p0] = true;
    gen.c_inDeck[p1] = true;
    return meanSum / double(numMeans);
  }

  float secondRoundMean(std::string p0Str, std::string p1Str, std::string c0Str,
                        std::string c1Str, std::string c2Str) {
    int p0Target = this->deckMap[p0Str];
    int p1Target = this->deckMap[p1Str];
    int c0Target = this->deckMap[c0Str];
    int c1Target = this->deckMap[c1Str];
    int c2Target = this->deckMap[c2Str];
    int p0, p1, c0, c1, c2, c3, c4;
    c0 = c0Target;
    c1 = c1Target;
    c2 = c2Target;
    p0 = p0Target;
    p1 = p1Target;
    gen.c_inDeck[c0] = false;
    gen.c_inDeck[c1] = false;
    gen.c_inDeck[c2] = false;
    gen.c_inDeck[p0] = false;
    gen.c_inDeck[p1] = false;
    float mean = 0;
    float meanSum = 0;
    int numMeans = 0;
    for (c3 = 0; c3 < 51; c3++)
      if (gen.c_inDeck[c3])
        for (c4 = c3 + 1; c4 < 52; c4++) {
          if (gen.c_inDeck[c4]) {
            gen.c_inDeck[c3] = false;
            gen.c_inDeck[c4] = false;

            gen.pCards[0] = p0;
            gen.pCards[1] = p1;
            gen.pCards[2] = c0;
            gen.pCards[3] = c1;
            gen.pCards[4] = c2;
            gen.pCards[5] = c3;
            gen.pCards[6] = c4;
            std::sort(gen.pCards.begin(), gen.pCards.end());
            gen.eCards[0] = 0;
            gen.eCards[1] = 0;
            gen.eCards[2] = c0;
            gen.eCards[3] = c1;
            gen.eCards[4] = c2;
            gen.eCards[5] = c3;
            gen.eCards[6] = c4;
            mean = gen.getMean();
            meanSum += mean;
            numMeans += 1;
            gen.c_inDeck[c3] = true;
            gen.c_inDeck[c4] = true;
          }
        }
    gen.c_inDeck[c0] = true;
    gen.c_inDeck[c1] = true;
    gen.c_inDeck[c2] = true;
    gen.c_inDeck[p0] = true;
    gen.c_inDeck[p1] = true;
    return meanSum / float(numMeans);
  }

  float getMeanTest(std::string p0Str, std::string p1Str, std::string c0Str,
                    std::string c1Str, std::string c2Str, std::string c3Str,
                    std::string c4Str) {
    int p0 = this->deckMap[p0Str];
    int p1 = this->deckMap[p1Str];
    int c0 = this->deckMap[c0Str];
    int c1 = this->deckMap[c1Str];
    int c2 = this->deckMap[c2Str];
    int c3 = this->deckMap[c3Str];
    int c4 = this->deckMap[c4Str];
    gen.c_inDeck[p0] = false;
    gen.c_inDeck[p1] = false;
    gen.c_inDeck[c0] = false;
    gen.c_inDeck[c1] = false;
    gen.c_inDeck[c2] = false;
    gen.c_inDeck[c3] = false;
    gen.c_inDeck[c4] = false;
    gen.pCards[0] = p0;
    gen.pCards[1] = p1;
    gen.pCards[2] = c0;
    gen.pCards[3] = c1;
    gen.pCards[4] = c2;
    gen.pCards[5] = c3;
    gen.pCards[6] = c4;
    std::sort(gen.pCards.begin(), gen.pCards.end());
    gen.eCards[0] = 45;
    gen.eCards[1] = 51;
    gen.eCards[2] = c0;
    gen.eCards[3] = c1;
    gen.eCards[4] = c2;
    gen.eCards[5] = c3;
    gen.eCards[6] = c4;
    float mean = gen.getMean();
    gen.c_inDeck[p0] = true;
    gen.c_inDeck[p1] = true;
    gen.c_inDeck[c0] = true;
    gen.c_inDeck[c1] = true;
    gen.c_inDeck[c2] = true;
    gen.c_inDeck[c3] = true;
    gen.c_inDeck[c4] = true;
    return mean;
  }

  float getBucketTest(std::string p0Str, std::string p1Str, std::string c0Str,
                      std::string c1Str, std::string c2Str, std::string c3Str,
                      std::string c4Str) {
    int p0 = this->deckMap[p0Str];
    int p1 = this->deckMap[p1Str];
    int c0 = this->deckMap[c0Str];
    int c1 = this->deckMap[c1Str];
    int c2 = this->deckMap[c2Str];
    int c3 = this->deckMap[c3Str];
    int c4 = this->deckMap[c4Str];
    gen.c_inDeck[p0] = false;
    gen.c_inDeck[p1] = false;
    gen.c_inDeck[c0] = false;
    gen.c_inDeck[c1] = false;
    gen.c_inDeck[c2] = false;
    gen.c_inDeck[c3] = false;
    gen.c_inDeck[c4] = false;
    gen.pCards[0] = p0;
    gen.pCards[1] = p1;
    gen.pCards[2] = c0;
    gen.pCards[3] = c1;
    gen.pCards[4] = c2;
    gen.pCards[5] = c3;
    gen.pCards[6] = c4;
    std::sort(gen.pCards.begin(), gen.pCards.end());
    gen.eCards[0] = 45;
    gen.eCards[1] = 51;
    gen.eCards[2] = c0;
    gen.eCards[3] = c1;
    gen.eCards[4] = c2;
    gen.eCards[5] = c3;
    gen.eCards[6] = c4;
    gen.computeBuckets();
    std::vector<size_t> bucketSize(8, 0);
    size_t hash = 0;
    for (size_t i7 = 0; i7 < 51; i7++)
      for (size_t i8 = i7 + 1; i8 < 52; i8++) {
        if (gen.c_inDeck[i8] && gen.c_inDeck[i7]) {
          bucketSize[FourthRoundGenerator::clusterMap[hash]] += 2;
        }
        hash += 1;
      }
    float mean = 0;
    for (size_t i = 0; i < gen.buckets.size(); i++)
      mean += gen.buckets[i] * bucketSize[i];
    mean /= 1980; // 47 choose 2
    gen.c_inDeck[p0] = true;
    gen.c_inDeck[p1] = true;
    gen.c_inDeck[c0] = true;
    gen.c_inDeck[c1] = true;
    gen.c_inDeck[c2] = true;
    gen.c_inDeck[c3] = true;
    gen.c_inDeck[c4] = true;
    return mean;
  }

  void expectHandType(std::string s0, std::string s1, std::string s2,
                      std::string s3, std::string s4, std::string s5,
                      std::string s6, HandIndex handIndex, int c0, int c1,
                      int c2, int c3, int c4,
                      std::unique_ptr<HandType> &handType) {
    std::array<int, 7> cards = {this->deckMap[s0], this->deckMap[s1],
                                this->deckMap[s2], this->deckMap[s3],
                                this->deckMap[s4], this->deckMap[s5],
                                this->deckMap[s6]};
    std::sort(cards.begin(), cards.end());
    gen.findHandType(handType, cards);
    EXPECT_EQ(handIndex, handType->handIndex);
    EXPECT_EQ(c0, handType->keyCardIndex[0]);
    EXPECT_EQ(c1, handType->keyCardIndex[1]);
    EXPECT_EQ(c2, handType->keyCardIndex[2]);
    EXPECT_EQ(c3, handType->keyCardIndex[3]);
    EXPECT_EQ(c4, handType->keyCardIndex[4]);
  }

  void expectHandComparison(std::string p0, std::string p1, std::string e0,
                            std::string e1, std::string c0, std::string c1,
                            std::string c2, std::string c3, std::string c4,
                            int expected) {
    gen.pCards[0] = this->deckMap[p0];
    gen.pCards[1] = this->deckMap[p1];
    gen.pCards[2] = this->deckMap[c0];
    gen.pCards[3] = this->deckMap[c1];
    gen.pCards[4] = this->deckMap[c2];
    gen.pCards[5] = this->deckMap[c3];
    gen.pCards[6] = this->deckMap[c4];
    gen.eCards[0] = this->deckMap[e0];
    gen.eCards[1] = this->deckMap[e1];
    gen.eCards[2] = this->deckMap[c0];
    gen.eCards[3] = this->deckMap[c1];
    gen.eCards[4] = this->deckMap[c2];
    gen.eCards[5] = this->deckMap[c3];
    gen.eCards[6] = this->deckMap[c4];
    std::sort(gen.pCards.begin(), gen.pCards.end());
    std::sort(gen.eCards.begin(), gen.eCards.end());
    int result = gen.getValue();
    EXPECT_EQ(expected, result)
        << "gen.pCards:(" << p0 << "," << p1 << ") gen.eCards:(" << e0 << ","
        << e1 << ") cCards:(" << c0 << "," << c1 << "," << c2 << "," << c3
        << "," << c4 << ")\n";
  }

  // Objects declared here can be used by all tests in the test case for
  // Project1.
  FourthRoundGenerator gen;
  const float maxEquityDiff = 0.01;
  std::unique_ptr<HandType> handType;
  std::unordered_map<std::string, int> deckMap = {
      {"2H", 0},  {"2D", 1},  {"2C", 2},  {"2S", 3},  {"3H", 4},  {"3D", 5},
      {"3C", 6},  {"3S", 7},  {"4H", 8},  {"4D", 9},  {"4C", 10}, {"4S", 11},
      {"5H", 12}, {"5D", 13}, {"5C", 14}, {"5S", 15}, {"6H", 16}, {"6D", 17},
      {"6C", 18}, {"6S", 19}, {"7H", 20}, {"7D", 21}, {"7C", 22}, {"7S", 23},
      {"8H", 24}, {"8D", 25}, {"8C", 26}, {"8S", 27}, {"9H", 28}, {"9D", 29},
      {"9C", 30}, {"9S", 31}, {"TH", 32}, {"TD", 33}, {"TC", 34}, {"TS", 35},
      {"JH", 36}, {"JD", 37}, {"JC", 38}, {"JS", 39}, {"QH", 40}, {"QD", 41},
      {"QC", 42}, {"QS", 43}, {"KH", 44}, {"KD", 45}, {"KC", 46}, {"KS", 47},
      {"AH", 48}, {"AD", 49}, {"AC", 50}, {"AS", 51}};
  std::unordered_map<int, std::string> deckRevMap = {
      {0, "2H"},  {1, "2D"},  {2, "2C"},  {3, "2S"},  {4, "3H"},  {5, "3D"},
      {6, "3C"},  {7, "3S"},  {8, "4H"},  {9, "4D"},  {10, "4C"}, {11, "4S"},
      {12, "5H"}, {13, "5D"}, {14, "5C"}, {15, "5S"}, {16, "6H"}, {17, "6D"},
      {18, "6C"}, {19, "6S"}, {20, "7H"}, {21, "7D"}, {22, "7C"}, {23, "7S"},
      {24, "8H"}, {25, "8D"}, {26, "8C"}, {27, "8S"}, {28, "9H"}, {29, "9D"},
      {30, "9C"}, {31, "9S"}, {32, "TH"}, {33, "TD"}, {34, "TC"}, {35, "TS"},
      {36, "JH"}, {37, "JD"}, {38, "JC"}, {39, "JS"}, {40, "QH"}, {41, "QD"},
      {42, "QC"}, {43, "QS"}, {44, "KH"}, {45, "KD"}, {46, "KC"}, {47, "KS"},
      {48, "AH"}, {49, "AD"}, {50, "AC"}, {51, "AS"}};
};

TEST_F(FourthRoundGeneratorTests, HighCard) {
  expectHandType("2H", "4D", "5C", "6H", "7H", "9H", "TD", HIGH_CARD, 6, 5, 4,
                 3, 2, handType);
}

TEST_F(FourthRoundGeneratorTests, OnePair) {
  expectHandType("2H", "2D", "5C", "6H", "7H", "9H", "TD", ONE_PAIR, 1, 0, 6, 5,
                 4, handType);
}

TEST_F(FourthRoundGeneratorTests, TwoPair) {
  expectHandType("2H", "2D", "5C", "6H", "7H", "7D", "TD", TWO_PAIR, 5, 4, 1, 0,
                 6, handType);
}

TEST_F(FourthRoundGeneratorTests, ThreeOfAKind) {
  expectHandType("2H", "2D", "2C", "6H", "7H", "9D", "TD", THREE_OF_A_KIND, 2,
                 1, 0, 6, 5, handType);
}

TEST_F(FourthRoundGeneratorTests, Straight) {
  expectHandType("2H", "3D", "4C", "5H", "6H", "9D", "TD", STRAIGHT, 4, 3, 2, 1,
                 0, handType);
  expectHandType("2H", "3D", "4C", "5H", "8H", "9D", "AD", STRAIGHT, 3, 2, 1, 0,
                 6, handType);
}

TEST_F(FourthRoundGeneratorTests, Flush) {
  expectHandType("2H", "3H", "4H", "5H", "7H", "9D", "TD", FLUSH, 4, 3, 2, 1, 0,
                 handType);
}

TEST_F(FourthRoundGeneratorTests, FullHouse) {
  expectHandType("2H", "2D", "2C", "5H", "9H", "9D", "TD", FULL_HOUSE, 2, 1, 0,
                 5, 4, handType);
}

TEST_F(FourthRoundGeneratorTests, FourOfAKind) {
  expectHandType("2H", "2D", "2C", "2S", "9H", "9D", "TD", FOUR_OF_A_KIND, 3, 2,
                 1, 0, 6, handType);
}

TEST_F(FourthRoundGeneratorTests, StraightFlush) {
  expectHandType("2H", "3H", "4H", "5H", "6H", "9D", "TD", STRAIGHT_FLUSH, 4, 3,
                 2, 1, 0, handType);
  expectHandType("2H", "3H", "4H", "5H", "8C", "9D", "AH", STRAIGHT_FLUSH, 3, 2,
                 1, 0, 6, handType);
}

TEST_F(FourthRoundGeneratorTests, RoyalFlush) {
  expectHandType("9C", "9H", "TD", "JD", "QD", "KD", "AD", ROYAL_FLUSH, 6, 5, 4,
                 3, 2, handType);
}

TEST_F(FourthRoundGeneratorTests, HighCardVSOnePair) {
  expectHandComparison("AC", "2H",                    // hero cards
                       "3D", "5S",                    // villan cards
                       "3C", "8H", "TD", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("3D", "5S",                    // hero cards
                       "AC", "2H",                    // villan cards
                       "3C", "8H", "TD", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, HighCardVSTwoPair) {
  expectHandComparison("AC", "2H",                    // hero cards
                       "3D", "TS",                    // villan cards
                       "3C", "8H", "TD", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("3D", "TS",                    // hero cards
                       "AC", "2H",                    // villan cards
                       "3C", "8H", "TD", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, HighCardVSThreeOfAKind) {
  expectHandComparison("AC", "2H",                    // hero cards
                       "3D", "3H",                    // villan cards
                       "3C", "8H", "TD", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("3D", "3H",                    // hero cards
                       "AC", "2H",                    // villan cards
                       "3C", "8H", "TD", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, HighCardVSStraight) {
  expectHandComparison("AC", "2H",                    // hero cards
                       "9D", "KH",                    // villan cards
                       "3C", "8H", "TD", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("9D", "KH",                    // hero cards
                       "AC", "2H",                    // villan cards
                       "3C", "8H", "TD", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, HighCardVSFlush) {
  expectHandComparison("AC", "2H",                    // hero cards
                       "4H", "3H",                    // villan cards
                       "3C", "8H", "TD", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("4H", "3H",                    // hero cards
                       "AC", "2H",                    // villan cards
                       "3C", "8H", "TD", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, HighCardVSStraightFlush) {
  expectHandComparison("AC", "2H",                    // hero cards
                       "9H", "KH",                    // villan cards
                       "3C", "8D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("9H", "KH",                    // hero cards
                       "AC", "2H",                    // villan cards
                       "3C", "8D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, HighCardVSRoyalFlush) {
  expectHandComparison("AC", "2H",                    // hero cards
                       "KH", "AH",                    // villan cards
                       "3C", "8D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("KH", "AH",                    // hero cards
                       "AC", "2H",                    // villan cards
                       "3C", "8D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, OnePairVSTwoPair) {
  expectHandComparison("AC", "3S",                    // hero cards
                       "3H", "8C",                    // villan cards
                       "3C", "8D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("3H", "8C",                    // hero cards
                       "AC", "3S",                    // villan cards
                       "3C", "8D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, OnePairVSThreeOfAKind) {
  expectHandComparison("AC", "3S",                    // hero cards
                       "3H", "3D",                    // villan cards
                       "3C", "8D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("3H", "3D",                    // hero cards
                       "AC", "3S",                    // villan cards
                       "3C", "8D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, OnePairVSStraight) {
  expectHandComparison("AC", "3S",                    // hero cards
                       "9H", "KD",                    // villan cards
                       "3C", "8D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("9H", "KD",                    // hero cards
                       "AC", "3S",                    // villan cards
                       "3C", "8D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, OnePairVSFlush) {
  expectHandComparison("AC", "QS",                    // hero cards
                       "2H", "3H",                    // villan cards
                       "3C", "8D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("2H", "3H",                    // hero cards
                       "AC", "QS",                    // villan cards
                       "3C", "8D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, OnePairVSFullHouse) {
  expectHandComparison("AC", "KS",                    // hero cards
                       "2H", "3H",                    // villan cards
                       "3C", "3D", "2H", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("2H", "3H",                    // hero cards
                       "AC", "KS",                    // villan cards
                       "3C", "3D", "2H", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, OnePairVSFourOfAKind) {
  expectHandComparison("AC", "KS",                    // hero cards
                       "3S", "3H",                    // villan cards
                       "3C", "3D", "2H", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("3S", "3H",                    // hero cards
                       "AC", "KS",                    // villan cards
                       "3C", "3D", "2H", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, OnePairVSStraightFlush) {
  expectHandComparison("AC", "QS",                    // hero cards
                       "8H", "9H",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("8H", "9H",                    // hero cards
                       "AC", "QS",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, OnePairVSRoyalFlush) {
  expectHandComparison("AC", "QS",                    // hero cards
                       "KH", "AH",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("KH", "AH",                    // hero cards
                       "AC", "QS",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, TwoPairVThreeOfAKind) {
  expectHandComparison("AC", "QS",                    // hero cards
                       "2H", "3S",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("2H", "3S",                    // hero cards
                       "AC", "QS",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, TwoPairVSStraight) {
  expectHandComparison("AC", "QS",                    // hero cards
                       "8H", "9S",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("8H", "9S",                    // hero cards
                       "AC", "QS",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, TwoPairVSFlush) {
  expectHandComparison("AC", "QS",                    // hero cards
                       "8H", "2H",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("8H", "2H",                    // hero cards
                       "AC", "QS",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, TwoPairVSFullHouse) {
  expectHandComparison("AC", "QS",                    // hero cards
                       "3H", "TC",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("3H", "TC",                    // hero cards
                       "AC", "QS",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, TwoPairVSFourOfAKind) {
  expectHandComparison("AC", "QS",                    // hero cards
                       "3H", "3S",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("3H", "3S",                    // hero cards
                       "AC", "QS",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, TwoPairVSStraightFlush) {
  expectHandComparison("AC", "QS",                    // hero cards
                       "8H", "9H",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("8H", "9H",                    // hero cards
                       "AC", "QS",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, TwoPairVSRoyalFlush) {
  expectHandComparison("AC", "QS",                    // hero cards
                       "KH", "AH",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("KH", "AH",                    // hero cards
                       "AC", "QS",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, ThreeOfAKindVSStraight) {
  expectHandComparison("AC", "3S",                    // hero cards
                       "8H", "9S",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("8H", "9S",                    // hero cards
                       "AC", "3S",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, ThreeOfAKindVSFlush) {
  expectHandComparison("AC", "3S",                    // hero cards
                       "8H", "4H",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("8H", "4H",                    // hero cards
                       "AC", "3S",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, ThreeOfAKindVSFullHouse) {
  expectHandComparison("AC", "3S",                    // hero cards
                       "3H", "TD",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("3H", "TD",                    // hero cards
                       "AC", "3S",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, ThreeOfAKindVSFourOfAKind) {
  expectHandComparison("QD", "QS",                    // hero cards
                       "3H", "3S",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("3H", "3S",                    // hero cards
                       "QD", "QS",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, ThreeOfAKindVSStraightFlush) {
  expectHandComparison("AC", "3S",                    // hero cards
                       "8H", "9H",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("8H", "9H",                    // hero cards
                       "AC", "3S",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, ThreeOfAKindVSRoyalFlush) {
  expectHandComparison("AC", "3S",                    // hero cards
                       "KH", "AH",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("KH", "AH",                    // hero cards
                       "AC", "3S",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, StraightVSFlush) {
  expectHandComparison("KC", "AC",                    // hero cards
                       "2H", "3H",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("2H", "3H",                    // hero cards
                       "KC", "AC",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, StraightVSFullHouse) {
  expectHandComparison("KC", "AC",                    // hero cards
                       "3H", "TD",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("3H", "TD",                    // hero cards
                       "KC", "AC",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, StraightVSFourOfAKind) {
  expectHandComparison("KC", "AC",                    // hero cards
                       "3H", "3S",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("3H", "3S",                    // hero cards
                       "KC", "AC",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, StraightVSStraightFlush) {
  expectHandComparison("KC", "AC",                    // hero cards
                       "8H", "9H",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("8H", "9H",                    // hero cards
                       "KC", "AC",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, StraightVSRoyalFlush) {
  expectHandComparison("KC", "AC",                    // hero cards
                       "KH", "AH",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("KH", "AH",                    // hero cards
                       "KC", "AC",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, FlushVSFullHouse) {
  expectHandComparison("6H", "7H",                    // hero cards
                       "3S", "TD",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("3S", "TD",                    // hero cards
                       "6H", "7H",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, FlushVSFourOfAKind) {
  expectHandComparison("6H", "7H",                    // hero cards
                       "3S", "3H",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("3S", "3H",                    // hero cards
                       "6H", "7H",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, FlushVSStraightFlush) {
  expectHandComparison("6H", "AH",                    // hero cards
                       "8H", "9H",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("8H", "9H",                    // hero cards
                       "6H", "AH",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, FlushVSRoyalFlush) {
  expectHandComparison("6H", "7H",                    // hero cards
                       "KH", "AH",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("KH", "AH",                    // hero cards
                       "6H", "7H",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, FullHouseVSFourOfAKind) {
  expectHandComparison("QC", "QS",                    // hero cards
                       "3H", "3S",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("3H", "3S",                    // hero cards
                       "QC", "QS",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, FullHouseVSStraightFlush) {
  expectHandComparison("QC", "QS",                    // hero cards
                       "8H", "9H",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("8H", "9H",                    // hero cards
                       "QC", "QS",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, FullHouseVSRoyalFlush) {
  expectHandComparison("QC", "QS",                    // hero cards
                       "KH", "AH",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("KH", "AH",                    // hero cards
                       "QC", "QS",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, FourOfAKindVSStraightFlush) {
  expectHandComparison("3H", "3S",                    // hero cards
                       "8H", "9H",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("8H", "9H",                    // hero cards
                       "3H", "3S",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, FourOfAKindVSRoyalFlush) {
  expectHandComparison("3H", "3S",                    // hero cards
                       "KH", "AH",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("KH", "AH",                    // hero cards
                       "3H", "3S",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, StraightFlushVSRoyalFlush) {
  expectHandComparison("8H", "9H",                    // hero cards
                       "KH", "AH",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("KH", "AH",                    // hero cards
                       "8H", "9H",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);
}

TEST_F(FourthRoundGeneratorTests, HighCardVSHighCard) {
  expectHandComparison("4S", "5C",                    // hero cards
                       "6C", "4C",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("6C", "4C",                    // hero cards
                       "4S", "5C",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       WIN);

  expectHandComparison("4C", "AS",                    // hero cards
                       "5C", "AC",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("5C", "AC",                    // hero cards
                       "4C", "AS",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       WIN);

  expectHandComparison("7S", "8S",                    // hero cards
                       "7D", "8D",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       TIE);
  expectHandComparison("7D", "8D",                    // hero cards
                       "7S", "8S",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       TIE);

  expectHandComparison("3H", "5C",                    // hero cards
                       "2D", "5S",                    // villan cards
                       "4S", "AC", "TH", "JH", "QH",  // community cards
                       TIE);
  expectHandComparison("2D", "5S",                    // hero cards
                       "3H", "5C",                    // villan cards
                       "4S", "AC", "TH", "JH", "QH",  // community cards
                       TIE);

  expectHandComparison("6S", "7S",                    // hero cards
                       "2D", "3D",                    // villan cards
                       "9S", "AC", "TH", "JH", "QH",  // community cards
                       TIE);
  expectHandComparison("2D", "3D",                    // hero cards
                       "6S", "7S",                    // villan cards
                       "9S", "AC", "TH", "JH", "QH",  // community cards
                       TIE);
}

TEST_F(FourthRoundGeneratorTests, OnePairVSOnePair) {
  expectHandComparison("2C", "5C",                    // hero cards
                       "3D", "4C",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("3D", "4C",                    // hero cards
                       "2C", "5C",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       WIN);

  expectHandComparison("2S", "KC",                    // hero cards
                       "2H", "AC",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("2H", "AC",                    // hero cards
                       "2S", "KC",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       WIN);

  expectHandComparison("2H", "9C",                    // hero cards
                       "2S", "8C",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       TIE);
  expectHandComparison("2S", "8C",                    // hero cards
                       "2H", "9C",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       TIE);
}

TEST_F(FourthRoundGeneratorTests, TwoPairVSTwoPair) {
  expectHandComparison("TD", "3S",                    // hero cards
                       "QD", "JS",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("QD", "JS",                    // hero cards
                       "TD", "3S",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       WIN);

  expectHandComparison("JD", "TC",                    // hero cards
                       "QD", "TD",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("QD", "TD",                    // hero cards
                       "JD", "TC",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       WIN);

  expectHandComparison("QS", "TC",                    // hero cards
                       "QD", "JC",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("QD", "JC",                    // hero cards
                       "QS", "TC",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       WIN);

  expectHandComparison("QS", "KC",                    // hero cards
                       "QD", "AC",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("QD", "AC",                    // hero cards
                       "QS", "KC",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);

  expectHandComparison("QS", "TC",                    // hero cards
                       "QD", "JC",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("QD", "JC",                    // hero cards
                       "QS", "TC",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       WIN);

  expectHandComparison("QS", "4C",                    // hero cards
                       "QD", "5C",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       TIE);
  expectHandComparison("QD", "5C",                    // hero cards
                       "QS", "4C",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       TIE);

  expectHandComparison("QS", "TC",                    // hero cards
                       "QD", "TD",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       TIE);
  expectHandComparison("QD", "TD",                    // hero cards
                       "QS", "TC",                    // villan cards
                       "3C", "3D", "TH", "JH", "QH",  // community cards
                       TIE);
}

TEST_F(FourthRoundGeneratorTests, ThreeOfAKindVSThreeOfAKind) {
  expectHandComparison("2H", "2C",                    // hero cards
                       "3D", "3H",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       LOSS);
  expectHandComparison("3D", "3H",                    // hero cards
                       "2H", "2C",                    // villan cards
                       "3C", "2D", "TH", "JH", "QH",  // community cards
                       WIN);

  expectHandComparison("2H", "2C",                    // hero cards
                       "AD", "AC",                    // villan cards
                       "3C", "2D", "TH", "JH", "AH",  // community cards
                       LOSS);
  expectHandComparison("AD", "AC",                    // hero cards
                       "2H", "2C",                    // villan cards
                       "3C", "2D", "TH", "JH", "AH",  // community cards
                       WIN);

  expectHandComparison("3S", "9C",                    // hero cards
                       "3H", "TH",                    // villan cards
                       "3C", "3D", "6H", "8H", "QH",  // community cards
                       LOSS);
  expectHandComparison("3H", "TH",                    // hero cards
                       "3S", "9C",                    // villan cards
                       "3C", "3D", "6H", "8H", "QH",  // community cards
                       WIN);

  expectHandComparison("3S", "KH",                    // hero cards
                       "3H", "AH",                    // villan cards
                       "3C", "3D", "6H", "8H", "QH",  // community cards
                       LOSS);
  expectHandComparison("3H", "AH",                    // hero cards
                       "3S", "KH",                    // villan cards
                       "3C", "3D", "6H", "8H", "QH",  // community cards
                       WIN);

  expectHandComparison("3S", "TH",                    // hero cards
                       "3H", "9C",                    // villan cards
                       "3C", "3D", "6H", "JH", "QH",  // community cards
                       TIE);
  expectHandComparison("3H", "9C",                    // hero cards
                       "3S", "TH",                    // villan cards
                       "3C", "3D", "6H", "JH", "QH",  // community cards
                       TIE);
}

TEST_F(FourthRoundGeneratorTests, StraightVSStraight) {
  expectHandComparison("TS", "8S",                    // hero cards
                       "TC", "KC",                    // villan cards
                       "5D", "6D", "9H", "JH", "QC",  // community cards
                       LOSS);
  expectHandComparison("TC", "KC",                    // hero cards
                       "TS", "8S",                    // villan cards
                       "5D", "6D", "9H", "JH", "QC",  // community cards
                       WIN);

  expectHandComparison("AS", "8S",                    // hero cards
                       "6S", "8D",                    // villan cards
                       "5D", "4D", "3H", "2H", "QC",  // community cards
                       LOSS);
  expectHandComparison("6S", "8D",                    // hero cards
                       "AS", "8S",                    // villan cards
                       "5D", "4D", "3H", "2H", "QC",  // community cards
                       WIN);

  expectHandComparison("6D", "AC",                    // hero cards
                       "6S", "2D",                    // villan cards
                       "5D", "4D", "3H", "2H", "QC",  // community cards
                       TIE);
  expectHandComparison("6S", "2D",                    // hero cards
                       "6D", "AC",                    // villan cards
                       "5D", "4D", "3H", "2H", "QC",  // community cards
                       TIE);
}

TEST_F(FourthRoundGeneratorTests, FlushVSFlush) {
  expectHandComparison("9H", "QH",                    // hero cards
                       "KH", "AH",                    // villan cards
                       "5D", "4H", "6H", "8H", "TH",  // community cards
                       LOSS);
  expectHandComparison("KH", "AH",                    // hero cards
                       "9H", "QH",                    // villan cards
                       "5D", "4H", "6H", "8H", "TH",  // community cards
                       WIN);

  expectHandComparison("7H", "AD",                    // hero cards
                       "9H", "2D",                    // villan cards
                       "5D", "4H", "6H", "8H", "TH",  // community cards
                       LOSS);
  expectHandComparison("9H", "2D",                    // hero cards
                       "7H", "AD",                    // villan cards
                       "5D", "4H", "6H", "8H", "TH",  // community cards
                       WIN);

  expectHandComparison("5H", "AD",                    // hero cards
                       "7H", "2D",                    // villan cards
                       "5D", "4H", "6H", "8H", "TH",  // community cards
                       LOSS);
  expectHandComparison("7H", "2D",                    // hero cards
                       "5H", "AD",                    // villan cards
                       "5D", "4H", "6H", "8H", "TH",  // community cards
                       WIN);

  expectHandComparison("3H", "AD",                    // hero cards
                       "5H", "2D",                    // villan cards
                       "5D", "4H", "6H", "8H", "TH",  // community cards
                       LOSS);
  expectHandComparison("5H", "2D",                    // hero cards
                       "3H", "AD",                    // villan cards
                       "5D", "4H", "6H", "8H", "TH",  // community cards
                       WIN);

  expectHandComparison("7H", "2H",                    // hero cards
                       "9H", "2D",                    // villan cards
                       "5D", "4H", "6H", "8H", "TH",  // community cards
                       LOSS);
  expectHandComparison("9H", "2D",                    // hero cards
                       "7H", "2H",                    // villan cards
                       "5D", "4H", "6H", "8H", "TH",  // community cards
                       WIN);

  expectHandComparison("AS", "3H",                    // hero cards
                       "3D", "2D",                    // villan cards
                       "5H", "4H", "6H", "8H", "TH",  // community cards
                       TIE);
  expectHandComparison("3D", "2D",                    // hero cards
                       "AS", "3H",                    // villan cards
                       "5H", "4H", "6H", "8H", "TH",  // community cards
                       TIE);
}

TEST_F(FourthRoundGeneratorTests, FullHouseVSFullHouse) {
  expectHandComparison("8D", "8C",                    // hero cards
                       "TD", "TC",                    // villan cards
                       "4D", "4H", "6H", "8H", "TH",  // community cards
                       LOSS);
  expectHandComparison("TD", "TC",                    // hero cards
                       "8D", "8C",                    // villan cards
                       "4D", "4H", "6H", "8H", "TH",  // community cards
                       WIN);

  expectHandComparison("8D", "4S",                    // hero cards
                       "TD", "4C",                    // villan cards
                       "4D", "4H", "6H", "8H", "TH",  // community cards
                       LOSS);
  expectHandComparison("TD", "4C",                    // hero cards
                       "8D", "4S",                    // villan cards
                       "4D", "4H", "6H", "8H", "TH",  // community cards
                       WIN);

  expectHandComparison("TS", "4S",                    // hero cards
                       "6D", "6C",                    // villan cards
                       "4D", "4H", "6H", "8H", "TH",  // community cards
                       LOSS);
  expectHandComparison("6D", "6C",                    // hero cards
                       "TS", "4S",                    // villan cards
                       "4D", "4H", "6H", "8H", "TH",  // community cards
                       WIN);

  expectHandComparison("6S", "4S",                    // hero cards
                       "6D", "4C",                    // villan cards
                       "4D", "4H", "6H", "8H", "TH",  // community cards
                       TIE);
  expectHandComparison("6D", "4C",                    // hero cards
                       "6S", "4S",                    // villan cards
                       "4D", "4H", "6H", "8H", "TH",  // community cards
                       TIE);
}

TEST_F(FourthRoundGeneratorTests, FourOfAKindVSFourOfAKind) {
  expectHandComparison("4C", "4S",                    // hero cards
                       "8D", "8C",                    // villan cards
                       "4D", "4H", "8S", "8H", "TH",  // community cards
                       LOSS);
  expectHandComparison("8D", "8C",                    // hero cards
                       "4C", "4S",                    // villan cards
                       "4D", "4H", "8S", "8H", "TH",  // community cards
                       WIN);

  expectHandComparison("TD", "TS",                    // hero cards
                       "QD", "2D",                    // villan cards
                       "8D", "8C", "8S", "8H", "TH",  // community cards
                       LOSS);
  expectHandComparison("QD", "2D",                    // hero cards
                       "TD", "TS",                    // villan cards
                       "8D", "8C", "8S", "8H", "TH",  // community cards
                       WIN);

  expectHandComparison("9H", "7H",                    // hero cards
                       "3D", "2D",                    // villan cards
                       "8D", "8C", "8S", "8H", "TH",  // community cards
                       TIE);
  expectHandComparison("3D", "2D",                    // hero cards
                       "9H", "7H",                    // villan cards
                       "8D", "8C", "8S", "8H", "TH",  // community cards
                       TIE);

  expectHandComparison("9H", "JH",                    // hero cards
                       "JC", "2D",                    // villan cards
                       "8D", "8C", "8S", "8H", "TH",  // community cards
                       TIE);
  expectHandComparison("JC", "2D",                    // hero cards
                       "9H", "JH",                    // villan cards
                       "8D", "8C", "8S", "8H", "TH",  // community cards
                       TIE);
}

TEST_F(FourthRoundGeneratorTests, StraightFlushVSStraightFlush) {
  expectHandComparison("AH", "2H",                    // hero cards
                       "6H", "7H",                    // villan cards
                       "5H", "3H", "4H", "TC", "TH",  // community cards
                       LOSS);
  expectHandComparison("6H", "7H",                    // hero cards
                       "AH", "2H",                    // villan cards
                       "5H", "3H", "4H", "TC", "TH",  // community cards
                       WIN);

  expectHandComparison("3H", "2H",                    // hero cards
                       "7H", "8H",                    // villan cards
                       "5H", "6H", "4H", "TC", "TH",  // community cards
                       LOSS);
  expectHandComparison("7H", "8H",                    // hero cards
                       "3H", "2H",                    // villan cards
                       "5H", "6H", "4H", "TC", "TH",  // community cards
                       WIN);

  expectHandComparison("6H", "AH",                    // hero cards
                       "JH", "2D",                    // villan cards
                       "9H", "8H", "7H", "TC", "TH",  // community cards
                       LOSS);
  expectHandComparison("JH", "2D",                    // hero cards
                       "6H", "AH",                    // villan cards
                       "9H", "8H", "7H", "TC", "TH",  // community cards
                       WIN);

  expectHandComparison("AS", "AH",                    // hero cards
                       "TS", "TC",                    // villan cards
                       "9H", "8H", "7H", "JH", "TH",  // community cards
                       TIE);
  expectHandComparison("TS", "TC",                    // hero cards
                       "AS", "AH",                    // villan cards
                       "9H", "8H", "7H", "JH", "TH",  // community cards
                       TIE);
}

// Integration Tests
// Expected percentage values computed using pokerstove

TEST_F(FourthRoundGeneratorTests, GetMeanTest) {
  EXPECT_NEAR(0.955556,
              getMeanTest("2H", "2D",                   // hero cards
                          "2C", "2S", "3D", "3H", "3C"  // community cards
                          ),
              maxEquityDiff);
  EXPECT_NEAR(0.99899,
              getMeanTest("KS", "AH",                   // hero cards
                          "AD", "AC", "AS", "KC", "QC"  // community cards
                          ),
              maxEquityDiff);
  EXPECT_NEAR(0.00000,
              getMeanTest("3H", "3C",                   // hero cards
                          "2D", "2H", "2C", "2S", "3D"  // community cards
                          ),
              maxEquityDiff);
}

TEST_F(FourthRoundGeneratorTests, GetBucketTest) {
  EXPECT_NEAR(getMeanTest("2H", "2D",                     // hero cards
                          "2C", "2S", "3D", "3H", "3C"    // community cards
                          ),
              getBucketTest("2H", "2D",                   // hero cards
                            "2C", "2S", "3D", "3H", "3C"  // community cards
                            ),
              maxEquityDiff);
  EXPECT_NEAR(getMeanTest("KS", "AH",                     // hero cards
                          "AD", "AC", "AS", "KC", "QC"    // community cards
                          ),
              getBucketTest("KS", "AH",                   // hero cards
                           "AD", "AC", "AS", "KC", "QC"   // community cards
                           ),
              maxEquityDiff);
  EXPECT_NEAR(getMeanTest("3H", "3C",                     // hero cards
                          "2D", "2H", "2C", "2S", "3D"    // community cards
                          ),
              getBucketTest("3H", "3C",                   // hero cards
                            "2D", "2H", "2C", "2S", "3D"  // community cards
                            ),
              maxEquityDiff);
}

TEST_F(FourthRoundGeneratorTests, SecondRoundIntegration) {
  EXPECT_NEAR(0.999664,
              secondRoundMean("2H", "2D",       // hero cards
                              "2C", "2S", "3D"  // community cards
                              ),
              maxEquityDiff);
  EXPECT_NEAR(0.999972,
              secondRoundMean("KS", "AH",       // hero cards
                              "AD", "AC", "AS"  // community cards
                              ),
              maxEquityDiff);
  EXPECT_NEAR(0.189307,
              secondRoundMean("4D", "5H",       // hero cards
                              "2H", "7C", "9D"  // community cards
                              ),
              maxEquityDiff);
}

TEST_F(FourthRoundGeneratorTests, FirstRoundIntegration) {
  EXPECT_NEAR(0.345836,
              firstRoundMean("2D", "7H"  // hero cards
                             ),
              maxEquityDiff);
  EXPECT_NEAR(0.653201,
              firstRoundMean("KS", "AH"  // hero cards
                             ),
              maxEquityDiff);
}
