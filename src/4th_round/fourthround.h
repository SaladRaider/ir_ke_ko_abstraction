#ifndef __FOURTHROUND_H_INCLUDED_
#define __FOURTHROUND_H_INCLUDED_

#include <array>
#include <vector>
#include <unordered_map>
#include "handtype.h"

class FourthRoundGenerator {
public:
    void findHighCard(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards);
    void findOnePair(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards);
    void findTwoPair(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards);
    void findThreeOfAKind(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards);
    void findFourOfAKind(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards);
    void findFullHouse(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards);
    void findFlush(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards,
                   std::array<std::array<int, 5>, 4> *suitIdx,
                   std::array<int, 4> *suitIdxSize);
    void findStraight(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards,
                      std::array<int, 15> *straightMask);
    void findStraightFlush(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards,
                           std::array<std::array<int, 5>, 4> *sFlush,
                           std::array<int, 4> *sFlushSize);
    void findRoyalFlush(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards,
                        std::array<std::array<int, 5>, 4> *sFlush,
                        std::array<int, 4> *sFlushSize);
    void findHandType(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards,
                      std::array<std::array<int, 5>, 4> *sFlush,
                      std::array<int, 4> *sFlushSize,
                      std::array<int, 15> *straightMask);
    int hashGetValueArgs(std::array<int, 7> *cards);
    int getValue(std::array<int, 7> *pCards, std::array<int, 7> *eCards,
                 std::unordered_map<int, std::unique_ptr<HandType>> *handTypeCache,
                 std::array<std::array<int, 5>, 4> *sFlush,
                 std::array<int, 4> *sFlushSize,
                 std::array<int, 15> *straightMask);
    float getMean(std::array<int, 7> *pCards, std::array<int, 7> *eCards,
                  std::unordered_map<int, std::unique_ptr<HandType>> *handTypeCache,
                  std::array<std::array<int, 5>, 4> *sFlush,
                  std::array<int, 4> *sFlushSize,
                  std::array<int, 15> *straightMask);
    void compute4thRound(int tid, unsigned long start, unsigned long limit, std::string fileName);
    static const std::array<int, 52> c_cardValue;
    static const std::array<Suit, 52> c_cardSuit;
    std::array<bool, 52> c_inDeck = {
        true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true
    };
};

#endif