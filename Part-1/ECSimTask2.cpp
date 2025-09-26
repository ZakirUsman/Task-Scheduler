//
//  ECSimTask2.cpp
//  
//
//
#include "ECSimTask2.h"

using namespace std;

ECMultiIntervalsTask::ECMultiIntervalsTask(const string &tid): ECSimTask(tid){

}

void ECMultiIntervalsTask::AddInterval(int a, int b){
    mult_intervals.push_back({a, b});
}

bool ECMultiIntervalsTask::IsReadyToRun(int tick)const{
    for(const auto & inter: mult_intervals){
        if(tick>= inter.first && tick<= inter.second){
            return true;
        }
    }
    return false;
}

bool ECMultiIntervalsTask::IsFinished(int tick) const{
    for( const auto &inter: mult_intervals){
        if(tick<= inter.second){
            //have we reached end time?
            return false;
        }
    }
    return true;
}

ECHardIntervalTask::ECHardIntervalTask(const string &tid, int tmStart, int tmEnd)
    : ECSimTask(tid), start(tmStart), end(tmEnd), try_start(false), running(false){}

bool ECHardIntervalTask::IsReadyToRun(int tick) const{
    if(tick<start||tick>end){
        return false;
    }
    if(try_start&&!running){
        return false;
    }
    bool result = (tick == start || running);
    return result;
}

bool ECHardIntervalTask::IsFinished(int tick) const{

    return (tick>end ||(!running&&try_start));
}

void ECHardIntervalTask::Run(int tick, int duration){
    if  (tick==start){
        running =true;
    }
    if(tick>end|| (tick!=start && !running)){
        return;
    }
    ECSimTask::Run(tick, duration);
    try_start =true;
}

void ECHardIntervalTask::Wait(int tick, int duration){
    if(tick==start){
        try_start = true;
        running = false;
    }
    ECSimTask::Wait(tick, duration);
}

ECConsecutiveIntervalTask::ECConsecutiveIntervalTask(const string &tid, int a, int b)
    : ECSimTask(tid), start(a), end(b), started(false), interrupted(false){}

bool ECConsecutiveIntervalTask::IsReadyToRun(int tick) const{
    bool check = !interrupted && tick >= start && tick <=end;
    return check;
}

bool ECConsecutiveIntervalTask::IsFinished(int tick) const{
    return interrupted || (started&&tick>end);
}


void ECConsecutiveIntervalTask::Run(int tick, int duration){
    if(!started){
        started = true;
    }

    ECSimTask::Run(tick, duration);
}

void ECConsecutiveIntervalTask::Wait(int tick, int duration){
    if(started){
        interrupted=true;
    }
    ECSimTask::Wait(tick, duration);
}

ECPeriodicTask::ECPeriodicTask(const string &tid, int tstart, int runLength, int sleepLength)
    :ECSimTask(tid), start(tstart), run_time(runLength), sleep_time(sleepLength), curr_cycle_start(tstart), remaining_ticks(0){}

bool ECPeriodicTask::IsReadyToRun(int tick) const{
    if (tick < start) return false;

    int task_time = run_time + sleep_time;
    int effective_cycle_start = curr_cycle_start;

    while (tick >= effective_cycle_start + task_time) {
        effective_cycle_start += task_time;
    }

    return tick >= effective_cycle_start && tick < effective_cycle_start + run_time;
}

bool ECPeriodicTask::IsFinished(int tick) const{
    return false;
}

void ECPeriodicTask::Run(int tick, int duration){
    ECSimTask::Run(tick, duration);

    int task_time = run_time + sleep_time;
    while (tick >= curr_cycle_start + task_time) {
        curr_cycle_start += task_time;
    }

    if (tick == curr_cycle_start) {
        remaining_ticks = run_time;
    }

    if (remaining_ticks > 0) {
        remaining_ticks -= 1;
        if (remaining_ticks == 0) {
            curr_cycle_start += task_time;
        }
    }
}

void ECPeriodicTask::Wait(int tick, int duration){
    ECSimTask::Wait(tick, duration);
}