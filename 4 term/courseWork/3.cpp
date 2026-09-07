#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <queue>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Item {
    int id, quality;
    double time;
};

struct Machine {
    bool busy = false;
    double remain = 0;
    int mods = 0;
    vector<Item> input;
    int outId;
    double start;
    double speed() { return 1.0 + 0.1 * mods; }
};

int main(int argc, char* argv[]) {
    if (argc < 6) {
        cerr << "Usage: " << argv[0] << " AM RM n col1 ... coln t_sb t_pr s1 ... sn\n";
        return 1;
    }

    int AM = stoi(argv[1]);
    int RM = stoi(argv[2]);
    int n = stoi(argv[3]);

    vector<int> recipe(n);
    for (int i = 0; i < n; ++i)
        recipe[i] = stoi(argv[4 + i]);

    double t_sb = stod(argv[4 + n]);
    double t_pr = stod(argv[5 + n]);

    vector<double> s(n);
    for (int i = 0; i < n; ++i)
        s[i] = stod(argv[6 + n + i]);

    mt19937 rng(random_device{}());
    vector<int> base(n, 0);
    vector<double> frac(n, 0.0);
    vector<vector<int>> high(n);
    queue<Item> products;
    vector<double> times3, times4, times5;
    int nextId = 1;
    double time = 0;
    bool done = false;
    int totalProducts = 0;
    vector<int> qualityCounts(6, 0);

    vector<Machine> assemblers(AM), recyclers(RM);

    for (auto& m : assemblers) m.mods = 4;
    for (auto& m : recyclers) m.mods = 4;

    auto canTake = [&]() {
        for (int i = 0; i < n; ++i)
            if (base[i] + (int)high[i].size() < recipe[i]) return false;
        return true;
    };

    auto take = [&]() {
        vector<Item> comps;
        for (int i = 0; i < n; ++i) {
            int need = recipe[i];
            auto& h = high[i];
            sort(h.begin(), h.end(), greater<int>());
            while (need > 0 && !h.empty()) {
                comps.push_back({-1, h.back(), time});
                h.pop_back();
                --need;
            }
            while (need > 0 && base[i] > 0) {
                comps.push_back({-1, 1, time});
                --base[i];
                --need;
            }
        }
        return comps;
    };

    auto record = [&](const Item& it) {
        totalProducts++;
        int q = it.quality;
        if (q >= 1 && q <= 5) qualityCounts[q]++;
        if (q == 3) times3.push_back(it.time);
        if (q == 4) times4.push_back(it.time);
        if (q == 5) times5.push_back(it.time);
        if (q >= 3) products.push(it);
        if (times5.size() >= 25) done = true;
    };

    while (!done) {
        for (int i = 0; i < n; ++i) {
            frac[i] += s[i];
            if (frac[i] >= 1.0) {
                int add = (int)frac[i];
                base[i] += add;
                frac[i] -= add;
            }
        }

        for (auto& m : assemblers) {
            if (!m.busy && canTake()) {
                auto comps = take();
                m.busy = true;
                m.remain = t_sb * m.speed();
                m.start = time;
                m.input = comps;
                m.outId = nextId++;
            }
        }

        for (auto& m : recyclers) {
            if (!m.busy && !products.empty()) {
                auto p = products.front(); products.pop();
                m.busy = true;
                m.remain = t_pr * m.speed();
                m.start = time;
                m.input = {{-1, p.quality, p.time}};
                m.outId = -1;
            }
        }

        for (auto& m : assemblers) {
            if (m.busy) {
                m.remain -= 1.0;
                if (m.remain <= 0.001) {
                    m.busy = false;
                    int maxQ = 1;
                    for (auto& c : m.input) if (c.quality > maxQ) maxQ = c.quality;
                    int L = maxQ;
                    if (m.mods > 0) {
                        for (int k = 1; k <= m.mods; ++k) {
                            if (L + k <= 5) {
                                double p = 52.0 * pow(10.0, -2.0 - k);
                                bernoulli_distribution d(p);
                                if (d(rng)) { L += k; break; }
                            }
                        }
                    }
                    if (L > 5) L = 5;
                    record({m.outId, L, m.start});
                    m.input.clear();
                }
            }
        }

        for (auto& m : recyclers) {
            if (m.busy) {
                m.remain -= 1.0;
                if (m.remain <= 0.001) {
                    m.busy = false;
                    int q = m.input[0].quality;
                    if (m.mods > 0) {
                        for (int k = 1; k <= m.mods; ++k) {
                            if (q + k <= 5) {
                                double p = 52.0 * pow(10.0, -2.0 - k);
                                bernoulli_distribution d(p);
                                if (d(rng)) { q += k; break; }
                            }
                        }
                    }
                    if (q > 5) q = 5;
                    for (int i = 0; i < n; ++i) {
                        int cnt = recipe[i] / 4;
                        for (int k = 0; k < cnt; ++k) high[i].push_back(q);
                    }
                    m.input.clear();
                }
            }
        }

        time += 1.0;

    }

    cout << "\n=== Коллекции времён создания продуктов ===\n";
    cout << "Уровень 3: ";
    for (double t : times3) cout << t << " ";
    cout << "\n";
    cout << "Уровень 4: ";
    for (double t : times4) cout << t << " ";
    cout << "\n";
    cout << "Уровень 5: ";
    for (double t : times5) cout << t << " ";
    cout << "\n\n";

    cout << "СТАТИСТИКА КАЧЕСТВА (из " << totalProducts << " продуктов):\n";
    const char* names[] = {"", "Обычные", "Необычные", "Редкие", "Эпические", "Легендарные"};
    for (int q = 1; q <= 5; ++q) {
        double pct = 100.0 * qualityCounts[q] / totalProducts;
        cout << "- " << names[q] << " (" << q << "): " << qualityCounts[q]
             << " (" << fixed << setprecision(2) << pct << "%)\n";
    }

    return 0;
}
// 2 1 2 3 4 10 5 5 3