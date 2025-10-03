// Author: Tate Maguire
// September 1, 2025

#include <iostream>
#include "geometry.h"

using std::cout;
using std::endl;

// THESE TESTS ARE KINDA LAME
// They aren't self-checking and I didn't think much about edge cases
// But hey it's something!

int matrixTest(int argc, char* argv[]) {

    Matrix<float> H({
        {2,5,0,8},
        {1,4,2,6},
        {7,8,9,3},
        {1,5,7,8}});

    cout << H << endl;
    
    // Test: Empty matrix
    Matrix<float> G(4,4);
    cout << G << endl;

    // Test: Identity matrix
    Matrix<int> I = Matrix<int>::identity(10);
    cout << I << endl;
    I -= 2;
    cout << I << endl;

    // Test: Copy assignment
    G = H;
    G(0, 0) = 70;
    cout << H << endl;
    cout << G << endl;

    H = std::move(G);

    cout << H << endl;


    H = Matrix<float>();
    cout << ((G == H)?"true":"false") << endl;
    
    Matrix<double> A = Matrix<double>::identity(2);
    Matrix<double> B = Matrix<double>({{1,0}, {0,1}});
    
    cout << ((A == B)?"true":"false") << endl;

    // iterator tests
    for (double& val : A) {
        val += 2;
    }
    for (double& val : B) {
        val *= 3;
    }
    cout << A << endl;
    cout << A*B << endl;

    // scalar arithmetic tests
    cout << "A += 8\n" <<  (A += 8) << endl;
    cout << "A /= 2\n" <<  (A /= 2) << endl;
    cout << "A = 4 - H\n" <<  (A = 4. - A) << endl;
    cout << "A = 12 / H\n" << (A = 12. / A) << endl;

    // Test Inverse

    Matrix<float> mateo({
        {2,5,0,8},
        {1,4,2,6},
        {7,8,9,3},
        {1,5,7,8}});
    Matrix<float> maddox({
        {0.960893854748603,-1.916201117318436,0.078212290502793,0.446927374301676},
        {-1.033519553072626,2.357541899441341,0.067039106145251,-0.759776536312849},
        {-0.005586592178771,-0.273743016759777,0.011173184357542,0.206703910614525},
        {0.53072625698324,-0.994413407821229,-0.06145251396648,0.363128491620112}});

    // mateo.set(vals);
    
    Matrix<float> oetam = inverse(mateo);
    cout << endl << mateo << endl << endl;

    cout << (oetam == maddox ? "Correct" : "Incorrect") << endl;
    cout << oetam.to_string(8) << endl;

    return 0;
}