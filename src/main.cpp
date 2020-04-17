#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>
#include <array>
#include <unordered_map>

enum Suit { HEART, DIAMOND, CLUB, SPADE };
enum HandIndex {
    NO_HAND, HIGH_CARD, ONE_PAIR, TWO_PAIR, THREE_OF_A_KIND, STRAIGHT,
    FLUSH, FULL_HOUSE, FOUR_OF_A_KIND, STRAIGHT_FLUSH, ROYAL_FLUSH
};

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

const std::array<int, 52> c_cardValue = {
    2, 2, 2, 2,
    3, 3, 3, 3,
    4, 4, 4, 4,
    5, 5, 5, 5,
    6, 6, 6, 6,
    7, 7, 7, 7,
    8, 8, 8, 8,
    9, 9, 9, 9,
    10, 10, 10, 10,
    11, 11, 11, 11,
    12, 12, 12, 12,
    13, 13, 13, 13,
    14, 14, 14, 14
};
const std::array<Suit, 52> c_cardSuit = {
    HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE,
    HEART, DIAMOND, CLUB, SPADE
};
std::array<bool, 52> c_inDeck = {
    true, true, true, true, true, true, true, true, true, true, true, true, true,
    true, true, true, true, true, true, true, true, true, true, true, true, true,
    true, true, true, true, true, true, true, true, true, true, true, true, true,
    true, true, true, true, true, true, true, true, true, true, true, true, true
};

struct HandType
{
    HandIndex handIndex;
    std::array<int, 5> keyCardIndex = {-1, -1, -1, -1, -1};
    
    HandType()
        : handIndex(NO_HAND) {};
    friend bool operator> (const HandType &h1, const HandType &h2);
};

bool operator> (const HandType &h1, const HandType &h2)
{
    if (h1.handIndex != h2.handIndex)
        return h1.handIndex > h2.handIndex;
    for (int i = 0; i < 4; i++)
    if (h1.keyCardIndex.at(i) != h2.keyCardIndex.at(i))
        return c_cardValue[h1.keyCardIndex.at(i)] > c_cardValue[h2.keyCardIndex.at(i)];
    return c_cardValue[h1.keyCardIndex.at(4)] > c_cardValue[h2.keyCardIndex.at(4)];
}

void findHighCard(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards)
{
    handType->handIndex = HIGH_CARD;
    for (int i = 0; i < 5; i++)
    handType->keyCardIndex.at(i) = 6 - i;
}

void findOnePair(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards)
{
    handType->keyCardIndex.at(2) = -1;
    handType->keyCardIndex.at(3) = -1;
    handType->keyCardIndex.at(4) = -1;
    int j = 2;
    for (int i = 6; i >= 0; i--)
    if (i >= 1 && c_cardValue[cards->at(i)] == c_cardValue[cards->at(i - 1)])
    {
        handType->handIndex = ONE_PAIR;
        handType->keyCardIndex.at(0) = i;
        handType->keyCardIndex.at(1) = i - 1;
        i -= 1;
    }
    else if (j < 5 && handType->keyCardIndex.at(j) != -1)
    {
        handType->keyCardIndex.at(j) = i; 
        j += 1;
    }
    return;
}

void findTwoPair(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards)
{
    handType->keyCardIndex.at(0) = -1;
    handType->keyCardIndex.at(4) = -1;
    for (int i = 6; i >= 0; i--)
    if (i >= 1 && c_cardValue[cards->at(i)] == c_cardValue[cards->at(i - 1)])
    {
        if (handType->keyCardIndex.at(0) == -1)
        {
            handType->keyCardIndex.at(0) = i;
            handType->keyCardIndex.at(1) = i - 1;
        }
        if (handType->keyCardIndex.at(2) == -1)
        {
            handType->handIndex = TWO_PAIR;
            handType->keyCardIndex.at(2) = i;
            handType->keyCardIndex.at(3) = i - 1;
        }
        i -= 1;
    }
    else if (handType->keyCardIndex.at(4) == -1)
        handType->keyCardIndex.at(4) = i;
    return;
}

