#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include "params.h"
#include "chua.h"
#include "dopri5.h"

using namespace std;

int main() {
    double state[3] = {initial_x, initial_y, initial_z};
    double next[3], error[3];
    double tau = 0.0;
    double step = first_step;

    ofstream file("trajectory.csv");
    if (!file) return 1;
    file << setprecision(17) << "tau,x,y,z\n";
    file << tau << ',' << state[0] << ',' << state[1] << ',' << state[2] << '\n';

    while (tau < tau_end) {
        double remaining = tau_end - tau;
        step = min(step, min(max_step, remaining));
        dopri5_step(derivatives, state, step, next, error);

        // оценка ошибки
        double err = 0.0;
        for (int i = 0; i < 3; i++) {
            double scale = abs_tol + rel_tol * max(abs(state[i]), abs(next[i]));
            double ratio = abs(error[i]) / scale;
            if (!isfinite(next[i]) || !isfinite(ratio)) {
                cerr << "Решение стало нечисловым. Проверь параметры.\n";
                return 1;
            }
            err = max(err, ratio);
        }

        // принятие шага
        if (err <= 1.0) {
            tau = (step == remaining) ? tau_end : tau + step;
            for (int i = 0; i < 3; i++) state[i] = next[i];
            file << tau << ',' << state[0] << ',' << state[1] << ',' << state[2] << '\n';
            if (abs(state[0]) > amplitude_limit || abs(state[1]) > amplitude_limit
                || abs(state[2]) > amplitude_limit) {
                file << "# amplitude_limit\n";
                file.close();
                cout << "Превышен порог амплитуды. Траектория сохранена до остановки.\n";
                return file ? 0 : 1;
            }

        }

        // если шаг отклонен
        double factor = (err == 0.0) ? 5.0 : 0.9 * pow(err, -0.2);
        step *= max(0.2, min(5.0, factor));
    }
    file << "# finished\n";
    file.close();
    if (!file) return 1;
    cout << "DOPRI5: trajectory.csv готов\n";
}
