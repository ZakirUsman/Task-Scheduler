//
//  ECSimTaskScheduler.cpp
//  
//
//  DON'T CHANGE THIS FILE!!
//

#include <vector>
#include <algorithm>
#include <iterator>
#include <climits>
#include <iostream>
using namespace std;

#include "ECSimTaskScheduler.h"
#include "ECSimTask.h"

//***********************************************************
// Simulation task scheduler

ECSimTaskScheduler :: ECSimTaskScheduler() : timeCurr(0), pTaskCurr(NULL)
{
}

ECSimTaskScheduler :: ~ECSimTaskScheduler()
{
}

// Add a task to be scheduled
void ECSimTaskScheduler :: AddTask(ECSimTask *pTask)
{
    listTasks.push_back(pTask);
}

// Remove a task from the list of tasks to be scheduled
void ECSimTaskScheduler :: RemoveTask(ECSimTask *pTask)
{
    listTasks.erase(std::remove(listTasks.begin(), listTasks.end(), pTask), listTasks.end());
}

// Run simulation for the period of duration. If duration < 0, then run until there is no tasks is left
// Return the number of ticks that it runs (can be smaller than duration if it terminates earlier)
// Caution: this potneitally can enter an infinite loop
int ECSimTaskScheduler :: Simulate(int duration)
{
    int durationUse = duration;
    if( durationUse < 0 )
    {
        durationUse = INT_MAX;
    }
    int numStepsRuns = 0;
    for(int step = 0; step<durationUse; ++step)
    {
        // first make sure there is some task to simulate
        // update the list of tasks; remove those that are already finished; again, use lambda
        // If a task is to expire at the next tick, consider it finished
        int tmCur = GetTime();
        auto it1 = std::remove_if( this->listTasks.begin(), this->listTasks.end(), [tmCur](ECSimTask *px) { return px->IsFinished(tmCur+1); }  );
//cout << "Number of tasks completed: " << listTasks.end()-it1 << endl;
        listTasks.erase(it1, this->listTasks.end());
/*ECSimTask *ptc = GetCurrTask();
if( ptc != NULL )
{
cout << "Current task: " << ptc->GetId() << endl;
}
else
{
cout << "No task active\n";
}*/
        // stop simulation if no simulation left
        if( this->listTasks.size() == 0 )
        {
            break;
        }
        
        ++numStepsRuns;
        // update time each time we run simulation
        int tmNew = GetTime()+1;
        SetTime(tmNew);
//cout << "Simulaton: " << tmNew << endl;
        // Find out all ready-to-run tasks. YW: use Lambda here to make code shorter
        vector<ECSimTask *> listReadyTasks;
        std::copy_if( this->listTasks.begin(), this->listTasks.end(), std::back_inserter(listReadyTasks), [tmNew](ECSimTask *px) { return px->IsReadyToRun(tmNew); } );
        
        // find the task to schedule for this time
        ECSimTask *ptNext = ChooseTaskToSchedule(listReadyTasks);
        
        // let all other ready tasks to wait
        if( ptNext != NULL )
        {
            ptNext->Run(tmNew, 1);
            for(auto x : listReadyTasks)
            {
                if( x != ptNext )
                {
                    x->Wait(tmNew, 1);
                }
            }
        }
        SetTask( ptNext );
    }
    return numStepsRuns;
}

//***********************************************************
// Simple first-come-first-serve or first-in-first-out scheduler

ECSimFIFOTaskScheduler :: ECSimFIFOTaskScheduler()
{
}

// Choose from a list of tasks that are ready to run
ECSimTask * ECSimFIFOTaskScheduler :: ChooseTaskToSchedule(const std::vector<ECSimTask *> &listReadyTasks) const
{
    // simply return the first one
    if( listReadyTasks.size() > 0 )
    {
        return listReadyTasks[0];
    }
    else
    {
        return NULL;
    }
}
    
