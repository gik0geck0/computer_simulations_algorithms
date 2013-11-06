#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define VERBOSE 0

struct welford_store {
    double mean;
    double variance;
    int n;
};

void addpoint(struct welford_store* wdata, double xi);
double addmean(struct welford_store* wdata, double xi);
double addvariance(struct welford_store* wdata, double xi);

// Note: This order is a requirement. n++ first, then variance, then mean
// NOTE!!!! Always calculate the standard deviation FIRST
void addpoint(struct welford_store* wdata, double xi) {
    wdata->n++;
    double newvariance = addvariance(wdata, xi);
    double newmean = addmean(wdata, xi);

    if (VERBOSE) {
        printf("Adding point %f\n", xi);
        printf("New variance=%f\n", newvariance);
        printf("New mean=%f\n", newmean);
    }
}

double addmean(struct welford_store* wdata, double xi) {
    if (wdata->n <= 0) {
        wdata->mean = xi;
    } else {
        wdata->mean = wdata->mean + (xi - wdata->mean) / wdata->n;
    }
    return wdata->mean;
}

double addvariance(struct welford_store* wdata, double xi) {
    if (wdata->n <= 1) {
        wdata->variance = 0;
    } else {
        wdata->variance = wdata->variance + (((double) (wdata->n -1))/(double) wdata->n) * ((xi - wdata->mean)*(xi - wdata->mean));
    }
    return wdata->variance;
}

double getvariance(struct welford_store* wdata) {
    return wdata->variance / wdata->n;
}

double getstdev(struct welford_store* wdata) {
    return sqrt(getvariance(wdata));
}

int main() {
    // Testing welford usage

    int points = 20;
    // Inclusive range
    int min = 0;
    int max = 10;

    int seed = 1;
    srand(seed);

    struct welford_store* wfrdata = malloc(sizeof(struct welford_store));
    wfrdata->mean = 0;
    wfrdata->variance = 0;

    for (int i=0; i < points; i++) {
        double nextpoint = rand() % (max - min+1) + min;
        //double nextpoint = i;
        printf("Next point is %f\n", nextpoint);
        addpoint(wfrdata, nextpoint);
    }
    printf("For %i points, Calculated mean=%f, variance=%f, stdev=%f\n", wfrdata->n, wfrdata->mean, getvariance(wfrdata), getstdev(wfrdata));

    free(wfrdata);
}
