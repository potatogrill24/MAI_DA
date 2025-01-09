#include <iostream>
#include <cmath>

unsigned int len(unsigned long long n) {
    unsigned int l = 0;
    while (n) {
        n /= 10;
        l++;
    }
    return l;
}

long long count(unsigned long long n, unsigned long long m) {
    long long count;

    if (n % m == 0) {
        count = -1;
    }
    else {
        count = 0;
    }

    while (n != 0) {
        if (len(n) != len(n - 1)) {
            break;
        }
        unsigned long long limit = pow(10, len(n) - 1);
        if (limit % m != 0 || limit == 0) {
            limit = limit;
        }
        else {
            limit--;
        }
        count += n / m - limit / m;
        n /= 10;
    }
    return count;
}

int main () {
    unsigned long long n, m;
    long long res = 0;
    std::cin >> n >> m;
    res = count(n, m);
    if (res < 0) {
        std::cout << 0 << std::endl;
    }
    else {
        std::cout << res << std::endl;
    }
    return 0;
}