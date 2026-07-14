/*
 * Calculate the centers of two possible circles given the radius and two points
 * that lie on such circles.
 */

#include <math.h>
#include <stdio.h>

int main() {
        double a;
        double b;
        double radius;
        double x0;
        double x1;
        double x2;
        double x3;
        double x4;
        double xa;
        double y0;
        double y1;
        double y2;
        double y3;
        double y4;
        double ya;

        printf("Enter the first point (x y): ");
        scanf("%lf %lf", &x1, &y1);

        printf("Enter the second point (x y): ");
        scanf("%lf %lf", &x2, &y2);

        a = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) / 2.0;

        printf("Enter the radius (min. %.4f): ", a);
        scanf("%lf", &radius);

        b = sqrt(radius * radius - a * a);

        x0 = (x1 + x2) / 2.0;
        y0 = (y1 + y2) / 2.0;

        xa = (x2 - x1) / 2.0;
        ya = (y2 - y1) / 2.0;

        x3 = x0 + (b / a) * ya;
        y3 = y0 - (b / a) * xa;
        x4 = x0 - (b / a) * ya;
        y4 = y0 + (b / a) * xa;

        printf("\nCenter 1: (%f, %f)\n", x3, y3);
        printf("Center 2: (%f, %f)\n", x4, y4);

        return 0;
}
