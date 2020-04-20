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

size_t getHash(size_t p0, size_t p1, size_t c0,
               size_t c1, size_t c2, size_t c3) {
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
    size_t cHash, pHash;
    cHash =((((206-c0)*c0-15911)*c0+546106)*c0-530400)/24 -
           (c0-c1+1)*((c0+c1-154)*c0+(c1-155)*c1+7956)/6 +
           (c1-c2+1)*(c1+c2-104)/2-c2+c3-1;
    pHash = ((97-p0)*p0-96)/2-p0+p1-1;
    return pHash + 1128*cHash;
}

const size_t c_numBuckets = 50;
std::array<std::array<std::int8_t, c_numBuckets>, 305377800> buckets;

void compute3rdRound(unsigned long start, unsigned long stop, std::string fileInName) {
    unsigned long count = 0;
    size_t p0, p1, c0, c1, c2, c3, c4, bucketIdx;
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
                continue;
            }
            std::getline(infile, fileBuffer);
            std::istringstream iss(fileBuffer);
            iss >> mean;
            bucketIdx = size_t(floor(mean * 50));
            if (bucketIdx == 50)
                bucketIdx = 49;
            buckets[getHash(p0, p1, c0, c1, c2, c3)][bucketIdx] += 1;
            buckets[getHash(p0, p1, c0, c1, c2, c4)][bucketIdx] += 1;
            buckets[getHash(p0, p1, c0, c1, c3, c4)][bucketIdx] += 1;
            buckets[getHash(p0, p1, c0, c2, c3, c4)][bucketIdx] += 1;
            buckets[getHash(p0, p1, c1, c2, c3, c4)][bucketIdx] += 1;
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

void saveBuckets(size_t start, size_t stop, std::string fileOutName) {
    std::ofstream f;
    f.open(fileOutName);
    for (size_t i = start; i < stop; i++) {
        for (size_t j = 0; j < 49; j++) {
            f << buckets[i][j] << ' ';
        }
        f << buckets[i][49] << '\n';
    }
    f.close();
}

int main() {
    unsigned long T;
    size_t numProcesses;
    printf("T:\n");
    std::cin >> T;
    printf("numProcesses:\n");
    std::cin >> numProcesses;

    if (T < 0)
    {
        T = 2809475760; // 52 choose 5 * 47 choose 2
    }

    unsigned long tPerThread = T / numProcesses;
    unsigned long tLast = 0;
    printf("T=%lu, tPerThread=%lu\n", T, tPerThread);
    printf("Starting computation...\n");

    size_t pid;
    for (pid = 0; pid < numProcesses - 1; pid++)
    {
        std::stringstream fileInName;
        fileInName << std::setfill('0') << std::setw(3) << pid;
        printf("preparing to read from %s for values between %lu-%lu\n",
                fileInName.str().c_str(), tLast, tLast+tPerThread);
        compute3rdRound(tLast, tLast + tPerThread, "distributions/_" + fileInName.str() + ".csv");
    }
    std::stringstream fileInName;
    fileInName << std::setfill('0') << std::setw(3) << pid;
        printf("preparing to read from %s for values between %lu-%lu\n", fileInName.str().c_str(), tLast, T);
    compute3rdRound(tLast, T, "distributions/_" + fileInName.str() + ".csv");

    int lastPID = 1;
    pid = 0;
    T = 305377800;
    tPerThread = T / numProcesses;
    tLast = 0;
    for (pid = 0; pid < numProcesses - 1; pid++)
    {
        lastPID = fork();
        printf("lastPID: %d\n", lastPID);
        if (lastPID == 0)
        {
            std::stringstream fileOutName;
            fileOutName << std::setfill('0') << std::setw(3) << pid;
            printf("preparing to write to %s for values between %lu-%lu\n", fileOutName.str().c_str(), tLast, tLast+tPerThread);
            saveBuckets(tLast, tLast + tPerThread, "distributions_3rd/_" + fileOutName.str() + ".csv");
            break;
        }
        tLast += tPerThread;
    }
    if (lastPID > 0)
    {
        std::stringstream fileOutName;
        fileOutName << std::setfill('0') << std::setw(3) << pid;
            printf("preparing to write to %s for values between %lu-%lu\n", fileOutName.str().c_str(), tLast, T);
        saveBuckets(tLast, T, "distributions_3rd/_" + fileOutName.str() + ".csv");
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
    return 0;
}
