#pragma once 

#include <vector>

struct Point2 {
	double x;
	double y;
	Point2() {
		x = .0;
		y = .0;
	}
};

struct Cluster {
    Point2 mode;
    std::vector<Point2> original_points;
    std::vector<Point2> shifted_points;
};

class MeanShift {
public:
    MeanShift() { set_kernel(NULL); }
    MeanShift(double (*_kernel_func)(double,double)) { set_kernel(kernel_func); }
    std::vector<Point2> meanshift(const std::vector<Point2> &, double);
    std::vector<Cluster> cluster(const std::vector<Point2> &, double);

private:
    double (*kernel_func)(double,double);
    void set_kernel(double (*_kernel_func)(double,double));
    Point2 shift_point(const Point2 &, const std::vector<Point2> &, double);
    std::vector<Cluster> cluster(const std::vector<Point2> &, const std::vector<Point2> &);
};
