#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "welford.h"

using namespace std;

int main(int argc, char** argv){
    if(argc != 1 && argc != 3){
        cout << "Usage: welfordtest /PATH/TO/testdata-N-L.dat /PATH/TO/results-N-L.dat" << endl;
        cout << "Search for N=1000, L=10 in the working directory by default" << endl;
        return 1;
    }

    const char* inFName = "testdata-1000-10.dat";
    const char* outFName = "results-1000-10.dat";
    // Received explicit file locations
    if(argc == 3){
        inFName = argv[1];
        outFName = argv[2];
    }

    ifstream data(inFName);
    if(!data.is_open()){
    	cerr << "Could not open input data file testdata-1000-10.dat" << endl;
    	return 1;
    }

    WelfordStore w(10);
    double d;
    while(data >> d){
        w.addPoint(d);
    }
    data.close();

    ifstream results(outFName);
    if(!results.is_open()){
    	cerr << "Could not open results data file results-1000-10.dat" << endl;
    	return 1;
    }

    std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
    std::cout.precision(8);
    int count;
    double rMu, rSVar, rSCov, rj;
    double wMu, wSVar, wSCov, wj;
    double eMu, eSVar, eSCov, ej;

    cout << "If a line appears such as " << endl;
    cout << "  '- err: (...)" << endl;
    cout << "The numbers on that line represent the error factor by which the value calculated by the library deviated from the expected value given by the input file." << endl;
    cout << "These error lines are currently set to .1% tolerance on the sample mean and 5% tolerance on the sample variance, covariance, and auto-correlation coefficient." << endl;

    cout << endl;

    // Table labels
    cout << "Lag" << "\t" << "N" << "\t" << "Mu" << "\t\t" << "SVar" << "\t\t" << "SCovar" << "\t\t" << "r_j" << endl;

    for(int l = 0; l <= 10; l++){
        // Read expected values
        results >> l >> count >> rMu >> rSVar >> rSCov >> rj;
        // Collect calculated values
        wMu = w.lagBackMean(l);
        wSVar = w.lagBackVar(l);
        wSCov = w.laggedVariance(l);
        wj = w.laggedAutoCorr(l);
        // Collect error factors
        eMu = 1-wMu/rMu;
        eSVar = 1-wSVar/rSVar;
        eSCov = 1-wSCov/rSCov;
        ej = 1-wj/rj;
        // Output, with err values if necessary
        cout << l         << "\t" << 1000-l << "\t" << wMu << "\t" << wSVar << "\t" << wSCov << "\t" << wj << endl;
        if(eMu > 0.001 || eSVar > 0.05 || eSCov > 0.05 || ej > 0.05){
        cout << "'-err: " << "\t" << 0      << "\t" << eMu << "\t" << eSVar << "\t" << eSCov << "\t" << ej << endl << endl;
        }
    }

    results.close();

    cout << endl;
    cout << endl;

    cout << "Testing WelfordCoVar object:" << endl;
    cout << endl;

    WelfordStore* w1 = new WelfordStore();
    WelfordStore* w2 = new WelfordStore();
    WelfordCoVar* wc = new WelfordCoVar(w1, w2);

    cout << "Left" << "\t" << "Right" << "\t" << "Running CoVariance" << endl;
    w1->addPoint(1);
    w2->addPoint(2);
    cout << 1 << "\t" << 2 << "\t" << wc->corrCoeff() << endl;
    w1->addPoint(2);
    w2->addPoint(4);
    cout << 2 << "\t" << 4 << "\t" << wc->corrCoeff() << endl;
    w1->addPoint(3);
    w2->addPoint(6);
    cout << 3 << "\t" << 6 << "\t" << wc->corrCoeff() << endl;
    w1->addPoint(4);
    w2->addPoint(4);
    cout << 4 << "\t" << 4 << "\t" << wc->corrCoeff() << endl;

    // Delete a WelfordStore source, and the WelfordCoVar shouldn't segfault
    delete w2;
    wc->corrCoeff();
    // Delete a WelfordCoVar listener, and the WelfordStore shouldn't segfault
    delete wc;
    w1->addPoint(0);
    w1->mean();
    delete w1;
    cout << "Objects delete'd without segfaulting." << endl;

    return 0;
}
