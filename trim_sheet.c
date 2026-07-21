/*
 * Compute the values that are needed to add a trim cut in SDS.
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
        double x1;
        double x2;
        double x3;
        double y;

        fprintf(stderr, "Point on left line (x y): ");
        scanf("%lf %lf", &x1, &y);

        fprintf(stderr, "Right line (x): ");
        scanf("%lf", &x2);

        fprintf(stderr, "Rightmost edge of sheet (x): ");
        scanf("%lf", &x3);

        printf("Start: %8.3f \t End: %8.3f\n", -(x1 + 0.06), -(x3 - x2 + 0.06));
        printf("Border: %3d\n", (int) (-y));

        exit(EXIT_SUCCESS);
}
