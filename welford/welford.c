#include <stdlib.h>
#include <stdio.h>

#define VERBOSE 0

struct welford_store {
    double mean;
    double stdev;
    int n;
};

void addpoint(struct welford_store* wdata, double xi);
double addmean(struct welford_store* wdata, double xi);
double addstdev(struct welford_store* wdata, double xi);

void addpoint(struct welford_store* wdata, double xi) {
    wdata->n++;
    double newmean = addmean(wdata, xi);
    double newstdev = addstdev(wdata, xi);

    if (VERBOSE) {
        printf("Adding podouble %f\n", xi);
        printf("New mean=%f\n", newmean);
        printf("New stdev=%f\n", newstdev);
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

double addstdev(struct welford_store* wdata, double xi) {
    if (wdata->n <= 1) {
        wdata->stdev = 0;
    } else {
        wdata->stdev = wdata->stdev + ((wdata->n-1)/wdata->n) * ((xi - wdata->stdev)*(xi - wdata->stdev));
    }
    return wdata->stdev;
}

int main() {
    // Testing welford usage

    int points = 256;
    // Inclusive range
    int min = 0;
    int max = 10;

    int seed = 1;
    srand(seed);

    struct welford_store* wfrdata = malloc(sizeof(struct welford_store));
    wfrdata->mean = 0;
    wfrdata->stdev = 0;

    for (int i=0; i < points; i++) {
        double nextpoint = rand() % (max - min+1) + min;
        //double nextpoint = i;
        printf("Next point is %f\n", nextpoint);
        addpoint(wfrdata, nextpoint);
    }
    printf("For %i points, Calculated mean=%f and stdev=%f\n", wfrdata->n, wfrdata->mean, wfrdata->stdev);

    free(wfrdata);
}
