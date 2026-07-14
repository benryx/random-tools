#include <stdio.h>

int main() {
        double height;
        double weight;

        printf("Enter WT size (?x?): ");
        scanf("%lf x%lf", &height, &weight);

        printf("Corresponding WF profile: W%.0fX%.0f\n", height * 2.0f, 
                                                         weight * 2.0f);

        return 0;
}
