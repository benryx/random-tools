#include <stdio.h>

int main() {
	// Density of A572-50 plate steel (lbs/in^3)
	float density = 0.282;
	
	float dollars_per_pound = 1.75;
	
	float thickness;
	int length;
	int width;
	
	float weight, cost, hours;
	
	printf("Enter plate dimensions (T L W): ");
	scanf("%f %d %d", &thickness, &length, &width);
	
	weight = density * thickness * length * width;
	cost = weight * dollars_per_pound;
	hours = cost / 20.0;

	printf("\n%.1f lbs\n", weight);
	printf("Cost = $%.0f (%.1f hrs)\n", cost, hours);

	return 0;
}
