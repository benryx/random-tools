#include <stdio.h>
#include <stdlib.h>

#define HOURLY_RATE 25.0

int main(int argc, char *argv[]) {
        /* Density of A572-50 plate steel (lbs/in^3) */
        double density = 0.282;

        double dollars_per_pound = 1.75;

        double a;
        double b;
        double c;

        double weight;
        double cost;
        double hours;

        if (argc == 4) {
                a = strtod(argv[1], NULL);
                b = strtod(argv[2], NULL);
                c = strtod(argv[3], NULL);
        } else {
                fprintf(stderr, "Enter plate dimensions (space-separated): ");
                scanf("%lf%lf%lf", &a, &b, &c);
        }

        weight = density * a * b * c;
        cost = weight * dollars_per_pound;
        hours = cost / HOURLY_RATE;

        printf("\n%.2f lbs\n", weight);
        printf("Cost = $%.2f [%.0fh %dm]\n\n",
               cost, hours, (int)(hours * 60) % 60);

        exit(EXIT_SUCCESS);
}
