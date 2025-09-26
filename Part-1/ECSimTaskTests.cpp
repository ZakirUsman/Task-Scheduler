// Test task simulations
// Build: c++ -std=c++11 ECSimTask.cpp ECSimTask2.cpp ECSimTaskScheduler.cpp ECSimTaskScheduler2.cpp ECSimTaskTests.cpp -o test

#include "ECSimTask.h"
#include "ECSimTask2.h"
#include "ECSimTaskScheduler.h"
#include "ECSimTaskScheduler2.h"
#include <iostream>
using namespace std;

template<class T>
void ASSERT_EQ(T x, T y)
{
  if( x == y )
  {
    cout << "Test passed: equal: " << x << "  " << y << endl;
  }
  else
  {
    cout << "Test FAILED: equal: " << x << "  " << y << endl;
  }
}

// Two soft intervals
static void Test0()
{
    cout << "****Test0\n";
    // FIFO scheduler: two simple tasks
    ECSoftIntervalTask t1("t1", 3, 5);
    ECSoftIntervalTask t2("t2", 4, 6);
    ECSimFIFOTaskScheduler scheduler;
    scheduler.AddTask(&t1);
    scheduler.AddTask(&t2);
    int tmSimTot = 10;
    int tmSimRun = scheduler.Simulate(tmSimTot);
    // simulate [1, 6]
    ASSERT_EQ(tmSimRun, 6);
    ASSERT_EQ( t1.GetTotRunTime(), 3);
    ASSERT_EQ( t1.GetTotWaitTime(), 0);
    ASSERT_EQ( t2.GetTotRunTime(), 1);
    ASSERT_EQ( t2.GetTotWaitTime(), 2);
}
/*
// FIFO scheduler. Two multi-intervals
static void Test1()
{
    cout << "****Test1\n";
    // FIFO scheduler: two simple tasks
    ECMultiIntervalsTask t1("t1");
    t1.AddInterval(3,5);
    t1.AddInterval(8,9);
    ECMultiIntervalsTask t2("t2");
    t2.AddInterval(2,2);
    t2.AddInterval(4,4);
    t2.AddInterval(7,8);
    ECSimFIFOTaskScheduler scheduler;
    scheduler.AddTask(&t1);
    scheduler.AddTask(&t2);
    int tmSimTot = 10;
    int tmSimRun = scheduler.Simulate(tmSimTot);
    ASSERT_EQ( tmSimRun, 9);
    // t1 run: [3,5], [8,9], no wait
    ASSERT_EQ( t1.GetTotRunTime(), 5);
    ASSERT_EQ( t1.GetTotWaitTime(), 0);
    // t2 run [2,2], [7,7], wait: [4,4], [8,8]
    ASSERT_EQ( t2.GetTotRunTime(), 2);
    ASSERT_EQ( t2.GetTotWaitTime(), 2);
}

// One soft interval and one hard interval
static void Test2()
{
    cout << "****Test2\n";
    // FIFO scheduler: two simple tasks
    ECSoftIntervalTask t1("t1", 3, 5);
    ECHardIntervalTask t2("t2", 4, 6);
    ECSimFIFOTaskScheduler scheduler;
    scheduler.AddTask(&t1);
    scheduler.AddTask(&t2);
    int tmSimTot = 10;
    int tmSimRun = scheduler.Simulate(tmSimTot);
    // Sim [1,5]: t2 termiinates at tick 5 (since it missed deadline)
    ASSERT_EQ( tmSimRun, 5);
    ASSERT_EQ( t1.GetTotRunTime(), 3);
    ASSERT_EQ( t1.GetTotWaitTime(), 0);
    ASSERT_EQ( t2.GetTotRunTime(), 0);
    // note: wait time only counts the time that it is ready to run but cannot; since once missed the deadline, the task is no longer active; so wait time is 1
    ASSERT_EQ( t2.GetTotWaitTime(), 1);
}

// Two soft intervals and one consecutive interval
static void Test3()
{
    cout << "****Test3\n";
    // FIFO scheduler: two simple tasks
    ECSoftIntervalTask t1("t1", 3, 5);
    ECSoftIntervalTask t2("t2", 7, 7);
    ECConsecutiveIntervalTask t3("t3", 4, 9);
    ECSimFIFOTaskScheduler scheduler;
    scheduler.AddTask(&t1);
    scheduler.AddTask(&t2);
    scheduler.AddTask(&t3);
    int tmSimTot = 10;
    int tmSimRun = scheduler.Simulate(tmSimTot);
    // Sim [1, 7]:  t3 stop running at tick 6 and so it finishes
    ASSERT_EQ( tmSimRun, 7);
    ASSERT_EQ( t1.GetTotRunTime(), 3);
    ASSERT_EQ( t1.GetTotWaitTime(), 0);
    ASSERT_EQ( t2.GetTotRunTime(), 1);
    ASSERT_EQ( t2.GetTotWaitTime(), 0);
    // t3 can only run from [6,6]. Since it must be consecutive, it cannot restart after t2
    ASSERT_EQ( t3.GetTotRunTime(), 1);
    // wait for 3: first waiting [4,5], then [7,7]
    ASSERT_EQ( t3.GetTotWaitTime(), 3);
}

// One soft intervals and one periodic task
static void Test4()
{
    // FIFO scheduler: two simple tasks
    cout << "****Test4\n";
    ECSoftIntervalTask t1("t1", 3, 5);
    ECPeriodicTask t2("t2", 4, 1, 2);  // start at time 4, run for 1 and sleep 2
    ECSimFIFOTaskScheduler scheduler;
    scheduler.AddTask(&t1);
    scheduler.AddTask(&t2);
    int tmSimTot = 12;
    int tmSimRun = scheduler.Simulate(tmSimTot);
    // The periodic task just keeps running
    ASSERT_EQ(  tmSimRun,  12);
    ASSERT_EQ( t1.GetTotRunTime(), 3);
    ASSERT_EQ( t1.GetTotWaitTime(), 0);
    // t2 runs: [7,7], [10,10]
    ASSERT_EQ( t2.GetTotRunTime(), 2);
    // wait: [4,4]
    ASSERT_EQ( t2.GetTotWaitTime(), 1);
}

// Longest wait time first scheudler; two soft intervals
static void Test5()
{
    cout << "****Test5\n";
    // Longest wait time first scheduler: two simple tasks
    ECSoftIntervalTask t1("t1", 3, 10);
    ECSoftIntervalTask t2("t2", 5, 13);
    ECSimLWTFTaskScheduler scheduler;
    scheduler.AddTask(&t1);
    scheduler.AddTask(&t2);
    int tmSimTot = 10;
    int tmSimRun = scheduler.Simulate(tmSimTot);
    ASSERT_EQ(tmSimRun, 10);
    // the two tasks will take turns to run
    // t1: run [3,5],[7,7], [9,9]
    ASSERT_EQ( t1.GetTotRunTime(), 5);
    // t1: wait [6,6], [8,8], [10,10]. Note: time starts at 1
    ASSERT_EQ( t1.GetTotWaitTime(), 3);
    // t2: run [6,6], [8,8], [10,10]
    ASSERT_EQ( t2.GetTotRunTime(), 3);
    // t2: wait [5,5], [7,7], [9,9]
    ASSERT_EQ( t2.GetTotWaitTime(), 3);
}

// Priority scheudler; two soft intervals
static void Test6()
{
    cout << "****Test6\n";
    // Priority scheduler: two simple tasks
    ECSoftIntervalTask t1("t1", 3, 10);
    ECSoftIntervalTask t2("t2", 5, 13);
    // set t2 to priority -1, which is higher than default
    t2.SetPriority(-1);
    ECSimPriorityScheduler scheduler;
    scheduler.AddTask(&t1);
    scheduler.AddTask(&t2);
    int tmSimTot = 10;
    int tmSimRun = scheduler.Simulate(tmSimTot);
    ASSERT_EQ(tmSimRun, 10);
    // t1: run [3,4]
    ASSERT_EQ( t1.GetTotRunTime(), 2);
    // t1: wait [5,10]
    ASSERT_EQ( t1.GetTotWaitTime(), 6);
    // t2: run [5,10]
    ASSERT_EQ( t2.GetTotRunTime(), 6);
    // t2: wait [5,5], [7,7], [9,9]
    ASSERT_EQ( t2.GetTotWaitTime(), 0);
}

// Round-robiin scheudler; tw0 intervals and one periodic interval
static void Test7()
{
    cout << "****Test7\n";
    // Round-robin scheduler: three simple tasks
    ECSoftIntervalTask t1("t1", 3, 5);
    ECSoftIntervalTask t2("t2", 7, 9);
    ECPeriodicTask t3("t3", 2, 2, 1);  // start at time 2, run for 2 and sleep 1
    ECSimRoundRobinTaskScheduler scheduler;
    scheduler.AddTask(&t1);
    scheduler.AddTask(&t2);
    scheduler.AddTask(&t3);
    int tmSimTot = 10;
    int tmSimRun = scheduler.Simulate(tmSimTot);
    ASSERT_EQ(tmSimRun, 10);
    // t1: run [3,4]: note at time 5, t1 has been schedule twice [3,4] but t3 only once [2,2]. So t3 is preferred
    ASSERT_EQ( t1.GetTotRunTime(), 2);
    // t1: wait [5,5]]
    ASSERT_EQ( t1.GetTotWaitTime(), 1);
    // t2: run [7,9]
    ASSERT_EQ( t2.GetTotRunTime(), 3);
    // t2: wait none
    ASSERT_EQ( t2.GetTotWaitTime(), 0);
    // t3: run [2,2], [5,6],
    ASSERT_EQ( t3.GetTotRunTime(), 3);
    // t3: wait [5,5], [8,9]
    ASSERT_EQ( t3.GetTotWaitTime(), 3);
}
*/

// Un-comment out test cases when you get the implementaiton

int main()
{
    Test0();
/*
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    Test6();
    Test7(); */
}
