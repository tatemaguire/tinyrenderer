// Author: Tate Maguire
// December 9, 2025

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

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