#include <iostream>
#include <vector>


using namespace std;

struct Root {
    vector<double> roots;
    bool inf = false;
};


Root solve(double a, double b, double c) {
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


int main()
{
    double a, b, c;
    cin >> a >> b >> c;

    Root root = solve(a, b, c);

    if (root.inf) {
        cout << "Infinity solutions" << endl;
        return 0;
    }

    if (!root.roots.size())
        cout << "No solutions" << endl;

    for (double r : root.roots)
        cout << r << " ";
    cout << endl;
    return 0;
}
