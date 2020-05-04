#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "thirdround.h"

int main() {
    unsigned long T3, T2, T1;
    size_t numProcesses;
    printf("numProcesses:\n");
    std::cin >> numProcesses;

    T3 = 2809475760;    // 52 choose 5 * 47 choose 2
    T2 = 305377800;     // 52 choose 4 * 48 choose 2
    T1 = 25989600;      // 52 choose 3 * 49 choose 2

    unsigned long tPerThread3 = T3 / numProcesses;
    unsigned long tLast3 = 0;
    unsigned long tPerThread2 = T2 / numProcesses;
    unsigned long tLast2 = 0;
    unsigned long tPerThread1 = T1 / numProcesses;
    unsigned long tLast1 = 0;
    printf("T=%lu, tPerThread=%lu\n", T3, tPerThread3);
    printf("Starting computation...\n");

    size_t pid = 0;
    printf("preparing to read from all for values between %lu-%lu\n", tLast3, T3);
    ThirdRoundGenerator *gen = new ThirdRoundGenerator();
    gen->compute3rdRound(0, T3, "distributions_4th_upstream/all.csv");
    gen->compute2ndRound(0, T2);
    gen->compute1stRound(0, T1);

    int lastPID = 1;
    pid = 0;
    T3 = T2;
    T2 = T1;
    T1 = 1326;
    tPerThread3 = T3 / numProcesses;
    tLast3 = 0;
    tPerThread2 = T2 / numProcesses;
    tLast2 = 0;
    tPerThread1 = T1 / numProcesses;
    tLast1 = 0;
 
    for (pid = 0; pid < numProcesses - 1; pid++)
    {
        lastPID = fork();
        printf("lastPID: %d\n", lastPID);
        if (lastPID == 0)
        {
            std::stringstream fileOutName;
            fileOutName << std::setfill('0') << std::setw(3) << pid;
            printf("preparing to write to %s for values between %lu-%lu\n", fileOutName.str().c_str(), tLast3, tLast3+tPerThread3);
            gen->saveBuckets(tLast3, tLast3 + tPerThread3, "distributions_3rd/_" + fileOutName.str() + ".csv");
            printf("preparing to write to %s for values between %lu-%lu\n", fileOutName.str().c_str(), tLast2, tLast2+tPerThread2);
            gen->saveBuckets2nd(tLast2, tLast2 + tPerThread2, "distributions_2nd/_" + fileOutName.str() + ".csv");
            printf("preparing to write to %s for values between %lu-%lu\n", fileOutName.str().c_str(), tLast1, tLast1+tPerThread1);
            gen->saveBuckets1st(tLast1, tLast1 + tPerThread1, "distributions_1st/_" + fileOutName.str() + ".csv");
            break;
        }
        tLast3 += tPerThread3;
        tLast2 += tPerThread2;
        tLast1 += tPerThread1;
    }
    if (lastPID > 0)
    {
        std::stringstream fileOutName;
        fileOutName << std::setfill('0') << std::setw(3) << pid;
        printf("preparing to write to %s for values between %lu-%lu\n", fileOutName.str().c_str(), tLast3, T3);
        gen->saveBuckets(tLast3, T3, "distributions_3rd/_" + fileOutName.str() + ".csv");
        printf("preparing to write to %s for values between %lu-%lu\n", fileOutName.str().c_str(), tLast2, T2);
        gen->saveBuckets2nd(tLast2, T2, "distributions_2nd/_" + fileOutName.str() + ".csv");
        printf("preparing to write to %s for values between %lu-%lu\n", fileOutName.str().c_str(), tLast1, T1);
        gen->saveBuckets1st(tLast1, T1, "distributions_1st/_" + fileOutName.str() + ".csv");
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
    delete gen;
    return 0;
}
