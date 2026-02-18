#include "../include/factorial.hpp"

void run_permutation_experiment(int N, int i, int j) {
    std::vector<int> perm(N);
    std::iota(perm.begin(), perm.end(), 1);
    double total = static_cast<double>(factorial(N));

    double count_i = 0.0, count_j = 0.0, count_ij = 0.0, count_union = 0.0;
    std::vector<std::vector<int>> union_perms, inter_perms;

    do {
        bool ai = (perm[i-1] == i);
        bool aj = (perm[j-1] == j);
        if (ai) count_i += 1.0;
        if (aj) count_j += 1.0;
        if (ai && aj) {
            count_ij += 1.0;
            inter_perms.push_back(perm);
        }
        if (ai || aj) {
            count_union += 1.0;
            union_perms.push_back(perm);
        }
    } while (std::next_permutation(perm.begin(), perm.end()));

    print_results3(i, j, union_perms, inter_perms,
                  count_i, count_j, count_ij, count_union, total);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Использование: " << argv[0] << " N [i j]" << std::endl;
        return 1;
    }
    int N = std::stoi(argv[1]);
    int i = 1, j = 2;             
    if (argc >= 4) {
        i = std::stoi(argv[2]);
        j = std::stoi(argv[3]);
    }
    if (i < 1 || i > N || j < 1 || j > N || i == j) {
        std::cerr << "Ошибка: i и j должны быть различными числами от 1 до N" << std::endl;
        return 1;
    }

    run_permutation_experiment(N, i, j);
    return 0;
}