#include "fourthround.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <vector>

#include "handtype.h"

// 4th round estimated file size: 74.19 GB
//     6.18 GB per file
//     12 4th round files
// estimated time to compute w/ 12 threads: 4.27 hours

// memory requirements:
// 1-3 betting round distribution map item size: 416 bytes
// 4th betting round distribution map item size: 28 bytes
// will use < 8 GB of memory total

// compute 4th round distributions and means
//     save each distribution to disk at a time
// compute 3rd round distributions and means using 4th round values
//     save each distribution to disk at a time
// save 4th round distributions to disk
// free 4th round distributions from memeory
// compute 2nd round distributions and means using
// etc..

const std::array<int, 52> FourthRoundGenerator::c_cardValue = {
    2,  2,  2,  2,  3,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  6,  6,
    6,  6,  7,  7,  7,  7,  8,  8,  8,  8,  9,  9,  9,  9,  10, 10, 10, 10,
    11, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14};
const std::array<Suit, 52> FourthRoundGenerator::c_cardSuit = {
    HEART, DIAMOND, CLUB, SPADE, HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE, HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE, HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE, HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE, HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE, HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE};

bool operator>(const HandType &h1, const HandType &h2) {
  if (h1.handIndex != h2.handIndex) return h1.handIndex > h2.handIndex;
  for (size_t i = 0; i < 4; i++)
    if (FourthRoundGenerator::c_cardValue[h1.keyCardIndex[i]] !=
        FourthRoundGenerator::c_cardValue[h2.keyCardIndex[i]])
      return FourthRoundGenerator::c_cardValue[h1.keyCardIndex[i]] >
             FourthRoundGenerator::c_cardValue[h2.keyCardIndex[i]];
  return FourthRoundGenerator::c_cardValue[h1.keyCardIndex[4]] >
         FourthRoundGenerator::c_cardValue[h2.keyCardIndex[4]];
}

void FourthRoundGenerator::findHighCard(std::unique_ptr<HandType> &handType,
                                        const std::array<int, 7> &cards) {
  handType->handIndex = HIGH_CARD;
  for (size_t i = 0; i < 5; i++) handType->keyCardIndex[i] = 6 - i;
}

void FourthRoundGenerator::findOnePair(std::unique_ptr<HandType> &handType,
                                       const std::array<int, 7> &cards) {
  handType->keyCardIndex[2] = -1;
  handType->keyCardIndex[3] = -1;
  handType->keyCardIndex[4] = -1;
  handType->handIndex = HIGH_CARD;
  int j = 2;
  for (int i = 6; i >= 0; i--)
    if (i >= 1 && handType->handIndex != ONE_PAIR &&
        c_cardValue[cards[i]] == c_cardValue[cards[i - 1]]) {
      handType->handIndex = ONE_PAIR;
      handType->keyCardIndex[0] = i;
      handType->keyCardIndex[1] = i - 1;
      i -= 1;
    } else if (j < 5 && handType->keyCardIndex[j] == -1) {
      handType->keyCardIndex[j] = i;
      j += 1;
    }
  return;
}

void FourthRoundGenerator::findTwoPair(std::unique_ptr<HandType> &handType,
                                       const std::array<int, 7> &cards) {
  handType->keyCardIndex[4] = -1;
  handType->keyCardIndex[0] = -1;
  handType->keyCardIndex[2] = -1;
  for (int i = 6; i >= 0; i--)
    if (i >= 1 && c_cardValue[cards[i]] == c_cardValue[cards[i - 1]]) {
      if (handType->keyCardIndex[0] == -1) {
        handType->keyCardIndex[0] = i;
        handType->keyCardIndex[1] = i - 1;
        i -= 1;
      } else if (handType->keyCardIndex[2] == -1) {
        handType->handIndex = TWO_PAIR;
        handType->keyCardIndex[2] = i;
        handType->keyCardIndex[3] = i - 1;
        i -= 1;
      }
    } else if (handType->keyCardIndex[4] == -1)
      handType->keyCardIndex[4] = i;
  return;
}

