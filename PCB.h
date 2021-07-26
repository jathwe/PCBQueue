//
// Created by sunday on 6/28/21.
//

#ifndef OSPCBQUEUE_PCB_H
#define OSPCBQUEUE_PCB_H

#include <iostream>

using namespace std;

class PCB {
private:
    int process_id;
    int arrival_time;
    int burst_time;
    int priority;

public:
    //constructor
    PCB(int id, int arrival, int burst, int prio){
        process_id = id;
        arrival_time = arrival;
        burst_time = burst;
        priority = prio;
    }

    //returns PID
    int getPID() const{
        return process_id;
    }

    //returns Arrival Time
    int getArrival() const{
        return arrival_time;
    }

    //returns Burst Time
    int getBurst() const{
        return burst_time;
    }

    //returns Priority ranking
    int getPriority() const{
        return priority;
    }

    //Used for comparisons of PIDs, for searching.
    bool operator==(const PCB& comp) const{
        return this->process_id == comp.process_id;
    }

    //operator overloaded to easily print a PCB
    friend ostream& operator<<(ostream& os, const PCB& pcb){
        os<<pcb.process_id<<", "<<pcb.arrival_time<<", "<<pcb.burst_time<<", "<<pcb.priority<<endl;
        return os;
    }
};


#endif //OSPCBQUEUE_PCB_H
