#ifndef __HANDTYPE_H_INCLUDED_
#define __HANDTYPE_H_INCLUDED_

enum Suit { HEART, DIAMOND, CLUB, SPADE };
enum HandIndex {
  NO_HAND,
  HIGH_CARD,
  ONE_PAIR,
  TWO_PAIR,
  THREE_OF_A_KIND,
  STRAIGHT,
  FLUSH,
  FULL_HOUSE,
  FOUR_OF_A_KIND,
  STRAIGHT_FLUSH,
  ROYAL_FLUSH
};

struct HandType {
  HandIndex handIndex;
  std::array<int, 5> keyCardIndex;

  HandType() : handIndex(NO_HAND){};
  friend bool operator>(const HandType &h1, const HandType &h2);
};

bool operator>(const HandType &h1, const HandType &h2);

#endif