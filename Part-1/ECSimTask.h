//
//  ECSimTask.h
//  
//
//  Simulation task: different types
//  DON'T CHANGE THIS FILE!!
//

#ifndef ECSimTask_h
#define ECSimTask_h

#include <string>

//***********************************************************
// Generic simulation task

class ECSimTask
{
public:
    // Each task has a name
    ECSimTask(const std :: string &tid);
    
    std::string GetId() const { return tid; }
    
    // Is task ready to run at certain time? tick: the current clock time (in simulation unit)
    virtual bool IsReadyToRun(int tick) const = 0;
    
    // Is task complete at certain time? If so, scheduler may remove it from the list. tick the current clock time (in simulation unit)
    virtual bool IsFinished(int tick) const = 0;
    
    // Run the task for some duration (usually 1, but can be more) starting from time tick
    virtual void Run(int tick, int duration) { tmTotRun += duration; }
    
    // Wait for some duration (usually 1, but can be more), starting from time tick
    virtual void Wait(int tick, int duration) { tmTotWait += duration; }
    
    // How much total time does the task has to wait to get its turn so far?
    virtual int GetTotWaitTime() const { return tmTotWait; }
    
    // Get total run-time (so far)
    virtual int GetTotRunTime() const { return tmTotRun; }
    
    // priority
    void SetPriority(int p) { pri = p; }
    int GetPriority() const { return pri; }
    
private:
    std::string tid;
    int tmTotWait;
    int tmTotRun;
    int pri;
};

//***********************************************************
// One-shot task: a task spans a single interval [a,b] (inclusive of both ends) of time; this task has soft deadline: it can only run within [a,b] but differently from hard interval: it can run partially as long as the time is within [a,b]

class ECSoftIntervalTask : public ECSimTask
{
public:
    ECSoftIntervalTask(const std::string &tid, int tmStart, int tmEnd);
    
    // Is task ready to run at certain time? tick: the current clock time (in simulation unit)
    virtual bool IsReadyToRun(int tick) const;
    
    // Is task complete at certain time? If so, scheduler may remove it from the list. tick the current clock time (in simulation unit)
    virtual bool IsFinished(int tick) const;
    
private:
    int tmStart;
    int tmEnd;
};

#endif /* ECSimTask_h */
