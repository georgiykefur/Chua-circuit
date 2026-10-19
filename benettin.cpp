#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "params.h"
#include "chua.h"
#include "dopri5.h"
#include "AB4.h"

using namespace std;

void calculate_lyapunov(bool use_ab4) {
    const char* method = use_ab4 ? "AB4" : "DOPRI5";

    int steps = lround(lyap_interval / lyap_step);
    int warm = lround(lyap_warmup / lyap_interval);
    int blocks = lround(lyap_duration / lyap_interval);

    // два близких начальных состояния
    double state[3] = {initial_x, initial_y, initial_z};
    double nearby[3];

    for (int i = 0; i < 3; i++) {
        nearby[i] = state[i] + lyap_delta / sqrt(3.0);
    }

    double sum = 0.0;

    ofstream file(use_ab4 ? "lyapunov_ab4.csv" : "lyapunov.csv");
    file << setprecision(17);
    file << "tau,measurement_time,lambda,local_lambda\n";

    for (int block = 0; block < warm + blocks; block++) {
        double history[4][3] = {};
        double near_history[4][3] = {};

        if (use_ab4) {
            derivatives(state, history[0]);
            derivatives(nearby, near_history[0]);
        }

        for (int n = 0; n < steps; n++) {
            double next[3];
            double near_next[3];
            double error[3];

            if (use_ab4 && n >= 3) {
                AB4_step(state, history, lyap_step, next);
                AB4_step(nearby, near_history, lyap_step, near_next);
            } else {
                dopri5_step(derivatives, state, lyap_step, next, error);
                dopri5_step(derivatives, nearby, lyap_step, near_next, error);
            }

            for (int i = 0; i < 3; i++) {
                if (!isfinite(next[i]) || !isfinite(near_next[i])
                    || abs(next[i]) > amplitude_limit
                    || abs(near_next[i]) > amplitude_limit) {
                    cout << method << "превышен порог амплитуды\n";
                    return;
                }

                state[i] = next[i];
                nearby[i] = near_next[i];
            }

            if (use_ab4) {
                for (int j = 3; j > 0; j--) {
                    for (int i = 0; i < 3; i++) {
                        history[j][i] = history[j - 1][i];
                        near_history[j][i] = near_history[j - 1][i];
                    }
                }

                derivatives(state, history[0]);
                derivatives(nearby, near_history[0]);
            }
        }

        // расстояние между траекториями
        double distance = 0.0;

        for (int i = 0; i < 3; i++) {
            double difference = nearby[i] - state[i];
            distance += difference * difference;
        }

        distance = sqrt(distance);
        
        if (distance == 0.0 || !isfinite(distance)) {
            cout << method << ": расстояние между траекториями потеряно. ";
            return;
        }

        // логарифм увеличения расстояния
        double stretching = log(distance / lyap_delta);

        if (block >= warm) {
            sum += stretching;

            double elapsed = (block - warm + 1) * lyap_interval;
            double lambda = sum / elapsed;

            file << (block + 1) * lyap_interval << ','
                 << elapsed << ','
                 << lambda << ','
                 << stretching / lyap_interval << '\n';
        }

        for (int i = 0; i < 3; i++) {
            nearby[i] = state[i]
                + lyap_delta * (nearby[i] - state[i]) / distance;
        }
    }

    file << "# finished\n";

    cout << method << ": показатель Ляпунова = "
         << sum / lyap_duration << '\n';
}

int main() {
    calculate_lyapunov(false);  // DOPRI5
    calculate_lyapunov(true);   // AB4

    return 0;
}