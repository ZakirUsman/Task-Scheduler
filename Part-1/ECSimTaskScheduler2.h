//
//  ECSimTaskScheduler2.h
//  
//
//  

#ifndef ECSimTaskScheduler2_h
#define ECSimTaskScheduler2_h

#include "ECSimTaskScheduler.h"
using namespace std;

// Now define your new schedulers here...

//***********************************************************
// Longest wait-time first scheduler: choose the task that has waited the longest so far; break ties by the order of requests receiving
class ECSimLWTFTaskScheduler : public ECSimTaskScheduler
{
public:
    ECSimLWTFTaskScheduler();
    // your code here    

protected:
    virtual ECSimTask *ChooseTaskToSchedule(const vector<ECSimTask *> &listReadyTasks) const override;
};

//***********************************************************
// Priority-based scheduler. By default, each task has the same priority 0; this priorty can be set (the smaller the higher priority, as in Unix/Linux).
class ECSimPriorityScheduler : public ECSimTaskScheduler
{
public:
    ECSimPriorityScheduler();
    
    // your code here    
protected:
    virtual ECSimTask *ChooseTaskToSchedule(const vector<ECSimTask *> &listReadyTasks) const override;
};


//***********************************************************
// Round-robin scheduler: the task having run the fewest in the past, get the highest priority
class ECSimRoundRobinTaskScheduler : public ECSimTaskScheduler
{
public:
    ECSimRoundRobinTaskScheduler();
    
    // your code here    

protected:
    virtual ECSimTask *ChooseTaskToSchedule(const vector<ECSimTask *> &listReadyTasks) const override;
};


#endif /* ECSimTaskScheduler2_h */
