#include <stdio.h>
#include <stdlib.h>

#define HOURLY_RATE 25.0

int main(int argc, char *argv[]) {
        /* Density of A572-50 plate steel (lbs/in^3) */
        double density = 0.282;

        double dollars_per_pound = 1.75;

        double thickness;
        double length;
        double width;

        double weight;
        double cost;
        double hours;

        if (argc == 4) {
                thickness = strtod(argv[1], NULL);
                length = strtod(argv[2], NULL);
                width = strtod(argv[3], NULL);
        } else {
                fprintf(stderr, "Enter plate dimensions (T L W): ");
                scanf("%lf%lf%lf", &thickness, &length, &width);
        }

        weight = density * thickness * length * width;
        cost = weight * dollars_per_pound;
        hours = cost / HOURLY_RATE;

        printf("\n%.2f lbs\n", weight);
        printf("Cost = $%.2f (%.2f hrs)\n\n", cost, hours);

        exit(EXIT_SUCCESS);
}
