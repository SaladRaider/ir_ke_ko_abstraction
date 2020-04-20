#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <memory>
#include <algorithm>
#include <array>
#include <vector>
#include <unordered_map>

enum Suit { HEART, DIAMOND, CLUB, SPADE };
enum HandIndex {
    NO_HAND, HIGH_CARD, ONE_PAIR, TWO_PAIR, THREE_OF_A_KIND, STRAIGHT,
    FLUSH, FULL_HOUSE, FOUR_OF_A_KIND, STRAIGHT_FLUSH, ROYAL_FLUSH
};

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
    int handValue = 0;
    HandIndex handIndex;
    std::array<int, 5> keyCardIndex;
    
    HandType()
        : handIndex(NO_HAND) {};
    friend bool operator> (const HandType &h1, const HandType &h2);
};

bool operator> (const HandType &h1, const HandType &h2)
{
    return h1.handValue > h2.handValue;
    /*
    if (h1.handIndex != h2.handIndex)
        return h1.handIndex > h2.handIndex;
    for (int i = 0; i < 4; i++)
    if (c_cardValue[h1.keyCardIndex[i]] != c_cardValue[h2.keyCardIndex[i]])
        return c_cardValue[h1.keyCardIndex[i]] > c_cardValue[h2.keyCardIndex[i]];
    return c_cardValue[h1.keyCardIndex[4]] > c_cardValue[h2.keyCardIndex[4]];
    */
}

void findHighCard(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards)
{
    handType->handIndex = HIGH_CARD;
    for (int i = 0; i < 5; i++)
    handType->keyCardIndex[i] = 6 - i;
}

void findOnePair(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards)
{
    handType->keyCardIndex[2] = -1;
    handType->keyCardIndex[3] = -1;
    handType->keyCardIndex[4] = -1;
    handType->handIndex = HIGH_CARD;
    int j = 2;
    for (int i = 6; i >= 0; i--)
    if (i >= 1 && handType->handIndex != ONE_PAIR &&
        c_cardValue[(*cards)[i]] == c_cardValue[(*cards)[i - 1]])
    {
        handType->handIndex = ONE_PAIR;
        handType->keyCardIndex[0] = i;
        handType->keyCardIndex[1] = i - 1;
        i -= 1;
    }
    else if (j < 5 && handType->keyCardIndex[j] == -1)
    {
        handType->keyCardIndex[j] = i; 
        j += 1;
    }
    return;
}

void findTwoPair(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards)
{
    handType->keyCardIndex[4] = -1;
    handType->keyCardIndex[0] = -1;
    handType->keyCardIndex[2] = -1;
    for (int i = 6; i >= 0; i--)
    if (i >= 1 && c_cardValue[(*cards)[i]] == c_cardValue[(*cards)[i - 1]])
    {
        if (handType->keyCardIndex[0] == -1)
        {
            handType->keyCardIndex[0] = i;
            handType->keyCardIndex[1] = i - 1;
            i -= 1;
        }
        else if (handType->keyCardIndex[2] == -1)
        {
            handType->handIndex = TWO_PAIR;
            handType->keyCardIndex[2] = i;
            handType->keyCardIndex[3] = i - 1;
            i -= 1;
        }
    }
    else if (handType->keyCardIndex[4] == -1)
        handType->keyCardIndex[4] = i;
    return;
}

void findThreeOfAKind(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards)
{
    int j = 3;
    handType->keyCardIndex[3] = -1;
    handType->keyCardIndex[4] = -1;
    handType->handIndex = HIGH_CARD;
    for (int i = 6; i >= 0; i--)
    if (i >= 2 && handType->handIndex != THREE_OF_A_KIND &&
        c_cardValue[(*cards)[i]] == c_cardValue[(*cards)[i - 1]] &&
        c_cardValue[(*cards)[i]] == c_cardValue[(*cards)[i - 2]])
    {
        handType->handIndex = THREE_OF_A_KIND;
        handType->keyCardIndex[0] = i;
        handType->keyCardIndex[1] = i - 1;
        handType->keyCardIndex[2] = i - 2;
        i -= 2;
    }
    else if (j < 5 && handType->keyCardIndex[j] == -1)
    {
        handType->keyCardIndex[j] = i;
        j += 1;
    }
    return;
}

