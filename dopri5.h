#pragma once

using RightPart = void (*)(double*, double*);

void dopri5_step(RightPart f, double state[3], double step,
                double next[3], double error[3]);
