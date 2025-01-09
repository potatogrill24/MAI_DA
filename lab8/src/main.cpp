#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

double calculate_area(int a, int b, int c) {
    double s = (a + b + c) / 2.0;
    return std::sqrt(s * (s - a) * (s - b) * (s - c));
}

std::vector<int> read_segments(int N) {
    std::vector<int> segments(N);
    for (int& segment : segments) {
        std::cin >> segment;
    }
    return segments;
}

bool is_valid_triangle(int a, int b, int c) {
    return a < b + c;
}

std::tuple<double, int, int, int> find_max_area_triangle(const std::vector<int>& segments) {
    double max_area = 0.0;
    int a = 0, b = 0, c = 0;

    for (size_t i = 0; i < segments.size() - 2; ++i) {
        if (is_valid_triangle(segments[i], segments[i + 1], segments[i + 2])) {
            double area = calculate_area(segments[i], segments[i + 1], segments[i + 2]);
            if (area > max_area) {
                max_area = area;
                a = segments[i + 2];
                b = segments[i + 1];
                c = segments[i];
            }
        }
    }

    return {max_area, a, b, c};
}

void print_result(double max_area, int a, int b, int c) {
    if (max_area == 0.0) {
        std::cout << "0" << std::endl;
    } else {
        std::cout << std::fixed << std::setprecision(3) << max_area << std::endl;
        std::cout << a << " " << b << " " << c << std::endl;
    }
}

int main() {
    int N;
    std::cin >> N;

    auto segments = read_segments(N);
    std::sort(segments.rbegin(), segments.rend());

    auto [max_area, a, b, c] = find_max_area_triangle(segments);
    print_result(max_area, a, b, c);

    return 0;
}