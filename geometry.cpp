// Author: Tate Maguire
// September 3, 2025

#include "geometry.h"
#include <vector>
#include <cmath>
#include <limits>
#include <cassert>
#include <sstream>

Matrix::Matrix(): rows{0}, cols{0} {
    m = new float[0];
}

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

Matrix::Matrix(int r, int c, std::string vals): rows{r}, cols{c} {
    if (r<0 || c<0) {
        throw std::domain_error("Matrix construction: both dimensions must be > 0");
    }
    m = new float[r*c];
    char trash;
    std::istringstream iss {vals.c_str()};
    for (int i=0; i<r; i++) {
        for (int j=0; j<c; j++) {
            float val;
            iss >> val >> trash;
            m[getindex(i, j)] = val;
        }
    }
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
    float& val = m[getindex(r, c)];
    return val;
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

void Matrix::add_row_scaled(int from, float scale, int to) {
    for (int i=0; i<cols; i++) {
        float& elem = get(to, i);
        elem += get(from, i) * scale;
        // if close to zero set to zero
        if (std::abs(elem) < 1e-5) elem = 0;
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
    Matrix A = *this;
    Matrix B = Matrix::identity(size);

    int current_row = 0;
    // first pass of gaussian elimination
    for (int col=0; col<size; col++) {
        // bring first row with non-zero entry up to current_row
        bool zero_col = true;
        for (int row=current_row; row<size; row++) {
            if (A.get(row, col) != 0) {
                A.swap_rows(current_row, row);
                B.swap_rows(current_row, row);
                zero_col = false;
                break;
            }
        }
        if (zero_col) continue;
        assert(A.get(current_row, col) != 0);
        // make all rows below current_row have a zero in this column
        for (int row=current_row+1; row<size; row++) {
            if (A.get(row, col) != 0) {
                float scale = -A.get(row, col)/A.get(current_row, col);
                A.add_row_scaled(current_row, scale, row);
                B.add_row_scaled(current_row, scale, row);
            }
            assert(A.get(row, col) == 0);
        }
        // scale current_row to have the pivot entry 1
        float scale = 1.f/A.get(current_row, col);
        A.scale_row(current_row, scale);
        B.scale_row(current_row, scale);

        // std::cout << current_row << ": \n" << A << std::endl << std::endl;

        current_row++;
        if (current_row == size) break;
    }

    // A is now in row echelon form
    // second pass of gaussian elimination
    for (current_row=size-1; current_row>=0; current_row--) {
        for (int col=0; col<size; col++) {
            if (A.get(current_row, col) != 0) {
                for (int row=current_row-1; row>=0; row--) {
                    if (A.get(row, col) != 0) {
                        float scale = -A.get(row, col)/A.get(current_row, col);
                        A.add_row_scaled(current_row, scale, row);
                        B.add_row_scaled(current_row, scale, row);
                    }
                    assert(A.get(row, col) == 0);
                }
                break;
            }
        }
    }

    // A is now in reduced row echelon form, check if it is the identity matrix
    if (A != Matrix::identity(size)) {
        // if not identity, there is no inverse
        throw std::domain_error("Matrix: inverse(): this matrix has no inverse");
    }
    // B is now the inverse of *this
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
            float diff = this->get(i, j) - b.get(i, j);
            if (std::abs(diff) > 1e-5) {
                return false;
            }
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix& b) const {
    return !(*this == b);
}

std::ostream& operator<<(std::ostream& s, const Matrix& m) {
    s << '[';
    for (int r=0; r<m.nrows(); r++) {
        for (int c=0; c<m.ncols(); c++) {
            float val = m.get(r, c);
            if (val == 0) val = 0; // turn negative zero into positive zero
            std::string str = std::to_string(val).substr(0,5);
            s << str << (c == m.ncols()-1 ? "," : ", ");
        }
        s << (r == m.nrows()-1 ? "]" : "\n ");
    }
    return s;
}