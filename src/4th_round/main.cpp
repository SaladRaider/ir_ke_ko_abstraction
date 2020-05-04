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
#include "fourthround.h"

int main()
{
    unsigned long T;
    unsigned long numProcesses;
    printf("T:\n");
    std::cin >> T;
    printf("numProcesses:\n");
    std::cin >> numProcesses;
    //unsigned long int innerCount = compute4thRound(T, "4th_round_distributions.csv");

    if (T > 2809475760)
    {
        T = 2809475760; // 52 choose 5 * 47 choose 2
    }

    unsigned long tPerThread = T / numProcesses;
    unsigned long tLast = 0;
    printf("T=%lu, tPerThread=%lu\n", T, tPerThread);
    int lastPID = 1;
    int parentPID = getpid();
    int pid;

    FourthRoundGenerator gen;

    for (pid = 0; pid < numProcesses - 1; pid++)
    {
        lastPID = fork();
        printf("lastPID: %d\n");
        if (lastPID == 0)
        {
            std::stringstream fileName;
            fileName << std::setfill('0') << std::setw(3) << pid;
            printf("preparing to write to %s for values between %lu-%lu\n", fileName.str().c_str(), tLast, tLast+tPerThread);
            gen.compute4thRound(pid, tLast, tLast + tPerThread,
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
        gen.compute4thRound(pid, tLast, T, "distributions/_" + fileName.str() + ".csv");
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
