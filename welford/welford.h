#include <cmath>
#include <cstdio>
#include <cstdlib>

class WelfordStore {
    double mean;
    double variance;
    int n;

    public:
    WelfordStore();
    void addpoint(double xi);
    double getvariance();
    double getstdev();
    double getmean();
    int getcount();

    private:
    double addmean(double xi);
    double addvariance(double xi);
};