void findFourOfAKind(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards)
{
    handType->keyCardIndex[4] = -1;
    handType->handIndex = HIGH_CARD;
    for (int i = 6; i >= 0; i--)
    if (i >= 3 && handType->handIndex != FOUR_OF_A_KIND &&
        c_cardValue[(*cards)[i]] == c_cardValue[(*cards)[i - 1]] &&
        c_cardValue[(*cards)[i]] == c_cardValue[(*cards)[i - 2]] &&
        c_cardValue[(*cards)[i]] == c_cardValue[(*cards)[i - 3]])
    {
        handType->handIndex = FOUR_OF_A_KIND;
        handType->keyCardIndex[0] = i;
        handType->keyCardIndex[1] = i - 1;
        handType->keyCardIndex[2] = i - 2;
        handType->keyCardIndex[3] = i - 3;
        i -= 3;
    }
    else if (handType->keyCardIndex[4] == -1)
        handType->keyCardIndex[4] = i;
    return;
}

void findFullHouse(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards)
{
    findThreeOfAKind(handType, cards);
    if (handType->handIndex != THREE_OF_A_KIND)
        return;
    int i0 = handType->keyCardIndex[0];
    int i1 = handType->keyCardIndex[1];
    int i2 = handType->keyCardIndex[2];
    for (int i = 6; i > 0; i--)
    if ((i > i0 || i < i2) && c_cardValue[cards->at(i)] == c_cardValue[cards->at(i - 1)])
    {
        handType->handIndex = FULL_HOUSE;
        handType->keyCardIndex[3] = i;
        handType->keyCardIndex[4] = i - 1;
        return;
    }
    return;
}

void findFlush(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards,
               std::array<std::array<int, 5>, 4> *suitIdx,
               std::array<int, 4> *suitIdxSize)
{
    suitIdxSize->fill(0);
    int suit;
    for (int i = 6; i >= 0; i--)
    {
        suit = c_cardSuit[(*cards)[i]];
        (*suitIdx)[suit][(*suitIdxSize)[suit]] = i;
        (*suitIdxSize)[suit] += 1;
        if ((*suitIdxSize)[suit] == 5)
        {
            handType->handIndex = FLUSH;
            for (int j = 4; j >= 0; j--)
                handType->keyCardIndex[j] = (*suitIdx)[suit][j];
            return;
        }
    }
    return;
}

std::array<int, 5> straight;
void findStraight(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards,
                  std::array<int, 15> *straightMask)
{
    int cardValue;
    for (int i = 0; i < 15; i++)
    {
        (*straightMask)[i] = -1;
    }
    for (int i = 0; i < 7; i++)
    {
        cardValue = c_cardValue[(*cards)[i]];
        (*straightMask)[cardValue] = i;
    }
    if ((*straightMask)[14] != -1 &&
        (*straightMask)[2] != -1 &&
        (*straightMask)[3] != -1 &&
        (*straightMask)[4] != -1 &&
        (*straightMask)[5] != -1)
    {
        handType->handIndex = STRAIGHT;
        handType->keyCardIndex[0] = (*straightMask)[14];
        handType->keyCardIndex[1] = (*straightMask)[2];
        handType->keyCardIndex[2] = (*straightMask)[3];
        handType->keyCardIndex[3] = (*straightMask)[4];
        handType->keyCardIndex[4] = (*straightMask)[5];
    }
    int count = 0;
    for (int i = 14; i >= 2; i--)
    {
        if ((*straightMask)[i] != -1)
        {
            count += 1;
            if (count == 5)
            {
                handType->handIndex = STRAIGHT;
                handType->keyCardIndex[0] = (*straightMask)[i + 4];
                handType->keyCardIndex[1] = (*straightMask)[i + 3];
                handType->keyCardIndex[2] = (*straightMask)[i + 2];
                handType->keyCardIndex[3] = (*straightMask)[i + 1];
                handType->keyCardIndex[4] = (*straightMask)[i];
                return;
            }
        }
        else
            count = 0;
    }
    /*int straightSize = 0;
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
    }*/
}

