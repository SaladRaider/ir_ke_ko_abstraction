#ifndef __FOURTHROUND_H_INCLUDED_
#define __FOURTHROUND_H_INCLUDED_

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>

#include "handtype.h"

class FourthRoundGenerator {
 private:
  std::unordered_map<int, std::unique_ptr<HandType>> handTypeCache;
  std::array<std::array<size_t, 5>, 4> sFlush;
  std::array<size_t, 4> sFlushSize;
  std::array<size_t, 15> straightMask;
  std::array<size_t, 4> suitIdxSize;
  std::array<std::array<size_t, 5>, 4> suitIdx;
  std::array<float, 8> buckets;

 public:
  std::array<int, 7> pCards;
  std::array<int, 7> eCards;
  void findHighCard(std::unique_ptr<HandType> &handType,
                    const std::array<int, 7> &cards);
  void findOnePair(std::unique_ptr<HandType> &handType,
                   const std::array<int, 7> &cards);
  void findTwoPair(std::unique_ptr<HandType> &handType,
                   const std::array<int, 7> &cards);
  void findThreeOfAKind(std::unique_ptr<HandType> &handType,
                        const std::array<int, 7> &cards);
  void findFourOfAKind(std::unique_ptr<HandType> &handType,
                       const std::array<int, 7> &cards);
  void findFullHouse(std::unique_ptr<HandType> &handType,
                     const std::array<int, 7> &cards);
  void findFlush(std::unique_ptr<HandType> &handType,
                 const std::array<int, 7> &cards);
  void findStraight(std::unique_ptr<HandType> &handType,
                    const std::array<int, 7> &cards);
  void findStraightFlush(std::unique_ptr<HandType> &handType,
                         const std::array<int, 7> &cards);
  void findRoyalFlush(std::unique_ptr<HandType> &handType,
                      const std::array<int, 7> &cards);
  void findHandType(std::unique_ptr<HandType> &handType,
                    const std::array<int, 7> &cards);
  void computeBuckets();
  size_t hashGetValueArgs(const std::array<int, 7> &cards);
  int getValue();
  float getMean();
  void compute4thRound(int tid, unsigned long start, unsigned long limit,
                       std::string fileName);
  static const std::array<int, 52> c_cardValue;
  static const std::array<Suit, 52> c_cardSuit;
  static std::unordered_map<size_t, size_t> clusterMap;
  std::array<bool, 52> c_inDeck = {
      true, true, true, true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true, true, true, true,
      true, true, true, true, true, true, true, true};
};

#endif