// Author: Tate Maguire
// September 1, 2025

#include <iostream>
#include "geometry.h"

int matrixTest(int argc, char* argv[]) {
    Matrix matthew = Matrix(4, 4);
    matthew.set(0, 3, 1445.5);
    matthew.set(0, 1, 1.5382);
    matthew.set(2, 1, 1382);

    Matrix maddie = Matrix::identity(4);
    maddie.set(0, 0, 2);
    maddie *= 2;

    matthew.get(0, 1) += 3;

    std::cout << matthew << std::endl << std::endl;
    std::cout << maddie << std::endl << std::endl;
    std::cout << maddie*matthew << std::endl<<std::endl;

    Matrix madthew = maddie*matthew;
    madthew = madthew.transpose();
    std::cout << madthew <<std::endl<<std::endl;
    
    madthew.swap_rows(0, 1);
    std::cout << madthew <<std::endl<<std::endl;
    
    madthew.scale_row(0,0.5);
    std::cout << madthew <<std::endl<<std::endl;
    
    madthew.add_row_scaled(0, 4, 3);
    std::cout << madthew <<std::endl<<std::endl;



    // float vals[9] {1, 1, -1, 1, 0, 1, 2, 1, 1};
    // Matrix mateo = Matrix(3, 3, vals);
    // float solvals[9] {1, 2, -1, -1, -3, 2, -1, -1, 1};
    // Matrix solution = Matrix(3, 3, solvals);

    // float vals[4] {-1, 1, 1, 0};
    // Matrix mateo = Matrix(2, 2, vals);
    // float solvals[4] {0, 1, 1, 1};
    // Matrix solution = Matrix(2, 2, solvals);

    // float vals[4] {0, 1, 1, 3};
    // Matrix mateo = Matrix(2, 2, vals);
    // float solvals[4] {0, 1, 1, 1};
    // Matrix solution = Matrix(2, 2, solvals);

    Matrix mateo = Matrix(4, 4,
        "2,5,0,8,"
        "1,4,2,6,"
        "7,8,9,3,"
        "1,5,7,8");
    Matrix maddox = Matrix(4, 4, 
        "0.960893854748603,-1.916201117318436,0.078212290502793,0.446927374301676,"
        "-1.033519553072626,2.357541899441341,0.067039106145251,-0.759776536312849,"
        "-0.005586592178771,-0.273743016759777,0.011173184357542,0.206703910614525,"
        "0.53072625698324,-0.994413407821229,-0.06145251396648,0.363128491620112");

    // mateo.set(vals);
    
    Matrix oetam = mateo.inverse();
    std::cout << std::endl << mateo << std::endl << std::endl;

    std::cout << (oetam == maddox ? "Correct" : "Incorrect") << std::endl;
    std::cout << oetam << std::endl;

    return 0;
}