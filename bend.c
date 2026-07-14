#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265

#define DEFAULT_ANGLE 90.0
#define DEFAULT_THICKNESS 0.25
#define K 0.415

int main(int argc, char *argv[]) {
        double allowance;        /* Bend Allowance */
        double bend_angle;
        double deduction;        /* Bend Deduction */
        double deduction_avg = 0;
        double deduction_sum = 0;
        double radii_range[2];
        double setback;          /* Outside Setback */
        double thickness;        /* Plate thickness */

        int i;

        if (argc > 1) {
                thickness = strtod(argv[1], NULL);
        } else {
                thickness = DEFAULT_THICKNESS;
        }
        if (argc > 2) {
                bend_angle = strtod(argv[2], NULL);
        } else {
                bend_angle = DEFAULT_ANGLE;
        }

        radii_range[0] = thickness;
        radii_range[1] = 2 * thickness;

        printf("\n%16s: %.4g\n", "Plate Thickness", thickness);
        printf("%16s: %.4g\n", "Bend Angle (deg)", bend_angle);
        printf("%16s: %.4g\n", "K-factor", K);

        printf("\n%7s |%8s%8s%8s\n", "Radius", "BA", "OSSB", "BD");

        for (i = 0; i < 2; i++) {
                double radius = radii_range[i]; /* Bend Radius (Inner) */
                allowance = bend_angle * PI/180.0 * (radius + K * thickness);
                setback = (radius + thickness) * tan(bend_angle * PI/180.0 / 2.0);
                deduction = 2.0 * setback - allowance;

                printf("%7.3f |%8.3f%8.3f%8.3f\n", radius, allowance,
                       setback, deduction);

                deduction_sum += deduction;
        }

        deduction_avg = deduction_sum / 2.0;
        printf("\nBend Deduction (avg): %.3f ~ %.0f/16\n\n",
               deduction_avg,
               deduction_avg * 16);

        exit(EXIT_SUCCESS);
}
