//
//  ECSimTaskScheduler2.cpp
//  
//
//

#include <vector>
#include <iostream>
using namespace std;

#include "ECSimTaskScheduler2.h"
#include "ECSimTask.h"
using namespace std;

ECSimLWTFTaskScheduler::ECSimLWTFTaskScheduler(){

}

ECSimTask *ECSimLWTFTaskScheduler::ChooseTaskToSchedule(const vector<ECSimTask *> &listReadyTasks) const{
    if (listReadyTasks.empty()){
        return nullptr;
    }

    ECSimTask* selected = listReadyTasks[0];
    int maxWait = selected->GetTotWaitTime();

    for (size_t i = 1; i < listReadyTasks.size(); i++) {
        int waitTime = listReadyTasks[i]->GetTotWaitTime();
        if (waitTime > maxWait) {
            maxWait = waitTime;
            selected = listReadyTasks[i];
        }
    }
    return selected;

}

ECSimPriorityScheduler::ECSimPriorityScheduler(){

}

ECSimTask *ECSimPriorityScheduler::ChooseTaskToSchedule(const vector<ECSimTask *> &listReadyTasks) const{
    if (listReadyTasks.empty()) {
        return nullptr;
    }

    ECSimTask* selected = listReadyTasks[0];
    int best = selected->GetPriority();

    for (size_t i = 1; i < listReadyTasks.size(); i++) {
        int pri = listReadyTasks[i]->GetPriority();
        if (pri < best) {
            best = pri;
            selected = listReadyTasks[i];
        }
    }
    return selected;
}

ECSimRoundRobinTaskScheduler::ECSimRoundRobinTaskScheduler() {}

ECSimTask *ECSimRoundRobinTaskScheduler::ChooseTaskToSchedule(const vector<ECSimTask *> &listReadyTasks) const{
    
    if (listReadyTasks.empty()){
        return nullptr;
    }

    ECSimTask* selected = listReadyTasks[0];
    int minRun = selected->GetTotRunTime();

    for (size_t i = 1; i < listReadyTasks.size(); i++) {
        int runTime = listReadyTasks[i]->GetTotRunTime();
        if (runTime < minRun) {
            minRun = runTime;
            selected = listReadyTasks[i];
        }
    }

    return selected;

}
