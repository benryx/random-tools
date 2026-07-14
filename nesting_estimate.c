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
                printf("Please enter quantity and area (q a): ");
                scanf("%d %lf", &quantity, &area);
        }

        estimate = ceil(5.7 * pow(quantity * area, 0.31) / 15) * 15;

        printf("Quantity: %d\n", quantity);
        printf("Area: %.1lf\n", area);
        printf("Estimate: %.0lf minutes\n", estimate);

        return 0;
}
