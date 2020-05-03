#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <array>
#include <algorithm>
#include <math.h>
#include "secondround.h"

/*  4TH ROUND FILE FORMAT
    hand0,hand1,cc0,cc1,cc2,cc3,cc4,b0,b1,b2,mean
    5,6,    0,1,2,3,4,    0,990,0,0.5
    5,7,    0,1,2,3,4,    0,990,0,0.5
    5,8,    0,1,2,3,4,    0,990,0,0.5
*/

SecondRoundGenerator::SecondRoundGenerator() {
    for (size_t i = 0; i < buckets.size(); i++) {
        buckets[i].fill(int8_t(0));
    }
}

SecondRoundGenerator::SecondRoundGenerator(int n) {}

size_t SecondRoundGenerator::getHash(int p0, int p1, int c0, int c1, int c2) {
    p0 += 1;
    p1 += 1;
    c0 += 1;
    c1 += 1;
    c2 += 1;
    if (c2 < p0) {
        p0 -= 3;
    } else if (c1 < p0) {
        p0 -= 2;
    } else if (c0 < p0) {
        p0 -= 1;
    }
    if (c2 < p1) {
        p1 -= 3;
    } else if (c1 < p1) {
        p1 -= 2;
    } else if (c0 < p1) {
        p1 -= 1;
    }
    int cHash, pHash;
    cHash = (((c0-156)*c0+8111)*c0-7956)/6+
            (c0-c1+1)*(c0+c1-104)/2-c1+c2-1;
    pHash = ((99-p0)*p0-98)/2-p0+p1-1;
    return size_t(pHash + 1176*cHash);
}

void SecondRoundGenerator::compute2ndRound(unsigned long start, unsigned long stop,
                                           std::string fileInName) {
    unsigned long count = 0;
    unsigned long printInterval = 2936325;
    size_t p0, p1, c0, c1, c2, c3, i;
    size_t h0, h1, h2, h3;
    unsigned int bucketVal;
    std::ifstream infile;
    std::string fileBuffer;
    infile.open(fileInName);
    for (c0 = 0; c0 < 49; c0++)
    for (c1 = c0 + 1; c1 < 50; c1++)
    for (c2 = c1 + 1; c2 < 51; c2++)
    for (c3 = c2 + 1; c3 < 52; c3++) {
        c_inDeck[c0] = false;
        c_inDeck[c1] = false;
        c_inDeck[c2] = false;
        c_inDeck[c3] = false;
        for (p0 = 0; p0 < 51; p0++)
        if(c_inDeck[p0])
        for (p1 = p0 + 1; p1 < 52; p1++)
        if (c_inDeck[p1]) {
            if (count < start) {
                count += 1;
                continue;
            }
            std::getline(infile, fileBuffer);
            std::istringstream iss(fileBuffer);
            h0 = getHash(p0,p1,c0,c1,c2);
            h1 = getHash(p0,p1,c0,c1,c3);
            h2 = getHash(p0,p1,c0,c2,c3);
            h3 = getHash(p0,p1,c1,c2,c3);
            if (h0==0||h1==0||h2==0||h3==0) {
                printf("%lu: (%zu,%zu) (%zu,%zu,%zu,%zu) [%zu,%zu,%zu,%zu]\nbuckets:[",
                       count,p0,p1,c0,c1,c2,c3,h0,h1,h2,h3);
            }
            for (i = 0; i < c_numBuckets; i++) {
                iss >> bucketVal;
                buckets[h0][i] += bucketVal;
                buckets[h1][i] += bucketVal;
                buckets[h2][i] += bucketVal;
                buckets[h3][i] += bucketVal;
                if (h0==0||h1==0||h2==0||h3==0)
                    printf("%u ", bucketVal);
            }
            if (h0==0||h1==0||h2==0||h3==0)
                printf("]\n");
            if (count % printInterval == 0) {
                printf("(%zu,%zu) (%zu,%zu,%zu,%zu), [%zu,%zu,%zu,%zu]\n",
                       p0,p1,c0,c1,c2,c3,h0,h1,h2,h3
                );
            }
            count += 1;
            if (count >= stop) {
                printf("Processed %lu-%lu of %s.\n", start, stop, fileInName.c_str());
                infile.close();
                return;
            }
        }
        c_inDeck[c0] = true;
        c_inDeck[c1] = true;
        c_inDeck[c2] = true;
        c_inDeck[c3] = true;
    }
    printf("Processed %lu-%lu of %s.\n", start, stop, fileInName.c_str());
    infile.close();
    return;
}

void SecondRoundGenerator::saveBuckets(size_t start, size_t stop, std::string fileOutName) {
    std::ofstream f;
    f.open(fileOutName);
    for (size_t i = start; i < stop; i++) {
        for (size_t j = 0; j < 49; j++) {
            f << int(buckets[i][j]) << ' ';
        }
        f << int(buckets[i][49]) << '\n';
    }
    f.close();
}