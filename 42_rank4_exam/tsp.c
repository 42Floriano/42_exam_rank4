#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define INF 1e9
#define MAX_CITIES 10

double distance_matrix[MAX_CITIES][MAX_CITIES];
double min_distance = INF;
int best_path[MAX_CITIES];
int current_path[MAX_CITIES];

double calculate_distance(double x0, double x1, double y0, double y1) {
    double dx = x1 - x0;
    double dy = y1 - y0;
    return sqrt(dx * dx + dy * dy);
}

void swap_int(int *a, int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void find_shortes_path(int *perm, int atart, int len, double x[], double y[], double *min_distance)
{

}

int main() {
    int i = 0, len = 0;
    double x[11];
    double y[11];
    double start_x = 0, start_y = 0, distance = 0, total_distance = 0;

    while(scanf(stdin, "%lf", "%lf", &x[i], &y[i]) == 2){
        printf("First: %lf, Second: %lf", x[i], y[i]);
        i++;
    }
    len = i;
    i = 0;
    while (i < len){
        start_x = x[i];
        start_y = y[i];
        distance = calculate_distance(start_x, x[i + 1], start_y, y[i + 1]);
        total_distance += distance;
        printf("Total distance: %lf", total_distance);
        i++;
    }
}
