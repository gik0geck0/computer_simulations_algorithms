/*
 * welford.h
 *
 *  Created on: Nov 12, 2013
 *      Author: jack
 */

#ifndef WELFORD_H_
#define WELFORD_H_

#include <queue>

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
	std::vector<std::deque<double> > autoCorrWindows;

	int valCount;
	double runningMean;
	double runningVariance;

	std::vector<WelfordCoVar*> listeners;
};

class WelfordCoVar{
public:
	WelfordCoVar(Welford* lhs, Welford* rhs);

	double coVar();
private:
};



#endif /* WELFORD_H_ */
