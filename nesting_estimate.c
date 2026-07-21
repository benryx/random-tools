/*
 * Estimate how long it will take me to nest some plate, given the quantity and
 * area of the plate to be nested.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
        double area;
        double estimate;
        int quantity;

        if (argc > 2) {
                quantity = strtol(argv[1], NULL, 10);
                area = strtod(argv[2], NULL);
        } else {
                fprintf(stderr, "Please enter quantity and area (q a): ");
                scanf("%d%lf", &quantity, &area);
        }

        /* Custom function I discovered gives a good estimate */
        estimate = ceil(5.7 * pow(quantity * area, 0.31) / 15) * 15;

        printf("Quantity: %d\n", quantity);
        printf("Area: %.1f\n", area);
        printf("Estimate: %.0f minutes\n", estimate);

        exit(EXIT_SUCCESS);
}
