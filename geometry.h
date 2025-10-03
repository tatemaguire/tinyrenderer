// Vector Author: Dimitry V. Sokolov (I think?)
// Matrix Author: Tate Maguire
// September 3, 2025

#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <cassert>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class t> struct Vec2 {
	union {
		struct {t u, v;};
		struct {t x, y;};
		t raw[2];
	};
	Vec2() : u(0), v(0) {}
	Vec2(t _u, t _v) : u(_u),v(_v) {}
	inline Vec2<t> operator +(const Vec2<t> &V) const { return Vec2<t>(u+V.u, v+V.v); }
	inline Vec2<t> operator -(const Vec2<t> &V) const { return Vec2<t>(u-V.u, v-V.v); }
	inline Vec2<t> operator *(float f)          const { return Vec2<t>(u*f, v*f); }
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
};

template <class t> struct Vec3 {
	union {
		struct {t x, y, z;};
		struct { t ivert, iuv, inorm; };
		t raw[3];
	};
	Vec3() : x(0), y(0), z(0) {}
	Vec3(t _x, t _y, t _z) : x(_x),y(_y),z(_z) {}
	inline Vec3<t> operator ^(const Vec3<t> &v) const { return Vec3<t>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
	inline Vec3<t> operator +(const Vec3<t> &v) const { return Vec3<t>(x+v.x, y+v.y, z+v.z); }
	inline Vec3<t> operator -(const Vec3<t> &v) const { return Vec3<t>(x-v.x, y-v.y, z-v.z); }
	inline Vec3<t> operator *(float f)          const { return Vec3<t>(x*f, y*f, z*f); }
	inline t       operator *(const Vec3<t> &v) const { return x*v.x + y*v.y + z*v.z; }
	float norm () const { return std::sqrt(x*x+y*y+z*z); }
	Vec3<t> & normalize(t l=1) { *this = (*this)*(l/norm()); return *this; }
	template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;

template <class t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v) {
	s << "(" << v.x << ", " << v.y << ")";
	return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, Vec3<t>& v) {
	s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return s;
}

// ------------------------------------------------------------------------------------
// ------------------------------------ Matrix ----------------------------------------
// ------------------------------------------------------------------------------------

template <typename T>
class Matrix {
private:
	// Matrix represented as a vector of 
	std::vector<T> m;
	size_t rows, cols;

	T& get(size_t r, size_t c) {return m[r*cols + c];}
	const T& get(size_t r, size_t c) const {return m[r*cols + c];}
public:
	// Constructors and Destructor
	inline Matrix(): Matrix(0, 0) {}
	Matrix(size_t r, size_t c);
	Matrix(std::initializer_list<std::initializer_list<T>>);

	static Matrix identity(size_t size);

	T& at(size_t r, size_t c);
	const T& at(size_t r, size_t c) const;
	inline T& operator()(size_t r, size_t c) {return get(r, c);}
	inline const T& operator()(size_t r, size_t c) const {return get(r, c);}
	inline size_t nrows() const {return rows;}
	inline size_t ncols() const {return cols;}

	std::string to_string(unsigned char digits=5) const;

	Matrix& operator+=(const Matrix& M);
	Matrix& operator-=(const Matrix& M);
	Matrix& operator*=(const Matrix& M);
	
	inline Matrix& operator+=(T b) {for (T& val : m) {val += b;} return *this;}
	inline Matrix& operator-=(T b) {for (T& val : m) {val -= b;} return *this;}
	inline Matrix& operator*=(T b) {for (T& val : m) {val *= b;} return *this;}
	inline Matrix& operator/=(T b) {for (T& val : m) {val /= b;} return *this;}

	inline size_t size() const {return m.size();}
	inline auto begin() {return m.begin();}
	inline auto begin() const {return m.begin();}
	inline auto end() {return m.end();}
	inline auto end() const {return m.end();}

	void scale_row(size_t r, T s);
	void add_row_scaled(T s, size_t from, size_t to);
	void swap_rows(size_t r1, size_t r2);
};

template <typename T>
inline std::ostream& operator<<(std::ostream& s, const Matrix<T>& M) {return s << M.to_string(5);}

template <typename T> inline Matrix<T> operator+(Matrix<T> A, T b) {return A += b;}
template <typename T> inline Matrix<T> operator+(T b, Matrix<T> A) {return A += b;}
template <typename T> inline Matrix<T> operator-(Matrix<T> A, T b) {return A -= b;}
template <typename T> inline Matrix<T> operator-(T b, Matrix<T> A) {for (T& val : A) {val = b-val;} return A;}
template <typename T> inline Matrix<T> operator*(Matrix<T> A, T b) {return A *= b;}
template <typename T> inline Matrix<T> operator*(T b, Matrix<T> A) {return A *= b;}
template <typename T> inline Matrix<T> operator/(Matrix<T> A, T b) {return A /= b;}
template <typename T> inline Matrix<T> operator/(T b, Matrix<T> A) {for (T& val : A) {val = b/val;} return A;}

template <typename T> inline Matrix<T> operator+(Matrix<T> A, const Matrix<T>& B) {return A += B;}
template <typename T> inline Matrix<T> operator-(Matrix<T> A, const Matrix<T>& B) {return A -= B;}
template <typename T> inline Matrix<T> operator*(Matrix<T> A, const Matrix<T>& B) {return A *= B;}
template <typename T> inline Matrix<T> operator-(Matrix<T> A) {return A *= -1;}

template <typename T> bool operator==(const Matrix<T>& A, const Matrix<T>& B);
template <typename T> inline bool operator!=(const Matrix<T>& A, const Matrix<T>& B) {return !(A == B);}

template <typename T> Matrix<T> transpose(const Matrix<T>&);
template <typename T> Matrix<T> inverse(const Matrix<T>&);

///////////////////////////////////////////////////////////////////////
///////////////////// Matrix Method Definitions ///////////////////////
///////////////////////////////////////////////////////////////////////

//////////////// Constructors and Destructor //////////////////

// Empty Constructor
// Matrix::Matrix(): Matrix(0, 0) {}

// Zero Constructor
template <typename T>
Matrix<T>::Matrix(size_t r, size_t c):
    m{},
    rows{r},
    cols{c}
{
	m = std::vector<T>(r*c, 0);
}

// Initializer List Constructor
template <typename T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> ilil):
    m{},
    rows{ilil.size()},
    cols{ilil.begin()->size()}
{
	for (const auto& initializerRow : ilil) {
		for (const T& element : initializerRow) {
			m.push_back(element);
		}
	}
}

