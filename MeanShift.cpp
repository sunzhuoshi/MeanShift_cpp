#include <stdio.h>
#include <math.h>
#include "MeanShift.h"

#if USE_ISPC
#include "ispc/core_ispc.h"
#endif

using namespace std;

#define EPSILON 0.00000001
#define CLUSTER_EPSILON 0.5

double euclidean_distance(const Point2 &point_a, const Point2 &point_b){
    double total = 0;
	total += (point_a.x - point_b.x) * (point_a.x - point_b.x);
	total += (point_a.y - point_b.y) * (point_a.y - point_b.y);
    return sqrt(total);
}

double gaussian_kernel(double distance, double kernel_bandwidth){
    double temp =  exp(-1.0/2.0 * (distance*distance) / (kernel_bandwidth*kernel_bandwidth));
    return temp;
}

void MeanShift::set_kernel( double (*_kernel_func)(double,double) ) {
    if(!_kernel_func){
        kernel_func = gaussian_kernel;
    } else {
        kernel_func = _kernel_func;    
    }
}

Point2 MeanShift::shift_point(const Point2 &point, const vector<Point2> &points, double kernel_bandwidth) {
    Point2 shifted_point;
    double total_weight = 0;
    for(size_t i=0; i<points.size(); i++){
        const Point2 &temp_point = points[i];
        double distance = euclidean_distance(point, temp_point);
        double weight = kernel_func(distance, kernel_bandwidth);
		shifted_point.x += temp_point.x * weight;
		shifted_point.y += temp_point.y * weight;
        total_weight += weight;
    }

	shifted_point.x /= total_weight;
	shifted_point.y /= total_weight;
    return shifted_point;
}

vector<Point2> MeanShift::meanshift(const vector<Point2> & points, double kernel_bandwidth){
    vector<bool> stop_moving(points.size(), false);
    vector<Point2> shifted_points = points;
    double max_shift_distance;
    do {
        max_shift_distance = 0;
        for(size_t i=0; i<shifted_points.size(); i++){
            if (!stop_moving[i]) {
                Point2 point_new = shift_point(shifted_points[i], points, kernel_bandwidth);
                double shift_distance = euclidean_distance(point_new, shifted_points[i]);
                if(shift_distance > max_shift_distance){
                    max_shift_distance = shift_distance;
                }
                if(shift_distance <= EPSILON) {
                    stop_moving[i] = true;
                }
                shifted_points[i] = point_new;
            }
        }
        // DO NOT print log when calculating time
        //printf("max_shift_distance: %f\n", max_shift_distance);
    } while (max_shift_distance > EPSILON);
    return shifted_points;
}

vector<Cluster> MeanShift::cluster(
    const vector<Point2> & points, 
    const vector<Point2> & shifted_points)
{
    vector<Cluster> clusters;

    for (size_t i = 0; i < shifted_points.size(); i++) {
		size_t c;
        for (c=0; c < clusters.size(); c++) {
            if (euclidean_distance(shifted_points[i], clusters[c].mode) <= CLUSTER_EPSILON) {
                break;
            }
        }

        if (c == clusters.size()) {
            Cluster clus;
            clus.mode = shifted_points[i];
            clusters.push_back(clus);
        }

        clusters[c].original_points.push_back(points[i]);
        clusters[c].shifted_points.push_back(shifted_points[i]);
    }

    return clusters;
}

vector<Cluster> MeanShift::cluster(const vector<Point2> & points, double kernel_bandwidth){
    vector<Point2> shifted_points = meanshift(points, kernel_bandwidth);
    return cluster(points, shifted_points);
}
