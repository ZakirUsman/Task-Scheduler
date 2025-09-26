//
//  ECSimTask3.cpp
//  
//

#include "ECSimTask3.h"
#include <iostream>
using namespace std;

//***********************************************************
// Basic task
//***********************************************************

//***********************************************************
// Interval task: a single interval.
// YW: you shouldn't need to change this class!

ECSimIntervalTask :: ECSimIntervalTask(const string &tidIn, int tmStartIn, int tmEndIn) : tid(tidIn), tmStart(tmStartIn), tmEnd(tmEndIn), tmTotWait(0), tmTotRun(0)
{
}

// Is task ready to run at certain time? tick: the current clock time (in simulation unit)
bool ECSimIntervalTask :: IsReadyToRun(int tick) const
{
    return tick >= tmStart && tick <= tmEnd;
}

// Is task complete at certain time? If so, scheduler may remove it from the list. tick the current clock time (in simulation unit)
bool ECSimIntervalTask :: IsFinished(int tick) const
{
    return tick > tmEnd;
}


//***********************************************************
// Consecutive task: a task that can early abort

ECSimConsecutiveTask :: ECSimConsecutiveTask(ECSimTask *pTask): task(pTask), started(false),interrupted(false)
{
}
// your code here


bool ECSimConsecutiveTask::IsReadyToRun(int tick) const {
    return !interrupted && task->IsReadyToRun(tick);
}

bool ECSimConsecutiveTask::IsFinished(int tick) const {
    return interrupted|| task->IsFinished(tick);
}

bool ECSimConsecutiveTask::IsAborted(int tick) const {
    return interrupted|| task->IsAborted(tick);
}

void ECSimConsecutiveTask::Run(int tick, int duration) {
    started = true;
    task->Run(tick, duration);
}

void ECSimConsecutiveTask::Wait(int tick, int duration) {
    if (started &&task->IsReadyToRun(tick)) {
        interrupted = true;
    }
    task->Wait(tick, duration);
}

int ECSimConsecutiveTask::GetTotWaitTime() const {
    return task->GetTotWaitTime();
}

int ECSimConsecutiveTask::GetTotRunTime() const {
    return task->GetTotRunTime();
}

string ECSimConsecutiveTask::GetId() const {
    return task->GetId();
}


//***********************************************************
// Periodic task: a task that can early abort

ECSimPeriodicTask :: ECSimPeriodicTask(ECSimTask *pTask, int ls)
    : task(pTask), sleep_time(ls), base_start(-1), base_end(-1), period_runtime(0), period_idx(0), curr_period_ticks(0), ran_this_period(false) {

    }

// your code here
string ECSimPeriodicTask::GetId() const {
    return task->GetId();
}



bool ECSimPeriodicTask::IsReadyToRun(int tick) const {
    if (base_start == -1 && task->IsReadyToRun(tick)) {
        base_start = tick;

        while (!task->IsReadyToRun(base_start)){ 
            base_start++;
        }
        base_end = base_start;
        while (task->IsReadyToRun(base_end + 1)) {
            base_end++;
        }

        period_runtime = base_end - base_start + 1;
    }

    if (base_start == -1) {
        return false;
    }

    while (true) {
        int st = base_start + period_idx * (period_runtime + sleep_time);
        int en = st + period_runtime - 1;
        int sleep_end = en + sleep_time;

        if (tick < st) return false;

        if (tick > sleep_end) {
            period_idx++;
            
            curr_period_ticks = 0;
            ran_this_period = false;
            
            continue;
        }

        if (tick >= st && tick <= en) {
            int mtick = base_start + (tick - st);
            return task->IsReadyToRun(mtick);
        }

        return false;
    }
}



bool ECSimPeriodicTask::IsFinished(int tick) const {
    if (base_start == -1) {
        return false;
    }

    int st = base_start + period_idx * (period_runtime + sleep_time);
    int en = st + period_runtime - 1;

    if (tick < st || tick > en) {
        return false;
    }

    int mtick = base_start + (tick - st);
    
    return task->IsFinished(mtick);

}

bool ECSimPeriodicTask::IsAborted(int tick) const {
    if (base_start == -1) {
        return false;
    }

    int st = base_start + period_idx * (period_runtime + sleep_time);
    
    
    int en = st + period_runtime - 1;

    if (tick < st || tick > en) {
        return false;
    }

    int mtick = base_start + (tick - st);

    return task->IsAborted(mtick);
}

void ECSimPeriodicTask::Run(int tick, int duration) {
    int st = base_start + period_idx * (period_runtime + sleep_time);
    int mtick = base_start + (tick - st);

    if (curr_period_ticks == 0){
        ran_this_period = true;
    }

    task->Run(mtick, duration);
    curr_period_ticks++;
}

void ECSimPeriodicTask::Wait(int tick, int duration) {
    int st = base_start + period_idx * (period_runtime + sleep_time);
    int en = st + period_runtime - 1;

    if (tick >= st && tick <= en) {
        int offset = tick-st;
        int mtick = base_start + offset;
        task->Wait(mtick, duration);
    }
}