/////////////////// Alternate Constructor //////////////////////////
template <typename T>
Matrix<T> Matrix<T>::identity(size_t size) {
    Matrix M = Matrix(size, size);
    for (size_t i=0; i<size; i++) {
        M(i,i) = 1;
    }
    return M;
}

///////////////// Get/Subscript Operations /////////////////

// Get reference with boundary check
template <typename T>
T& Matrix<T>::at(size_t r, size_t c) {
    if (r>=rows || c>=cols) throw std::out_of_range("Matrix: at(): index out of range");
    return get(r, c);
}

// Get const reference with boundary check
template <typename T>
const T& Matrix<T>::at(size_t r, size_t c) const {
    if (r>=rows || c>=cols) throw std::out_of_range("Matrix: at(): index out of range");
    return get(r, c);
}

//////////////////// Convert To String //////////////////////

// convert to string where each val has char length 'digits'
template <typename T>
std::string Matrix<T>::to_string(unsigned char digits) const {
    std::string s{'['};
    if (rows*cols == 0) return s + ']';
    for (size_t r=0; r<rows; r++) {
        for (size_t c=0; c<cols; c++) {
            T val = get(r,c);
            if (val == 0) val = 0; // turn negative zero into positive zero
            s += std::to_string(val).substr(0, digits);
            s += (c == cols-1 ? "" : ", ");
        }
        s += (r == rows-1 ? "]" : ",\n ");
    }
    return s;
}

//////////////// Matrix Arithmetic Assignment //////////////////

// In-place matrix addition
template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& M) {
    if (cols != M.cols || rows != M.rows) throw std::out_of_range("Matrix Addition: Matrices don't match in size");

    for (size_t i=0; i<rows; i++) {
        for (size_t j=0; j<cols; j++) {
            get(i, j) += M(i, j);
        }
    }

    return *this;
}

// In-place matrix subtraction
template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& M) {
    if (cols != M.cols || rows != M.rows) throw std::out_of_range("Matrix Subtraction: Matrices don't match in size");

    for (size_t i=0; i<rows; i++) {
        for (size_t j=0; j<cols; j++) {
            get(i, j) -= M(i, j);
        }
    }

    return *this;
}

