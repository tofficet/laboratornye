#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <cstdint>
#include <iostream>
#include <numeric>

using namespace std;

// полином в кольце Z[X]/(X^N - 1)
class ntru_poly {
public:
    size_t n;  // степень полинома
    vector<int64_t> coeffs;  // коэффициенты полинома

    ntru_poly(size_t degree) : n(degree), coeffs(degree, 0) {}

    static ntru_poly multiply(const ntru_poly& a, const ntru_poly& b, int64_t mod) {
        size_t n = a.n;
        ntru_poly result(n);
        
        for (size_t i = 0; i < n; ++i) {
            if (a.coeffs[i] == 0) 
                continue;
            for (size_t j = 0; j < n; ++j) {
                if (b.coeffs[j] == 0) continue;
                size_t k = (i + j) % n;
                result.coeffs[k] = (result.coeffs[k] + a.coeffs[i] * b.coeffs[j]) % mod;
            }
        }
        
        for (auto& c : result.coeffs) {
            if (c < 0) 
                c += mod;
        }
        return result;
    }

    void mod_poly(int64_t mod) {
        for (auto& c : coeffs) {
            c %= mod;
            if (c < 0) 
                c += mod;
        }
    }

    // приведение к диапазону [-mod/2, mod/2]
    void center_lift(int64_t mod) {
        int64_t half = mod / 2;
        for (auto& c : coeffs) {
            c %= mod;
            if (c < 0) 
                c += mod;
            if (c > half) 
                c -= mod;
        }
    }
};

class ntru_cipher {
private:
    size_t n;  // степень полинома
    int64_t p;  // малый модуль (обычно 3)
    int64_t q;  // большой модуль (степень двойки)
    int df;  // количество единиц в полиноме f
    int dg;  // количество единиц в полиноме g
    
    ntru_poly h;  // открытый ключ
    ntru_poly f;  // закрытый ключ
    ntru_poly fp;  // f^(-1) mod p
    ntru_poly fq;  // f^(-1) mod q
    
    mt19937_64 gen;

    // обратный элемент по модулю
    static int64_t mod_inverse(int64_t a, int64_t m) {
        a = a % m;
        for (int64_t x = 1; x < m; x++) {
            if ((a * x) % m == 1) 
                return x;
        }
        return 1;
    }

    // обращение полинома по модулю (метод Гаусса)
    bool invert_modp(const ntru_poly& poly, ntru_poly& inv, int64_t mod) {
        size_t N = poly.n;
        
        // строим матрицу N x 2N
        vector<vector<int64_t>> mat(N, vector<int64_t>(2 * N, 0));
        
        // заполняем циркулянтную матрицу
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                mat[i][j] = poly.coeffs[(N + i - j) % N] % mod;
                if (mat[i][j] < 0) 
                    mat[i][j] += mod;
            }
            mat[i][N + i] = 1;  // единичная матрица справа
        }

        // приведение к ступенчатому виду (метод Гаусса)
        for (size_t i = 0; i < N; ++i) {
            // поиск ненулевого элемента в столбце
            if (mat[i][i] == 0) {
                size_t swap_row = i;
                for (size_t k = i + 1; k < N; ++k) {
                    if (mat[k][i] != 0) {
                        swap_row = k;
                        break;
                    }
                }
                if (swap_row == i) 
                    return false;  // матрица необратима
                swap(mat[i], mat[swap_row]);
            }
            
            // нормализация строки
            int64_t inv_val = mod_inverse(mat[i][i], mod);
            for (size_t j = i; j < 2 * N; ++j)
                mat[i][j] = (mat[i][j] * inv_val) % mod;
            
            // обнуление остальных строк
            for (size_t k = 0; k < N; ++k) {
                if (k != i && mat[k][i] != 0) {
                    int64_t factor = mat[k][i];
                    for (size_t j = i; j < 2 * N; ++j) {
                        mat[k][j] = (mat[k][j] - factor * mat[i][j]) % mod;
                        if (mat[k][j] < 0) 
                            mat[k][j] += mod;
                    }
                }
            }
        }
        
        // извлекаем обратный полином из правой части матрицы
        for (size_t i = 0; i < N; ++i)
            inv.coeffs[i] = mat[i][N];

        return true;
    }

    // обращение полинома по модулю q (метод Hensel lifting)
    bool invert_modq(const ntru_poly& poly, ntru_poly& inv, int64_t q_target) {
        // обращаем по модулю 2
        if (!invert_modp(poly, inv, 2)) 
            return false;
        
        // Hensel lifting (поднимаем до q)
        int64_t current_q = 2;
        while (current_q < q_target) {
            current_q *= 2;
            ntru_poly temp = ntru_poly::multiply(poly, inv, current_q);
            for (auto& c : temp.coeffs) {
                c = (2 - c) % current_q;
                if (c < 0) 
                    c += current_q;
            }
            inv = ntru_poly::multiply(inv, temp, current_q);
        }
        inv.mod_poly(q_target);
        return true;
    }

    // генерация тернарного полинома с заданным количеством 1 и -1
    ntru_poly generate_ternary_poly(int num_ones, int num_minus_ones) {
        ntru_poly res(n);
        vector<int> positions(n);
        iota(positions.begin(), positions.end(), 0);
        shuffle(positions.begin(), positions.end(), gen);

        for (int i = 0; i < num_ones; ++i)
            res.coeffs[positions[i]] = 1;

        for (int i = num_ones; i < num_ones + num_minus_ones; ++i)
            res.coeffs[positions[i]] = -1;

        return res;
    }