void findThreeOfAKind(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards)
{
    int j = 3;
    handType->keyCardIndex.at(3) = -1;
    handType->keyCardIndex.at(4) = -1;
    for (int i = 6; i >= 0; i--)
    if (i >= 2 &&
        c_cardValue[cards->at(i)] == c_cardValue[cards->at(i - 1)] &&
        c_cardValue[cards->at(i)] == c_cardValue[cards->at(i - 2)])
    {
        handType->handIndex = THREE_OF_A_KIND;
        handType->keyCardIndex.at(0) = i;
        handType->keyCardIndex.at(1) = i - 1;
        handType->keyCardIndex.at(2) = i - 2;
        return;
    }
    else if (j < 5 && handType->keyCardIndex.at(j) == -1)
    {
        handType->keyCardIndex.at(j) = i;
        j += 1;
    }
    return;
}

void findFourOfAKind(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards)
{
    handType->keyCardIndex.at(4) = -1;
    for (int i = 6; i >= 0; i--)
    if (i >= 3 &&
        c_cardValue[cards->at(i)] == c_cardValue[cards->at(i - 1)] &&
        c_cardValue[cards->at(i)] == c_cardValue[cards->at(i - 2)] &&
        c_cardValue[cards->at(i)] == c_cardValue[cards->at(i - 3)])
    {
        handType->handIndex = FOUR_OF_A_KIND;
        handType->keyCardIndex.at(0) = i;
        handType->keyCardIndex.at(1) = i - 1;
        handType->keyCardIndex.at(2) = i - 2;
        handType->keyCardIndex.at(3) = i - 3;
        return;
    }
    else if (handType->keyCardIndex.at(4) == -1)
        handType->keyCardIndex.at(4) = i;
    return;
}

void findFullHouse(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards)
{
    findThreeOfAKind(handType, cards);
    if (handType->handIndex != THREE_OF_A_KIND)
        return;
    int i0 = handType->keyCardIndex.at(0);
    int i1 = handType->keyCardIndex.at(1);
    int i2 = handType->keyCardIndex.at(2);
    for (int i = 6; i > 0; i--)
    if ((i > i0 || i < i2) && c_cardValue[cards->at(i)] == c_cardValue[cards->at(i - 1)])
    {
        handType->handIndex = FULL_HOUSE;
        handType->keyCardIndex.at(3) = i;
        handType->keyCardIndex.at(4) = i - 1;
    }
    return;
}

std::array<std::array<int, 5>, 4> suitIdx;
std::array<int, 4> suitIdxSize;
void findFlush(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards)
{
    suitIdxSize.fill(0);
    int suit;
    for (int i = 6; i >= 0; i--)
    {
        suit = c_cardSuit[cards->at(i)];
        if (suitIdxSize.at(suit) < 5)
        {
            suitIdx.at(suit).at(suitIdxSize.at(suit)) = i;
            suitIdxSize.at(suit) += 1;
        }   
        else
        {
            handType->handIndex = FLUSH;
            for (int j = 4; j >= 0; j--)
                handType->keyCardIndex.at(j) = suitIdx.at(suit).at(j);
            return;
        }
    }
    return;
}

std::array<int, 5> straight;
void findStraight(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards)
{
    int straightSize = 0;
    int k;
    int kk;
    int cV0, cV1;
    for (int i = 13; i >= 6; i--)
    {
    k = i % 7;
    kk = (i + 1) % 7;
    cV0 = c_cardValue[cards->at(k)];
    cV1 = c_cardValue[cards->at(kk)];
    if (cV0 - 1 == cV1 ||
        cV0 + 12 == cV1)
    {
        straight.at(straightSize) = k;
        straightSize += 1;
        if (straightSize == 5)
        {
            handType->handIndex = STRAIGHT;
            if (cV0 == 14)
            {
                for (int j = 0; j < 4; j++)
                    handType->keyCardIndex.at(j + 1) = straight.at(j);
                handType->keyCardIndex.at(0) = k;
            }
            else
            {
                for (int j = 0; j < 5; j++)
                    handType->keyCardIndex.at(j) = straight.at(j);
            }
            return;
        }
    }
    else if (cV0 != cV1)
    {
        straightSize = 1;
        straight.at(0) = k;
    }
    }
}

