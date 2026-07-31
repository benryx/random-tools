#include <stdio.h>
#include <stdlib.h>

#define HOURLY_RATE 25.0

int main(int argc, char *argv[]) {
        /* Density of A572-50 plate steel (lbs/in^3) */
        double density = 0.282;

        double dollars_per_pound = 1.75;

        double thickness;
        int length;
        int width;

        double weight;
        double cost;
        double hours;

        if (argc == 4) {
                thickness = strtod(argv[1], NULL);
                length = strtol(argv[2], NULL, 10);
                width = strtol(argv[3], NULL, 10);
        } else {
                fprintf(stderr, "Enter plate dimensions (T L W): ");
                scanf("%lf%d%d", &thickness, &length, &width);
        }

        weight = density * thickness * length * width;
        cost = weight * dollars_per_pound;
        hours = cost / HOURLY_RATE;

        printf("\n%.1f lbs\n", weight);
        printf("Cost = $%.0f (%.1f hrs)\n\n", cost, hours);

        exit(EXIT_SUCCESS);
}