void findStraightFlush(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards,
                       std::array<std::array<int, 5>, 4> *sFlush,
                       std::array<int, 4> *sFlushSize)
{
    findFlush(handType, cards, sFlush, sFlushSize);
    if (handType->handIndex != FLUSH)
        return;
    sFlushSize->fill(0);
    int k;
    int cV0;
    int suit;
    for (int i = 13; i >= 3; i--)
    {
        k = i % 7;
        cV0 = c_cardValue[(*cards)[k]];
        suit = (int) c_cardSuit[(*cards)[k]];
        if ((*sFlushSize)[suit] == 0)
        {
            (*sFlush)[suit][0] = k;
            (*sFlushSize)[suit] += 1;
        }
        else if (c_cardValue[(*sFlush)[suit][(*sFlushSize)[suit] - 1]] - 1 == cV0 ||
                 c_cardValue[(*sFlush)[suit][(*sFlushSize)[suit] - 1]] + 12 == cV0)
        {
            (*sFlush)[suit][(*sFlushSize)[suit]] = k;
            (*sFlushSize)[suit] += 1;
            if ((*sFlushSize)[suit] == 5)
            {
                handType->handIndex = STRAIGHT_FLUSH;
                if (cV0 == 14)
                {
                    for (int j = 0; j < 4; j++)
                        handType->keyCardIndex[j + 1] = (*sFlush)[suit][j];
                    handType->keyCardIndex[0] = k;
                }
                else
                {
                    for (int j = 0; j < 5; j++)
                        handType->keyCardIndex[j] = (*sFlush)[suit][j];
                }
                return;
            }
        }
        else
        {
            (*sFlush)[suit][0] = k;
            (*sFlushSize)[suit] = 1;
        }
    }
    return;
}

void findRoyalFlush(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards,
                    std::array<std::array<int, 5>, 4> *sFlush,
                    std::array<int, 4> *sFlushSize)
{
    findStraightFlush(handType, cards, sFlush, sFlushSize);
    if (handType->handIndex == STRAIGHT_FLUSH &&
        c_cardValue[(*cards)[handType->keyCardIndex[0]]] == 14 &&
        c_cardValue[(*cards)[handType->keyCardIndex[4]]] == 10)
        handType->handIndex = ROYAL_FLUSH;
    return;
}

void findHandType(std::unique_ptr<HandType> &handType, std::array<int, 7> *cards,
                  std::array<std::array<int, 5>, 4> *sFlush,
                  std::array<int, 4> *sFlushSize,
                  std::array<int, 15> *straightMask)
{
    findRoyalFlush(handType, cards, sFlush, sFlushSize);
    if (handType->handIndex == ROYAL_FLUSH) {
        return;
    }
    findStraightFlush(handType, cards, sFlush, sFlushSize);
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
    findFlush(handType, cards, sFlush, sFlushSize);
    if (handType->handIndex == FLUSH) {
        return;
    }
    findStraight(handType, cards, straightMask);
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
    int hash = 0;
    for (int i = 0; i < 7; i++)
    {
        hash = (hash+(*cards)[i]+1) * 100;
    }
    return hash;
}

