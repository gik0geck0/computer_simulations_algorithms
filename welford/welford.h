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

// Forward declare so we can use a pointer type
class WelfordCoVar;

class WelfordStore {
public:
    // The constructed Welford object will track at most maxLag points for autocorrelation
    explicit WelfordStore(int maxLag = 0);
    // We have pointers to objects which might have pointers to us, so we need a destructor
    ~WelfordStore();

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

    // the mean lag inputs ago
    double lagBackMean(int& lag);

    // the variance lag inputs ago
    double lagBackVar(int& lag);

    // Add or remove a covariance listener object to notify when points are added
    void registerCoVarListener(WelfordCoVar* listener);
    void unregisterCoVarListener(WelfordCoVar* listener);

private:
    int valCount;
    double runningMean;

    std::deque<double> lagWindow;
    std::vector<double> variances;

    std::deque<double> backVars;
    std::vector<double> means;

    std::set<WelfordCoVar*> listeners;

    void clampLag(int& lag);
};



class WelfordCoVar{
public:
	// Need precisely two WelfordStores to generate data points for us
    WelfordCoVar(WelfordStore* lhs, WelfordStore* rhs);
    // We have pointers to objects which might have pointers to us, so we need a destructor
    ~WelfordCoVar();

    // Inform a WelfordStore that we are no longer listening
    void removeSource(WelfordStore* src);

    // prevMean is the mean for values in src before p is processed
    // points originating from the same src without matching points from the other source tracked by this object are buffered
    void addPoint(WelfordStore* src, double p, double prevMean);

    // Coefficient of correlation between the data points added to the source WelfordStore objects
    double corrCoeff();

private:
    int count;
    double runningVariance;

    double lStdDev;
    double rStdDev;

    WelfordStore* l;
    WelfordStore* r;

    // Buffer inputs in case they are added asymmetrically
    std::stack<double> lValBuffer, rValBuffer, lMeanBuffer, rMeanBuffer;
};



#endif /* WELFORD_H_ */
