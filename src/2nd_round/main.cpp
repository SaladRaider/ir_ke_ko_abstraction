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
#include "secondround.h"

int main() {
    unsigned long T;
    size_t numProcesses;
    printf("numProcesses:\n");
    std::cin >> numProcesses;

    T = 305377800; // 52 choose 4 * 48 choose 2

    unsigned long tPerThread = T / numProcesses;
    unsigned long tLast = 0;
    printf("T=%lu, tPerThread=%lu\n", T, tPerThread);
    printf("Starting computation...\n");

    SecondRoundGenerator gen;

    size_t pid;
    for (pid = 0; pid < numProcesses - 1; pid++)
    {
        std::stringstream fileInName;
        fileInName << std::setfill('0') << std::setw(3) << pid;
        printf("preparing to read from %s for values between %lu-%lu\n",
                fileInName.str().c_str(), tLast, tLast+tPerThread);
        gen.compute2ndRound(tLast, tLast + tPerThread, "distributions_3rd/_" + fileInName.str() + ".csv");
        tLast += tPerThread;
    }
    std::stringstream fileInName;
    fileInName << std::setfill('0') << std::setw(3) << pid;
        printf("preparing to read from %s for values between %lu-%lu\n", fileInName.str().c_str(), tLast, T);
    gen.compute2ndRound(tLast, T, "distributions_3rd/_" + fileInName.str() + ".csv");

    int lastPID = 1;
    pid = 0;
    T = 25989600; // 52 choose 3 * 49 choose 2
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
            gen.saveBuckets(tLast, tLast + tPerThread, "distributions_2nd/_" + fileOutName.str() + ".csv");
            break;
        }
        tLast += tPerThread;
    }
    if (lastPID > 0)
    {
        std::stringstream fileOutName;
        fileOutName << std::setfill('0') << std::setw(3) << pid;
            printf("preparing to write to %s for values between %lu-%lu\n", fileOutName.str().c_str(), tLast, T);
        gen.saveBuckets(tLast, T, "distributions_2nd/_" + fileOutName.str() + ".csv");
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