int getValue(std::array<int, 7> *pCards, std::array<int, 7> *eCards,
             std::unordered_map<int, std::unique_ptr<HandType>> *handTypeCache,
             std::array<std::array<int, 5>, 4> *sFlush,
             std::array<int, 4> *sFlushSize,
             std::array<int, 15> *straightMask)
{
    int pHash = hashGetValueArgs(pCards);
    int eHash = hashGetValueArgs(eCards);
    if (handTypeCache->find(pHash) == handTypeCache->end())
    {
        (*handTypeCache)[pHash] = std::unique_ptr<HandType> (new HandType);
        findHandType((*handTypeCache)[pHash], pCards, sFlush, sFlushSize, straightMask);
        int hash = ((*handTypeCache)[pHash]->handIndex) * 100;
        for (int i = 0; i < 5; i++)
        {
            hash = (hash+(*handTypeCache)[pHash]->keyCardIndex[i]+1) * 100;
        }
        (*handTypeCache)[pHash]->handValue = hash;
        /*
        for (int i = 0; i < 5; i++) {
            if ((*handTypeCache)[pHash]->keyCardIndex[i] < 0 ||
                (*handTypeCache)[pHash]->keyCardIndex[i] > 6) {
                printf("ERROR: %d,%d,%d,%d,%d,%d\n",
                       (*hadTypeCache)[pHash]->handIndex,
                       (*handTypeCache)[pHash]->keyCardIndex[0],
                       (*handTypeCache)[pHash]->keyCardIndex[1],
                       (*handTypeCache)[pHash]->keyCardIndex[2],
                       (*handTypeCache)[pHash]->keyCardIndex[3],
                       (*handTypeCache)[pHash]->keyCardIndex[4]
                );
            }
        }
        */
        (*handTypeCache)[pHash]->keyCardIndex[0] = (*pCards)[(*handTypeCache)[pHash]->keyCardIndex[0]];
        (*handTypeCache)[pHash]->keyCardIndex[1] = (*pCards)[(*handTypeCache)[pHash]->keyCardIndex[1]];
        (*handTypeCache)[pHash]->keyCardIndex[2] = (*pCards)[(*handTypeCache)[pHash]->keyCardIndex[2]];
        (*handTypeCache)[pHash]->keyCardIndex[3] = (*pCards)[(*handTypeCache)[pHash]->keyCardIndex[3]];
        (*handTypeCache)[pHash]->keyCardIndex[4] = (*pCards)[(*handTypeCache)[pHash]->keyCardIndex[4]];
    }
    if (handTypeCache->find(eHash) == handTypeCache->end())
    {
        (*handTypeCache)[eHash] = std::unique_ptr<HandType> (new HandType);
        findHandType((*handTypeCache)[eHash], eCards, sFlush, sFlushSize, straightMask);
        int hash = ((*handTypeCache)[eHash]->handIndex) * 100;
        for (int i = 0; i < 5; i++)
        {
            hash = (hash+(*handTypeCache)[eHash]->keyCardIndex[i]+1) * 100;
        }
        (*handTypeCache)[eHash]->handValue = hash;
        /*
        for (int i = 0; i < 5; i++) {
            if ((*handTypeCache)[eHash]->keyCardIndex[i] < 0 ||
                (*handTypeCache)[eHash]->keyCardIndex[i] > 6) {
                printf("ERROR: %d,%d,%d,%d,%d,%d\n",
                       (*handTypeCache)[eHash]->handIndex,
                       (*handTypeCache)[eHash]->keyCardIndex[0],
                       (*handTypeCache)[eHash]->keyCardIndex[1],
                       (*handTypeCache)[eHash]->keyCardIndex[2],
                       (*handTypeCache)[eHash]->keyCardIndex[3],
                       (*handTypeCache)[eHash]->keyCardIndex[4]
                );
            }
        }
        */
        (*handTypeCache)[eHash]->keyCardIndex[0] = (*eCards)[(*handTypeCache)[eHash]->keyCardIndex[0]];
        (*handTypeCache)[eHash]->keyCardIndex[1] = (*eCards)[(*handTypeCache)[eHash]->keyCardIndex[1]];
        (*handTypeCache)[eHash]->keyCardIndex[2] = (*eCards)[(*handTypeCache)[eHash]->keyCardIndex[2]];
        (*handTypeCache)[eHash]->keyCardIndex[3] = (*eCards)[(*handTypeCache)[eHash]->keyCardIndex[3]];
        (*handTypeCache)[eHash]->keyCardIndex[4] = (*eCards)[(*handTypeCache)[eHash]->keyCardIndex[4]];
    }
    if (*(*handTypeCache)[pHash] > *(*handTypeCache)[eHash])
    {
        return 2;
    }
    else if (*(*handTypeCache)[eHash] > *(*handTypeCache)[pHash])
    {
        return 0;
    }
    return 1;
}