// In-place matrix multiplication
template <typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& M) {
    if (cols != M.rows) throw std::out_of_range("Matrix multiplication: matrices don't match in size");

    Matrix<T> A(rows, M.cols);
    for (size_t i=0; i<A.rows; i++) {
        for (size_t j=0; j<A.cols; j++) {
            T product{0};
            for (size_t k=0; k<rows; k++) {
                product += get(k, i) * M(j, k);
            }
            A(i, j) = product;
        }
    }

    return *this = std::move(A);
}

///////////////////////// Row Operations //////////////////////////

// Swap rows in-place
template <typename T>
void Matrix<T>::swap_rows(size_t r1, size_t r2) {
    for (size_t i=0; i<cols; i++) {
        std::swap(get(r1,i), get(r2,i));
    }
}

// Scale a row by the scalar s
template <typename T>
void Matrix<T>::scale_row(size_t r, T s) {
    for (size_t i=0; i<cols; i++) {
        get(r,i) *= s;
    }
}

// Add row 'from' times scalar s to row 'to'
template <typename T>
void Matrix<T>::add_row_scaled(T s, size_t from, size_t to) {
    for (size_t i=0; i<cols; i++) {
        T& elem = get(to,i);
        elem += get(from,i) * s;
        // if close to zero set to zero
        if (std::abs(elem) < 1e-5) elem = 0;
    }
}

/////////////// Comparison Operators ////////////////

template <typename T>
bool operator==(const Matrix<T>& A, const Matrix<T>& B) {
    if (A.nrows() != B.nrows() || A.ncols() != B.ncols()) {
        return false;
    }
    for (size_t i=0; i<A.nrows(); i++) {
        for (size_t j=0; j<A.ncols(); j++) {
            T diff = A(i, j) - B(i, j);
            if (std::abs(diff) > 1e-5) {
                return false;
            }
        }
    }
    return true;
}

//////////////////// Special Matrix Operations //////////////////

// Get transpose of matrix
template <typename T>
Matrix<T> transpose(const Matrix<T>& M) {
    Matrix<T> A {M.ncols(), M.nrows()};
    for (size_t i=0; i<M.nrows(); i++) {
        for (size_t j=0; j<M.ncols(); j++) {
            A(j, i) = M(i, j);
        }
    }
    return A;
}

// Get inverse of matrix if it exists, throw otherwise
template <typename T>
Matrix<T> inverse(const Matrix<T>& M) {
    if (M.nrows() != M.ncols()) throw std::out_of_range("Matrix: inverse(): matrix must be a square matrix (rows==cols)");

    size_t size = M.nrows();
    Matrix<T> A = M;
    Matrix<T> B = Matrix<T>::identity(size);

    size_t current_row = 0;
    // first pass of gaussian elimination
    for (size_t col=0; col<size; col++) {
        // bring first row with non-zero entry up to current_row
        bool zero_col = true;
        for (size_t row=current_row; row<size; row++) {
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
        for (size_t row=current_row+1; row<size; row++) {
            if (A(row, col) != 0) {
                T scale = -A(row, col)/A(current_row, col);
                A.add_row_scaled(scale, current_row, row);
                B.add_row_scaled(scale, current_row, row);
            }
            assert(A(row, col) == 0);
        }
        // scale current_row to have the pivot entry 1
        T scale = 1.f/A(current_row, col);
        A.scale_row(current_row, scale);
        B.scale_row(current_row, scale);

        current_row++;
        if (current_row == size) break;
    }

    // A is now in row echelon form
    // second pass of gaussian elimination
    for (current_row=size-1; current_row>0; current_row--) {
        for (size_t col=0; col<size; col++) {
            if (A(current_row, col) != 0) {
                for (size_t row=current_row-1; row!=(size_t)(-1); row--) {
                    if (A(row, col) != 0) {
                        T scale = -A(row, col)/A(current_row, col);
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
    if (A != Matrix<T>::identity(size)) {
        // if not identity, there is no inverse
        throw std::domain_error("Matrix: inverse(): this matrix has no inverse");
    }
    // B is now the inverse of M
    return B;
}

#endif //__GEOMETRY_H__
