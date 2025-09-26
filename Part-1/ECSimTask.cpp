//
//  ECSimTask.cpp
//  
//
//  DON'T CHANGE THIS FILE!!
//

#include "ECSimTask.h"

//***********************************************************
// Generic simulation task


// Each task has a name
ECSimTask :: ECSimTask(const std :: string &tidIn) : tid(tidIn), tmTotWait(0), tmTotRun(0), pri(0)
{
}
  
//***********************************************************
// One-shot task: a task spans a single interval [a,b] of time; this task has soft deadline: it can only run within [a,b] but differently from hard interval: it can run partially as long as the time is within [a,b]

ECSoftIntervalTask :: ECSoftIntervalTask(const std::string &tid, int tmStartIn, int tmEndIn) : ECSimTask(tid), tmStart(tmStartIn), tmEnd(tmEndIn)
{
}

// Is task ready to run at certain time? tick: the current clock time (in simulation unit)
bool ECSoftIntervalTask :: IsReadyToRun(int tick) const
{
    // as long as tick is within the specified interval, it is ready
    return tick >= tmStart && tick <= tmEnd;
}

// Is task complete at certain time? If so, scheduler may remove it from the list. tick the current clock time (in simulation unit)
bool ECSoftIntervalTask :: IsFinished(int tick) const
{
    return tick > tmEnd;
}
