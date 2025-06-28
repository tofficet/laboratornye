#include <cmath>
#include <iostream>
#include <stdexcept>

class matrix {
   private:
    double** x;
    size_t n, m;

    void allocate_memory();
    void deallocate_memory();
    void copy_data(const matrix& other);
    void check_dimensions(const matrix& other, const std::string& op) const;
    void check_square(const std::string& op) const;
    bool is_zero(double value) const;

   public:
    matrix(size_t n, size_t m);
    virtual ~matrix();
    matrix(const matrix& other);
    matrix& operator=(const matrix& other);

    matrix operator+(const matrix& other) const;
    matrix operator*(const matrix& other) const;
    matrix operator*(double scalar) const;
    matrix operator-(const matrix& other) const;
    matrix operator!() const;
    double operator~() const;

    matrix inverse() const;
    friend std::ostream& operator<<(std::ostream& os, const matrix& mat);
    double* operator[](size_t i) { return x[i]; }
    const double* operator[](size_t i) const { return x[i]; }
};

void matrix::allocate_memory() {
    x = new double*[n];
    for (size_t i = 0; i < n; ++i) {
        x[i] = new double[m];
        for (size_t j = 0; j < m; ++j) {
            x[i][j] = 0.0;
        }
    }
}

void matrix::deallocate_memory() {
    for (size_t i = 0; i < n; ++i) {
        delete[] x[i];
    }
    delete[] x;
}

void matrix::copy_data(const matrix& other) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            x[i][j] = other.x[i][j];
        }
    }
}

void matrix::check_dimensions(const matrix& other, const std::string& op) const {
    if (n != other.n || m != other.m) {
        throw std::invalid_argument("Matrix dimensions mismatch for " + op);
    }
}

void matrix::check_square(const std::string& op) const {
    if (n != m) {
        throw std::invalid_argument("Matrix must be square for " + op);
    }
}

bool matrix::is_zero(double value) const { return std::abs(value) < 1e-10; }

matrix::matrix(size_t n, size_t m) : n(n), m(m) { allocate_memory(); }

matrix::~matrix() { deallocate_memory(); }

matrix::matrix(const matrix& other) : n(other.n), m(other.m) {
    allocate_memory();
    copy_data(other);
}

matrix& matrix::operator=(const matrix& other) {
    if (this != &other) {
        deallocate_memory();
        n = other.n;
        m = other.m;
        allocate_memory();
        copy_data(other);
    }
    return *this;
}

matrix matrix::operator+(const matrix& other) const {
    check_dimensions(other, "addition");
    matrix result(n, m);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            result.x[i][j] = x[i][j] + other.x[i][j];
        }
    }
    return result;
}

matrix matrix::operator*(const matrix& other) const {
    if (m != other.n) {
        throw std::invalid_argument("Matrix dimensions mismatch for multiplication");
    }
    matrix result(n, other.m);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < other.m; ++j) {
            for (size_t k = 0; k < m; ++k) {
                result.x[i][j] += x[i][k] * other.x[k][j];
            }
        }
    }
    return result;
}

matrix matrix::operator*(double scalar) const {
    matrix result(n, m);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            result.x[i][j] = x[i][j] * scalar;
        }
    }
    return result;
}

matrix matrix::operator-(const matrix& other) const {
    check_dimensions(other, "subtraction");
    matrix result(n, m);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            result.x[i][j] = x[i][j] - other.x[i][j];
        }
    }
    return result;
}

matrix matrix::operator!() const {
    matrix result(m, n);
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            result.x[i][j] = x[j][i];
        }
    }
    return result;
}

double matrix::operator~() const {
    check_square("determinant");
    matrix temp(*this);
    double det = 1.0;
    size_t swapCount = 0;

    for (size_t i = 0; i < n; ++i) {
        size_t maxRow = i;
        for (size_t k = i + 1; k < n; ++k) {
            if (std::abs(temp.x[k][i]) > std::abs(temp.x[maxRow][i])) {
                maxRow = k;
            }
        }

        if (is_zero(temp.x[maxRow][i])) {
            return 0.0;
        }

        if (maxRow != i) {
            std::swap(temp.x[i], temp.x[maxRow]);
            swapCount++;
        }

        for (size_t k = i + 1; k < n; ++k) {
            double factor = temp.x[k][i] / temp.x[i][i];
            for (size_t j = i; j < n; ++j) {
                temp.x[k][j] -= factor * temp.x[i][j];
            }
        }

        det *= temp.x[i][i];
    }

    return (swapCount % 2) ? -det : det;
}

matrix matrix::inverse() const {
    check_square("inverse");
    double det = ~(*this);
    if (is_zero(det)) {
        throw std::runtime_error("Matrix is singular (determinant is zero)");
    }

    matrix result(n, n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            matrix minor(n - 1, n - 1);
            for (size_t k = 0; k < n; ++k) {
                for (size_t l = 0; l < n; ++l) {
                    if (k != i && l != j) {
                        size_t minorRow = k < i ? k : k - 1;
                        size_t minorCol = l < j ? l : l - 1;
                        minor[minorRow][minorCol] = x[k][l];
                    }
                }
            }
            double cofactor = ~minor;
            if ((i + j) % 2) {
                cofactor = -cofactor;
            }
            result[j][i] = cofactor / det;
        }
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const matrix& mat) {
    for (size_t i = 0; i < mat.n; ++i) {
        for (size_t j = 0; j < mat.m; ++j) {
            os << mat.x[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}

int main() {
    try {
        matrix mat1(2, 2);
        matrix mat2(2, 2);

        mat1[0][0] = 4;
        mat1[0][1] = 7;
        mat1[1][0] = 2;
        mat1[1][1] = 6;

        mat2[0][0] = 1;
        mat2[0][1] = 3;
        mat2[1][0] = 5;
        mat2[1][1] = 2;

        std::cout << "Matrix 1:\n" << mat1 << std::endl;
        std::cout << "Matrix 2:\n" << mat2 << std::endl;

        std::cout << "Sum:\n" << mat1 + mat2 << std::endl;
        std::cout << "Element-wise multiplication:\n" << mat1 * mat2 << std::endl;
        std::cout << "Multiplication by scalar (5):\n" << mat1 * 5 << std::endl;
        std::cout << "Subtraction:\n" << mat1 - mat2 << std::endl;
        std::cout << "Transpose:\n" << !mat1 << std::endl;
        std::cout << "Determinant: " << ~mat1 << std::endl;
        std::cout << "Inverse matrix:\n" << mat1.inverse() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}