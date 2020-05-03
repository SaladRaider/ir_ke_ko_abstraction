#ifndef __SECONDROUND_H_INCLUDED_
#define __SECONDROUND_H_INCLUDED_

#include <array>
#include <string>
#include <unistd.h>

class SecondRoundGenerator {
private:
    const size_t c_numBuckets = 50;
std::array<std::array<unsigned int, 50>, 25989600> buckets;
public:
    std::array<bool, 52> c_inDeck = {
        true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true
    };
    SecondRoundGenerator();
    SecondRoundGenerator(int n);
    size_t getHash(int p0, int p1, int c0, int c1, int c2);
    void compute2ndRound(unsigned long start, unsigned long stop, std::string fileInName);
    void saveBuckets(size_t start, size_t stop, std::string fileOutName);
};

#endif // __SECONDROUND_H_INCLUDED_
