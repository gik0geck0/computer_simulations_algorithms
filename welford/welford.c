#include <stdlib.h>
#include <stdio.h>

struct welford_store {
    int mean;
    int stdev;
    int n;
};

void addpoint(struct welford_store* wdata, int xi);
int addmean(struct welford_store* wdata, int xi);
int addstdev(struct welford_store* wdata, int xi);

void addpoint(struct welford_store* wdata, int xi) {
    printf("Adding point %i\n", xi);
    wdata->n++;
    addmean(wdata, xi);
    addstdev(wdata, xi);
    return;
}

int addmean(struct welford_store* wdata, int xi) {
    wdata->mean = wdata->mean + (xi - wdata->mean) / wdata->n;
    return wdata->mean;
}

int addstdev(struct welford_store* wdata, int xi) {
    if (wdata->n <= 1) {
        return wdata->stdev;
    } else {
        wdata->stdev = wdata->stdev + ((wdata->n-1)/wdata->n) * ((xi - wdata->stdev)*(xi - wdata->stdev));
    }
    return wdata->stdev;
}

int main() {
    // Testing welford usage

    int points = 256;
    int min = 0;
    int max = 1;
    int seed = 1;
    srand(seed);

    struct welford_store* wfrdata = malloc(sizeof(struct welford_store));
    wfrdata->mean = 0;
    wfrdata->stdev = 0;

    for (int i=0; i < points; i++) {
        //int nextpoint = rand() % (max - min) + min;
        int nextpoint = i;
        addpoint(wfrdata, nextpoint);
    }

    printf("For %i points, Calculated mean=%i and stdev=%i\n", wfrdata->n, wfrdata->mean, wfrdata->stdev);

    free(wfrdata);
}
