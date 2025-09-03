// Author: Tate Maguire
// September 1, 2025

#include <iostream>
#include "geometry.h"

int main(int argc, char* argv[]) {
    // Matrix matthew = Matrix(4, 4);
    // matthew.set(0, 3, 1445.5);
    // matthew.set(0, 1, 1.5382);
    // matthew.set(2, 1, 1382);

    // Matrix maddie = Matrix::identity(4);
    // maddie.set(0, 0, 2);
    // maddie *= 2;

    // matthew.get(0, 1) += 3;

    // std::cout << matthew << std::endl << std::endl;
    // std::cout << maddie << std::endl << std::endl;
    // std::cout << maddie*matthew << std::endl<<std::endl;

    // Matrix madthew = maddie*matthew;
    // madthew = madthew.transpose();
    // std::cout << madthew <<std::endl<<std::endl;
    
    // madthew.swap_rows(0, 1);
    // std::cout << madthew <<std::endl<<std::endl;
    
    // madthew.scale_row(0,0.5);
    // std::cout << madthew <<std::endl<<std::endl;
    
    // madthew.add_to_row(0, 3, 4);
    // std::cout << madthew <<std::endl<<std::endl;

    float vals[9] {1, 1, -1, 1, 0, 1, 2, 1, 1};
    Matrix mateo = Matrix(3, 3, vals);
    Matrix carrick = Matrix(3, 3, vals);
    float solvals[9] {1, 2, -1, -1, -3, 2, -1, -1, 1};
    Matrix solution = Matrix(3, 3, solvals);
    // mateo.set(vals);
    
    const Matrix inv = mateo.inverse();
    std::cout << mateo << std::endl << std::endl;

    std::cout << (inv == solution ? "Correct" : "Incorrect") << std::endl;
    std::cout << inv << std::endl;

    return 0;
}