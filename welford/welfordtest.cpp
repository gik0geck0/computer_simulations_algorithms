#include <cstdlib>
#include <iostream>


#include "welford.h"

using namespace std;

int main(int argc, char** argv){
    // Testing welford usage

    int points = 20;
    // Inclusive range
    int min = 0;
    int max = 10;
    int seed = 1;
    srand(seed);

    Welford w1(5);

    cout << endl;

    cout << "Inserting 20 points into Welford tracker. {";
    w1.addData(6); cout << 6 << ", ";
    w1.addData(10); cout << 10 << ", ";
    w1.addData(6); cout << 6 << ", ";
    w1.addData(2); cout << 2 << ", ";
    w1.addData(1); cout << 1 << ", ";
    w1.addData(4); cout << 4 << ", ";
    w1.addData(0); cout << 0 << ", ";
    w1.addData(6); cout << 6 << ", ";
    w1.addData(3); cout << 3 << ", ";
    w1.addData(1); cout << 1 << ", ";
    w1.addData(8); cout << 8 << ", ";
    w1.addData(7); cout << 7 << ", ";
    w1.addData(5); cout << 5 << ", ";
    w1.addData(3); cout << 3 << ", ";
    w1.addData(7); cout << 7 << ", ";
    w1.addData(4); cout << 4 << ", ";
    w1.addData(9); cout << 9 << ", ";
    w1.addData(10); cout << 10 << ", ";
    w1.addData(2); cout << 2 << ", ";
    w1.addData(0); cout << 0 << ", ";
    cout << "}" << endl;

    int singleC = w1.count();
    double singleM = w1.mean();
    double singleSD = w1.stdDev();
    double singleV = w1.var();

    cout << "For " << singleC << " points, calculated:" << endl;

    cout << "Mean: " << singleM;
    if(singleM != 4.7){
        cout << ", expected 4.7" << endl;
        return 0;
    }
    cout << endl;

    // Arbitrary accuracy minimum of calculations
    double eps = 0.000001;

    cout << "Std Dev: " << singleSD;
    if((singleSD - 3.116087) > eps){
        cout << ", expected 3.116087, error of " << singleM - 3.116087 << endl;
        return 0;
    }
    cout << endl;

    cout << "Variance: " << singleV;
    if((singleV - 194.2) > eps){
        cout << ", expected 194.2, error of" << singleV - 194.2 << endl;
        return 0;
    }
    cout << endl;

    // TODO: Covariance, lag 1
    // TODO: Covariance, lag 5
    cout << "Autovariance, lag 1: " << endl;
    cout << "Autovariance, lag 5: " << endl;

    cout << endl;

    cout << "Testing covariance" << endl;

    Welford w2;
    Welford w3;
    WelfordCoVar wc(&w3, &w2);

    return 0;
}
