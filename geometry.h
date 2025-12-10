// Vector Author: Dimitry V. Sokolov (I think?)
// Matrix Author: Tate Maguire
// September 3, 2025

#pragma once

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
	Vec2<t> operator +(const Vec2<t> &V) const { return Vec2<t>(u+V.u, v+V.v); }
	Vec2<t> operator -(const Vec2<t> &V) const { return Vec2<t>(u-V.u, v-V.v); }
	Vec2<t> operator *(float f)          const { return Vec2<t>(u*f, v*f); }
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
	Vec3<t> operator ^(const Vec3<t> &v) const { return Vec3<t>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
	Vec3<t> operator +(const Vec3<t> &v) const { return Vec3<t>(x+v.x, y+v.y, z+v.z); }
	Vec3<t> operator -(const Vec3<t> &v) const { return Vec3<t>(x-v.x, y-v.y, z-v.z); }
	Vec3<t> operator *(float f)          const { return Vec3<t>(x*f, y*f, z*f); }
	t       operator *(const Vec3<t> &v) const { return x*v.x + y*v.y + z*v.z; }
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
	// Matrix represented as a vector of 
	std::vector<double> m;
	size_t rows, cols;

	double& get(size_t r, size_t c) {return m[r*cols + c];}
	const double& get(size_t r, size_t c) const {return m[r*cols + c];}
public:
	// Constructors and Destructor
	Matrix(): Matrix(0, 0) {}
	Matrix(size_t r, size_t c);
	Matrix(std::initializer_list<std::initializer_list<double>>);

	static Matrix identity(size_t size);

	double& operator()(size_t r, size_t c) {return get(r, c);}
	const double& operator()(size_t r, size_t c) const {return get(r, c);}
	size_t nrows() const {return rows;}
	size_t ncols() const {return cols;}
    
	size_t size() const {return m.size();}
	auto begin() {return m.begin();}
	auto begin() const {return m.begin();}
	auto end() {return m.end();}
	auto end() const {return m.end();}

	std::string to_string(unsigned char digits=5) const;

	void scale_row(size_t r, double s);
	void add_row_scaled(double s, size_t from, size_t to);
	void swap_rows(size_t r1, size_t r2);
};

std::ostream& operator<<(std::ostream& s, const Matrix& M);

bool operator==(const Matrix& A, const Matrix& B);
bool operator!=(const Matrix& A, const Matrix& B);

Matrix transpose(const Matrix&);
Matrix inverse(const Matrix&);
