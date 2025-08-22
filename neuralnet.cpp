#include <cstddef>
#include <iostream>
#include <numeric>
#include <vector>
#include <cmath>

double f1(const double &x) {
    return 1 / (1 + std::exp(-x));
}

double f2(const double &x) {
    return x * (1 - x);
}

std::vector<double> f3(const std::vector<double> &v) {
    std::vector<double> r(v.size());
    for (std::size_t i = 0; i < v.size(); i++) {
        r[i] = std::exp(v[i]);
    }
    double s = std::accumulate(r.begin(), r.end(), 0.0);
    for (std::size_t i = 0; i < v.size(); i++) {
        r[i] /= s;
    }
    return r;
}

int main() {
    std::vector<std::vector<double>> a = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}
    };

    std::vector<std::vector<double>> b = {
        {0},
        {1},
        {1},
        {0}
    };

    int n1 = 2, n2 = 2, n3 = 1;

    std::vector<std::vector<double>> w1(n1, std::vector<double>(n2));
    std::vector<double> b1(n2);
    std::vector<std::vector<double>> w2(n2, std::vector<double>(n3));
    std::vector<double> b2(n3);

    int e = 10000;
    double lr = 0.1;

    for (int t = 0; t < e; ++t) {
        for (int i = 0; i < a.size(); ++i) {
            std::vector<double> h(n2);
            for (int j = 0; j < n2; ++j) {
                double s = 0;
                for (int k = 0; k < n1; ++k) {
                    s += a[i][k] * w1[k][j];
                }
                h[j] = f1(s + b1[j]);
            }

            std::vector<double> o(n3);
            for (int j = 0; j < n3; ++j) {
                double s = 0;
                for (int k = 0; k < n2; ++k) {
                    s += h[k] * w2[k][j];
                }
                o[j] = f1(s + b2[j]);
            }

            std::vector<double> d3(n3);
            for (int j = 0; j < n3; ++j) {
                d3[j] = (b[i][j] - o[j]) * f2(o[j]);
            }

            std::vector<double> d2(n2);
            for (int j = 0; j < n2; ++j) {
                double s = 0;
                for (int k = 0; k < n3; ++k) {
                    s += d3[k] * w2[j][k];
                }
                d2[j] = s * f2(h[j]);
            }

            for (int j = 0; j < n3; ++j) {
                for (int k = 0; k < n2; ++k) {
                    w2[k][j] += lr * d3[j] * h[k];
                }
                b2[j] += lr * d3[j];
            }

            for (int j = 0; j < n2; ++j) {
                for (int k = 0; k < n1; ++k) {
                    w1[k][j] += lr * d2[j] * a[i][k];
                }
                b1[j] += lr * d2[j];
            }
        }
    }

    for (int i = 0; i < a.size(); ++i) {
        std::vector<double> h(n2);
        for (int j = 0; j < n2; ++j) {
            double s = 0;
            for (int k = 0; k < n1; ++k) {
                s += a[i][k] * w1[k][j];
            }
            h[j] = f1(s + b1[j]);
        }

        std::vector<double> o(n3);
        for (int j = 0; j < n3; ++j) {
            double s = 0;
            for (int k = 0; k < n2; ++k) {
                s += h[k] * w2[k][j];
            }
            o[j] = f1(s + b2[j]);
        }

        std::cout << "Input: " << a[i][0] << " " << a[i] << ", Output: " << o << std::endl;
    }

    return 0;
}
