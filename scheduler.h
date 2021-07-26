//
// Created by sunday on 6/28/21.
//

#ifndef OSPCBQUEUE_SCHEDULER_H
#define OSPCBQUEUE_SCHEDULER_H

#include "PCB.h"
#include "PCBQueue.h"
#include <fstream>
#include <vector>

class scheduler{
private:
    //queues
    PCBQueue* ready = new PCBQueue[4];
    PCBQueue* waiting = new PCBQueue[4];
    PCBQueue reading;
    PCBQueue arrival;
    PCBQueue robin;
    //used to track time left on robin queues
    vector<int> burstTimesRemaining;
    int robinPos=-1;

    //RR q value
    int qValue =0;
    //time spent scheduling
    int time =0;
    //time left in current execution
    int timeLeft=0;
    //current PID that is being executed
    int currentPID=0;
    //total time spent waiting
    double totalWait=0;
    //number of PCBs, used for calculating average wait time
    double denominator =0;

public:
    scheduler() = default;

    //enqueues
    void readyUp(PCB&);
    void onWait(PCB&);
    void read(PCB&);
    void waitingToReady();
    void onRobin(PCB&);

    //removal
    void removeProcess(int);

    //prints
    void printReady();
    void printWaiting();
    void printScheduler();

    //management
    void parseFile(char*);
    void sortArrival();
    void executeNext();
    bool readyEmpty();
    bool robinEmpty();
    void executeRobin();

    //Algorithms
    double FCFS();
    double npPrio();
    double RR();


};

//puts PCB into ready queue
void scheduler::readyUp(PCB& pcb){
    int prio = pcb.getPriority();

    if(prio==1) ready[0].enqueue(pcb);
    else if(prio==2) ready[1].enqueue(pcb);
    else if(prio==3) ready[2].enqueue(pcb);
    else if(prio==4) ready[3].enqueue(pcb);
}

//puts PCB into waiting queue
void scheduler::onWait(PCB& pcb){
    int prio = pcb.getPriority();

    if(prio==1) waiting[0].enqueue(pcb);
    else if(prio==2) waiting[1].enqueue(pcb);
    else if(prio==3) waiting[2].enqueue(pcb);
    else if(prio==4) waiting[3].enqueue(pcb);
}

//inserts pcb into reading queue, for parsing
void scheduler::read(PCB& pcb){
    reading.enqueue(pcb);
}

//inserts PCB into Robin Queue
void scheduler::onRobin(PCB &pcb) {
    robin.enqueue(pcb);
}

//removes a process from any queue
void scheduler::removeProcess(int PID) {
    //check ready queues
    for(int i=0;i<4;i++)
        if(ready[i].deletePCB(PID)) return;

    //check waiting queues
    for(int i=0;i<4;i++)
        if(waiting[i].deletePCB(PID)) return;


}

//prints ready queue
void scheduler::printReady(){
    cout<<"Ready PCBS grouped by Priority"<<endl;
    for(int i=0;i<4;i++)
        ready[i].print();
}

//prints waiting queue
void scheduler::printWaiting(){
    cout<<"Waiting PCBS grouped by Priority"<<endl;
    for(int i=0;i<4;i++)
        waiting[i].print();
}

//prints all queues
void scheduler::printScheduler() {
    cout<<"CURRENT STATE OF QUEUES"<<endl;
    cout<<"------------------------"<<endl;
    printReady();
    cout<<"------------------------"<<endl;
    printWaiting();
    cout<<"------------------------"<<endl;
    cout<<"ARRIVAL QUEUE"<<endl;
    arrival.print();
}

//checks if ready queue is empty
bool scheduler::readyEmpty() {
    for(int i=0;i<4;i++)
        if(!ready[i].isEmpty())
            return false;

        return true;
}

//checks if robin queue is empty
bool scheduler::robinEmpty() {
    return robin.isEmpty();
}

//parses the file and files reading queue
void scheduler::parseFile(char * filename) {
    char buffer[80];
    ifstream input;
    input.open(filename);
    int temp[4];
    input.getline(buffer,80,'=');
    input.getline(buffer,80);
    qValue = atoi(buffer);
    //junk line
    input.getline(buffer,80);
    while(!input.eof()){
        input.getline(buffer,80,',');
        temp[0] = atoi(buffer);
        input.getline(buffer,80,',');
        temp[1] = atoi(buffer);
        input.getline(buffer,80,',');
        temp[2] = atoi(buffer);
        input.getline(buffer,80);
        temp[3] = atoi(buffer);

        PCB tempPCB(temp[0],temp[1],temp[2],temp[3]);
        read(tempPCB);
    }
    sortArrival();
}

//sorts the reading queue into arrival queue by arrival time
void scheduler::sortArrival(){
    while(!reading.isEmpty()){
        PCB next = reading.first();
        arrival.enqueue(next);
    }
}

