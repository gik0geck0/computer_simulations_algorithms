#include <cstdlib>
#include <iostream>
#include <fstream>

#include "welford.h"

using namespace std;

int main(int argc, char** argv){
    // Testing welford usage

	// TODO: Argument specified input data
	ifstream data("testdata-1000-10.dat");

	WelfordStore w(10);
	double d;
	while(data >> d){
		w.addPoint(d);
	}

	// TODO: Argument specified results data
	ifstream results("results-1000-10.dat");
	data.close();
	results.close();

	for(int l = 0; l <= 10; l++){
		cout << l << "\t" << 1000-l << "\t" << w.mean() << "\t" << w.variance() << "\t" << w.laggedVariance(l) << "\t" << w.laggedAutoCorr(l) << endl;
	}

return 0;
	cout << "*****" << endl;

//    int points = 20;
//    // Inclusive range
//    int min = 0;
//    int max = 10;
    int seed = 1;
    srand(seed);

    WelfordStore w1(5);

    cout << endl;

    cout << "Inserting 20 points into Welford tracker." << endl;
    cout << "{";
    w1.addPoint(6); cout << 6 << ", ";
    w1.addPoint(10); cout << 10 << ", ";
    w1.addPoint(6); cout << 6 << ", ";
    w1.addPoint(2); cout << 2 << ", ";
    w1.addPoint(1); cout << 1 << ", ";
    w1.addPoint(4); cout << 4 << ", ";
    w1.addPoint(0); cout << 0 << ", ";
    w1.addPoint(6); cout << 6 << ", ";
    w1.addPoint(3); cout << 3 << ", ";
    w1.addPoint(1); cout << 1 << ", ";
    w1.addPoint(8); cout << 8 << ", ";
    w1.addPoint(7); cout << 7 << ", ";
    w1.addPoint(5); cout << 5 << ", ";
    w1.addPoint(3); cout << 3 << ", ";
    w1.addPoint(7); cout << 7 << ", ";
    w1.addPoint(4); cout << 4 << ", ";
    w1.addPoint(9); cout << 9 << ", ";
    w1.addPoint(10); cout << 10 << ", ";
    w1.addPoint(2); cout << 2 << ", ";
    w1.addPoint(0); cout << 0 << ", ";
    cout << "}" << endl;

    int singleC = w1.count();
    double singleM = w1.mean();
    double singleSD = w1.stdDev();
    double singleV = w1.variance();

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
        cout << ", expected 3.116087, off by " << singleM - 3.116087 << endl;
        return 0;
    }
    cout << endl;

    cout << "Variance: " << singleV;
    if((singleV - 194.2) > eps){
        cout << ", expected 194.2, off by " << singleV - 194.2 << endl;
        return 0;
    }
    cout << endl;

    // TODO: Covariance, lag 1
    // TODO: Covariance, lag 5
    cout << "Autovariance, lag 1: " << endl;
    cout << "Autovariance, lag 5: " << endl;

    cout << endl;

    cout << "Testing covariance" << endl;

    WelfordStore* w2 = new WelfordStore();
    WelfordStore* w3 = new WelfordStore();
    WelfordCoVar* wc = new WelfordCoVar(w3, w2);


    return 0;
}
