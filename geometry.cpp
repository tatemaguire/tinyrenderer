// Author: Tate Maguire
// September 1, 2025

#include "geometry.h"
#include <vector>
#include <cmath>

Matrix::Matrix(int r, int c): rows{r}, cols{c} {
    if (r<0 || c<0) {
        throw std::domain_error("Matrix construction: both dimensions must be > 0");
    }
    m = new float[r*c]{0};
}

Matrix::Matrix(int r, int c, float vals[]): rows{r}, cols{c} {
    if (r<0 || c<0) {
        throw std::domain_error("Matrix construction: both dimensions must be > 0");
    }
    m = new float[r*c];
    set(vals);
}

Matrix::Matrix(const Matrix& mat): rows{mat.nrows()}, cols{mat.ncols()} {
    m = new float[rows*cols];
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            m[getindex(i, j)] = mat.get(i, j);
        }
    }
}

Matrix Matrix::identity(int size) {
    if (size<0) throw std::domain_error("Matrix: identity(): size must be >0");
    float* vals = new float[size*size];
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            vals[i + j*size] = i==j ? 1 : 0;
        }
    }
    Matrix mat = Matrix(size, size, vals);
    delete[] vals;
    return mat;
}

Matrix::~Matrix() {
    delete[] m;
}

int Matrix::getindex(int r, int c) const {
    return r + c*rows;
}

float& Matrix::get(int r, int c) const {
    if (r<0 || r>=rows || c<0 || c>=cols) {
        throw std::domain_error("Matrix: get(): row or column out of domain");
    }
    return m[getindex(r, c)];
}

void Matrix::set(int r, int c, float val) {
    if (r<0 || r>=rows || c<0 || c>=cols) {
        throw std::domain_error("Matrix: set(): row or column out of domain");
    }
    m[getindex(r, c)] = val;
}

// vals must be of length rows*cols
void Matrix::set(float vals[]) {
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            m[getindex(i, j)] = vals[j + i*cols];
        }
    }
}

// in-place row operations
void Matrix::swap_rows(int r1, int r2) {
    for (int i=0; i<cols; i++) {
        std::swap(get(r1, i), get(r2, i));
    }
}

void Matrix::scale_row(int r, float s) {
    for (int i=0; i<cols; i++) {
        get(r, i) *= s;
    }
}

void Matrix::add_to_row(int to, int from, float s) {
    for (int i=0; i<cols; i++) {
        get(to, i) += get(from, i) * s;
    }
}

Matrix Matrix::transpose() const {
    Matrix T {cols, rows};
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            T.set(j, i, this->get(i, j));
        }
    }
    return T;
}

Matrix Matrix::inverse() const {
    if (rows != cols) throw std::domain_error("Matrix: inverse(): matrix must be a square matrix (rows==cols)");

    int size = rows;
    // Matrix A {3, 3}, B {3, 3};
    Matrix A = *this;
    Matrix B = Matrix::identity(size);

    return B;
}

Matrix Matrix::operator*(const Matrix& b) const {
    if (this->cols != b.rows) throw std::domain_error("Matrix operator *: invalid matrix sizes");
    Matrix c = Matrix(this->rows, b.cols);
    for (int i=0; i<this->rows; i++) {
        for (int j=0; j<b.cols; j++) {
            float product {0};
            for (int k=0; k<this->cols; k++) { //reminder: this->cols == b.rows
                product += this->get(i, k) * b.get(k, j);
            }
            c.set(i, j, product);
        }
    }
    return c;
}

Matrix Matrix::operator*(const float b) const {
    Matrix c = Matrix(this->rows, this->cols);
    for (int i=0; i<c.nrows(); i++) {
        for (int j=0; j<c.ncols(); j++) {
            c.set(i, j, this->get(i, j) * b);
        }
    }
    return c;
}

Matrix& Matrix::operator=(const Matrix& m) {
    for (int i=0; i<m.nrows(); i++) {
        for (int j=0; j<m.ncols(); j++) {
            this->set(i, j, m.get(i, j));
        }
    }
    return *this;
}

Matrix& Matrix::operator*=(const float b) {
    Matrix c = *this * b;
    return *this = c;
}

bool Matrix::operator==(const Matrix& b) const {
    if (this->rows != b.rows || this->cols != b.cols) {
        return false;
    }
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            if (this->get(i, j) != b.get(i, j)) {
                return false;
            }
        }
    }
    return true;
}

std::ostream& operator<<(std::ostream& s, const Matrix& m) {
    s << '[';
    for (int r=0; r<m.nrows(); r++) {
        for (int c=0; c<m.ncols(); c++) {
            auto val = std::to_string(m.get(r, c)).substr(0, 5);
            s << val << (c == m.ncols()-1 ? "," : ",\t");
        }
        s << (r == m.nrows()-1 ? "]" : "\n ");
    }
    return s;
}