//moves current waiting queue into ready queue
void scheduler::waitingToReady(){
    for(int i=0;i<4;i++)
        while(!waiting[i].isEmpty()){
            PCB shiftToReady = waiting[i].dequeue();
            readyUp(shiftToReady);
        }
}

//If there is nothing executing (timeLeft<=0) begins execution of next thing in ready queue with highest priority
void scheduler::executeNext() {
    if(timeLeft<=0){
        if(!ready[0].isEmpty()){
            PCB executing = ready[0].dequeue();
            timeLeft=executing.getBurst();
            totalWait+=time-executing.getArrival();
        }
        else if(!ready[1].isEmpty()){
            PCB executing = ready[1].dequeue();
            timeLeft=executing.getBurst();
            totalWait+=time-executing.getArrival();
        }
        else if(!ready[2].isEmpty()){
            PCB executing = ready[2].dequeue();
            timeLeft=executing.getBurst();
            totalWait+=time-executing.getArrival();
        }
        else if(!ready[3].isEmpty()){
            PCB executing = ready[3].dequeue();
            timeLeft=executing.getBurst();
            totalWait+=time-executing.getArrival();
        }
    }
}

//If quantum time is up, move onto next process and if process is finished, handle accordingly
void scheduler::executeRobin() {
    //checks if quantum time is up for current process
    if(timeLeft<=0){
        //reset qvalue
        timeLeft=qValue;
        //if this is not the first time we are doing this, meaning nothing has run yet
        if(robinPos>-1){
            //reduce burst time remaining for current PCB
            burstTimesRemaining[robinPos]-=2;
            //if that is done with burst time
            if(burstTimesRemaining[robinPos]<=0){
                //calculate time spent waiting, as well as remove PCB from all queues and bursttimeremaining
                PCB* completedPCB = robin.getPCB(currentPID);
                totalWait = time-completedPCB->getArrival()-completedPCB->getBurst();
                robin.deletePCB(currentPID);
                burstTimesRemaining.erase(burstTimesRemaining.begin() + robinPos);
                //reset robinpos if needed
                if(robinPos>burstTimesRemaining.size()-1)
                    robinPos=0;
                //as long as robin has more, reassign current PID to next one to run
                if(robin.size()!=0)
                    currentPID = robin[robinPos];
            }
            //if PCB not finished, but quantum time up.
            else{
                //go to next process
                robinPos++;
                if(robinPos>burstTimesRemaining.size()-1)
                    robinPos=0;
                currentPID = robin[robinPos];
            }
        }
        //first PCB to being execution
        else{
            robinPos++;
            //be safe in case no PCBs were added
            if(robinPos>burstTimesRemaining.size()-1)
                robinPos=0;
            currentPID = robin[robinPos];
        }
    }
}


//Scheduling Algorithms, returns a double with the average waiting time per PCB
//execute First Come First Serve scheduling (very simple, calculation uses arrival queue and manipulates inside the queue data structure)
double scheduler::FCFS(){
    return arrival.executeFCFS();
}

//execute Non-Preemptive Priority scheduling
double scheduler::npPrio(){
    //collect number of PCBs
    denominator = arrival.size();

    //loop while arrival or ready queue has pcbs
    while(!arrival.isEmpty()||!readyEmpty()){
        while(true) {
            //check to see if there are pcbs that have not arrived yet, then check if that time has come
            if (!arrival.isEmpty() && arrival.arrivalPeak() <= time) {
                //dequeue from waiting and put into ready if arrival time has come
                PCB shiftToWaiting = arrival.dequeue();
                onWait(shiftToWaiting);
                //break to see if there are more that arrived at this time
                break;
            }
            //shift everything in waiting queue to ready queue because they have arrived and are ready to execute
            waitingToReady();
            //subtract time from current execution
            timeLeft--;
            //check if ready to execute next, and if so begin execution
            executeNext();
            //add time
            time++;
            //repeat
            break;
        }
    }
    return totalWait/denominator;
}

//execute Round Robin scheduling
double scheduler::RR(){
    //collect number of PCBs
    denominator = arrival.size();

    //loop while arrival or robin have pcbs in queue
    while(!arrival.isEmpty()||!robinEmpty()){
        while(true) {
            //same logic as NPPrio, but using robin queue instead of ready
            if (!arrival.isEmpty() && arrival.arrivalPeak() <= time) {
                PCB shiftToRobin = arrival.dequeue();
                burstTimesRemaining.push_back(shiftToRobin.getBurst());
                onRobin(shiftToRobin);
                break;
            }
            timeLeft--;
            executeRobin();
            time++;
            break;
        }
    }
    return totalWait/denominator;
}


#endif //OSPCBQUEUE_SCHEDULER_H
