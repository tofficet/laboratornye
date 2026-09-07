#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

string permToString(const vector<int>& p) {
    string s = "(";
    for (size_t k = 0; k < p.size(); ++k) {
        s += to_string(p[k]);
        if (k + 1 < p.size()) s += ", ";
    }
    s += ")";
    return s;
}

void printAll(const vector<vector<int>>& perms, const string& label) {
    if (perms.empty()) {
        cout << label << "нет\n";
        return;
    }
    cout << label << perms.size() << " шт.:\n";
    for (const auto& p : perms)
        cout << "  " << permToString(p) << "\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        cerr << "Использование: " << argv[0] << " N [--list]\n";
        cerr << "  N      – количество элементов (целое >= 2)\n";
        cerr << "  --list – выводить все благоприятствующие перестановки\n";
        return 1;
    }

    int N;
    try {
        N = stoi(argv[1]);
    } catch (...) {
        cerr << "Ошибка: N должно быть целым числом.\n";
        return 1;
    }
    if (N < 2) {
        cerr << "Ошибка: N должно быть >= 2 (нужны как минимум элементы 1 и 2).\n";
        return 1;
    }

    bool showPerms = false;
    if (argc == 3) {
        string opt = argv[2];
        if (opt == "--list" || opt == "-l")
            showPerms = true;
        else {
            cerr << "Неизвестный аргумент: " << opt << "\n";
            return 1;
        }
    }

    const int i = 1, j = 2;

    vector<int> perm(N);
    for (int k = 0; k < N; ++k)
        perm[k] = k + 1;

    vector<vector<int>> listAi, listAj, listInter, listUnion;
    long long total = 0;
    long long cntAi = 0, cntAj = 0, cntInter = 0;

    do {
        ++total;
        bool Ai = (perm[0] == 1);
        bool Aj = (perm[1] == 2);

        if (Ai) {
            ++cntAi;
            if (showPerms) listAi.push_back(perm);
        }
        if (Aj) {
            ++cntAj;
            if (showPerms) listAj.push_back(perm);
        }
        if (Ai && Aj) {
            ++cntInter;
            if (showPerms) listInter.push_back(perm);
        }
        if (Ai || Aj) {
            if (showPerms) listUnion.push_back(perm);
        }
    } while (next_permutation(perm.begin(), perm.end()));

    long long cntUnion = cntAi + cntAj - cntInter;

    double pAi_th = 1.0 / N;
    double pAj_th = 1.0 / N;
    double pInter_th = 1.0 / (N * (N - 1.0));
    double pUnion_th = 2.0 / N - pInter_th;

    double pAi = static_cast<double>(cntAi) / total;
    double pAj = static_cast<double>(cntAj) / total;
    double pInter = static_cast<double>(cntInter) / total;
    double pUnion = static_cast<double>(cntUnion) / total;
    double pUnion_formula = pAi + pAj - pInter;
    double diff = fabs(pUnion - pUnion_formula);

    cout << "\n========= РЕЗУЛЬТАТЫ ДЛЯ N = " << N << " =========\n\n";
    cout << "Общее число перестановок: " << total;
    if (N <= 12) cout << " = " << N << "!";
    cout << "\n\n";

    cout << "|A" << i << "| = " << cntAi
         << ", P(A" << i << ") = " << fixed << setprecision(6) << pAi
         << " (аналитически: 1/" << N << " = " << pAi_th << ")\n";

    cout << "|A" << j << "| = " << cntAj
         << ", P(A" << j << ") = " << pAj
         << " (аналитически: 1/" << N << " = " << pAj_th << ")\n";

    cout << "|A" << i << "∩A" << j << "| = " << cntInter
         << ", P(A" << i << "∩A" << j << ") = " << pInter
         << " (аналитически: 1/(" << N << "·" << (N-1) << ") = " << pInter_th << ")\n";

    cout << "|A" << i << "∪A" << j << "| = " << cntUnion
         << ", P(A" << i << "∪A" << j << ") = " << pUnion << "\n\n";

    cout << "Проверка теоремы сложения:\n";
    cout << "P(A" << i << ") + P(A" << j << ") - P(A" << i << "∩A" << j << ") = "
         << pAi << " + " << pAj << " - " << pInter << " = " << pUnion_formula << "\n";
    cout << "P(A" << i << "∪A" << j << ") = " << pUnion << "\n";
    cout << "Разница: " << diff << "\n\n";

    if (diff < 1e-12)
        cout << "✓ Соответствует ТЕОРЕМЕ СЛОЖЕНИЯ\n";
    else
        cout << "✗ НЕ СООТВЕТСТВУЕТ (ошибка вычислений)\n";

    if (showPerms) {
        cout << "\n--- БЛАГОПРИЯТСТВУЮЩИЕ ИСХОДЫ ---\n";
        printAll(listAi, "A" + to_string(i) + ": ");
        printAll(listAj, "A" + to_string(j) + ": ");
        printAll(listInter, "A" + to_string(i) + "∩A" + to_string(j) + ": ");
        printAll(listUnion, "A" + to_string(i) + "∪A" + to_string(j) + ": ");
    }

    return 0;
}