#ifndef __THIRDROUND_H_INCLUDED_
#define __THIRDROUND_H_INCLUDED_

#include <array>
#include <string>
#include <unistd.h>

class ThirdRoundGenerator {
private:
    const size_t c_numBuckets = 50;
    std::array<std::array<std::int8_t, 50>, 305377800> buckets;
    std::array<std::array<unsigned int, 50>, 25989600> buckets2nd;
    std::array<std::array<unsigned int, 50>, 1326> buckets1st;
public:
    std::array<bool, 52> c_inDeck = {
        true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true
    };
    ThirdRoundGenerator();
    ThirdRoundGenerator(int n);
    size_t getHash(int p0, int p1, int c0, int c1, int c2, int c3);
    size_t getHash2nd(int p0, int p1, int c0, int c1, int c2);
    size_t getHash1st(int p0, int p1);
    void compute3rdRound(unsigned long start, unsigned long stop, std::string fileInName);
    void compute2ndRound(unsigned long start, unsigned long stop);
    void compute1stRound(unsigned long start, unsigned long stop);
    void saveBuckets(size_t start, size_t stop, std::string fileOutName);
    void saveBuckets2nd(size_t start, size_t stop, std::string fileOutName);
    void saveBuckets1st(size_t start, size_t stop, std::string fileOutName);
};

#endif // __THIRDROUND_H_INCLUDED_