void FourthRoundGenerator::findThreeOfAKind(std::unique_ptr<HandType> &handType,
                                            const std::array<int, 7> &cards) {
  int j = 3;
  handType->keyCardIndex[3] = -1;
  handType->keyCardIndex[4] = -1;
  handType->handIndex = HIGH_CARD;
  for (int i = 6; i >= 0; i--)
    if (i >= 2 && handType->handIndex != THREE_OF_A_KIND &&
        c_cardValue[cards[i]] == c_cardValue[cards[i - 1]] &&
        c_cardValue[cards[i]] == c_cardValue[cards[i - 2]]) {
      handType->handIndex = THREE_OF_A_KIND;
      handType->keyCardIndex[0] = i;
      handType->keyCardIndex[1] = i - 1;
      handType->keyCardIndex[2] = i - 2;
      i -= 2;
    } else if (j < 5 && handType->keyCardIndex[j] == -1) {
      handType->keyCardIndex[j] = i;
      j += 1;
    }
  return;
}

void FourthRoundGenerator::findFourOfAKind(std::unique_ptr<HandType> &handType,
                                           const std::array<int, 7> &cards) {
  handType->keyCardIndex[4] = -1;
  handType->handIndex = HIGH_CARD;
  for (int i = 6; i >= 0; i--)
    if (i >= 3 && handType->handIndex != FOUR_OF_A_KIND &&
        c_cardValue[cards[i]] == c_cardValue[cards[i - 1]] &&
        c_cardValue[cards[i]] == c_cardValue[cards[i - 2]] &&
        c_cardValue[cards[i]] == c_cardValue[cards[i - 3]]) {
      handType->handIndex = FOUR_OF_A_KIND;
      handType->keyCardIndex[0] = i;
      handType->keyCardIndex[1] = i - 1;
      handType->keyCardIndex[2] = i - 2;
      handType->keyCardIndex[3] = i - 3;
      i -= 3;
    } else if (handType->keyCardIndex[4] == -1)
      handType->keyCardIndex[4] = i;
  return;
}

void FourthRoundGenerator::findFullHouse(std::unique_ptr<HandType> &handType,
                                         const std::array<int, 7> &cards) {
  findThreeOfAKind(handType, cards);
  if (handType->handIndex != THREE_OF_A_KIND) return;
  int i0 = handType->keyCardIndex[0];
  int i1 = handType->keyCardIndex[1];
  int i2 = handType->keyCardIndex[2];
  for (size_t i = 6; i > 0; i--)
    if ((i > i0 || i < i2) &&
        c_cardValue[cards[i]] == c_cardValue[cards[i - 1]]) {
      handType->handIndex = FULL_HOUSE;
      handType->keyCardIndex[3] = i;
      handType->keyCardIndex[4] = i - 1;
      return;
    }
  return;
}

void FourthRoundGenerator::findFlush(std::unique_ptr<HandType> &handType,
                                     const std::array<int, 7> &cards) {
  suitIdxSize.fill(0);
  size_t suit;
  for (int i = 6; i >= 0; i--) {
    suit = c_cardSuit[cards[i]];
    suitIdx[suit][suitIdxSize[suit]] = i;
    suitIdxSize[suit] += 1;
    if (suitIdxSize[suit] == 5) {
      handType->handIndex = FLUSH;
      for (int j = 4; j >= 0; j--) handType->keyCardIndex[j] = suitIdx[suit][j];
      return;
    }
  }
  return;
}

void FourthRoundGenerator::findStraight(std::unique_ptr<HandType> &handType,
                                        const std::array<int, 7> &cards) {
  int cardValue;
  for (size_t i = 0; i < 15; i++) {
    straightMask[i] = -1;
  }
  for (size_t i = 0; i < 7; i++) {
    cardValue = c_cardValue[cards[i]];
    straightMask[cardValue] = i;
  }
  if (straightMask[14] != -1 && straightMask[2] != -1 &&
      straightMask[3] != -1 && straightMask[4] != -1 && straightMask[5] != -1) {
    handType->handIndex = STRAIGHT;
    handType->keyCardIndex[0] = straightMask[5];
    handType->keyCardIndex[1] = straightMask[4];
    handType->keyCardIndex[2] = straightMask[3];
    handType->keyCardIndex[3] = straightMask[2];
    handType->keyCardIndex[4] = straightMask[14];
  }
  int count = 0;
  for (size_t i = 14; i >= 2; i--) {
    if (straightMask[i] != -1) {
      count += 1;
      if (count == 5) {
        handType->handIndex = STRAIGHT;
        handType->keyCardIndex[0] = straightMask[i + 4];
        handType->keyCardIndex[1] = straightMask[i + 3];
        handType->keyCardIndex[2] = straightMask[i + 2];
        handType->keyCardIndex[3] = straightMask[i + 1];
        handType->keyCardIndex[4] = straightMask[i];
        return;
      }
    } else
      count = 0;
  }
}

