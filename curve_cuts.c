#include <math.h>
#include <stdio.h>

int main(void) {
        double length = 15.3125;
        double height = 6;
        double h = 5.25;
        double r = 4.305;

        double x = sqrt(pow(r, 2) - pow(h / 2, 2));

        double xc = length + x;
        double yc = height / 2;

        double x1 = length;
        double y1 = (height / 2) - (h / 2);

        double x2 = length;
        double y2 = (height / 2) + (h / 2);

        double new_x = sqrt(pow(r, 2) - pow((h / 2) - .125, 2));
        double x3 = xc - new_x;
        double y3 = y1 + .125;
        double x4 = xc - new_x;
        double y4 = y2 - .125;


        printf("(%.3f, %.3f) - (%.3f, %.3f), (%.3f, %.3f)\n", xc, yc,
                                                                    x1, y1,
                                                                    x2, y2);
        printf("\nNew values...\n");
        printf("(%.3f, %.3f)\n", x3, y3);
        printf("\t(%.3f, %.3f)\n", xc, yc);
        printf("(%.3f, %.3f)\n", x4, y4);

        printf("\nBack side...\n");
        printf("(%.3f, %.3f)\n", length - x3, y3);
        printf("\t(%.3f, %.3f)\n", length - xc, yc);
        printf("(%.3f, %.3f)\n", length - x4, y4);

        return 0;
}
