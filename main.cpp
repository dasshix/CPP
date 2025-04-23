#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cstdlib>
#include <fstream>
#include <map>


using namespace std;

struct Root {
    vector<double> roots;
    bool inf = false;

    Root() {
        roots = {};
        inf = false;
    }
};

bool compare_roots(Root& r1, Root& r2) {
    if (r1.inf) {
        if (r2.inf)
            return true;
        return false;
    }

    if (r1.roots.size() != r2.roots.size())
        return false;

    for (int i = 0; i < r1.roots.size();i++)
        if (r1.roots[i] != r2.roots[i])
            return false;
    return true;
}

Root solve_square_equation(double a, double b, double c) {
    Root root;

    root.inf = (!a && !b && !c);
    if (root.inf)
        return root;

    if (!a && !b)
        return root;

    if (!a) {
        root.roots.push_back(-c / b);
        return root;
    }

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return root;

    root.roots.push_back((-b - sqrt(discriminant)) / (2 * a));

    if (!discriminant)
        return root;
    root.roots.push_back((-b + sqrt(discriminant)) / (2 * a));

    return root;
}

struct Task {
    int a;
    int b;
    int c;
    Root root;
    string name;

    Task(int a, int b, int c) : a(a), b(b), c(c) {};
};

class Teacher {
private:
    queue<Task> tasks_queue;

public:
    void get_task(Task& task) {
        tasks_queue.push(task);
    }

    map<string, int> check_all_tasks() {
        map<string, int> results;

        while (!tasks_queue.empty()) {
            Task& task = tasks_queue.front();

            Root root = solve_square_equation(task.a, task.b, task.c);

            results[task.name]++;
            if (!compare_roots(root, task.root))
                results[task.name]--;
            
            tasks_queue.pop();
        }

        return results;
    }
};

class BaseStudent {
public:
    string name;

    virtual void solve(Task& task) = 0;

    BaseStudent(string name) : name(name) {};
};

class GoodStudent : public BaseStudent {
public:
    void solve(Task& task) override {
        task.root = solve_square_equation(task.a, task.b, task.c);
        task.name = name;
    };

    GoodStudent(string _name) : BaseStudent(_name) {};
};

class AverageStudent : public BaseStudent {
public:
    void solve(Task& task) override {
        task.name = name;
        if (rand() % 100 >= 50)
            task.root = solve_square_equation(task.a, task.b, task.c);
        else
            task.root = Root();
    };

    AverageStudent(string _name) : BaseStudent(_name) {};
};

class BadStudent : public BaseStudent {
public:
    void solve(Task& task) override {
        task.name = name;
        task.root = Root();
    };

    BadStudent(string _name) : BaseStudent(_name) {};
};


vector<Task> load_tasks(string filename) {
    vector<Task> tasks;

    ifstream in(filename);
    
    vector<double> task{ 0, 0, 0 };
    int coefs_counter = 0;

    while(!in.eof()) {
        in >> task[coefs_counter];
        coefs_counter++;
        
        if (coefs_counter == 3) {
            Task current_task(task[0], task[1], task[2]);
            tasks.push_back(current_task);
            coefs_counter = 0;
        }
    };

    in.close();

    return tasks;
}

vector<BaseStudent*> load_students(string filename) {
    vector<BaseStudent*> students;

    ifstream in(filename);


    while (!in.eof()) {
        int grade;
        string name;

        in >> grade;
        in >> name;

        if (grade == 0) 
            students.push_back(new BadStudent(name));
        else if (grade == 1)
            students.push_back(new AverageStudent(name));
        else if (grade == 2)
            students.push_back(new GoodStudent(name));
    };

    in.close();

    return students;
}

int main() {
    srand(0);

    Teacher teacher;

    // Загружаем задание
    vector<Task> tasks = load_tasks("equations.txt");

    // Загружаем студентов
    vector<BaseStudent*> students = load_students("students.txt");

    // Решаем задание каждым студентом
    for (BaseStudent* student : students) {
        for (Task& task : tasks) {
            student->solve(task);
            teacher.get_task(task);
        }
    }

    // Учитель проверяет задания
    map<string, int> results = teacher.check_all_tasks();

    // Выводим результат в таблицу
    ofstream out("results.txt");
    
    for (map<string, int>::iterator iter = results.begin(); iter != results.end(); ++iter)
        out << iter->first << " " << iter->second << endl;
    
    out.close();

    return 0;
}