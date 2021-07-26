cmake_minimum_required(VERSION 3.17)
project(OSPCBQUEUE)

set(CMAKE_CXX_STANDARD 14)

add_executable(OSPCBQUEUE main.cpp LinkedList.h PCB.h PCBQueue.h scheduler.h)