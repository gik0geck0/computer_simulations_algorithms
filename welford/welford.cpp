#include "welford.h"

#define VERBOSE 0

WelfordStore::WelfordStore() {
    n = 0;
    mean = 0;
    variance = 0;
}

// Note: This order is a requirement. n++ first, then variance, then mean
// NOTE!!!! Always calculate the standard deviation FIRST
void WelfordStore::addpoint(double xi) {
    n++;
    double newvariance = addvariance(xi);
    double newmean = addmean(xi);

    if (VERBOSE) {
        printf("Adding point %f\n", xi);
        printf("New variance=%f\n", newvariance);
        printf("New mean=%f\n", newmean);
    }
}

double WelfordStore::addmean(double xi) {
    if (n <= 0) {
        mean = xi;
    } else {
        mean = mean + (xi - mean) / n;
    }
    return mean;
}

double WelfordStore::addvariance(double xi) {
    if (n <= 1) {
        variance = 0;
    } else {
        variance = variance + (((double) (n -1))/(double) n) * ((xi - mean)*(xi - mean));
    }
    return variance;
}

double WelfordStore::getvariance() {
    return variance / n;
}

double WelfordStore::getstdev() {
    return sqrt(getvariance());
}

int WelfordStore::getcount() {
    return n;
}

double WelfordStore::getmean() {
    return mean;
}

int main() {
    // Testing welford usage

    int points = 20;
    // Inclusive range
    int min = 0;
    int max = 10;

    int seed = 1;
    srand(seed);

    WelfordStore wfrdata;

    for (int i=0; i < points; i++) {
        double nextpoint = rand() % (max - min+1) + min;
        //double nextpoint = i;
        printf("Next point is %f\n", nextpoint);
        wfrdata.addpoint(nextpoint);
    }
    printf("For %i points, Calculated mean=%f, variance=%f, stdev=%f\n", wfrdata.getcount(), wfrdata.getmean(), wfrdata.getvariance(), wfrdata.getstdev());
}
