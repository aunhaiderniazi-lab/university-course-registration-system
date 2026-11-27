// Author: Aun Haider Niazi
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
using namespace std;

class Person {
protected:
    int id;
    string name;
public:
    Person(int i, string n) : id(i), name(n) {}
    virtual ~Person() {}
    int getId() const { return id; }
    string getName() const { return name; }
    virtual string role() const = 0;
};

class Course {
public:
    string code, name, prereq;
    int credits, cap, taken;
    Course(string c, string n, int cr, int cp, string p = "none", int t = 0)
        : code(c), name(n), prereq(p), credits(cr), cap(cp), taken(t) {}
    bool full() const { return taken >= cap; }
};

class Student : public Person {
public:
    static const int MAX = 18;
    vector<string> done, regd;
    Student(int i, string n) : Person(i, n) {}
    string role() const override { return "Student"; }
    bool has(const vector<string>& v, const string& c) const {
        return find(v.begin(), v.end(), c) != v.end();
    }
};

class University {
    vector<Course> cs;
    vector<Student> ss;

    Course* course(const string& c) {
        for (auto& x : cs) if (x.code == c) return &x;
        return nullptr;
    }

    Student* student(int id) {
        for (auto& s : ss) if (s.getId() == id) return &s;
        return nullptr;
    }

    int credits(const Student& s) {
        int t = 0;
        for (const auto& c : s.regd) {
            Course* x = course(c);
            if (x) t += x->credits;
        }
        return t;
    }

    int readInt(const string& prompt) {
        int val;
        while (true) {
            cout << prompt;
            if (cin >> val) return val;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
        }
    }

public:
    University() {
        ifstream f("courses.txt");
        string c, n, p;
        int cr, cp, t;
        while (f >> c >> n >> cr >> cp >> t >> p)
            cs.emplace_back(c, n, cr, cp, p, t);

        ifstream g("students.txt");
        int id, k;
        string x;
        while (g >> id >> n >> k) {
            Student s(id, n);
            while (k-- > 0 && (g >> x)) s.done.push_back(x);
            if (g >> k) {
                while (k-- > 0 && (g >> x)) s.regd.push_back(x);
            }
            ss.push_back(s);
        }

        // Seed default catalog on first run
        if (cs.empty()) {
            cs.emplace_back("CS101", "Programming", 3, 30);
            cs.emplace_back("CS102", "OOP", 3, 25, "CS101");
            cs.emplace_back("CS201", "DataStructures", 3, 25, "CS102");
            cs.emplace_back("CS301", "Database", 3, 20, "CS201");
            cs.emplace_back("MT101", "Calculus", 3, 40);
        }
    }

    ~University() {
        ofstream f("courses.txt");
        for (const auto& c : cs)
            f << c.code << " " << c.name << " " << c.credits << " "
              << c.cap << " " << c.taken << " " << c.prereq << "\n";

        ofstream g("students.txt");
        for (const auto& s : ss) {
            g << s.getId() << " " << s.getName() << " " << s.done.size();
            for (const auto& d : s.done) g << " " << d;
            g << " " << s.regd.size();
            for (const auto& r : s.regd) g << " " << r;
            g << "\n";
        }
    }

    void list() const {
        cout << "\n" << left << setw(8) << "Code" << setw(16) << "Name" << setw(5) << "Cr"
             << setw(10) << "Seats" << "Prereq\n";
        cout << string(48, '-') << "\n";
        for (const auto& c : cs)
            cout << setw(8) << c.code << setw(16) << c.name << setw(5) << c.credits
                 << setw(10) << (to_string(c.cap - c.taken) + "/" + to_string(c.cap)) << c.prereq << "\n";
    }

