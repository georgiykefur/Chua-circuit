#include "AB4.h"

void AB4_step(
    const double state[3],
    const double history[4][3],
    double step,
    double next[3]
) {
    // AB4
    for (int i = 0; i < 3; i++) {
        next[i] = state[i] + step / 24.0 * (
            55.0 * history[0][i]
            - 59.0 * history[1][i]
            + 37.0 * history[2][i]
            - 9.0 * history[3][i]);
    }
}
