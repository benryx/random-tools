/*
 * Compute the values that are needed to add a trim cut in SDS.
 */

#include <stdio.h>

int main(void) {
        double x1;
        double x2;
        double x3;
        double y;

        printf("Point on left line (x y): ");
        scanf("%lf %lf", &x1, &y);

        printf("Right line (x): ");
        scanf("%lf", &x2);

        printf("Rightmost edge of sheet (x): ");
        scanf("%lf", &x3);

        printf("Start: %8.3f \t End: %8.3f\n", -(x1 + 0.06), -(x3 - x2 + 0.06));
        printf("Border: %3d\n", (int) (-y));

        return 0;
}
