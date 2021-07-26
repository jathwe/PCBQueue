#include <iostream>
#include "PCB.h"
#include "PCBQueue.h"
#include "scheduler.h"

using namespace std;

//This is the work of Jake Weems for the Class CS7343 at Southern Methodist University

int main() {
    
    //First Come First Serve Scheduling
    scheduler FCFS;
    FCFS.parseFile("../pcbs.txt");
    double AvgWait = FCFS.FCFS();
    cout << "FCFS Average Wait Time:" <<AvgWait<<endl;

    //Non-Preemptive Scheduling
    scheduler npPrio;
    npPrio.parseFile("../pcbs.txt");
    AvgWait = npPrio.npPrio();
    cout << "Non-Preemptive Average Wait Time:" <<AvgWait<<endl;

    //Round Robin Scheduling
    scheduler RR;
    RR.parseFile("../pcbs.txt");
    AvgWait = RR.RR();
    cout << "Round Robin Average Wait Time:" <<AvgWait<<endl;

}
