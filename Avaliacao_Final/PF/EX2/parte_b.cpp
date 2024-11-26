#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>

std::vector<double> normalize(std::vector<double>& v) {
    double sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (size_t i = 0; i < v.size(); ++i) {
        sum += v[i] * v[i];
    }

    double magnitude = sqrt(sum);
    #pragma omp parallel for
    for (size_t i = 0; i < v.size(); ++i) {
        v[i] /= magnitude;
    }

    return v;
}

int main() {
    std::vector<double> vec{1.0, 2.0, 3.0, 4.0};
    vec = normalize(vec);
    for (auto v : vec) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    return 0;
}