struct Point2 {
	double x;
	double y;
};

double euclidean_distance(const Point2 uniform &point_a, const Point2 &point_b){
    double total = 0.;
	Point2 tmp = point_a;
	total += (tmp.x - point_b.x) * (tmp.x - point_b.x);
	total += (tmp.y - point_b.y) * (tmp.y - point_b.y);
    return sqrt(total);
}

double gaussian_kernel(double distance, double kernel_bandwidth){
    double temp =  exp(-1.0/2.0 * (distance*distance) / (kernel_bandwidth*kernel_bandwidth));
    return temp;
}

export uniform double perform(const uniform Point2 &point, const Point2 * uniform points, uniform size_t point_count, uniform double kernel_bandwidth, uniform Point2 &shifted_point) {
	double total_weight = 0.;
	double delta_x = 0., delta_y = 0.;
	foreach (i = 1...point_count) {
		const Point2 temp_point = points[i];
		double distance = euclidean_distance(point, temp_point);
		double weight = gaussian_kernel(distance, kernel_bandwidth);

		delta_x += temp_point.x * weight;
		delta_y += temp_point.y * weight;
		total_weight += weight;
	}
	shifted_point.x += reduce_add(delta_x);
	shifted_point.y += reduce_add(delta_y);
	return reduce_add(total_weight);
}