void FourthRoundGenerator::findStraightFlush(
    std::unique_ptr<HandType> &handType, const std::array<int, 7> &cards) {
  findFlush(handType, cards);
  if (handType->handIndex != FLUSH) return;
  sFlushSize.fill(0);
  int k;
  int cV0;
  int suit;
  for (int i = 13; i >= 0; i--) {
    k = i % 7;
    cV0 = c_cardValue[cards[k]];
    suit = (int)c_cardSuit[cards[k]];
    if (sFlushSize[suit] == 0) {
      sFlush[suit][0] = k;
      sFlushSize[suit] += 1;
    } else if (c_cardValue[cards[sFlush[suit][sFlushSize[suit] - 1]]] - 1 ==
                   cV0 ||
               c_cardValue[cards[sFlush[suit][sFlushSize[suit] - 1]]] + 12 ==
                   cV0) {
      sFlush[suit][sFlushSize[suit]] = k;
      sFlushSize[suit] += 1;
      if (sFlushSize[suit] == 5) {
        handType->handIndex = STRAIGHT_FLUSH;
        for (int j = 0; j < 5; j++) handType->keyCardIndex[j] = sFlush[suit][j];
        return;
      }
    } else {
      sFlush[suit][0] = k;
      sFlushSize[suit] = 1;
    }
  }
  return;
}

void FourthRoundGenerator::findRoyalFlush(std::unique_ptr<HandType> &handType,
                                          const std::array<int, 7> &cards) {
  findStraightFlush(handType, cards);
  if (handType->handIndex == STRAIGHT_FLUSH &&
      c_cardValue[cards[handType->keyCardIndex[0]]] == 14 &&
      c_cardValue[cards[handType->keyCardIndex[4]]] == 10)
    handType->handIndex = ROYAL_FLUSH;
  return;
}

void FourthRoundGenerator::findHandType(std::unique_ptr<HandType> &handType,
                                        const std::array<int, 7> &cards) {
  findRoyalFlush(handType, cards);
  if (handType->handIndex == ROYAL_FLUSH) {
    return;
  }
  findStraightFlush(handType, cards);
  if (handType->handIndex == STRAIGHT_FLUSH) {
    return;
  }
  findFourOfAKind(handType, cards);
  if (handType->handIndex == FOUR_OF_A_KIND) {
    return;
  }

  findFullHouse(handType, cards);
  if (handType->handIndex == FULL_HOUSE) {
    return;
  }

  findFlush(handType, cards);
  if (handType->handIndex == FLUSH) {
    return;
  }

  findStraight(handType, cards);
  if (handType->handIndex == STRAIGHT) {
    return;
  }
  findThreeOfAKind(handType, cards);
  if (handType->handIndex == THREE_OF_A_KIND) {
    return;
  }
  findTwoPair(handType, cards);
  if (handType->handIndex == TWO_PAIR) {
    return;
  }
  findOnePair(handType, cards);
  if (handType->handIndex == ONE_PAIR) {
    return;
  }
  findHighCard(handType, cards);
  return;
}

size_t FourthRoundGenerator::hashGetValueArgs(const std::array<int, 7> &cards) {
  size_t hash = 0;
  for (size_t i = 0; i < 7; i++) {
    hash = (hash + cards[i] + 1) * 100;
  }
  return hash;
}

int FourthRoundGenerator::getValue() {
  size_t pHash = hashGetValueArgs(pCards);
  size_t eHash = hashGetValueArgs(eCards);
  if (handTypeCache.find(pHash) == handTypeCache.end()) {
    handTypeCache[pHash] = std::unique_ptr<HandType>(new HandType);
    findHandType(handTypeCache[pHash], pCards);
    handTypeCache[pHash]->keyCardIndex[0] =
        pCards[handTypeCache[pHash]->keyCardIndex[0]];
    handTypeCache[pHash]->keyCardIndex[1] =
        pCards[handTypeCache[pHash]->keyCardIndex[1]];
    handTypeCache[pHash]->keyCardIndex[2] =
        pCards[handTypeCache[pHash]->keyCardIndex[2]];
    handTypeCache[pHash]->keyCardIndex[3] =
        pCards[handTypeCache[pHash]->keyCardIndex[3]];
    handTypeCache[pHash]->keyCardIndex[4] =
        pCards[handTypeCache[pHash]->keyCardIndex[4]];
  }
  if (handTypeCache.find(eHash) == handTypeCache.end()) {
    handTypeCache[eHash] = std::unique_ptr<HandType>(new HandType);
    findHandType(handTypeCache[eHash], eCards);
    handTypeCache[eHash]->keyCardIndex[0] =
        eCards[handTypeCache[eHash]->keyCardIndex[0]];
    handTypeCache[eHash]->keyCardIndex[1] =
        eCards[handTypeCache[eHash]->keyCardIndex[1]];
    handTypeCache[eHash]->keyCardIndex[2] =
        eCards[handTypeCache[eHash]->keyCardIndex[2]];
    handTypeCache[eHash]->keyCardIndex[3] =
        eCards[handTypeCache[eHash]->keyCardIndex[3]];
    handTypeCache[eHash]->keyCardIndex[4] =
        eCards[handTypeCache[eHash]->keyCardIndex[4]];
  }
  if (*handTypeCache[pHash] > *handTypeCache[eHash]) {
    return 2;
  } else if (*handTypeCache[eHash] > *handTypeCache[pHash]) {
    return 0;
  }
  return 1;
}

