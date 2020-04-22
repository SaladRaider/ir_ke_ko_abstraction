#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <array>
#include <algorithm>
#include <math.h>


/*  4TH ROUND FILE FORMAT
    hand0,hand1,cc0,cc1,cc2,cc3,cc4,b0,b1,b2,mean
    5,6,    0,1,2,3,4,    0,990,0,0.5
    5,7,    0,1,2,3,4,    0,990,0,0.5
    5,8,    0,1,2,3,4,    0,990,0,0.5
*/

std::array<bool, 52> c_inDeck = {
    true, true, true, true, true, true, true, true, true, true, true, true, true,
    true, true, true, true, true, true, true, true, true, true, true, true, true,
    true, true, true, true, true, true, true, true, true, true, true, true, true,
    true, true, true, true, true, true, true, true, true, true, true, true, true
};

size_t getHash(int p0, int p1) {
    p0 += 1;
    p1 += 1;
    size_t pHash = size_t(((105-p0)*p0-106)/2-p0+p1-1);
    return pHash;
}

const size_t c_numBuckets = 50;
std::array<std::array<unsigned int, c_numBuckets>, 1326> buckets;

void compute1stRound(unsigned long start, unsigned long stop, std::string fileInName) {
    unsigned long count = 0;
    size_t p0, p1, c0, c1, c2, i;
    unsigned int bucketVal;
    unsigned long printInterval = 249900;
    std::ifstream infile;
    std::string fileBuffer;
    infile.open(fileInName);
    for (c0 = 0; c0 < 50; c0++)
    for (c1 = c0 + 1; c1 < 51; c1++)
    for (c2 = c1 + 1; c2 < 52; c2++) {
        c_inDeck[c0] = false;
        c_inDeck[c1] = false;
        c_inDeck[c2] = false;
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
            for (i = 0; i < c_numBuckets; i++) {
                iss >> bucketVal;
                buckets[getHash(p0, p1)][i] += bucketVal;
            }
            if (count % printInterval == 0) {
                printf("(%zu,%zu) (%zu,%zu,%zu), [%zu]\n",
                       p0,p1,c0,c1,c2, getHash(p0, p1)
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
    }
    printf("Processed %lu-%lu of %s.\n", start, stop, fileInName.c_str());
    infile.close();
    return;
}

void saveBuckets(size_t start, size_t stop, std::string fileOutName) {
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

int main() {
    unsigned long T;
    size_t numProcesses;
    printf("numProcesses:\n");
    std::cin >> numProcesses;

    T = 25989600; // 52 choose 3 * 49 choose 2

    unsigned long tPerThread = T / numProcesses;
    unsigned long tLast = 0;
    printf("T=%lu, tPerThread=%lu\n", T, tPerThread);
    printf("Starting computation...\n");

    for (size_t i = 0; i < buckets.size(); i++) {
        buckets[i].fill(int8_t(0));
    }

    size_t pid;
    for (pid = 0; pid < numProcesses - 1; pid++)
    {
        std::stringstream fileInName;
        fileInName << std::setfill('0') << std::setw(3) << pid;
        printf("preparing to read from %s for values between %lu-%lu\n",
                fileInName.str().c_str(), tLast, tLast+tPerThread);
        compute1stRound(tLast, tLast + tPerThread, "distributions_2nd/_" + fileInName.str() + ".csv");
        tLast += tPerThread;
    }
    std::stringstream fileInName;
    fileInName << std::setfill('0') << std::setw(3) << pid;
        printf("preparing to read from %s for values between %lu-%lu\n", fileInName.str().c_str(), tLast, T);
    compute1stRound(tLast, T, "distributions_2nd/_" + fileInName.str() + ".csv");

    printf("preparing to write to all.csv for values between %lu-%lu\n", 0, 1326);
    saveBuckets(0, 1326, "distributions_1st/all.csv");
    return 0;
}
