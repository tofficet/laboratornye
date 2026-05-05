#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <filesystem>

#include <boost/multiprecision/cpp_dec_float.hpp>

using namespace boost::multiprecision;

using Decimal = number<cpp_dec_float<1000>>;

struct Result {
    Decimal value;
    long long operations;
};

std::string format(const Decimal& val, int precision) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(precision) << val;
    return ss.str();
}

// e^r через ряд Тейлора
// e^r = sum (r^n / n!)
Result compute_e_power_r(int r, int precision) {
    Decimal result = 0;
    Decimal term = 1;

    long long n = 0;
    long long ops = 0;

    int extra = 10; 
    Decimal eps = pow(Decimal(10), -(precision + extra));

    while (true) {
        result += term; ops++;

        n++;

        Decimal next_term = term * Decimal(r) / Decimal(n);
        ops += 2;

        if (abs(next_term) < eps) break;

        term = next_term;
    }

    return {result, n, ops};
}

// arctan(x) = x - x^3/3 + x^5/5 - ...
Decimal arctan_series(const Decimal& x, int precision,
                      long long& iters, long long& ops) {
    Decimal result = 0;
    Decimal term = x;

    Decimal x2 = x * x; ops++;

    int extra = 10;
    Decimal eps = pow(Decimal(10), -(precision + extra));

    long long n = 0;

    for (long long k = 0;; k++) {
        long long m = 2 * k + 1;

        Decimal add = term / Decimal(m); ops++;

        if (k % 2 == 0)
            result += add;
        else
            result -= add;
        ops++;

        n++;

        if (abs(add) < eps) break;

        term *= x2; ops++;
    }

    iters = n;
    return result;
}

// по формуле Мэчина
// pi = 16 arctan(1/5) − 4 arctan(1/239)
Result compute_pi(int precision) {
    long long ops = 0;
    long long it1 = 0, it2 = 0;

    Decimal a1 = arctan_series(Decimal(1) / 5, precision, it1, ops);
    Decimal a2 = arctan_series(Decimal(1) / 239, precision, it2, ops);

    Decimal value = 16 * a1 - 4 * a2;
    ops += 3;

    return {value, it1 + it2, ops};
}

int main() {
    int r;
    std::cout << "Введите r (натуральное число): ";
    std::cin >> r;

    std::vector<int> precisions = {10, 20, 50, 100, 200, 500, 1000};

    std::filesystem::create_directory("data");

    std::ofstream e_file("data/e_series.csv");
    e_file << "precision,r,value,operations\n";

    std::ofstream pi_file("data/pi_series.csv");
    pi_file << "precision,value,operations\n";

    for (int prec : precisions) {
        std::cout << "\nТочность: " << prec << " знаков\n";

        auto pi_res = compute_pi(prec);

        std::cout << "pi = " << format(pi_res.value, prec) << "\n";
        std::cout << "  операций: " << pi_res.operations << "\n";

        pi_file << prec << ","
                << format(pi_res.value, prec) << ","
                << pi_res.operations << "\n";

        auto e_res = compute_e_power_r(r, prec);

        std::cout << "e^" << r << " = "
                  << format(e_res.value, prec) << "\n";
        std::cout << "  операций: " << e_res.operations << "\n";

        e_file << prec << ","
               << r << ","
               << format(e_res.value, prec) << ","
               << e_res.operations << "\n";
    }

    e_file.close();
    pi_file.close();

    return 0;
}