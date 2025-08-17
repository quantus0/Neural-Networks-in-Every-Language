#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double init_w() { return ((double)rand()) / ((double)RAND_MAX); }
double act(double x) { return 1 / (1 + exp(-x)); }
double dact(double x) { return x * (1 - x); }

void shuffle(int *arr, size_t n) {
    if (n > 1) {
        for (size_t i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = arr[j];
            arr[j] = arr[i];
            arr[i] = t;
        }
    }
}

#define IN_N 2
#define OUT_N 1
#define H_N 2
#define T_N 4

int main(void) {
    const double lr = 0.1;

    double h[H_N];
    double o[OUT_N];

    double hb[H_N];
    double ob[OUT_N];

    double hw[IN_N][H_N];
    double ow[H_N][OUT_N];

    double t_in[T_N][IN_N] = {
        {0.0, 0.0},
        {1.0, 0.0},
        {0.0, 1.0},
        {1.0, 1.0}
    };

    double t_out[T_N][OUT_N] = {
        {0.0},
        {1.0},
        {1.0},
        {0.0}
    };

    for (int i = 0; i < IN_N; i++) {
        for (int j = 0; j < H_N; j++) {
            hw[i][j] = init_w();
        }
    }
    for (int i = 0; i < H_N; i++) {
        for (int j = 0; j < OUT_N; j++) {
            ow[i][j] = init_w();
        }
    }
    for (int i = 0; i < H_N; i++) {
        hb[i] = init_w();
    }
    for (int i = 0; i < OUT_N; i++) {
        ob[i] = init_w();
    }

    int order[] = {0, 1, 2, 3};
    int epochs = 10000;

    FILE *f = fopen("nn_output.txt", "w");
    if (!f) {
        fprintf(stderr, "File error\n");
        return 1;
    }

    for (int e = 0; e < epochs; e++) {
        shuffle(order, T_N);

        for (int x = 0; x < T_N; x++) {
            int i = order[x];

            for (int j = 0; j < H_N; j++) {
                double a = hb[j];
                for (int k = 0; k < IN_N; k++) {
                    a += t_in[i][k] * hw[k][j];
                }
                h[j] = act(a);
            }

            for (int j = 0; j < OUT_N; j++) {
                double a = ob[j];
                for (int k = 0; k < H_N; k++) {
                    a += h[k] * ow[k][j];
                }
                o[j] = act(a);
            }

            char buf[256];
            snprintf(buf, sizeof(buf), "Input: (%g, %g)    Output: %g    Expected: %g\n",
                     t_in[i], t_in[i], o, t_out[i]);
            fputs(buf, f);

            double d_o[OUT_N];
            for (int j = 0; j < OUT_N; j++) {
                double err = (t_out[i][j] - o[j]);
                d_o[j] = err * dact(o[j]);
            }

            double d_h[H_N];
            for (int j = 0; j < H_N; j++) {
                double err = 0.0;
                for (int k = 0; k < OUT_N; k++) {
                    err += d_o[k] * ow[j][k];
                }
                d_h[j] = err * dact(h[j]);
            }

            for (int j = 0; j < OUT_N; j++) {
                ob[j] += d_o[j] * lr;
                for (int k = 0; k < H_N; k++) {
                    ow[k][j] += h[k] * d_o[j] * lr;
                }
            }

            for (int j = 0; j < H_N; j++) {
                hb[j] += d_h[j] * lr;
                for (int k = 0; k < IN_N; k++) {
                    hw[k][j] += t_in[i][k] * d_h[j] * lr;
                }
            }
        }
    }

    fputs("Final HW:\n", f);
    for (int i = 0; i < IN_N; i++) {
        for (int j = 0; j < H_N; j++) {
            char buf[64];
            snprintf(buf, sizeof(buf), "%g ", hw[i][j]);
            fputs(buf, f);
        }
        fputs("\n", f);
    }

    fputs("Final HB:\n", f);
    for (int i = 0; i < H_N; i++) {
        char buf;
        snprintf(buf, sizeof(buf), "%g ", hb[i]);
        fputs(buf, f);
    }
    fputs("\n", f);

    fputs("Final OW:\n", f);
    for (int i = 0; i < H_N; i++) {
        for (int j = 0; j < OUT_N; j++) {
            char buf[64];
            snprintf(buf, sizeof(buf), "%g ", ow[i][j]);
            fputs(buf, f);
        }
        fputs("\n", f);
    }

    fputs("Final OB:\n", f);
    for (int i = 0; i < OUT_N; i++) {
        char buf;
        snprintf(buf, sizeof(buf), "%g ", ob[i]);
        fputs(buf, f);
    }
    fputs("\n", f);

    fclose(f);
    return 0;
}