struct ComputeVars
{
    int i=0,i0=0,i1=0,i2=0,i3=0,i4=0,i5=0,i6=0,i7=0,i8=0;
};

void compute4thRound(int tid, unsigned long start, unsigned long limit, std::string fileName)
{
    std::ofstream f;
    f.open(fileName);
    f << "hand0,hand1,cc0,cc1,cc2,cc3,cc4,b0,b1,b2,mean\n";
    //f << "hand0,hand1,cc0,cc1,cc2,cc3,cc4,handIndex,h0,h1,h2,h3,h4\n";
    int n = 52;
    int k = 2;
    int v=0, sum=0, numCases=0, pHash;
    unsigned long count = 0;
    int i=0, i0=0, i1=0, i2=0, i3=0, i4=0, i5=0, i6=0, i7=0, i8=0;
    float mean=0.0f;
    unsigned long int innerCount = 0;
    std::array<int, 3> buckets;
    std::unordered_map<int, std::unique_ptr<HandType>> handTypeCache;
    std::array<int, 5> cCards;
    std::array<int, 7> pCards;
    std::array<int, 7> eCards;
    std::array<std::array<int, 5>, 4> sFlush;
    std::array<int, 4> sFlushSize;
    std::array<int, 15> straightMask;
    for (i2 = 0; i2 < n - 5 + 1; i2++)
    for (i3 = i2 + 1; i3 < n - 5 + 2; i3++)
    for (i4 = i3 + 1; i4 < n - 5 + 3; i4++)
    for (i5 = i4 + 1; i5 < n - 5 + 4; i5++)
    for (i6 = i5 + 1; i6 < n - 5 + 5; i6++)
    {
        c_inDeck[i2] = false;
        c_inDeck[i3] = false;
        c_inDeck[i4] = false;
        c_inDeck[i5] = false;
        c_inDeck[i6] = false;
        for (i0 = 0; i0 < n - 2 + 1; i0++)
        if(c_inDeck[i0])
        for (i1 = i0 + 1; i1 < n - 2 + 2; i1++)
        if (c_inDeck[i1])
        {
            //printf("%d %d %d %d %d %d %d %d %d\n", i0, i1, i2, i3, i4, i5, i6, i7, i8);
            if (count < start) {
                count += 1;
                continue;
            }
            c_inDeck[i0] = false;
            c_inDeck[i1] = false;
            //printf("tid: %d, count: %d\n", tid, count);
            cCards = {i2, i3, i4, i5, i6};
            pCards = {i0, i1, i2, i3, i4, i5, i6};
            std::sort(pCards.begin(), pCards.end());
            count += 1;
            sum = 0;
            numCases = 0;
            buckets.fill(0);
            for (i7 = 0; i7 < n - 2 + 1; i7++)
            if (c_inDeck[i7])
            for (i8 = i7 + 1; i8 < n - 2 + 2; i8++)
            if (c_inDeck[i8])
            {
                eCards = {i7, i8, i2, i3, i4, i5, i6};
                std::sort(eCards.begin(), eCards.end());
                v = getValue(&pCards, &eCards, &handTypeCache,
                             &sFlush, &sFlushSize, &straightMask);
                buckets[v] += 1;
                numCases += 1;
                sum += v;
            }
            if (handTypeCache.size() > 1081) // 52 choose 2 -old 8 * (52 choose 2) * (50 choose 2) : ~1 GB mem
            {
                for (auto it = handTypeCache.begin(); it != handTypeCache.end(); it++)
                {
                    it->second.reset(nullptr);
                }
                handTypeCache.clear();
            }
            mean = (float) sum / (numCases * 2);
            f << i0 << ',' << i1 << ',' << i2 << ',' << i3 << ',' << i4 << ',' << i5 << ',' << i6;
            f << ',' << buckets[0] << ',' << buckets[1] << ',' << buckets[2] << ',' << mean << '\n';
            /*
            eCards = {i7, i8, i2, i3, i4, i5, i6};
            std::sort(eCards.begin(), eCards.end());
            v = getValue(&pCards, &eCards, &handTypeCache,
                         &sFlush, &sFlushSize, &straightMask);
            pHash = hashGetValueArgs(&pCards);
            f << i0 << ',' << i1 << ',' << i2 << ',' << i3 << ',' << i4 << ',' << i5 << ',' << i6;
            f << ',' << handTypeCache[pHash]->handIndex;
            f << ',' << handTypeCache[pHash]->keyCardIndex[0];
            f << ',' << handTypeCache[pHash]->keyCardIndex[1];
            f << ',' << handTypeCache[pHash]->keyCardIndex[2];
            f << ',' << handTypeCache[pHash]->keyCardIndex[3];
            f << ',' << handTypeCache[pHash]->keyCardIndex[4] << '\n';
            */
            if (limit > 0 && count >= limit)
            {
                printf("handTypeCache.size() = %d\n", handTypeCache.size());
                f.close();
                return;
            }
            c_inDeck[i0] = true;
            c_inDeck[i1] = true;
        }
        c_inDeck[i2] = true;
        c_inDeck[i3] = true;
        c_inDeck[i4] = true;
        c_inDeck[i5] = true;
        c_inDeck[i6] = true;
    }
    printf("handTypeCache.size() = %d\n", handTypeCache.size());
    f.close();
    return;
}

