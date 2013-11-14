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

class Welford {

public:
    // The constructed Welford object will track at most maxLag points for autocorrelation
    Welford(int maxLag = 0);

    // Adds a data point
    void addData(double d);
    // Returns how many data points have been inserted so far
    int count();
    // Returns the accumulated mean of all data points so far
    double mean();
    // Returns the variance of all data points so far
    double var();
    // Convenience function for sqrt(getVar() / count());
    double stdDev();
    // Returns the r-value of autocorrelation
    double autoCorr(int& lag);

    // Add or remove a covariance listener object to notify when points are added
    void registerCoVarListener(WelfordCoVar* listener);
    void unregisterCoVarListener(WelfordCoVar* listener);

private:
    std::deque<double> autoCorrWindow;
    std::vector<double> autoCorrelations;

    int valCount;
    double runningMean;
    double runningVariance;

    std::set<WelfordCoVar*> listeners;
};

class WelfordCoVar{
public:
    WelfordCoVar(Welford* lhs, Welford* rhs);
    void removeSource(Welford* w);
    void addData(Welford* src, double d, double prevMean);
    double coVar();
private:
    int count;
    double runningVariance;
    Welford* l;
    Welford* r;
    std::stack<double> lValBuffer, rValBuffer, lMeanBuffer, rMeanBuffer;
};



#endif /* WELFORD_H_ */
