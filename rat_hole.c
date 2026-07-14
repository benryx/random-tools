#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265358979323846
#define R 0.375

void print_point(double x, double y, double cx, double cy, double theta);

int main(int argc, char *argv[]) {
        double a;
        double b;
        double cx;
        double cy;
        double d2;
        double pfx;
        double pfy;
        double pcx;
        double pcy;
        double theta1;
        double theta2;
        double x0;
        double x1;
        double x2;
        double y0;
        double y1;
        double y2;

        if (argc < 3) {
                fprintf(stderr, "Enter point on circle (x y): ");
                scanf("%lf %lf", &pcx, &pcy);
        } else {
                pcx = strtod(argv[1], NULL);
                pcy = strtod(argv[2], NULL);
        }
        if (argc < 5) {
                fprintf(stderr, "Enter point under flange (x y): ");
                scanf("%lf %lf", &pfx, &pfy);
        } else {
                pfx = strtod(argv[3], NULL);
                pfy = strtod(argv[4], NULL);
        }

        cx = pcx;
        if (pfy < pcy) {
                cy = pcy + R;
        } else {
                cy = pcy - R;
        }

        x0 = pfx - cx;
        y0 = pfy - cy;

        fprintf(stderr, "From center: (%f, %f)\n", x0, y0);

        d2 = (x0 * x0) + (y0 * y0);

        a = R * R / d2;
        b = R * sqrt(d2 - R * R) / d2;

        x1 = (a * x0 - b * y0);
        y1 = (a * y0 + b * x0);
        x2 = (a * x0 + b * y0);
        y2 = (a * y0 - b * x0);

        theta1 = 180.0 * atan(fabs((y0 - y1) / (x0 - x1))) / PI;
        theta2 = 180.0 * atan(fabs((y0 - y2) / (x0 - x2))) / PI;

        if (theta1 < theta2) {
                print_point(x1, y1, cx, cy, theta1);
        } else {
                print_point(x2, y2, cx, cy, theta2);
        }

        return 0;
}

void print_point(double x, double y, double cx, double cy, double theta) {
        fprintf(stderr, "[%.2f degrees / %.3f]\n", theta, theta / 25.4);
        fprintf(stderr, "%f, %f\n", cx + x, cy + y);

        fprintf(stdout, "%f %f\n", cx + x, cy + y);
}
