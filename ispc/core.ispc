struct Point2 {
	double x;
	double y;
};

uniform double euclidean_distance(const uniform Point2 &point_a, const uniform Point2 &point_b){
    uniform double total = 0;
	total += (point_a.x - point_b.x) * (point_a.x - point_b.x);
	total += (point_a.y - point_b.y) * (point_a.y - point_b.y);
    return sqrt(total);
}

uniform double gaussian_kernel(uniform double distance, uniform double kernel_bandwidth){
    uniform double temp =  exp(-1.0/2.0 * (distance*distance) / (kernel_bandwidth*kernel_bandwidth));
    return temp;
}

export uniform double perform(const uniform Point2 &point, const uniform Point2 * uniform points, uniform size_t point_count, uniform double kernel_bandwidth, uniform Point2 &shifted_point) {
	uniform double total_weight = 0.;
	for (uniform size_t i = 0; i < point_count; i++) {
		const uniform Point2 &temp_point = points[i];
		uniform double distance = euclidean_distance(point, temp_point);
		uniform double weight = gaussian_kernel(distance, kernel_bandwidth);
		shifted_point.x += temp_point.x * weight;
		shifted_point.y += temp_point.y * weight;
		total_weight += weight;
	}
	return total_weight;
}

