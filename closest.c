#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_PARTS 4

int main() {
        struct part {
                char *name;
                double length;
        };

        int i;
        int j;

        struct part list[NUM_PARTS] = {
                /*{"wt5", 673.1},*/
                {"wt6", 4629.15},
                {"wt10", 5143.5},
                {"wt11", 4965.7},
                {"wt16", 2971.86},
                /*{"wt20", 660.4},*/
                /*{"wt23", 673.1},*/
                /*{"wt24", 660.4},*/
        };


        for (i = 0; i < NUM_PARTS; i++) {
                for (j = i + 1; j < NUM_PARTS; j++) {
                        double difference;

                        difference = fabs(list[i].length - list[j].length);

                        printf("%4s, %4s: %7.2f\n", list[i].name,
                                                    list[j].name,
                                                    difference);
                }
        }

        return 0;
}