int main()
{
    unsigned long T;
    unsigned long numProcesses;
    printf("T:\n");
    std::cin >> T;
    printf("numProcesses:\n");
    std::cin >> numProcesses;
    //unsigned long int innerCount = compute4thRound(T, "4th_round_distributions.csv");

    if (T < 0)
    {
        T = 2809475760; // 52 choose 5 * 47 choose 2
    }

    unsigned long tPerThread = T / numProcesses;
    unsigned long tLast = 0;
    printf("T=%lu, tPerThread=%lu\n", T, tPerThread);

    int lastPID = 1;
    int parentPID = getpid();
    int pid;
    for (pid = 0; pid < numProcesses - 1; pid++)
    {
        lastPID = fork();
        printf("lastPID: %d\n");
        if (lastPID == 0)
        {
            std::stringstream fileName;
            fileName << std::setfill('0') << std::setw(3) << pid;
            printf("preparing to write to %s for values between %lu-%lu\n", fileName.str().c_str(), tLast, tLast+tPerThread);
            compute4thRound(pid, tLast, tLast + tPerThread,
                            "distributions/_" + fileName.str() + ".csv");
            break;
        }
        tLast += tPerThread;
    }
    if (lastPID > 0)
    {
        std::stringstream fileName;
        fileName << std::setfill('0') << std::setw(3) << pid;
            printf("preparing to write to %s for values between %lu-%lu\n", fileName.str().c_str(), tLast, T);
        compute4thRound(pid, tLast, T, "distributions/_" + fileName.str() + ".csv");
        while (numProcesses > 1) {
            int status;
            pid_t done = wait(&status);
            if (done == -1) {
                if (errno == ECHILD) break;
            } else {
                if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
                    std::cerr << "pid " << done << " failed\n";
                }
            }
        }
        printf("parent(%d) exited.\n", getpid());
    } else {
        printf("child(%d) exited.\n", getpid());
        return 0;
    }

    /*
    std::vector<std::thread> threads;
    for (int tid = 0; tid < numThreads; tid++)
    {
       threads.push_back(std::thread(compute4thRound, tid, tLast, tLast + tPerThread,
                                     "distributions/_" + std::to_string(tid) + ".csv"));
       tLast += tPerThread;
    }
    for (int tid = 0; tid < numThreads; tid++)
    {
        threads[tid].join();
        printf("Thread %d joined.\n", tid);
    }
    */
    printf("Done computing.\n");
    return 0;
}
