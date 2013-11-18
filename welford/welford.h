/*
 * welford.h
 *
 *  Created on: Nov 12, 2013
 *      Author: Matt Buland, John Kelly
 */

#ifndef WELFORD_H_
#define WELFORD_H_

#include <queue>
#include <stack>
#include <set>
#include <vector>

class WelfordCoVar;

class WelfordStore {

public:
    // The constructed Welford object will track at most maxLag points for autocorrelation
    explicit WelfordStore(int maxLag = 0);

    // Adds a data point
    void addPoint(double p);

    // Returns how many data points have been inserted so far
    int count();

    // Returns the accumulated mean of all data points so far
    double mean();

    // Returns the variance of all data points so far
    double variance();
    // Convenience function for sqrt(var() / count())
    double stdDev();

    // Returns the r-value of autocorrelation for a particular lag
    // After the call, @lag contains the lag interval returned
    // If lag was a valid interval for this object, it is unchanged
    // Otherwise, lag may be raised to 0 or lowered to the maximum lag being tracked, as appropriate
    double laggedAutoCorr(int& lag);
    // Direct access to the underlying lagged variance value
    double laggedVariance(int& lag);

    // Add or remove a covariance listener object to notify when points are added
    void registerCoVarListener(WelfordCoVar* listener);
    void unregisterCoVarListener(WelfordCoVar* listener);

private:
    int valCount;
    double runningMean;

    std::deque<double> lagWindow;
    std::vector<double> variances;

    std::set<WelfordCoVar*> listeners;
};

class WelfordCoVar{
public:
    WelfordCoVar(WelfordStore* lhs, WelfordStore* rhs);
    void removeSource(WelfordStore* w);
    void addPoint(WelfordStore* src, double p, double prevMean);
    double coVar();
private:
    int count;
    double runningVariance;
    WelfordStore* l;
    WelfordStore* r;
    std::stack<double> lValBuffer, rValBuffer, lMeanBuffer, rMeanBuffer;
};



#endif /* WELFORD_H_ */
/*
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

template <class T>
class Queue {
    std::vector<T> v;

    public:
    void push(T val);
    void pop();
    int size();
    T at(int index);
};

class WelfordStore {
    int n;
    std::vector<int> lags;
    std::vector<double> mean;
    std::vector<double> variance;
    Queue<double> laghistory;
    int maxlag;

    public:
    WelfordStore();
    WelfordStore(std::vector<int> lagsin);
    void addpoint(double xi);
    double getvariance();
    double getstdev();
    double getmean();
    double getvariance(int lag);
    double getstdev(int lag);
    double getmean(int lag);
    int getcount();
    int getcount(int lag);

    private:
    double addmean(int lag, double xi);
    double addvariance(int lag, double xi);
    void setup(std::vector<int> lagsin);
};
>>>>>>> master
*/
