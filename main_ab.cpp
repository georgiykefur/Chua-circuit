#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "params.h"
#include "chua.h"
#include "dopri5.h"
#include "AB4.h"

using namespace std;

int main() {
    double step = tau_end / ab4_steps;
    double state[3] = {initial_x, initial_y, initial_z};
    double next[3], error[3];
    double history[4][3] = {};
    derivatives(state, history[0]);

    ofstream file("trajectory_ab4.csv");
    if (!file) return 1;
    file << setprecision(17) << "tau,x,y,z\n";
    file << 0 << ',' << state[0] << ',' << state[1] << ',' << state[2] << '\n';

    for (int n = 0; n < ab4_steps; n++) {
        // первые три шага DOPRI5
        if (n < 3) dopri5_step(derivatives, state, step, next, error);
        else AB4_step(state, history, step, next);

        for (int i = 0; i < 3; i++) {
            state[i] = next[i];
        }
        for (int j = 3; j > 0; j--)
            for (int i = 0; i < 3; i++) history[j][i] = history[j - 1][i];
        derivatives(state, history[0]);

        double tau = (n + 1 == ab4_steps) ? tau_end : (n + 1) * step;
        file << tau << ',' << state[0] << ',' << state[1] << ',' << state[2] << '\n';
            if (abs(state[0]) > amplitude_limit || abs(state[1]) > amplitude_limit
                || abs(state[2]) > amplitude_limit) {
                file << "# amplitude_limit\n";
                file.close();
                return file ? 0 : 1;
            }

    }
    file << "# finished\n";
    file.close();
    if (!file) return 1;
    cout << "AB4: trajectory_ab4.csv готов, шаг = " << step << '\n';
}