public:
    ntru_cipher(size_t n_val = 107, int64_t p_val = 3, int64_t q_val = 128, int d_val = 15)
        : n(n_val), p(p_val), q(q_val), df(d_val), dg(d_val),
          h(n_val), f(n_val), fp(n_val), fq(n_val), gen(random_device{}()) {}

    /**
     * Генерация ключевой пары
     * 
     * Алгоритм:
     * 1. Генерируем случайный полином f (с df единицами и df-1 минус единицами)
     * 2. Проверяем, что f обратим по модулю p и q
     * 3. Генерируем случайный полином g
     * 4. Вычисляем h = p * fq * g mod q
     */
    void generate_keys() {
        while (true) {
            // генерируем полином f (с df единицами и df-1 минус единицами)
            f = generate_ternary_poly(df, df - 1);
            
            // проверяем, что f обратим по модулю p и q
            if (!invert_modp(f, fp, p)) 
                continue;
            if (!invert_modq(f, fq, q)) 
                continue;
            
            // генерируем полином g
            ntru_poly g = generate_ternary_poly(dg, dg);
            // h = p * fq * g mod q
            h = ntru_poly::multiply(fq, g, q);
            for (auto& c : h.coeffs)
                c = (c * p) % q;

            break;
        }
    }

    ntru_poly get_public_key() const { return h; }

    // e = r * h + m (mod q)
    ntru_poly encrypt_block(const ntru_poly& m) {
        ntru_poly r = generate_ternary_poly(dg, dg);
        ntru_poly e = ntru_poly::multiply(r, h, q);
        for (size_t i = 0; i < n; ++i) {
            e.coeffs[i] = (e.coeffs[i] + m.coeffs[i]) % q;
            if (e.coeffs[i] < 0) 
                e.coeffs[i] += q;
        }
        return e;
    }

    // a = f * e (mod q), m = a * fp (mod p)
    ntru_poly decrypt_block(const ntru_poly& e) {
        ntru_poly a = ntru_poly::multiply(f, e, q);
        a.center_lift(q);
        ntru_poly m = ntru_poly::multiply(a, fp, p);
        m.mod_poly(p);
        return m;
    }

    void encrypt_file(const string& in_path, const string& out_path) {
        ifstream in(in_path, ios::binary);
        if (!in) throw runtime_error("Cannot open input file: " + in_path);
        
        vector<uint8_t> data((istreambuf_iterator<char>(in)), {});
        in.close();

        // преобразуем байты в биты
        vector<int> bits;
        for (uint8_t b : data) {
            for (int i = 0; i < 8; ++i)
                bits.push_back((b >> i) & 1);
        }

        ofstream out(out_path, ios::binary);
        if (!out) 
            throw runtime_error("Cannot open output file: " + out_path);
        
        // сохраняем размер оригинальных данных
        uint64_t data_size = data.size();
        out.write(reinterpret_cast<const char*>(&data_size), sizeof(data_size));

        // шифруем каждый блок битов
        for (size_t i = 0; i < bits.size(); i += n) {
            ntru_poly m(n);
            for (size_t j = 0; j < n && i + j < bits.size(); ++j)
                m.coeffs[j] = bits[i + j];
            
            ntru_poly e = encrypt_block(m);
            
            // записываем коэффициенты как int8_t
            for (size_t j = 0; j < n; ++j) {
                int8_t val = static_cast<int8_t>(e.coeffs[j]);
                out.write(reinterpret_cast<const char*>(&val), 1);
            }
        }
    }

    void decrypt_file(const string& in_path, const string& out_path) {
        ifstream in(in_path, ios::binary);
        if (!in) throw runtime_error("Cannot open input file: " + in_path);

        // читаем размер оригинальных данных
        uint64_t data_size;
        in.read(reinterpret_cast<char*>(&data_size), sizeof(data_size));

        vector<int> bits;
        
        while (in.peek() != EOF) {
            ntru_poly e(n);
            bool read_success = false;
            
            // читаем коэффициенты
            for (size_t j = 0; j < n; ++j) {
                int8_t val = 0;
                if (in.read(reinterpret_cast<char*>(&val), 1)) {
                    e.coeffs[j] = val;
                    if (e.coeffs[j] < 0) 
                        e.coeffs[j] += q;
                    read_success = true;
                }
            }
            
            if (!read_success) 
                break;

            ntru_poly m = decrypt_block(e);
            for (size_t j = 0; j < n; ++j)
                bits.push_back(m.coeffs[j]);
        }

        // преобразуем биты обратно в байты
        ofstream out(out_path, ios::binary);
        if (!out) throw runtime_error("Cannot open output file: " + out_path);

        size_t total_bytes = 0;
        for (size_t i = 0; i < bits.size() && total_bytes < data_size; i += 8) {
            uint8_t b = 0;
            
            for (int j = 0; j < 8 && i + j < bits.size(); ++j)
                b |= (static_cast<uint8_t>(bits[i + j]) << j);

            out.write(reinterpret_cast<const char*>(&b), 1);
            total_bytes++;
        }
    }
};