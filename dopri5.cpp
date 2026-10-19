#include "dopri5.h"

void dopri5_step(RightPart f, double state[3], double step, double next[3], double error[3]) {
    double k1[3], k2[3], k3[3], k4[3], k5[3], k6[3], k7[3];
    double temp[3];

    // k1 — производные в начале шага
    f(state, k1);

    // k2
    for (int i = 0; i < 3; i++) {
        temp[i] = state[i] + step * k1[i] / 5.0;
    }
    f(temp, k2);

    // k3
    for (int i = 0; i < 3; i++) {
        temp[i] = state[i] + step * (
            3.0 / 40.0 * k1[i] + 9.0 / 40.0 * k2[i]);
    }
    f(temp, k3);

    // k4
    for (int i = 0; i < 3; i++) {
        temp[i] = state[i] + step * (
            44.0 / 45.0 * k1[i] - 56.0 / 15.0 * k2[i]
            + 32.0 / 9.0 * k3[i]);
    }
    f(temp, k4);

    // k5
    for (int i = 0; i < 3; i++) {
        temp[i] = state[i] + step * (
            19372.0 / 6561.0 * k1[i] - 25360.0 / 2187.0 * k2[i]
            + 64448.0 / 6561.0 * k3[i] - 212.0 / 729.0 * k4[i]);
    }
    f(temp, k5);

    // k6
    for (int i = 0; i < 3; i++) {
        temp[i] = state[i] + step * (
            9017.0 / 3168.0 * k1[i] - 355.0 / 33.0 * k2[i]
            + 46732.0 / 5247.0 * k3[i] + 49.0 / 176.0 * k4[i]
            - 5103.0 / 18656.0 * k5[i]);
    }
    f(temp, k6);

    // приближение пятого порядка
    for (int i = 0; i < 3; i++) {
        next[i] = state[i] + step * (
            35.0 / 384.0 * k1[i] + 500.0 / 1113.0 * k3[i]
            + 125.0 / 192.0 * k4[i] - 2187.0 / 6784.0 * k5[i]
            + 11.0 / 84.0 * k6[i]);
    }
    f(next, k7);

    // приближение четвертого порядка и оценка ошибки
    for (int i = 0; i < 3; i++) {
        double next4 = state[i] + step * (
            5179.0 / 57600.0 * k1[i] + 7571.0 / 16695.0 * k3[i]
            + 393.0 / 640.0 * k4[i] - 92097.0 / 339200.0 * k5[i]
            + 187.0 / 2100.0 * k6[i] + 1.0 / 40.0 * k7[i]);

        error[i] = next[i] - next4;
    }
}
