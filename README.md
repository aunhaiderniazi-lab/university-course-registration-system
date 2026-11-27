# University Course Registration System (C++ OOP)

A console-based **University Course Registration System** built in C++11, implementing real-world academic enrollment rules, role-based access control, and persistent storage.

**Author:** Aun Haider Niazi  
**Program:** Bachelor of Engineering Technology (Information) — Foundation University (FUSST)

---

## Features

- **Student Portal**
  - Create new student accounts with auto-generated IDs (`2001+`)
  - Browse available courses with real-time seat availability and prerequisites
  - Enroll in or drop courses with automated validation
  - View personal schedule, completed courses, and current credit load
- **Admin Portal** *(Default Password: `admin123`)*
  - Add new courses with custom credit hours, seat capacities, and prerequisites
  - Mark enrolled courses as completed for individual students (unlocking downstream prerequisites)
  - Audit live course enrollment counts
- **Enrollment Rule Engine**
  - Enforces prerequisite chains (e.g., `CS101` → `CS102` → `CS201` → `CS301`)
  - Caps semester load at **18 credit hours**
  - Blocks duplicate enrollments, completed course re-enrollment, and over-capacity sections
- **Persistent File Storage**
  - Automatically loads and saves course catalogs (`courses.txt`) and student records (`students.txt`) across sessions
- **Input Hardening**
  - Guards `cin` streams against non-numeric input to prevent infinite menu loops

---

## OOP Concepts Applied

| Concept | Implementation |
| :--- | :--- |
| **Abstract Base Class & Pure Virtual Function** | `Person` class with `virtual string role() const = 0` |
| **Inheritance & Polymorphism** | `Student` inherits from `Person` and overrides `role()` |
| **Encapsulation** | Private collections and helper methods inside `University` |
| **Static Constants** | `Student::MAX = 18` enforcing the credit limit |
| **Exception Handling** | `try` / `catch` blocks validating business rules on registration, drop, and admin actions |
| **File I/O & STL** | `ifstream` / `ofstream` persistence backed by `std::vector` and `<algorithm>` |

---

## Build & Run

```bash
g++ -std=c++11 reg.cpp -o reg
./reg
```

*(On Windows PowerShell or Command Prompt, run `reg.exe` after compiling.)*

---

## Sample Walkthrough

```text
Course Registration System - by Aun Haider Niazi

=== COURSE REGISTRATION ===
1.Student login 2.New student 3.Admin 4.Exit
Choice: 2
Name (one word): Aun
Account created! Your ID: 2001

=== COURSE REGISTRATION ===
1.Student login 2.New student 3.Admin 4.Exit
Choice: 1
Student ID: 2001

1.Courses 2.Register 3.Drop 4.My schedule 5.Back
Choice: 2

Code    Name            Cr   Seats     Prereq
------------------------------------------------
CS101   Programming     3    30/30     none
CS102   OOP             3    25/25     CS101
CS201   DataStructures  3    25/25     CS102
CS301   Database        3    20/20     CS201
MT101   Calculus        3    40/40     none
Course code: CS102
Error: Prerequisite needed: CS101
```
