/*
 * This is meant to be used in conjuction with the rat hole macro that I made in
 * Raptor. Unfortunately Raptor macros are too limited to handle all of this, so
 * I had to use a hybrid approach. Run the macro first and then use the points
 * that the macro generated to run this program, and it will give you the point
 * to finish the puzzle.
 *
 * The output of this program is intended to be redirected to your clipboard.
 * This will contain the x and y values of the point separated by a space.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265358979323846
#define RADIANS(x) ((x) * 180.0 / PI)
#define RADIUS 0.375

int main(int argc, char *argv[]) {
        struct point {
                double x;
                double y;
        };

        struct point final;
        struct point flange;
        struct point origin;
        struct point p0;
        struct point p1;
        struct point p2;

        double a;
        double b;
        double d2;
        double m1;
        double m2;
        double theta;


        if (argc < 3) {
                fprintf(stderr, "Enter point on circle (x y): ");
                scanf("%lf%lf", &(origin.x), &(origin.y));
        } else {
                origin.x = strtod(argv[1], NULL);
                origin.y = strtod(argv[2], NULL);
        }
        if (argc < 5) {
                fprintf(stderr, "Enter point touching the flange (x y): ");
                scanf("%lf%lf", &(flange.x), &(flange.y));
        } else {
                flange.x = strtod(argv[3], NULL);
                flange.y = strtod(argv[4], NULL);
        }

        /* Shift origin.y to the center of the circle */
        origin.y += (flange.y < origin.y) ? RADIUS : -RADIUS;

        /* Flange point relative to the origin (center of circle) */
        p0.x = flange.x - origin.x;
        p0.y = flange.y - origin.y;

        fprintf(stderr, "From center: (%f, %f)\n", p0.x, p0.y);

        d2 = (p0.x * p0.x) + (p0.y * p0.y);
        a = RADIUS * RADIUS / d2;
        b = RADIUS * sqrt(d2 - RADIUS * RADIUS) / d2;

        p1.x = (a * p0.x - b * p0.y);
        p1.y = (a * p0.y + b * p0.x);
        p2.x = (a * p0.x + b * p0.y);
        p2.y = (a * p0.y - b * p0.x);

        m1 = (p0.y - p1.y) / (p0.x - p1.x);
        m2 = (p0.y - p2.y) / (p0.x - p2.x);

        if (fabs(m1) < fabs(m2)) {
                theta = RADIANS(atan(fabs(m1)));
                final.x = origin.x + p1.x;
                final.y = origin.y + p1.y;
        } else {
                theta = RADIANS(atan(fabs(m2)));
                final.x = origin.x + p2.x;
                final.y = origin.y + p2.y;
        }

        fprintf(stderr, "[%.2f degrees / %.3f]\n", theta, theta / 25.4);
        fprintf(stderr, "%f, %f\n", final.x, final.y);

        /* I usually redirect this to the clipboard.
         * That's why I have everything else going to stderr.
         */
        fprintf(stdout, "%f %f\n", final.x, final.y);

        exit(EXIT_SUCCESS);
}
