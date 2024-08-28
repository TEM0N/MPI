#include <iostream>
#include <cmath>
#include <omp.h>
#include <chrono>

double f(double x) {
    return pow(x, 3);
}

double calc_h(double a, double b, int N) {
    return ((b - a) / N);
}

double Xk(double a, double h, int k) {
    return (a + h * k);
}

double calc_f2_(double x) {
    return 6 * x;
}

int main() {
    int N;
    std::cout << "N: ";
    std::cin >> N;

    double a = -1.0;
    double b = 3.0;

    double h = calc_h(a, b, N);
    double max_f2_ = -INFINITY;
    double f2_;
    auto start_time = omp_get_wtime();

#pragma omp parallel for reduction(max:max_f2_)
    for (int i = 0; i <= N; i++) {
        f2_ = abs((f(Xk(a, h, i + 1)) - 2 * f(Xk(a, h, i)) + f(Xk(a, h, i - 1))) / (h * h));
        if (max_f2_ < f2_) {
            max_f2_ = f2_;
        }
        std::cout << "Thread " << omp_get_thread_num() << ", f'' = " << f2_ << " fcalc: " << calc_f2_(Xk(a, h, i)) << std::endl;
    }

    std::cout << "Max value |f''(x)| on [" << a << ", " << b << "]" << " for N = " << N << " is " << max_f2_ << std::endl;

    auto time_end = omp_get_wtime();
    std::cout << "Time: " << time_end - start_time << " sec" << std::endl;

    return 0;
}
