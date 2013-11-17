/*
 * welford.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: Matt Buland, John Kelly
 */

#include <cmath>

#include "welford.h"

/* WELFORD */

// Count of 0
// Mean of 0
// Each lag value on [0, maxLag] gets an entry in the vector, initialized to 0
WelfordStore::WelfordStore(int maxLag) : valCount(0), runningMean(0), variances(maxLag+1,0){
}


void WelfordStore::addPoint(double p){
    valCount++;

    // Update the running variance total
    if (valCount <= 1) {
        variances[0] = 0;
    } else {
        variances[0] = variances[0] + (((double) (valCount -1))/(double) valCount) * ((p - runningMean)*(p - runningMean));
    }
    // When valCount is 1 and lag is 0, the expression will 0 out and set the variance to 0, no need for an explicit logic branch

    // Update lagged variances
    for(std::vector<double>::size_type l = 0; l < lagWindow.size(); ++l){
        variances[l+1] += (valCount / (valCount+1.0)) * (p - runningMean) * (lagWindow[l] - runningMean);
    }

    lagWindow.push_front(p);
    // Vars by Lag  : 0 1 2 3 4 5
    // Values by Lag: 0 1 2 3 4 5 6
    //                            ^-- Remove this, no variance being tracked for that much lag
    if(lagWindow.size() > variances.size()+1){
        lagWindow.pop_back();
    }

    // Inform any listening covariance objects that a point was added
    for(std::set<WelfordCoVar*>::iterator iter = listeners.begin(); iter != listeners.end(); ++iter){
        WelfordCoVar* listener = *iter;
        listener->addPoint(this, p, runningMean);
    }

    // Update the running mean total
    runningMean += (p - runningMean) / valCount;
}


int WelfordStore::count(){
    return valCount;
}

double WelfordStore::mean(){
    return runningMean;
}

double WelfordStore::var(){
    return variances[0];
}


double WelfordStore::stdDev(){
    return std::sqrt(variances[0] / valCount);
}

double WelfordStore::laggedVar(int& lag){
    return variances[lag-1];
}


void WelfordStore::registerCoVarListener(WelfordCoVar* listener){
    listeners.insert(listener);
}

void WelfordStore::unregisterCoVarListener(WelfordCoVar* listener){
    listeners.erase(listener);
}

/* END WELFORD */

/* WELFORDCOVAR */

WelfordCoVar::WelfordCoVar(WelfordStore* lhs, WelfordStore* rhs) : count(0){
    l = lhs;
    r = rhs;
    l->registerCoVarListener(this);
    r->registerCoVarListener(this);
    runningVariance = 0;
}

void WelfordCoVar::removeSource(WelfordStore* w){
    if(l == w){
        l = 0x0;
    }
    else if(r == w){
        r = 0x0;
    }
}

void WelfordCoVar::addPoint(WelfordStore* src, double d, double prevMean){
    if(src == l){
        lValBuffer.push(d);
        lMeanBuffer.push(prevMean);
    }
    else if(src == r){
        rValBuffer.push(d);
        rMeanBuffer.push(prevMean);
    }
    else{
        return;
    }
    // Very protectively consume as much of the buffers as possible
    while(l != 0x0 && r != 0x0 && lValBuffer.size() > 0 && rValBuffer.size() > 0){
        double l = lValBuffer.top();
        double r = rValBuffer.top();
        double lm = lMeanBuffer.top();
        double rm = rMeanBuffer.top();
        lValBuffer.pop();
        lMeanBuffer.pop();
        rValBuffer.pop();
        rMeanBuffer.pop();
        ++count;
        runningVariance += ((count - 1.0) / count) * (l - lm) * (r - rm);
    }
}

double WelfordCoVar::coVar(){
    return runningVariance;
}

