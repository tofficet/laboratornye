#include <iostream>
#include <cmath> 

class matrix {
private:
    double** x;
    size_t n, m;

public:
    matrix(size_t n, size_t m);
    ~matrix();
    matrix(const matrix& other);
    matrix& operator=(const matrix& other);

    matrix sum(const matrix& other) const;
    matrix elementwise_multiply(const matrix& other) const;
    matrix multiply_by_scalar(double scalar) const;
    matrix subtract(const matrix& other) const;
    matrix transpose() const;
    double determinant() const;
    matrix inverse() const; 
    void print() const;
    double* operator[](size_t i) {return x[i];}
    const double* operator[](size_t i) const {return x[i];}

};

matrix::matrix(size_t n, size_t m) : n(n), m(m) {
    x = new double*[n];
    for (size_t i = 0; i < n; ++i) {
        x[i] = new double[m];
        for (size_t j = 0; j < m; ++j) {
            x[i][j] = 0.0;
        }
    }
}

matrix::~matrix() {
    for (size_t i = 0; i < n; ++i) {
        delete[] x[i];
    }
    delete[] x;
}

matrix::matrix(const matrix& other) : n(other.n), m(other.m) {
    x = new double*[n];
    for (size_t i = 0; i < n; ++i) {
        x[i] = new double[m];
        for (size_t j = 0; j < m; ++j) {
            x[i][j] = other.x[i][j];
        }
    }
}

matrix& matrix::operator=(const matrix& other) {
    if (this == &other) return *this; 

    for (size_t i = 0; i < n; ++i) {
        delete[] x[i];
    }
    delete[] x;

    n = other.n;
    m = other.m;
    x = new double*[n];
    for (size_t i = 0; i < n; ++i) {
        x[i] = new double[m];
        for (size_t j = 0; j < m; ++j) {
            x[i][j] = other.x[i][j];
        }
    }

    return *this;
}

matrix matrix::sum(const matrix& other) const {
    matrix result(n, m);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            result.x[i][j] = x[i][j] + other.x[i][j];
        }
    }

    return result;
}

matrix matrix::elementwise_multiply(const matrix& other) const {

    matrix result(n, m);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            result.x[i][j] = x[i][j] * other.x[i][j];
        }
    }

    return result;
}


matrix matrix::multiply_by_scalar(double scalar) const {
    matrix result(n, m);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            result.x[i][j] = x[i][j] * scalar;
        }
    }

    return result;
}


matrix matrix::subtract(const matrix& other) const {

    matrix result(n, m);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            result.x[i][j] = x[i][j] - other.x[i][j];
        }
    }

    return result;
}

matrix matrix::transpose() const {
    matrix result(m, n);
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            result.x[i][j] = x[j][i];
        }
    }

    return result;
}


double matrix::determinant() const {
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

        if (temp.x[maxRow][i] == 0) {
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

    if (swapCount % 2 == 1) {
        det *= -1;
    }

    return det;
}


matrix matrix::inverse() const {

    double det = determinant();


    matrix result(n, n);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {

            matrix minor(n - 1, n - 1);
            for (size_t k = 0; k < n; ++k) {
                for (size_t l = 0; l < n; ++l) {
                    if (k != i && l != j) {
                        size_t minorRow = k < i ? k : k - 1;
                        size_t minorCol = l < j ? l : l - 1;
                        minor.x[minorRow][minorCol] = x[k][l];
                    }
                }
            }
            double cofactor = minor.determinant();
            if ((i + j) % 2 == 1) {
                cofactor *= -1;
            }
            result.x[j][i] = cofactor / det;
        }
    }

    return result;
}


void matrix::print() const {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            std::cout << x[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


int main() {
    matrix mat1(2, 2);
    matrix mat2(2, 2);

    mat1[0][0] = 4; mat1[0][1] = 7;
    mat1[1][0] = 2; mat1[1][1] = 6;

    mat2[0][0] = 1; mat2[0][1] = 3;
    mat2[1][0] = 5; mat2[1][1] = 2;


    mat1.print();
    mat2.print();
    
    matrix mat3 = mat1.sum(mat2);
    std::cout << "Sum: " << std::endl;
    mat3.print();

    matrix mat4 = mat1.elementwise_multiply(mat2);
    std::cout << "Element-wise multiplication: " << std::endl;
    mat4.print();

    matrix mat5 = mat1.multiply_by_scalar(5);
    std::cout << "Multiplication by scalar (5): " << std::endl;
    mat5.print();

    matrix mat6 = mat1.subtract(mat2);
    std::cout << "Subtraction: " << std::endl;
    mat6.print();

    matrix mat7 = mat1.transpose();
    std::cout << "Transpose: " << std::endl;
    mat7.print();

    double det = mat1.determinant();
    std::cout << "Determinant: " << det << std::endl;

    matrix inv = mat1.inverse();
    std::cout << "Inverse matrix: " << std::endl;
    inv.print();

    return 0;
}