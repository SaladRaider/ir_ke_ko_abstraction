#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <array>
#include <algorithm>
#include <math.h>
#include "thirdround.h"

/*  4TH ROUND FILE FORMAT
    hand0,hand1,cc0,cc1,cc2,cc3,cc4,b0,b1,b2,mean
    5,6,    0,1,2,3,4,    0,990,0,0.5
    5,7,    0,1,2,3,4,    0,990,0,0.5
    5,8,    0,1,2,3,4,    0,990,0,0.5
*/

ThirdRoundGenerator::ThirdRoundGenerator() {
    for (size_t i = 0; i < buckets.size(); i++) {
        buckets[i].fill(int8_t(0));
    }
}

ThirdRoundGenerator::ThirdRoundGenerator(int n) {}

size_t ThirdRoundGenerator::getHash(int p0, int p1, int c0,
                                    int c1, int c2, int c3) {
    p0 += 1;
    p1 += 1;
    c0 += 1;
    c1 += 1;
    c2 += 1;
    c3 += 1;
    if (c3 < p0) {
        p0 -= 4;
    } else if (c2 < p0) {
        p0 -= 3;
    } else if (c1 < p0) {
        p0 -= 2;
    } else if (c0 < p0) {
        p0 -= 1;
    }
    if (c3 < p1) {
        p1 -= 4;
    } else if (c2 < p1) {
        p1 -= 3;
    } else if (c1 < p1) {
        p1 -= 2;
    } else if (c0 < p1) {
        p1 -= 1;
    }
    int cHash, pHash;
    cHash =((((206-c0)*c0-15911)*c0+546106)*c0-530400)/24 -
           (c0-c1+1)*((c0+c1-154)*c0+(c1-155)*c1+7956)/6 +
           (c1-c2+1)*(c1+c2-104)/2-c2+c3-1;
    pHash = ((97-p0)*p0-96)/2-p0+p1-1;
    return size_t(pHash + 1128*cHash);
}

void ThirdRoundGenerator::compute3rdRound(unsigned long start,
                                          unsigned long stop,
                                          std::string fileInName) {
    unsigned long count = 0;
    unsigned long printInterval = 27014190;
    size_t p0, p1, c0, c1, c2, c3, c4, bucketIdx;
    size_t h0, h1, h2, h3, h4;
    size_t h = 2256;
    float mean;
    std::ifstream infile;
    std::string fileBuffer;
    infile.open(fileInName);
    for (c0 = 0; c0 < 48; c0++)
    for (c1 = c0 + 1; c1 < 49; c1++)
    for (c2 = c1 + 1; c2 < 50; c2++)
    for (c3 = c2 + 1; c3 < 51; c3++)
    for (c4 = c3 + 1; c4 < 52; c4++) {
        c_inDeck[c0] = false;
        c_inDeck[c1] = false;
        c_inDeck[c2] = false;
        c_inDeck[c3] = false;
        c_inDeck[c4] = false;
        for (p0 = 0; p0 < 51; p0++)
        if(c_inDeck[p0])
        for (p1 = p0 + 1; p1 < 52; p1++)
        if (c_inDeck[p1]) {
            if (count < start) {
                count += 1;
                printf("skipping...\n");
                continue;
            }
            std::getline(infile, fileBuffer);
            std::istringstream iss(fileBuffer);
            iss >> mean;
            bucketIdx = size_t(floor(mean * 50));
            if (bucketIdx == 50)
                bucketIdx = 49;
            h0 = getHash(p0,p1,c0,c1,c2,c3);
            h1 = getHash(p0,p1,c0,c1,c2,c4);
            h2 = getHash(p0,p1,c0,c1,c3,c4);
            h3 = getHash(p0,p1,c0,c2,c3,c4);
            h4 = getHash(p0,p1,c1,c2,c3,c4);
            buckets[h0][bucketIdx] += int8_t(1);
            buckets[h1][bucketIdx] += int8_t(1);
            buckets[h2][bucketIdx] += int8_t(1);
            buckets[h3][bucketIdx] += int8_t(1);
            buckets[h4][bucketIdx] += int8_t(1);
            if (h0==h||h1==h||h2==h||h3==h||h4==h) {
                printf("%lu: (%zu,%zu) (%zu,%zu,%zu,%zu,%zu) [%zu,%zu,%zu,%zu,%zu] [%f -> %zu]\n",
                        count,p0,p1,c0,c1,c2,c3,c4,h0,h1,h2,h3,h4,mean,bucketIdx);
            }
            if (count % printInterval == 0) {
                printf("(%zu,%zu) (%zu,%zu,%zu,%zu,%zu), [%zu,%zu,%zu,%zu,%zu] [%s -> %f -> %zu]\n",
                       p0,p1,c0,c1,c2,c3,c4,
                       getHash(p0,p1,c0,c1,c2,c3),
                       getHash(p0,p1,c0,c1,c2,c4),
                       getHash(p0,p1,c0,c1,c3,c4),
                       getHash(p0,p1,c0,c2,c3,c4),
                       getHash(p0,p1,c1,c2,c3,c4),
                       fileBuffer.c_str(), mean, bucketIdx
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
        c_inDeck[c4] = true;
    }
    printf("Processed %lu-%lu of %s.\n", start, stop, fileInName.c_str());
    infile.close();
    return;
}

void ThirdRoundGenerator::saveBuckets(size_t start, size_t stop,
                                      std::string fileOutName) {
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