std::array<std::array<int, 5>, 4> sFlush;
std::array<int, 4> sFlushSize;
void findStraightFlush(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards)
{
    findFlush(handType, cards);
    if (handType->handIndex != FLUSH)
        return;
    sFlushSize.fill(0);
    int k;
    int cV0;
    int suit;
    for (int i = 13; i >= 3; i--)
    {
        k = i % 7;
        cV0 = c_cardValue[cards->at(k)];
        suit = (int) c_cardSuit[cards->at(k)];
        if (sFlushSize.at(suit) == 0)
        {
            sFlush.at(suit).at(0) = k;
            sFlushSize.at(suit) += 1;
        }
        else if (c_cardValue[sFlush.at(suit).back()] - 1 == cV0 ||
                 c_cardValue[sFlush.at(suit).back()] + 12 == cV0)
        {
            sFlush.at(suit).at(sFlushSize.at(suit)) = k;
            sFlushSize.at(suit) += 1;
            if (sFlushSize.at(suit) == 5)
            {
                handType->handIndex = STRAIGHT_FLUSH;
                if (cV0 == 14)
                {
                    for (int j = 0; j < 4; j++)
                        handType->keyCardIndex.at(j + 1) = sFlush.at(suit).at(j);
                    handType->keyCardIndex.at(0) = k;
                }
                else
                {
                    for (int j = 0; j < 5; j++)
                        handType->keyCardIndex.at(j) = sFlush.at(suit).at(j);
                }
                return;
            }
        }
        else
        {
            sFlush.at(suit).at(0) = k;
            sFlushSize.at(suit) = 1;
        }
    }
    return;
}

void findRoyalFlush(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards)
{
    findStraightFlush(handType, cards);
    if (handType->handIndex == STRAIGHT_FLUSH &&
        c_cardValue[cards->at(handType->keyCardIndex.at(0))] == 14)
        handType->handIndex = ROYAL_FLUSH;
    return;
}

