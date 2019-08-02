#include <stdio.h>
#include <stdlib.h>
#ifdef _MSC_VER
#include <windows.h>
#else
#include <sys/times.h>
#include <unistd.h>
#endif
#include <iostream>
#include "MeanShift.h"

using namespace std;

#ifdef _MSC_VER
double Freq = 0.0;
__int64 CounterStart = 0;
#endif

void init() {
#ifdef _MSC_VER
	LARGE_INTEGER li;
	if (QueryPerformanceFrequency(&li)) {
		Freq = double(li.QuadPart) / 1000.0;

		QueryPerformanceCounter(&li);
		CounterStart = li.QuadPart;
	}
#endif
}

static double cputime(void)
{
#ifdef _MSC_VER
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / Freq;
#else
	struct tms t;
	times(&t);
	return (double)(t.tms_utime + t.tms_stime) / sysconf(_SC_CLK_TCK);
#endif 
}

vector<Point2> load_points(const char *filename) {
    vector<Point2> points;
    FILE *fp = fopen(filename, "r");
    char line[50];
    while (fgets(line, sizeof(line), fp) != NULL) {
        double x, y;
        char *x_str = line;
        char *y_str = line;
        while (*y_str != '\0') {
            if (*y_str == ',') {
                *y_str++ = 0;
                x = atof(x_str);
                y = atof(y_str);
                Point2 point;
                point.x = x;
                point.y = y;
                points.push_back(point);
                break;
            }
            ++y_str;
        }
    }
    fclose(fp);
    return points;
}

void print_points(vector<Point2> points){
    for(size_t i=0; i<points.size(); i++){
		printf("%f %f\n", points[i].x, points[i].y);
    }
}

int main(int argc, char **argv)
{
	init();
    MeanShift *msp = new MeanShift();
    double kernel_bandwidth = 3;

    vector<Point2> points = load_points("test.csv");

    printf("\n Action starts\n");
    printf("\n ...\n");
    double start = cputime();
    vector<Cluster> clusters = msp->cluster(points, kernel_bandwidth);
    double end = cputime();
    printf("\n Action ends, time: %fs\n", end - start);

    FILE *fp = fopen("result.csv", "w");
    if(!fp){
        perror("Couldn't write result.csv");
        exit(0);
    }

    printf("\n====================\n");
    printf("Found %lu clusters\n", clusters.size());
    printf("====================\n\n");

    printf("Press any key to continue\n");
    system("read");

    for(size_t cluster = 0; cluster < clusters.size(); cluster++) {
      printf("Cluster %i:\n", cluster);
      for(size_t point = 0; point < clusters[cluster].original_points.size(); point++) {
		  printf("%f %f", clusters[cluster].original_points[point].x, clusters[cluster].original_points[point].y);
          fprintf(fp, "%f,%f", clusters[cluster].original_points[point].x, clusters[cluster].original_points[point].y);
        printf(" -> ");
		printf("%f %f", clusters[cluster].shifted_points[point].x, clusters[cluster].shifted_points[point].y);
        printf("\n");
        fprintf(fp, "\n");
      }
      printf("\n");
    }
    fclose(fp);

    return 0;
}