int ECSimPeriodicTask::GetTotWaitTime() const {
    return task->GetTotWaitTime();
}

int ECSimPeriodicTask::GetTotRunTime() const {
    return task->GetTotRunTime();
}



//***********************************************************
// Task with a deadline to start: a task that must start by some time; otherwise terminate

ECSimStartDeadlineTask :: ECSimStartDeadlineTask(ECSimTask *pTask, int tmStartDeadlineIn) 
    :task(pTask), start_deadline(tmStartDeadlineIn), started(false), aborted(false){
}

bool ECSimStartDeadlineTask::IsReadyToRun(int tick) const {
    return !aborted && task->IsReadyToRun(tick);
}

bool ECSimStartDeadlineTask::IsFinished(int tick) const {
    return task->IsFinished(tick);
}

bool ECSimStartDeadlineTask::IsAborted(int tick) const {
    return aborted;
}

void ECSimStartDeadlineTask::Run(int tick, int duration) {
    if (!started && tick > start_deadline) {
        aborted = true;
        return;
    }
    started = true;
    task->Run(tick, duration);

}

void ECSimStartDeadlineTask::Wait(int tick, int duration) {
    if (!started && tick > start_deadline) {
        aborted = true;
    } else {
        task->Wait(tick, duration);
    }
    
    
}

int ECSimStartDeadlineTask::GetTotWaitTime() const {
    return task->GetTotWaitTime();
}

int ECSimStartDeadlineTask::GetTotRunTime() const {
    return task->GetTotRunTime();
}

string ECSimStartDeadlineTask::GetId() const {
    return task->GetId();
}

// your code here

//***********************************************************
// Task must end by some fixed time click: this is useful e.g. when a task is periodic

ECSimEndDeadlineTask :: ECSimEndDeadlineTask(ECSimTask *pTask, int tmEndDeadlineIn) 
    : task(pTask), end_deadline(tmEndDeadlineIn), aborted(false) {
}


// your code here


bool ECSimEndDeadlineTask::IsReadyToRun(int tick) const {
    bool check = tick <= end_deadline && task->IsReadyToRun(tick);
    return check;
}

bool ECSimEndDeadlineTask::IsFinished(int tick) const {
    bool check = tick > end_deadline || task->IsFinished(tick);
    return check;
}

bool ECSimEndDeadlineTask::IsAborted(int tick) const {
    if (tick>end_deadline) {
        aborted = true;
    }
    bool check = aborted || task->IsAborted(tick);
    return check;
}

void ECSimEndDeadlineTask::Run(int tick, int duration) {
    task->Run(tick, duration);
}

void ECSimEndDeadlineTask::Wait(int tick, int duration) {
    if(tick>end_deadline){
        aborted = true;
    }
    else{
        task->Wait(tick,duration);
    }

}

int ECSimEndDeadlineTask::GetTotWaitTime() const {
    return task->GetTotWaitTime();
}

int ECSimEndDeadlineTask::GetTotRunTime() const {
    return task->GetTotRunTime();
}

string ECSimEndDeadlineTask::GetId() const {
    return task->GetId();
}


//***********************************************************
// Composite task: contain multiple sub-tasks

ECSimCompositeTask :: ECSimCompositeTask(const string &tidIn) : tid(tidIn){

}

// your code here

string ECSimCompositeTask::GetId() const {
    return tid;
}

void ECSimCompositeTask::AddSubtask(ECSimTask *pt) {
    subtasks.push_back(pt);
}

bool ECSimCompositeTask::IsReadyToRun(int tick) const {
    for (auto task : subtasks) {
        if (task->IsReadyToRun(tick)){
            return true;
        }
    }
    return false;
}

bool ECSimCompositeTask::IsFinished(int tick) const {
    for (auto task : subtasks) {
        if (!task->IsFinished(tick)) {
            return false;
        }
    }
    return true;
}

bool ECSimCompositeTask::IsAborted(int tick) const {
    for (auto task : subtasks) {
        if (task->IsAborted(tick)){
            return true;
        }
    }
    return false;
}

void ECSimCompositeTask::Run(int tick, int duration) {
    for (auto task : subtasks) {
        if (task->IsReadyToRun(tick)) {
            task->Run(tick, duration);
            break;
        }
    }
}

void ECSimCompositeTask::Wait(int tick, int duration) {
    for (auto task : subtasks) {
        if (!task->IsFinished(tick)) {
            task->Wait(tick, duration);
            return;
        }
    }
}

int ECSimCompositeTask::GetTotWaitTime() const {
    int total = 0;
    for (auto task : subtasks){
        total += task->GetTotWaitTime();
    }
    return total;
}

int ECSimCompositeTask::GetTotRunTime() const {
    int total = 0;
    for (auto task : subtasks){
        total += task->GetTotRunTime();
    } 
    return total;
}