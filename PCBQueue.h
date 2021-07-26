//
// Created by sunday on 6/28/21.
//

#ifndef OSPCBQUEUE_PCBQUEUE_H
#define OSPCBQUEUE_PCBQUEUE_H

#include "LinkedList.h"
#include "PCB.h"

class PCBQueue{
private:
    LinkedList<PCB> queue;

public:
    //default constructor
    PCBQueue() = default;

    //pushes PCB to back of queue
    void enqueue(PCB& newPCB){
        queue.push_back(newPCB);
    }

    //pops front PCB from queue
    PCB dequeue(){
        PCB dequeued = queue.pop_front();
        return dequeued;
    }

    //peaks arrival time of next in queue
    int arrivalPeak(){
        return queue.front().getArrival();
    }

    //returns PCB that matches PID
    PCB* getPCB(int PID){
        PCB search(PID,0,0,0);
        return queue.getElement(search);
    }

    //removes PCB from queue by PID
    bool deletePCB(int PID){
        PCB search(PID,0,0,0);
        return queue.delete_element(search);
    }

    //prints the queue
    void print(){
        cout<<queue<<endl;
    }

    //checks if queue is empty
    bool isEmpty(){
        return queue.size()==0;
    }

    //used for sorting by arrival time.
    PCB first(){
        int lowest;
        int key = queue[0].getArrival();
        for(int i=0;i<queue.size();i++){
            int temp = queue[i].getArrival();
            if (temp<=key){
                lowest=i;
                key = temp;
            }
        }
        PCB ret = queue[lowest];
        bool rem = deletePCB(ret.getPID());
        return ret;
    }

    //FCFS execution algorithm
    //This algorithm was very simple and was best handled adding a single function to the PCBQueue data structure, might not be best practice but was easiest and created less functions needed overall
    double executeFCFS(){
        //set variables
        int time = 0;
        double totalWait = 0;
        double denominator = queue.size();

        //using math the results are easy to calculate based on arrival time and burst time going linerally through the queue.
        time += queue[0].getBurst();
        for(int i=1;i<denominator;i++){
            PCB temp = queue[i];
            int arriveTime = temp.getArrival();
            if(arriveTime>time)
                time = arriveTime;
            totalWait+=time-arriveTime;
            time+=temp.getBurst();
        }
        return (totalWait/denominator);
    }

    //returns size of queue
    int size(){
        return queue.size();
    }

    int operator[](int a) const{
        PCB temp = queue[a];
        return temp.getPID();
    }
};

#endif //OSPCBQUEUE_PCBQUEUE_H