float FourthRoundGenerator::getMean() {
  int sum = 0;
  int numCases = 0;
  int c0 = eCards[2];
  int c1 = eCards[3];
  int c2 = eCards[4];
  int c3 = eCards[5];
  int c4 = eCards[6];
  for (size_t i7 = 0; i7 < 51; i7++)
    if (c_inDeck[i7])
      for (size_t i8 = i7 + 1; i8 < 52; i8++)
        if (c_inDeck[i8]) {
          eCards[0] = i7;
          eCards[1] = i8;
          eCards[2] = c0;
          eCards[3] = c1;
          eCards[4] = c2;
          eCards[5] = c3;
          eCards[6] = c4;
          std::sort(eCards.begin(), eCards.end());
          numCases += 1;
          sum += getValue();
        }
  return float(sum) / float(numCases * 2);
}

void FourthRoundGenerator::compute4thRound(int tid, unsigned long start,
                                           unsigned long limit,
                                           std::string fileName) {
  std::ofstream f;
  f.open(fileName);
  int n = 52;
  int k = 2;
  int v = 0, sum = 0, numCases = 0, pHash;
  unsigned long count = 0;
  size_t i = 0, i0 = 0, i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0, i6 = 0, i7 = 0,
         i8 = 0;
  float mean = 0.0f;
  unsigned long int innerCount = 0;
  c_inDeck.fill(true);
  for (i2 = 0; i2 < 48; i2++)
    for (i3 = i2 + 1; i3 < 49; i3++)
      for (i4 = i3 + 1; i4 < 50; i4++)
        for (i5 = i4 + 1; i5 < 51; i5++)
          for (i6 = i5 + 1; i6 < 52; i6++) {
            c_inDeck[i2] = false;
            c_inDeck[i3] = false;
            c_inDeck[i4] = false;
            c_inDeck[i5] = false;
            c_inDeck[i6] = false;
            for (i0 = 0; i0 < 51; i0++)
              if (c_inDeck[i0])
                for (i1 = i0 + 1; i1 < 52; i1++)
                  if (c_inDeck[i1]) {
                    if (count < start) {
                      count += 1;
                      continue;
                    }
                    c_inDeck[i0] = false;
                    c_inDeck[i1] = false;

                    pCards[0] = i0;
                    pCards[1] = i1;
                    pCards[2] = i2;
                    pCards[3] = i3;
                    pCards[4] = i4;
                    pCards[5] = i5;
                    pCards[6] = i6;
                    std::sort(pCards.begin(), pCards.end());
                    eCards[0] = 0;
                    eCards[1] = 0;
                    eCards[2] = i2;
                    eCards[3] = i3;
                    eCards[4] = i4;
                    eCards[5] = i5;
                    eCards[6] = i6;

                    f << getMean() << '\n';

                    count += 1;
                    if (limit > 0 && count >= limit) {
                      f.close();
                      return;
                    }
                    c_inDeck[i0] = true;
                    c_inDeck[i1] = true;
                  }
            if (handTypeCache.size() > 1081)  // 52 choose 2 -old 8 * (52 choose
                                              // 2) * (50 choose 2) : ~1 GB mem
            {
              for (auto it = handTypeCache.begin(); it != handTypeCache.end();
                   it++) {
                it->second.reset(nullptr);
              }
              handTypeCache.clear();
            }
            c_inDeck[i2] = true;
            c_inDeck[i3] = true;
            c_inDeck[i4] = true;
            c_inDeck[i5] = true;
            c_inDeck[i6] = true;
          }
  f.close();
  return;
}
