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
    unsigned long T;
    size_t numProcesses;
    printf("numProcesses:\n");
    std::cin >> numProcesses;

    //printf("%lu\n", getHash(46,47,48,49,50,51));
    //return 0;
    T = 2809475760; // 52 choose 5 * 47 choose 2

    unsigned long tPerThread = T / numProcesses;
    unsigned long tLast = 0;
    printf("T=%lu, tPerThread=%lu\n", T, tPerThread);
    printf("Starting computation...\n");

    size_t pid = 0;
    /*for (pid = 0; pid < numProcesses - 1; pid++)
    {
        std::stringstream fileInName;
        fileInName << std::setfill('0') << std::setw(3) << pid;
        printf("preparing to read from %s for values between %lu-%lu\n",
                fileInName.str().c_str(), tLast, tLast+tPerThread);
        compute3rdRound(tLast, tLast + tPerThread, "distributions_4th_upstream/_" + fileInName.str() + ".csv");
        tLast += tPerThread;
    }*/
    //tLast = 2721679627;
    //T = 2809475760;
    //pid = 31;
    printf("preparing to read from all for values between %lu-%lu\n", tLast, T);
    //compute3rdRound(tLast, T, "distributions_4th_upstream/_" + fileInName.str() + ".csv");
    ThirdRoundGenerator gen;
    gen.compute3rdRound(0, T, "distributions_4th_upstream/all.csv");

    int lastPID = 1;
    pid = 0;
    T = 305377800;
    tPerThread = T / numProcesses;
    tLast = 0;
    //tLast = 305377799;
    //pid = 32;
 
    for (pid = 0; pid < numProcesses - 1; pid++)
    {
        lastPID = fork();
        printf("lastPID: %d\n", lastPID);
        if (lastPID == 0)
        {
            std::stringstream fileOutName;
            fileOutName << std::setfill('0') << std::setw(3) << pid;
            printf("preparing to write to %s for values between %lu-%lu\n", fileOutName.str().c_str(), tLast, tLast+tPerThread);
            gen.saveBuckets(tLast, tLast + tPerThread, "distributions_3rd/_" + fileOutName.str() + ".csv");
            break;
        }
        tLast += tPerThread;
    }
    if (lastPID > 0)
    {
        std::stringstream fileOutName;
        fileOutName << std::setfill('0') << std::setw(3) << pid;
            printf("preparing to write to %s for values between %lu-%lu\n", fileOutName.str().c_str(), tLast, T);
        gen.saveBuckets(tLast, T, "distributions_3rd/_" + fileOutName.str() + ".csv");
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