void findHandType(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards)
{
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

int hashGetValueArgs(std::array<int, 7> *cards)
{
    int k = 53;
    int hash = 0;
    for (int i = 0; i < 7; i++)
    {
        hash = (cards->at(i)+1) * k;
    }
    return hash;
}

std::unordered_map<int, std::unique_ptr<HandType>> handTypeCache;
std::unique_ptr<HandType> pHandp;
std::unique_ptr<HandType> eHandp;
HandType pHand;
HandType eHand;
std::array<int, 7> pCards;
std::array<int, 7> eCards;
std::array<int, 5> cCards;
int getValue(int p0, int p1, int e0, int e1, int c0, int c1, int c2, int c3, int c4)
{
    bool setFirstE = false;
    bool setSecondE = false;
    int j = 0;
    for (int i = 0; i < 7; i++)
    {
        if (!setFirstE && (j == 5 || e0 < cCards.at(j)))
        {
            setFirstE = true;
            eCards.at(i) = e0;
        }
        else if (setFirstE && !setSecondE && (j == 5 || e1 < cCards.at(j)))
        {
            setSecondE = true;
            eCards.at(i) = e1;
        }
        else
        {
            eCards.at(i) = cCards.at(j);
            j += 1;
        }
    }
    int pHash = hashGetValueArgs(&pCards);
    int eHash = hashGetValueArgs(&eCards);
    if (handTypeCache.find(pHash) == handTypeCache.end())
    {
        handTypeCache[pHash] = std::unique_ptr<HandType> (new HandType);
        findHandType(handTypeCache.at(pHash), &pCards);
    }
    if (handTypeCache.find(eHash) == handTypeCache.end())
    {
        handTypeCache[eHash] = std::unique_ptr<HandType> (new HandType);
        findHandType(handTypeCache.at(eHash), &eCards);
    }
    pHand = *handTypeCache.at(pHash);
    eHand = *handTypeCache.at(eHash);
    if (pHand > eHand)
    {
        return 2;
    }
    else if (eHand > pHand)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

unsigned long int compute4thRound(int limit)
{
    std::ofstream f;
    f.open("4th_round_distributions.csv");
    f << "hand0,hand1,cc0,cc1,cc2,cc3,cc4,b0,b1,b2,mean\n";
    int n = 52;
    int k = 2;
    int count = 0;
    unsigned long int innerCount = 0;
    for (int i0 = 0; i0 < n - 2 + 1; i0++)
    for (int i1 = i0 + 1; i1 < n - 2 + 2; i1++)
    {
        c_inDeck[i0] = false;
        c_inDeck[i1] = false;
        for (int i2 = 0; i2 < n - 5 + 1; i2++)
        if (c_inDeck.at(i2))
        for (int i3 = i2 + 1; i3 < n - 5 + 2; i3++)
        if (c_inDeck.at(i3))
        for (int i4 = i3 + 1; i4 < n - 5 + 3; i4++)
        if (c_inDeck.at(i4))
        for (int i5 = i4 + 1; i5 < n - 5 + 4; i5++)
        if (c_inDeck.at(i5))
        for (int i6 = i5 + 1; i6 < n - 5 + 5; i6++)
        if (c_inDeck.at(i6))
        {
            c_inDeck[i2] = false;
            c_inDeck[i3] = false;
            c_inDeck[i4] = false;
            c_inDeck[i5] = false;
            c_inDeck[i6] = false;
            cCards = {i2, i3, i4, i5, i6};
            bool setFirstP = false;
            bool setSecondP = false;
            int j = 0;
            for (int i = 0; i < 7; i++)
            {
                if (!setFirstP && (j == 5 || i0 < cCards.at(j)))
                {
                    setFirstP = true;
                    pCards.at(i) = i0;
                }
                else if (setFirstP && !setSecondP && (j == 5 || i1 < cCards.at(j)))
                {
                    setSecondP = true;
                    pCards.at(i) = i1;
                }
                else
                {
                    pCards.at(i) = cCards.at(j);
                    j += 1;
                }
            }
            std::sort(pCards.begin(), pCards.end());
            count += 1;
            int sum = 0;
            int numCases = 0;
            std::array<int, 3> buckets;
            for (int i = 0; i < 3; i++)
                buckets.at(i) = 0;
            for (int i7 = 0; i7 < n - 2 + 1; i7++)
            if (c_inDeck.at(i7))
            for (int i8 = i7 + 1; i8 < n - 2 + 2; i8++)
            if (c_inDeck.at(i8))
            {
                int v = getValue(i0, i1, i7, i8, i2, i3, i4, i5, i6);
                buckets.at(v) += 1;
                numCases += 1;
                sum += v;
                innerCount += 1;
            }
            if (handTypeCache.size() >= 1758276) // (52 choose 2)^2
                handTypeCache.clear();
            float mean = (float) sum / (numCases * 2);
            f << i0 << ',' << i1 << ',' << i2 << ',' << i3 << ',' << i4 << ',' << i5 << ',' << i6;
            f << ',' << buckets.at(0) << ',' << buckets.at(1) << ',' << buckets.at(2) << ',' << mean << '\n';
            if (count >= limit)
            {
                f.close();
                return innerCount;
            }
            c_inDeck[i2] = true;
            c_inDeck[i3] = true;
            c_inDeck[i4] = true;
            c_inDeck[i5] = true;
            c_inDeck[i6] = true;
        }
        c_inDeck[i0] = true;
        c_inDeck[i1] = true;
    }
    f.close();
    return innerCount;
}

int main()
{
    int T;
    printf("T:\n");
    std::cin >> T;
    unsigned long int innerCount = compute4thRound(T);
    HandType handType;
    handType.handIndex = HIGH_CARD;
    handType.keyCardIndex.at(0) = 52;
    handType.keyCardIndex.at(1) = 52;
    handType.keyCardIndex.at(2) = 52;
    handType.keyCardIndex.at(3) = 52;
    handType.keyCardIndex.at(4) = 52;
    printf("sizeof(handType) = %d\n", sizeof(handType));
    printf("innerCount = %d\n", innerCount);
    printf("handTypeCache.size() = %d\n", handTypeCache.size());
    printf("Done computing.\n");
}
