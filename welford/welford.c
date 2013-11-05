#include <stdlib.h>
#include <stdio.h>

#define VERBOSE 1

struct welford_store {
    double mean;
    double stdev;
    int n;
};

void addpoint(struct welford_store* wdata, double xi);
double addmean(struct welford_store* wdata, double xi);
double addstdev(struct welford_store* wdata, double xi);

// Note: This order is a requirement. n++ first, then stdev, then mean
// NOTE!!!! Always calculate the standard deviation FIRST
void addpoint(struct welford_store* wdata, double xi) {
    wdata->n++;
    double newstdev = addstdev(wdata, xi);
    double newmean = addmean(wdata, xi);

    if (VERBOSE) {
        printf("Adding point %f\n", xi);
        printf("New stdev=%f\n", newstdev);
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

double addstdev(struct welford_store* wdata, double xi) {
    if (wdata->n <= 1) {
        wdata->stdev = 0;
    } else {
        wdata->stdev = wdata->stdev + (((double) (wdata->n -1))/(double) wdata->n) * ((xi - wdata->mean)*(xi - wdata->mean));
    }
    return wdata->stdev;
}

double getstdev(struct welford_store* wdata) {
    return wdata->stdev / wdata->n;
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
