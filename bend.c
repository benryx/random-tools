/*
 * Approximate the bend deduction for the given bend angle and plate thickness.
 * This is just an estimate because I don't actually know the K-factor or the
 * bend radius.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265

#define DEFAULT_ANGLE 90.0
#define DEFAULT_THICKNESS 0.25
#define K 0.415

int main(int argc, char *argv[]) {
        double bend_allowance;
        double bend_angle;
        double bend_deduction;
        double deduction_avg = 0;
        double deduction_sum = 0;
        double radii_range[2];
        double outside_setback;
        double plate_thickness;

        int i;

        if (argc > 1) {
                plate_thickness = strtod(argv[1], NULL);
        } else {
                plate_thickness = DEFAULT_THICKNESS;
        }

        if (argc > 2) {
                bend_angle = strtod(argv[2], NULL);
        } else {
                bend_angle = DEFAULT_ANGLE;
        }

        radii_range[0] = plate_thickness;
        radii_range[1] = 2 * plate_thickness;

        printf("\n%16s: %.4g\n", "Plate Thickness", plate_thickness);
        printf("%16s: %.4g\n", "Bend Angle (deg)", bend_angle);
        printf("\n%s: %.4g\n", "K-factor", K);

        printf("\n%7s |%8s%8s%8s\n", "Radius", "BA", "OSSB", "BD");

        for (i = 0; i < 2; i++) {
                /* The (inner) bend radius */
                double radius = radii_range[i];

                bend_allowance = bend_angle * (
                        radius + (K * plate_thickness)
                ) * PI / 180.0;
                outside_setback = (radius + plate_thickness) * tan(
                        (bend_angle / 2.0) * (PI / 180.0)
                );
                bend_deduction = (2.0 * outside_setback) - bend_allowance;

                printf("%7.3f |%8.3f%8.3f%8.3f\n", radius, bend_allowance,
                       outside_setback, bend_deduction);

                deduction_sum += bend_deduction;
        }

        deduction_avg = deduction_sum / 2.0;
        printf("\nBend Deduction (avg): %.3f ~ %.0f/16\n\n",
               deduction_avg,
               deduction_avg * 16);

        exit(EXIT_SUCCESS);
}
