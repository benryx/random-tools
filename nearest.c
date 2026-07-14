/* Given decimal inches, compute the closest 16th */

#include <stdio.h>
#include <math.h>

int main() {
	float decimal;
	int total_sixteenths;
        int feet;
        int inches;
        int sixteenths;
        int total_inches;

	printf("--- Nearest 16th ---\n");
	printf("Enter decimal inches: ");
	scanf("%f", &decimal);

	total_sixteenths = round(decimal * 16);
	total_inches = total_sixteenths / 16;
	sixteenths = total_sixteenths % 16;
	feet = total_inches / 12;
	inches = total_inches % 12;

	if (sixteenths == 0) {
		printf("Nearest: %d' %d\"\n", feet, inches);
	} else if (sixteenths % 8 == 0) {
		printf("Nearest: %d' %d %d/2\"\n", feet, inches, sixteenths / 8);
	} else if (sixteenths % 4 == 0) {
		printf("Nearest: %d' %d %d/4\"\n", feet, inches, sixteenths / 4);
	} else if (sixteenths % 2 == 0) {
		printf("Nearest: %d' %d %d/8\"\n", feet, inches, sixteenths / 2);
	} else {
		printf("Nearest: %d' %d %d/16\"\n", feet, inches, sixteenths);
	}

	return 0;
}
