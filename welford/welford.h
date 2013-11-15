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
