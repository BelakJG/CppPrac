#include <iostream>
#include <cmath>
using namespace std;

double findDistance(double t) {
    double g = 9.80665;
    double d = 0;
    double time_step = .000001;

    for (double i = time_step; i <= t; i += time_step) {
        double velocity = 63.9 * tanh((g * i) / 63.9);
        d += velocity * time_step;
    }

    return d;
}

int main() {
    cout << findDistance(5) << " m over 5 s" << endl;

    return 0;
}