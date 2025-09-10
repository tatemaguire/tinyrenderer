// Author: Tate Maguire
// September 3, 2025

#include "geometry.h"
#include <vector>
#include <cmath>
#include <limits>
#include <cassert>
#include <sstream>

///////////////////////// Private Row Operations //////////////////////////

// Swap rows in-place
void Matrix::swap_rows(int r1, int r2) {
    for (int i=0; i<cols; i++) {
        std::swap(get(r1, i), get(r2, i));
    }
}

// Scale a row by the scalar s
void Matrix::scale_row(int r, float s) {
    for (int i=0; i<cols; i++) {
        get(r, i) *= s;
    }
}

// Add row 'from' times scalar s to row 'to'
void Matrix::add_row_scaled(float s, int from, int to) {
    for (int i=0; i<cols; i++) {
        float& elem = get(to, i);
        elem += get(from, i) * s;
        // if close to zero set to zero
        if (std::abs(elem) < 1e-5) elem = 0;
    }
}

//////////////// Constructors and Destructor ///////////////////////////////

// Empty Constructor
Matrix::Matrix(): Matrix(0, 0) {}

// Zero Constructor
Matrix::Matrix(int r, int c) {
    if (r<0 || c<0) {
        throw std::domain_error("Matrix construction: both dimensions must be >= 0");
    }
    if (r==0 || c==0) {
        rows = 0;
        cols = 0;
        m = nullptr;
    } else {
        rows = r;
        cols = c;
        m = new float[r*c]{0};
    }
}

// Initializer List Constructor
Matrix::Matrix(std::initializer_list<std::initializer_list<float>> il):
    Matrix(static_cast<int>(il.size()), static_cast<int>(il.begin()->size())) // delegate constructor
{
    int i{0}, j{0};
    for (const std::initializer_list<float>& row : il) {
        j = 0;
        for (const float& val : row) {
            m[getindex(i, j)] = val;
            j++;
        }
        i++;
    }
}

// Destructor
Matrix::~Matrix() {
    delete[] m;
}

// Copy Constructor
Matrix::Matrix(const Matrix& M):
    Matrix(M.nrows(), M.ncols()) // delegate constructor
{
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            get(i, j) = M.get(i, j);
        }
    }

    std::cout << "Matrix Copy Constructor" << std::endl;
}

// Move Constructor
Matrix::Matrix(Matrix&& M):
    Matrix() // delegate constructor
{
    std::swap(m, M.m);
    std::swap(rows, M.rows);
    std::swap(cols, M.cols);

    std::cout << "Matrix Move Constructor" << std::endl;
}

// Alternate Constructor: Identity Matrix
Matrix Matrix::identity(int size) {
    if (size<0) throw std::domain_error("Matrix: identity(): size must be >= 0");
    Matrix M = Matrix(size, size);
    for (int i=0; i<size; i++) {
        M.get(i, i) = 1;
    }
    return M;
}

//////////////////// Convert To String //////////////////////

// TODO: make it so empty lists have both brackets, make it so last element doesn't have comma after it
std::string Matrix::to_string() const {
    std::string s{'['};
    for (int r=0; r<rows; r++) {
        for (int c=0; c<cols; c++) {
            float val = get(r, c);
            if (val == 0) val = 0; // turn negative zero into positive zero
            s += std::to_string(val).substr(0,5);
            s += (c == cols-1 ? "," : ", ");
        }
        s += (r == rows-1 ? "]" : "\n ");
    }
    return s;
}

/////////////////// Assignment Operators /////////////////////////

// Copy Assignment
Matrix& Matrix::operator=(const Matrix& M) {
    if (this != &M) { // if it's not self assignment
        Matrix temp = M; // make copy

        std::swap(m, temp.m);
        std::swap(rows, temp.rows);
        std::swap(cols, temp.cols);

        // temp is destroyed here
    }

    std::cout << "Matrix Copy Assignment" << std::endl;

    return *this;
}

// Move Assignment
Matrix& Matrix::operator=(Matrix&& M) {
    if (this != &M) {
        std::swap(m, M.m);
        std::swap(rows, M.rows);
        std::swap(cols, M.cols);

        delete[] M.m;

        M.m = nullptr;
        M.rows = 0;
        M.cols = 0;
    }

    std::cout << "Matrix Move Assignment" << std::endl;
    
    return *this;
}

