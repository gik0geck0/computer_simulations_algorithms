/*
 * welford.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: Matt Buland, John Kelly
 */

#include <cmath>
#include <algorithm>

#include "welford.h"

/* WELFORDSTORE */

// Count of 0
// Mean of 0
// Each lag value on [0, maxLag] gets an entry in the vector, initialized to 0
WelfordStore::WelfordStore(int maxLag) : valCount(0), runningMean(0){
    for(int i = 0; i < maxLag+1; ++i){
        variances.push_back(0);
        means.push_back(0);
    }
}

WelfordStore::~WelfordStore(){
    // Inform any listening covariance objects that this object is no longer valid
    for(std::set<WelfordCoVar*>::iterator iter = listeners.begin(); iter != listeners.end(); ++iter){
        WelfordCoVar* listener = *iter;
        listener->removeSource(this);
    }
}

void WelfordStore::addPoint(double p){
    valCount++;

    lagWindow.push_front(p);
    // Vars by Lag  : 0 1 2 3 4 5
    // Values by Lag: 0 1 2 3 4 5 6
    //                            ^-- Remove this, no variance being tracked for that much lag
    if(lagWindow.size() > variances.size()+1){
        lagWindow.pop_back();
    }

    // Update variances
    for(std::vector<double>::size_type l = 0; l < lagWindow.size(); ++l){
        variances[l] += ((valCount-1.0) / valCount) * (p - means[0]) * (lagWindow[l] - means[0]);
    }

    // Stash the latest variance and drop an old one if needed
    backVars.push_front(variances[0]);
    if(backVars.size() > variances.size()+1){
        backVars.pop_back();
    }

    // Inform any listening covariance objects that a point was added
    for(std::set<WelfordCoVar*>::iterator iter = listeners.begin(); iter != listeners.end(); ++iter){
        WelfordCoVar* listener = *iter;
        listener->addPoint(this, p, means[0]);
    }

    // Update the running mean totals
    for(std::vector<double>::size_type l = 0; l < lagWindow.size(); ++l){
        means[l] += (p - means[l]) / (valCount - l);
    }
}


int WelfordStore::count(){
    return valCount;
}

double WelfordStore::mean(){
    return means[0];
}

double WelfordStore::variance(){
    return variances[0] / valCount;
}

double WelfordStore::stdDev(){
    return std::sqrt(variances[0] / valCount);
}

double WelfordStore::laggedAutoCorr(int& lag){
    clampLag(lag);
    return variances[lag]/variances[0];
}

double WelfordStore::laggedVariance(int& lag){
    clampLag(lag);
    return variances[lag] / (valCount - lag - 1);
}

double WelfordStore::lagBackMean(int& lag){
    return means[lag];
}
double WelfordStore::lagBackVar(int& lag){
    return backVars[lag] / valCount;
}

void WelfordStore::registerCoVarListener(WelfordCoVar* listener){
    listeners.insert(listener);
}

void WelfordStore::unregisterCoVarListener(WelfordCoVar* listener){
    listeners.erase(listener);
}

void WelfordStore::clampLag(int& lag){
    lag = std::max(0, std::min(lag, int(variances.size()-1)));
}

/* END WELFORDSTORE */

/* WELFORDCOVAR */

WelfordCoVar::WelfordCoVar(WelfordStore* lhs, WelfordStore* rhs) : count(0), lStdDev(0), rStdDev(0){
    l = lhs;
    r = rhs;
    l->registerCoVarListener(this);
    r->registerCoVarListener(this);
    runningVariance = 0;
}

WelfordCoVar::~WelfordCoVar(){
    if(l != 0x0){
        l->unregisterCoVarListener(this);
    }
    if(r != 0x0){
        r->unregisterCoVarListener(this);
    }
}

void WelfordCoVar::removeSource(WelfordStore* src){
    if(src == l){
        l = 0x0;
    }
    else if(src == r){
        r = 0x0;
    }
    else{
        return;
    }
}

void WelfordCoVar::addPoint(WelfordStore* src, double d, double prevMean){
    if(src == l){
        lValBuffer.push(d);
        lMeanBuffer.push(prevMean);
        lStdDev = src->stdDev();
    }
    else if(src == r){
        rValBuffer.push(d);
        rMeanBuffer.push(prevMean);
        rStdDev = src->stdDev();
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

double WelfordCoVar::corrCoeff(){
    return (runningVariance/count)/(lStdDev * rStdDev);
}