/* END WELFORDCOVAR */
/*
#include "welford.h"

#define VERBOSE 0

template <class T>
void Queue<T>::push(T val) {
    v.push_back(val);
}

template <class T>
void Queue<T>::pop() {
    // Remove position 0, and move all items down
    v.erase(v.begin());
}

template <class T>
int Queue<T>::size() {
    return v.size();
}

template <class T>
T Queue<T>::at(int index) {
    return v.at(index);
}

WelfordStore::WelfordStore(std::vector<int> lagsin) {
    setup(lagsin);
}

WelfordStore::WelfordStore() {
    std::vector<int> vect;
    setup(vect);
}

void WelfordStore::setup(std::vector<int> lagsin) {
    std::cout << "Making " << lagsin.size() << " lags" << std::endl;
    n = 0;
    maxlag = 0;
    mean = std::vector<double>(lagsin.size()+1);
    variance = std::vector<double>(lagsin.size()+1);
    lags = std::vector<int>(lagsin.size()+1);
    lags.push_back(0);
    for (unsigned int i=0; i < lagsin.size(); i++) {
        if (lagsin.at(i) > maxlag)
            maxlag = lagsin.at(i);
        lags.push_back(lagsin[i]);
        mean.push_back(0);
        variance.push_back(0);
    }
    std::cout << "Initializing lags with ";
    for (unsigned int i=0; i < lags.size(); i++) {
        std::cout << lags.at(i) << ", ";
    }
    std::cout << std::endl;
    mean.push_back(0);
    variance.push_back(0);
}

// Note: This order is a requirement. n++ first, then variance, then mean
// NOTE!!!! Always calculate the standard deviation FIRST
void WelfordStore::addpoint(double xi) {

    // Add the new point to the back.
    laghistory.push(xi);

    // Only remove from the history if we've filled our queue
    if (laghistory.size() > maxlag+1) {
        // "first" point falls off the map, and we are not to use it anymore
        laghistory.pop();
    }
    n++;

    // Recalculate all the lag-variances
    for (unsigned int i=0; i < lags.size(); i++) {
        double newvariance = addvariance(i, laghistory.at(lags.at(i)));
        double newmean = addmean(i, laghistory.at(lags.at(i)));

        if (VERBOSE) {
            printf("Lag %i's new variance=%f\n", lags.at(i), newvariance);
            printf("New %i's new mean=%f\n", lags.at(i), newmean);
        }
    }

}

// Lag is the lag-index: 0=lag of 0, 1=first supplied lag...
// There's a correspondance between mean[lag], variance[lag], and lags[lag], where all of these indexes all correspond to the point-lag of lags[lag]
double WelfordStore::addmean(int lag, double xi) {
    if (n <= 0) {
        mean[lag] = xi;
    } else {
        mean[lag] = mean[lag] + (xi - mean[lag]) / n;
    }
    return mean[lag];
}

double WelfordStore::addvariance(int lag, double xi) {
    if (n <= 1) {
        variance[lag] = 0;
    } else {
        variance[lag] = variance[lag] + (((double) (n -1))/(double) n) * ((xi - mean[lag])*(xi - mean[lag]));
    }
    return variance[lag];
}

double WelfordStore::getvariance() {
    return getvariance(0);
}

double WelfordStore::getvariance(int lag) {
    return variance[lag] / n;
}

double WelfordStore::getstdev() {
    return getstdev(0);
}

double WelfordStore::getstdev(int lag) {
    return sqrt(variance[lag]);
}

double WelfordStore::getmean() {
    return getmean(0);
}

double WelfordStore::getmean(int lag) {
    return mean[lag];
}

int WelfordStore::getcount() {
    return n;
}

// Gets the total number of points that the specified lag-index has come across
// A negative number will signify that points will begin to be added in X points
int WelfordStore::getcount(int lag) {
    return n-lags.at(lag);
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
    //printf("For lag %i, there have been %i points, Calculated mean=%f, variance=%f, stdev=%f\n", wfrdata.getcount(), wfrdata.getmean(1), wfrdata.getvariance(1), wfrdata.getstdev(1));
}
>>>>>>> master
*/
