#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#include <iostream>
#include "MeanShift.h"

using namespace std;

static double cputime(void)
{
    struct tms t;
    times(&t);
    return (double)(t.tms_utime + t.tms_stime)/  sysconf(_SC_CLK_TCK) ;
}

vector<vector<double> > load_points(const char *filename) {
    vector<vector<double> > points;
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
                vector<double> point;
                point.push_back(x);
                point.push_back(y);
                points.push_back(point);
                break;
            }
            ++y_str;
        }
    }
    fclose(fp);
    return points;
}

void print_points(vector<vector<double> > points){
    for(int i=0; i<points.size(); i++){
        for(int dim = 0; dim<points[i].size(); dim++) {
            printf("%f ", points[i][dim]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv)
{
    MeanShift *msp = new MeanShift();
    double kernel_bandwidth = 3;

    vector<vector<double> > points = load_points("test.csv");

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

    for(int cluster = 0; cluster < clusters.size(); cluster++) {
      printf("Cluster %i:\n", cluster);
      for(int point = 0; point < clusters[cluster].original_points.size(); point++){
        for(int dim = 0; dim < clusters[cluster].original_points[point].size(); dim++) {
          printf("%f ", clusters[cluster].original_points[point][dim]);
          fprintf(fp, dim?",%f":"%f", clusters[cluster].original_points[point][dim]);
        }
        printf(" -> ");
        for(int dim = 0; dim < clusters[cluster].shifted_points[point].size(); dim++) {
          printf("%f ", clusters[cluster].shifted_points[point][dim]);
        }
        printf("\n");
        fprintf(fp, "\n");
      }
      printf("\n");
    }
    fclose(fp);

    return 0;
}
