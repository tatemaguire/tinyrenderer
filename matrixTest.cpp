// Author: Tate Maguire
// September 1, 2025

#include <iostream>
#include "geometry.h"

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {


    Matrix H = Matrix({
        {2,5,0,8},
        {1,4,2,6},
        {7,8,9,3},
        {1,5,7,8}});

    cout << H << endl;
    
    // Test: Empty matrix
    Matrix G;
    cout << G << endl;
    // Test: Copy assignment
    G = H;
    G(0, 0) = 70;
    cout << H << endl;
    cout << G << endl;

    H = std::move(G);

    cout << H << endl;

    // Test: identity()
    G = Matrix::identity(4);
    cout << G << endl;
    G = Matrix::identity(1);
    cout << G << endl;
    G = Matrix::identity(0);
    H = Matrix();

    cout << ((G == H)?"true":"false") << endl;
    
    G = Matrix::identity(2);
    H = Matrix({{1, 0}, {0, 1}});
    
    cout << ((G == H)?"true":"false") << endl;
    H *= 2;

    cout << G << endl;
    cout << (G += 5) << endl;
    cout << (G -= 0.1) << endl;
    cout << (G *= 0.5) << endl << endl;
    cout << (G *= H) << endl;
    cout << (G /= 3) << endl;

    H = G / 7;
    cout << H << endl;

    // Test Inverse

    Matrix mateo = Matrix({
        {2,5,0,8},
        {1,4,2,6},
        {7,8,9,3},
        {1,5,7,8}});
    Matrix maddox = Matrix({
        {0.960893854748603,-1.916201117318436,0.078212290502793,0.446927374301676},
        {-1.033519553072626,2.357541899441341,0.067039106145251,-0.759776536312849},
        {-0.005586592178771,-0.273743016759777,0.011173184357542,0.206703910614525},
        {0.53072625698324,-0.994413407821229,-0.06145251396648,0.363128491620112}});

    // mateo.set(vals);
    
    Matrix oetam = inverse(mateo);
    cout << endl << mateo << endl << endl;

    cout << (oetam == maddox ? "Correct" : "Incorrect") << endl;
    cout << oetam.to_string(8) << endl;

    return 0;
}