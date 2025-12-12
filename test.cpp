// Author: Tate Maguire
// December 9, 2025

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <algorithm>
#include "geometry.h"

TEST_CASE("empty matrix test") {
    Matrix A {};

    CHECK(A.size() == 0);
    CHECK(A.ncols() == 0);
    CHECK(A.nrows() == 0);

    CHECK_THROWS_AS(A.at(0, 0), std::out_of_range);
    CHECK_THROWS_AS(A.at(1, 1), std::out_of_range);
    CHECK_THROWS_AS(A.at(-1, -1), std::out_of_range);

    CHECK(A.to_string() == "[]");

    double sum {};
    for (double e : A) {
        sum += e;
    }
    CHECK_MESSAGE(sum == 0, "for-range loop");

    CHECK(transpose(A) == A);
    CHECK_THROWS_AS(inverse(A), std::out_of_range);

    // Check different ways of constructing empty matrix
    Matrix B {0, 0};
    Matrix C {Matrix::identity(0)};
    CHECK(A == B);
    CHECK(A == C);
}

TEST_CASE ("2 by 2 identity matrix test") {
    Matrix A {Matrix::identity(2)};

    CHECK(A.to_string() == "[1.000, 0.000,\n 0.000, 1.000]");

    CHECK(A.size() == 4);
    CHECK(A.nrows() == 2);
    CHECK(A.ncols() == 2);
    CHECK(A(0, 0) == 1);
    CHECK(A(0, 1) == 0);
    CHECK(A.at(1, 0) == 0);
    CHECK(A.at(1, 1) == 1);

    CHECK_THROWS_AS(A.at(0, 2), std::out_of_range);

    Matrix B {{1, 0}, {0, 1}};

    CHECK(A == B);
}

TEST_CASE ("math on a 2 by 2 matrix") {
    Matrix A {{0, 1}, {2, 3}};

    CHECK(A == A);

    for (double& num : A) num *= 2;
    CHECK_MESSAGE(A == Matrix({{0, 2}, {4, 6}}), "testing for-range loop arithmetic");
    
    Matrix B = transpose(A);
    CHECK_MESSAGE(A == Matrix({{0, 2}, {4, 6}}), "checking transpose() doesn't effect parameter");
    CHECK_MESSAGE(B == Matrix({{0, 4}, {2, 6}}), "testing transpose");

    Matrix C = inverse(A);
    CHECK_MESSAGE(C == Matrix({{-0.75, 0.25}, {0.5, 0}}), "testing inverse");

    Matrix D = inverse(C);
    CHECK_MESSAGE(A == D, "testing symmetry of inversion");

    int count = std::count_if(C.begin(), C.end(), [](double& num){return num > 0;});
    CHECK_MESSAGE(count == 2, "testing iterators in an algorithm");
}

TEST_CASE("1 by 1 matrix") {
    Matrix A {1, 1};

    CHECK(A.size() == 1);
    CHECK(A.nrows() == 1);
    CHECK(A.ncols() == 1);
    CHECK(A(0, 0) == 0);

    A(0, 0) = 9.8;
    CHECK_MESSAGE(A(0, 0) == 9.8, "checking writing to matrix");

    CHECK(A.to_string() == "[9.800]");

    Matrix B {inverse(A)};
    CHECK_MESSAGE(B == Matrix({{1/9.8}}), "check inverse of 1 by 1");

    Matrix C {transpose(A)};
    CHECK_MESSAGE(C == A, "check transpose of 1 by 1");
}

TEST_CASE("const 3 by 3 identity matrix") {
    const Matrix A {Matrix::identity(3)};
    
    CHECK(A == Matrix({{1,0,0},{0,1,0},{0,0,1}}));

    CHECK(A.size() == 9);
    CHECK(A.nrows() == 3);
    CHECK(A.ncols() == 3);

    CHECK(A(0,0) == 1);
    CHECK(A.at(2,2) == 1);

    // A(0, 0) = 2;
    // A.at(0, 0) = 6;
    // A = Matrix(1, 1);

    Matrix B = A;
    B(0, 0) = 10;
    CHECK(B != A);
    CHECK(A == Matrix::identity(3));
}

TEST_CASE("row operations on 3 by 2") {
    Matrix A {{1, 2}, {3, 4}, {5, 6}};

    A.swap_rows(0, 1);
    CHECK_MESSAGE(A == Matrix({{3, 4}, {1, 2}, {5, 6}}), "test swap_rows");
    
    A.scale_row(2, 0.5);
    CHECK_MESSAGE(A == Matrix({{3, 4}, {1, 2}, {2.5, 3}}), "test scale_row");

    A.add_row_scaled(1, -3, 0);
    CHECK_MESSAGE(A == Matrix({{0, -2}, {1, 2}, {2.5, 3}}), "test add_row_scaled");

    CHECK_THROWS_AS(inverse(A), std::domain_error);
}