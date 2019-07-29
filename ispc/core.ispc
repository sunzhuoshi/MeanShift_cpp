export uniform double euclidean_distance(const uniform double point_a[], const uniform double point_b[], uniform size_t length)
{
    double total = 0;
    foreach(i = 0...length) {
        total += (point_a[i] - point_b[i]) * (point_a[i] - point_b[i]);
    }
    return sqrt(reduce_add(total));
}