    void reg(Student& s) {
        list();
        string c;
        cout << "Course code: ";
        cin >> c;
        Course* x = course(c);
        if (!x) throw string("Course not found");
        if (s.has(s.regd, c)) throw string("Already registered");
        if (s.has(s.done, c)) throw string("Already completed");
        if (x->full()) throw string("Course is full");
        if (x->prereq != "none" && !s.has(s.done, x->prereq))
            throw string("Prerequisite needed: " + x->prereq);
        if (credits(s) + x->credits > Student::MAX)
            throw string("Credit limit exceeded (max 18)");
        s.regd.push_back(c);
        x->taken++;
        cout << "Registered in " << x->name << "\n";
    }

    void drop(Student& s) {
        string c;
        cout << "Course code to drop: ";
        cin >> c;
        auto it = find(s.regd.begin(), s.regd.end(), c);
        if (it == s.regd.end()) throw string("You are not registered in that course");
        s.regd.erase(it);
        Course* x = course(c);
        if (x && x->taken > 0) x->taken--;
        cout << "Dropped " << c << "\n";
    }

    void schedule(Student& s) {
        cout << "\nStudent: " << s.getName() << " (#" << s.getId() << ")\nRegistered:";
        if (s.regd.empty()) cout << " none";
        for (const auto& c : s.regd) cout << " " << c;
        cout << "\nCompleted:";
        if (s.done.empty()) cout << " none";
        for (const auto& c : s.done) cout << " " << c;
        cout << "\nCredits: " << credits(s) << "/" << Student::MAX << "\n";
    }

    void studentMenu(Student& s) {
        int c;
        do {
            c = readInt("\n1.Courses 2.Register 3.Drop 4.My schedule 5.Back\nChoice: ");
            try {
                if (c == 1) list();
                else if (c == 2) reg(s);
                else if (c == 3) drop(s);
                else if (c == 4) schedule(s);
            } catch (const string& e) {
                cout << "Error: " << e << "\n";
            }
        } while (c != 5);
    }

    void newStudent() {
        string n;
        cout << "Name (one word): ";
        cin >> n;
        int id = 2001 + static_cast<int>(ss.size());
        ss.emplace_back(id, n);
        cout << "Account created! Your ID: " << id << "\n";
    }

    void login() {
        int id = readInt("Student ID: ");
        Student* s = student(id);
        if (!s) cout << "Student not found\n";
        else studentMenu(*s);
    }

    void admin() {
        string pw;
        cout << "Admin password: ";
        cin >> pw;
        if (pw != "admin123") {
            cout << "Wrong password\n";
            return;
        }
        int c;
        do {
            c = readInt("\n1.Add course 2.Mark completed 3.View courses 4.Back\nChoice: ");
            try {
                if (c == 1) {
                    string code, n, p;
                    cout << "Code Name(one word): ";
                    cin >> code >> n;
                    int cr = readInt("Credits: ");
                    int cp = readInt("Seats: ");
                    cout << "Prereq(or none): ";
                    cin >> p;
                    if (course(code)) throw string("Course already exists");
                    cs.emplace_back(code, n, cr, cp, p);
                    cout << "Course added\n";
                } else if (c == 2) {
                    int id = readInt("Student ID: ");
                    string code;
                    cout << "Course code: ";
                    cin >> code;
                    Student* s = student(id);
                    if (!s) throw string("Student not found");
                    auto it = find(s->regd.begin(), s->regd.end(), code);
                    if (it == s->regd.end()) throw string("Student is not registered in that course");
                    s->regd.erase(it);
                    s->done.push_back(code);
                    Course* x = course(code);
                    if (x && x->taken > 0) x->taken--;
                    cout << "Marked completed\n";
                } else if (c == 3) list();
            } catch (const string& e) {
                cout << "Error: " << e << "\n";
            }
        } while (c != 4);
    }
};

int main() {
    cout << "Course Registration System - by Aun Haider Niazi\n";
    University u;
    int c;
    do {
        c = 0;
        cout << "\n=== COURSE REGISTRATION ===\n1.Student login 2.New student 3.Admin 4.Exit\nChoice: ";
        if (!(cin >> c)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        if (c == 1) u.login();
        else if (c == 2) u.newStudent();
        else if (c == 3) u.admin();
    } while (c != 4);
    return 0;
}
