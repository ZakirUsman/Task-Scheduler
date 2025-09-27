# ⏱️ C++ Task Scheduler Simulation

## 📌 Overview
This project is a **Task Scheduler simulator built in C++** that models how different scheduling strategies manage tasks with constraints like deadlines, periodic execution, and consecutive execution.  

The goal was to apply **Object-Oriented Programming (OOP)** principles and **design patterns** (Decorator, Composite, Strategy) to design an extensible system. It demonstrates how schedulers handle fairness, deadlines, and efficiency in real-time systems.  

---

## 🛠️ Features
- **Soft Interval Tasks** → Run within a time window [a, b].  
- **Hard Interval Tasks** → Must start at `a` and end at `b`, or aborts.  
- **Consecutive Tasks** → Must run without interruption, otherwise terminate.  
- **Periodic Tasks** → Run repeatedly with defined sleep cycles.  
- **Start Deadline Tasks** → Must begin before a deadline or abort.  
- **End Deadline Tasks** → Must finish by a set deadline.  
- **Composite Tasks** → Group multiple subtasks; finish when all are complete.  

---

## 🎯 Scheduling Strategies
Implemented by extending a **base scheduler class** with the **Strategy Pattern**:  
- **FIFO (First-In-First-Out)** → Schedules tasks in arrival order.  
- **Longest Wait-Time First** → Chooses the task that has waited the most.  
- **Priority-Based** → Tasks assigned priorities (lower number = higher priority).  
- **Round Robin** → Tasks take turns to ensure fairness.  

---

## 🧑‍💻 Technologies & Concepts
- **Language:** C++11  
- **OOP Principles:** Encapsulation, Inheritance, Polymorphism  
- **Design Patterns:**  
  - **Decorator** → Wrap tasks with extra behavior (e.g., deadlines, periodicity).  
  - **Composite** → Treat groups of subtasks as a single task.  
  - **Strategy** → Plug in different scheduling algorithms without changing the core system.  
- **Unit Testing:** Test cases simulate real scenarios and check runtime/wait time.  

---

## 📂 Project Structure
- `ECSimTask.h / ECSimTask.cpp` → Base task class + soft interval tasks.  
- `ECSimTask2.*` → Extended tasks (hard, consecutive, periodic, multi-interval).  
- `ECSimTask3.*` → Advanced tasks (start deadline, end deadline, composite).  
- `ECSimTaskScheduler.*` → Base scheduler + FIFO scheduler.  
- `ECSimTaskScheduler2.*` → LWTF, Priority, Round Robin schedulers.  
- `ECSimTaskScheduler3.*` → Extended scheduling framework.  
- `ECSimTaskTests*.cpp` → Test files for validation.  
