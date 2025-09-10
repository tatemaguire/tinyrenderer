// Vector Author: Dimitry V. Sokolov (I think?)
// Matrix Author: Tate Maguire
// September 3, 2025

#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
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

class Matrix {
private:
	// Matrix represented as an array of entries
	float* m;
	int rows, cols;

	// get the index of m that represents element (r, c)
	inline int getindex(int r, int c) const {return c + r*cols;}

	// Row operations for gaussian elimination
	void scale_row(int r, float s);
	void add_row_scaled(float s, int from, int to);
	void swap_rows(int r1, int r2);

	float& get(int r, int c) const {return m[getindex(r, c)];}

public:
	// Constructors and Destructor
	Matrix();
	Matrix(int r, int c);
	Matrix(std::initializer_list<std::initializer_list<float>>);
	~Matrix();

	// Copy and move constructors
	Matrix(const Matrix&);
	Matrix(Matrix&&);

	// Alternate constructor, constructs identity natrix
	static Matrix identity(int size);

	// Get functions
	inline float& operator()(int r, int c) const {return m[getindex(r, c)];}
	inline int nrows() const {return rows;}
	inline int ncols() const {return cols;}

	// Convert To String
	std::string to_string() const;

	// Assignment operators
	Matrix& operator=(const Matrix&);
	Matrix& operator=(Matrix&&);
	Matrix& operator+=(float);
	Matrix& operator-=(float);
	Matrix& operator*=(float);
	Matrix& operator*=(const Matrix&);
	Matrix& operator/=(float);
	
	// Friends!!! :D <3
	friend std::ostream& operator<<(std::ostream& s, const Matrix& M);
	friend Matrix transpose(const Matrix&);
	friend Matrix inverse(const Matrix&);
};

// Arithmetic Operators
Matrix operator+(const Matrix& A, float b);
Matrix operator-(const Matrix& A, float b);
Matrix operator*(const Matrix& A, float b);
Matrix operator*(const Matrix& A, const Matrix& B);
Matrix operator/(const Matrix& A, float b);

// Comparison Operators
bool operator==(const Matrix& A, const Matrix& B);
bool operator!=(const Matrix& A, const Matrix& B);

// Special Matrix Operations
Matrix transpose(const Matrix&);
Matrix inverse(const Matrix&);

#endif //__GEOMETRY_H__
