#include <cmath>
#include "chua.h"
#include "params.h"

using namespace std;

// диод
double h(double x) {
    return m1 * x + (m0 - m1) / 2.0 * (abs(x + 1.0) - abs(x - 1.0));
}

void derivatives(double state[3], double ders[3]){
    double x = state[0];
    double y = state[1];
    double z = state[2];

    ders[0] = alpha * (y - x - h(x));
    ders[1] = x - y + z;
    ders[2] = -beta*y;
}
