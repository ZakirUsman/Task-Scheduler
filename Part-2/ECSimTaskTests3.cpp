// Test task simulations with design patterns
// Build: c++ -std=c++11 ECSimTask3.cpp ECSimTaskScheduler3.cpp ECSimTaskTests3.cpp -o test

#include "ECSimTask3.h"
#include "ECSimTaskScheduler3.h"
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

// Two simple intervals
static void Test0()
{
    cout << "****Test0\n";
    // FIFO scheduler: two simple tasks
    ECSimIntervalTask t1("t1", 3, 5);
    ECSimIntervalTask t2("t2", 4, 6);
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
// FIFO scheduler. One consecutive interval and one simple interval (which came earlier)
static void Test1()
{
    cout << "****Test1\n";
    // FIFO scheduler: two simple tasks
    ECSimIntervalTask t1("t1", 3, 7);
    ECSimIntervalTask t2("t2", 4, 6);
    // create a consecutive version
    ECSimConsecutiveTask t1c(&t1);
    
    ECSimFIFOTaskScheduler scheduler;
    //
    scheduler.AddTask(&t2);
    scheduler.AddTask(&t1c);
    int tmSimTot = 10;
    int tmSimRun = scheduler.Simulate(tmSimTot);
    // simulate [1, 6]
    ASSERT_EQ(tmSimRun, 6);
    // t1c runs: [3,3]
    ASSERT_EQ( t1c.GetTotRunTime(), 1);
    ASSERT_EQ( t1c.GetTotWaitTime(), 1);
    // t2 runs: [4,6]
    ASSERT_EQ( t2.GetTotRunTime(), 3);
    ASSERT_EQ( t2.GetTotWaitTime(), 0);
}

// A periodic interval and a regular intervaal (which arrives earlier)
static void Test2()
{
    cout << "****Test2\n";
    // FIFO scheduler: two simple tasks
    ECSimIntervalTask t1("t1", 3, 6);
    ECSimIntervalTask t2("t2", 6,9);
    // create a periodic version with sleep time 2
    ECSimPeriodicTask t1p(&t1, 2);
    
    ECSimFIFOTaskScheduler scheduler;
    // t2 is earlier!
    scheduler.AddTask(&t2);
    scheduler.AddTask(&t1p);
    int tmSimTot = 10;
    int tmSimRun = scheduler.Simulate(tmSimTot);
    // simulate [1, 10]
    ASSERT_EQ(tmSimRun, 10);
    // t1p runs: [3,5], [10,10], wait [6,6], [9,9]
    ASSERT_EQ( t1p.GetTotRunTime(), 4);
    ASSERT_EQ( t1p.GetTotWaitTime(), 2);
    // t2 runs: [6,9]. no wai
    ASSERT_EQ( t2.GetTotRunTime(), 4);
    ASSERT_EQ( t2.GetTotWaitTime(), 0);
}

// Almost the same as Test2, now t1p is also to be consecutive (i.e., cannot be put to wait)
static void Test3()
{
    cout << "****Test3\n";
    // FIFO scheduler: two simple tasks
    ECSimIntervalTask t1("t1", 3, 6);
    ECSimIntervalTask t2("t2", 6,9);
    // create a periodic version with sleep time 2
    ECSimPeriodicTask t1p(&t1, 2);
    // Make it consecutive
    ECSimConsecutiveTask t1pc(&t1p);
    
    ECSimFIFOTaskScheduler scheduler;
    // t2 is earlier!
    scheduler.AddTask(&t2);
    scheduler.AddTask(&t1pc);
    int tmSimTot = 10;
    int tmSimRun = scheduler.Simulate(tmSimTot);
    // simulate [1, 9]
    ASSERT_EQ(tmSimRun, 9);
    // t1p runs: [3,5], wait [6,6] then abort at time right after time 6
    ASSERT_EQ( t1pc.GetTotRunTime(), 3);
    ASSERT_EQ( t1pc.GetTotWaitTime(), 1);
    // t2 runs: [6,9]. no wait
    ASSERT_EQ( t2.GetTotRunTime(), 4);
    ASSERT_EQ( t2.GetTotWaitTime(), 0);
}

// Three interval tasks, one with start deadlines and one with end deadlines
static void Test4()
{
    cout << "****Test5\n";
    // FIFO scheduler: two simple tasks
    ECSimIntervalTask t1("t1", 3,7);
    ECSimIntervalTask t2("t2", 6,9);
    ECSimIntervalTask t3("t3", 4,10);
    // t1 has start deadline of 4, end deadline 6
    ECSimStartDeadlineTask t1s(&t1, 4);
    ECSimEndDeadlineTask t1se(&t1s, 6);
    // t2 arrives the first
    // t3: start deadline: 7
    ECSimStartDeadlineTask t3s(&t3, 7);
    
    ECSimFIFOTaskScheduler scheduler;
    // t2 is earlier!
    scheduler.AddTask(&t2);
    scheduler.AddTask(&t1se);
    scheduler.AddTask(&t3s);
    int tmSimTot = 10;
    int tmSimRun = scheduler.Simulate(tmSimTot);
    // simulate [1, 9]: t3 early abort
    ASSERT_EQ(tmSimRun, 9);
    // t1se runs: [3,5], wait [6,6] then terminate
    ASSERT_EQ( t1se.GetTotRunTime(), 3);
    ASSERT_EQ( t1se.GetTotWaitTime(), 1);
    // t2 runs: [6,9]. no wait
    ASSERT_EQ( t2.GetTotRunTime(), 4);
    ASSERT_EQ( t2.GetTotWaitTime(), 0);
    // t3s never runs till time 7 and then termiinates by end deadline. Wait: [4,7] (note the wait at time 7 causes it to abort)
    ASSERT_EQ(t3s.GetTotRunTime(), 0);
    ASSERT_EQ(t3s.GetTotWaitTime(), 4);
}

// A composite task with two intervals and a regular intervaal (which arrives earlier)
static void Test5()
{
    cout << "****Test5\n";
    // FIFO scheduler: two simple tasks
    ECSimIntervalTask t11("t11", 1, 4);
    ECSimIntervalTask t12("t12", 7,9);
    ECSimIntervalTask t2("t2", 3, 5 );
    // create a composite
    ECSimCompositeTask t1c("t1c");
    t1c.AddSubtask(&t11);
    t1c.AddSubtask(&t12);
    ECSimFIFOTaskScheduler scheduler;
    // t2 is earlier!
    scheduler.AddTask(&t2);
    scheduler.AddTask(&t1c);
    int tmSimTot = 10;
    int tmSimRun = scheduler.Simulate(tmSimTot);
    // simulate [1, 9]
    ASSERT_EQ(tmSimRun, 9);
    // t1c runs: [1,2], [7,9], wait [3,4]
    ASSERT_EQ( t1c.GetTotRunTime(), 5);
    ASSERT_EQ( t1c.GetTotWaitTime(), 2);
    // t2 runs: [3,5]. no wai
    ASSERT_EQ( t2.GetTotRunTime(), 3);
    ASSERT_EQ( t2.GetTotWaitTime(), 0);
}

// Continue from the previous. Now the composite task has an ending deadline 18 and is periodic (with waiting gap time 2)...
static void Test6()
{
    cout << "****Test6\n";
    // FIFO scheduler: two simple tasks
    ECSimIntervalTask t11("t11", 1, 4);
    ECSimIntervalTask t12("t12", 7,9);
    ECSimIntervalTask t2("t2", 8, 13 );
    // create a composite
    ECSimCompositeTask t1c("t1c");
    t1c.AddSubtask(&t11);
    t1c.AddSubtask(&t12);
    // periodic with wait gap 2
    ECSimPeriodicTask t1cp(&t1c, 2);
    // ending deadline
    ECSimEndDeadlineTask t1cep(&t1cp, 18);
    
    ECSimFIFOTaskScheduler scheduler;
    // t2 is earlier!
    scheduler.AddTask(&t2);
    scheduler.AddTask(&t1cep);
    int tmSimTot = 20;
    int tmSimRun = scheduler.Simulate(tmSimTot);
    // simulate [1, 18]
    ASSERT_EQ(tmSimRun, 18);
    // t1cep runs: [1,4], [7,7], [14,15], [18,18]  wait [8,9], [12,13]
    ASSERT_EQ( t1cep.GetTotRunTime(), 8);
    ASSERT_EQ( t1cep.GetTotWaitTime(), 4);
    // t2 runs: [8,13]. no wai
    ASSERT_EQ( t2.GetTotRunTime(), 6);
    ASSERT_EQ( t2.GetTotWaitTime(), 0);
}

// A composite task is consecutive, and is periodic (with waiting gap time 2). The 2nd task is periodic (comes first), with a ending deadline...
static void Test7()
{
    cout << "****Test7\n";
    // FIFO scheduler: two simple tasks
    ECSimIntervalTask t11("t11", 1, 2);
    ECSimIntervalTask t12("t12", 4,4);
    ECSimIntervalTask t2("t2", 8, 13 );
    // create a composite
    ECSimCompositeTask t1c("t1c");
    t1c.AddSubtask(&t11);
    t1c.AddSubtask(&t12);
    // periodic
    ECSimPeriodicTask t1cp(&t1c, 2);
    // consecutive
    ECSimConsecutiveTask t1cpc(&t1cp);
    // periodic
    ECSimPeriodicTask t2p(&t2, 2);
    // Also has an ending deadline
    ECSimEndDeadlineTask t2pe(&t2p, 18);
    
    ECSimFIFOTaskScheduler scheduler;
    // t2pe is earlier!
    scheduler.AddTask(&t2pe);
    scheduler.AddTask(&t1cpc);
    int tmSimTot = 20;
    int tmSimRun = scheduler.Simulate(tmSimTot);
    // simulate [1, 18]
    ASSERT_EQ(tmSimRun, 18);
    // t1cpc runs: [1,2], [4,4], [7,7], then end  wait [8,8]
    ASSERT_EQ( t1cpc.GetTotRunTime(), 4);
    ASSERT_EQ( t1cpc.GetTotWaitTime(), 1);
    // t2pe runs: [8,13], [16,18] no wait
    ASSERT_EQ( t2pe.GetTotRunTime(), 9);
    ASSERT_EQ( t2pe.GetTotWaitTime(), 0);
}
*/


// Un-comment out test cases when you get the implementaiton

int main()
{
    Test0();
/*    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    Test6();
    Test7(); */
}
