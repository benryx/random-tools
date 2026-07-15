/*
 * Approximate the bend deduction for the given bend angle and plate thickness.
 * This is just an estimate because I don't actually know the K-factor or the
 * bend radius.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265
#define RADIANS(x) ((x) * PI / 180.0)

#define DEFAULT_ANGLE 90.0
#define DEFAULT_THICKNESS 0.25
#define K 0.415

int main(int argc, char *argv[]) {
        double bend_allowance;
        double bend_angle = DEFAULT_ANGLE;
        double bend_deduction;
        double deduction_avg = 0;
        double deduction_sum = 0;
        double outside_setback;
        double plate_thickness = DEFAULT_THICKNESS;
        double radius;
        int i;

        if (argc > 1) {
                plate_thickness = strtod(argv[1], NULL);
        }
        if (argc > 2) {
                bend_angle = strtod(argv[2], NULL);
        }

        printf("\n%16s: %.4g\n", "Plate Thickness", plate_thickness);
        printf("%16s: %.4g\n", "Bend Angle (deg)", bend_angle);
        printf("\n%s: %.4g\n", "K-factor", K);

        printf("\n%7s |%8s%8s%8s\n", "Radius", "BA", "OSSB", "BD");

        for (i = 0; i < 2; i++) {
                /* The (inner) bend radius */
                radius = (i + 1) * plate_thickness;

                bend_allowance = RADIANS(bend_angle) * (
                        radius + (K * plate_thickness)
                );
                outside_setback = (radius + plate_thickness) * tan(
                        RADIANS(bend_angle / 2.0)
                );
                bend_deduction = (2.0 * outside_setback) - bend_allowance;

                printf("%7.3f |%8.3f%8.3f%8.3f\n", radius, bend_allowance,
                       outside_setback, bend_deduction);

                deduction_sum += bend_deduction;
        }

        deduction_avg = deduction_sum / i;
        printf("\nBend Deduction (avg): %.3f ~ %.0f/16\n\n",
               deduction_avg, deduction_avg * 16);

        exit(EXIT_SUCCESS);
}
