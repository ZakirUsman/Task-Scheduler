//
//  ECSimTaskScheduler.h
//  
//
//  DON'T CHANGE THIS FILE!!

#ifndef ECSimTaskScheduler3_h
#define ECSimTaskScheduler3_h

#include <map>
#include <vector>

class ECSimTask;

//***********************************************************
// Simulation task scheduler

class ECSimTaskScheduler
{
public:
    ECSimTaskScheduler();
    virtual ~ECSimTaskScheduler();
    
    // Add a task to be scheduled
    void AddTask(ECSimTask *pTask);
    
    // Remove a task from the list of tasks to be scheduled
    void RemoveTask(ECSimTask *pTask);
    
    // Run simulation for the period of duration. If duration < 0, then run until there is no tasks is ready to run
    // Return the number of ticks that it runs (can be smaller than duration if it terminates earlier)
    // Caution: this potneitally can enter an infinite loop
    virtual int Simulate(int duration);
    
    // Get current time
    int GetTime() const { return timeCurr; }
    
    // Get current scheduled task
    ECSimTask *GetCurrTask() const { return pTaskCurr; }
    
protected:
    // Choose from a list of tasks that are ready to run
    virtual ECSimTask *ChooseTaskToSchedule(const std::vector<ECSimTask *> &listReadyTasks) const = 0;
    void SetTime(int t) { timeCurr = t; }
    void SetTask(ECSimTask *pt) { pTaskCurr = pt; }
    
private:
    // impelementation
    
    // Order tasks by the order of receiving the schedule request
    std::vector<ECSimTask *> listTasks;
    
    // Current time
    int timeCurr;
    
    // Currently scheduled task
    ECSimTask *pTaskCurr;
};

//***********************************************************
// Simple first-come-first-serve or first-in-first-out scheduler
class ECSimFIFOTaskScheduler : public ECSimTaskScheduler
{
public:
    ECSimFIFOTaskScheduler();
    
protected:
    // Choose from a list of tasks that are ready to run
    virtual ECSimTask *ChooseTaskToSchedule(const std::vector<ECSimTask *> &listReadyTasks) const;
};

#endif /* ECSimTaskScheduler3_h */
