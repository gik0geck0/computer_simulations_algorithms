/*
 * welford.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: Matt Buland, John Kelly
 */

#include <cmath>

#include "welford.h"

/* WELFORD */

Welford::Welford(int maxLag) : valCount(0), runningMean(0), runningVariance(0){
    autoCorrelations.resize(maxLag);
}


void Welford::addData(double d){
    valCount++;

    // Update the running variance total
    if (valCount <= 1) {
        runningVariance = 0;
    } else {
        runningVariance = runningVariance + (((double) (valCount -1))/(double) valCount) * ((d - runningMean)*(d - runningMean));
    }

    // Update auto correlation data structures
    for(int l = 0; l < autoCorrWindow.size(); ++l){
        autoCorrelations[l] += (l / (l+1.0)) * (d - runningMean) * (autoCorrWindow[l] - runningMean);
    }
    autoCorrWindow.push_front(d);
    if(autoCorrWindow.size() > autoCorrelations.size()){
        autoCorrWindow.pop_back();
    }

    // Inform any listening covariance objects that a point was added
    for(std::set<WelfordCoVar*>::iterator iter = listeners.begin(); iter != listeners.end(); ++iter){
        WelfordCoVar* listener = *iter;
        listener->addData(this, d, runningMean);
    }

    // Update the running mean total
    runningMean += (d - runningMean) / valCount;
}


int Welford::count(){
    return valCount;
}

double Welford::mean(){
    return runningMean;
}

double Welford::var(){
    return runningVariance;
}


double Welford::stdDev(){
    return std::sqrt(runningVariance / valCount);
}

double Welford::autoCorr(int& lag){
    return autoCorrelations[lag-1];
}


void Welford::registerCoVarListener(WelfordCoVar* listener){
    listeners.insert(listener);
}

void Welford::unregisterCoVarListener(WelfordCoVar* listener){
    listeners.erase(listener);
}

/* END WELFORD */

/* WELFORDCOVAR */

WelfordCoVar::WelfordCoVar(Welford* lhs, Welford* rhs) : count(0){
    l = lhs;
    r = rhs;
    l->registerCoVarListener(this);
    r->registerCoVarListener(this);
    runningVariance = 0;
}

void WelfordCoVar::removeSource(Welford* w){
    if(l == w){
        l = 0x0;
    }
    else if(r == w){
        r = 0x0;
    }
}

void WelfordCoVar::addData(Welford* src, double d, double prevMean){
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
