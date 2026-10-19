#pragma once

// выбрать набор
const int preset = 1;

const double presets[][6] = {
    // L                    G    C1                   C2   Ga          Gb
    {0.14285714285714285,   0.7, 0.1111111111111111,   1.0, -0.8,       -0.5},       // 1: default
    {0.14285714285714285,   0.7, 0.1111111111111111,   1.0, -0.72,      -0.57},      // 2: onecoil
    {0.14285714285714285,   0.7, 0.1111111111111111,   1.0, -0.6,       -0.3},       // 3: periodic
    {0.14285714285714285,   0.5, 0.1111111111111111,   1.0, -0.3,       -0.1},       // 4: chaotic
    {0.1,                   1.0, 1.0 / 6.0,            1.0, -8.0 / 7.0, -5.0 / 7.0}  // 5: periodic2
};

const double L  = presets[preset - 1][0];
const double G  = presets[preset - 1][1];
const double C1 = presets[preset - 1][2];
const double C2 = presets[preset - 1][3];
const double Ga = presets[preset - 1][4];
const double Gb = presets[preset - 1][5];

const double alpha = C2 / C1;
const double beta = C2 / (L * G * G);
const double m0 = Ga / G;
const double m1 = Gb / G;

// начальные условия
const double initial_x = 0.1;
const double initial_y = 0.0;
const double initial_z = 0.0;
const double tau_end = 200.0;

const double first_step = 0.01;
const double max_step = 0.1;
const double abs_tol = 1e-9;
const double rel_tol = 1e-6;

const int ab4_steps = 200000;

const double amplitude_limit = 1e6;

const double lyap_step = 0.005;
const double lyap_delta = 1e-7;       // начальное расстояние между траекториями
const double lyap_interval = 0.5;    // как часто возвращаем расстояние к delta
const double lyap_warmup = 100.0;    // переходный процесс не входит в среднее
const double lyap_duration = 600.0;  // длительность измерения после перехода