//////////////// Arithmetic Assignment Operators //////////////////

// In-place addition
Matrix& Matrix::operator+=(float b) {

}

// In-place subtraction
Matrix& Matrix::operator-=(float b) {

}

// In-place scalar multiplication
Matrix& Matrix::operator*=(float b) {
    Matrix c = *this * b;
    return *this = c;
}

// In-place matrix multiplication
Matrix& Matrix::operator*=(const Matrix& M) {
    if (this->cols != M.rows) throw std::domain_error("Matrix operator *=: invalid matrix sizes");
    Matrix P = Matrix(this->rows, M.cols);
    for (int i=0; i<this->rows; i++) {
        for (int j=0; j<M.cols; j++) {
            float product {0};
            for (int k=0; k<this->cols; k++) { //reminder: this->cols == M.rows
                product += this->get(i, k) * M.get(k, j);
            }
            P.get(i, j) = product;
        }
    }
    return *this = P;
}

// In-place scalar division
Matrix& Matrix::operator/=(float b) {

}

//////////////////// Insertion Operator ////////////////////

std::ostream& operator<<(std::ostream& s, const Matrix& M) {
    return s << M.to_string();
 }

//////////////////// Arithmetic Operators ////////////////

// Matrix operator*(const float b) const {
//     Matrix c = Matrix(this->rows, this->cols);
//     for (int i=0; i<c.nrows(); i++) {
//         for (int j=0; j<c.ncols(); j++) {
//             c.set(i, j, this->get(i, j) * b);
//         }
//     }
//     return c;
// }

Matrix operator+(const Matrix& A, float b) {

}

Matrix operator-(const Matrix& A, float b) {

}

Matrix operator*(const Matrix& A, float b) {

}

Matrix operator*(const Matrix& A, const Matrix& B) {

}

Matrix operator/(const Matrix& A, float b) {

}

/////////////// Comparison Operators ////////////////

bool operator==(const Matrix& A, const Matrix& B) {
    if (A.nrows() != B.nrows() || A.ncols() != B.ncols()) {
        return false;
    }
    for (int i=0; i<A.nrows(); i++) {
        for (int j=0; j<A.ncols(); j++) {
            float diff = A(i, j) - B(i, j);
            if (std::abs(diff) > 1e-5) {
                return false;
            }
        }
    }
    return true;
}

bool operator!=(const Matrix& A, const Matrix& B) {
    return !(A == B);
}

//////////////////// Special Matrix Operations //////////////////

Matrix transpose(const Matrix& M) {
    Matrix T {M.ncols(), M.nrows()};
    for (int i=0; i<M.nrows(); i++) {
        for (int j=0; j<M.ncols(); j++) {
            T(j, i) = M(i, j);
        }
    }
    return T;
}

Matrix inverse(const Matrix& M) {
    if (M.nrows() != M.ncols()) throw std::domain_error("Matrix: inverse(): matrix must be a square matrix (rows==cols)");

    int size = M.nrows();
    Matrix A = M;
    Matrix B = Matrix::identity(size);

    int current_row = 0;
    // first pass of gaussian elimination
    for (int col=0; col<size; col++) {
        // bring first row with non-zero entry up to current_row
        bool zero_col = true;
        for (int row=current_row; row<size; row++) {
            if (A(row, col) != 0) {
                A.swap_rows(current_row, row);
                B.swap_rows(current_row, row);
                zero_col = false;
                break;
            }
        }
        if (zero_col) continue;
        assert(A(current_row, col) != 0);
        // make all rows below current_row have a zero in this column
        for (int row=current_row+1; row<size; row++) {
            if (A(row, col) != 0) {
                float scale = -A(row, col)/A(current_row, col);
                A.add_row_scaled(scale, current_row, row);
                B.add_row_scaled(scale, current_row, row);
            }
            assert(A(row, col) == 0);
        }
        // scale current_row to have the pivot entry 1
        float scale = 1.f/A(current_row, col);
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
            if (A(current_row, col) != 0) {
                for (int row=current_row-1; row>=0; row--) {
                    if (A(row, col) != 0) {
                        float scale = -A(row, col)/A(current_row, col);
                        A.add_row_scaled(scale, current_row, row);
                        B.add_row_scaled(scale, current_row, row);
                    }
                    assert(A(row, col) == 0);
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
    // B is now the inverse of M
    return B;
}
