#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_LENGTHS 15

int sum(char *list) {
	int total = 0;
	
	for (size_t i = 0; i < NUM_LENGTHS; ++i) {
		total = total + list[i];
	}

	return total;
}

int main() {
	float lengths[NUM_LENGTHS] = {
		774.70,  // wt25
		863.60,  // wt26
		1133.35, // wt31
		1557.27, // wt33
		1308.04, // wt48
		1565.21, // wt52
		1536.64, // wt54
		1006.35, // wt59
		925.45,  // wt60
		817.50,  // wt61
		1333.50, // wt65
		1308.10, // wt66
		927.10,  // wt67
		1056.03, // wt77
		325.35,  // wt78
	};
	char covered[NUM_LENGTHS] = {
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
	};

	float total_diff = 0.0;

	srand(time(NULL));

	while (sum(covered) < (NUM_LENGTHS - 3)) {
		float random_one;
		float closest;
		float diff;
		float min_diff = 1000.0;
		size_t min_j;

		size_t i = rand() % NUM_LENGTHS;

		if (covered[i] == 1) {
			continue;
		}
		
		random_one = lengths[i];
		covered[i] = 1;


		for (size_t j = 0; j < NUM_LENGTHS; ++j) {
			if (covered[j] == 1) {
				continue;
			}

			diff = fabs(random_one - lengths[j]);
			if (diff < min_diff) {
				min_j = j;
				min_diff = diff;
			}
		}
		closest = lengths[min_j];
		covered[min_j] = 1;

		printf("%7.2f, %7.2f  [%6.2f]\n", random_one, closest, min_diff);

		total_diff += min_diff;
	}

	float remainder = 0;
	float longest = 0;
	float remainder_diff = 0;

	for (size_t i = 0; i < NUM_LENGTHS; ++i) {
		if (covered[i] == 1) {
			continue;
		}

		remainder += lengths[i];

		if (lengths[i] > longest) {
			longest = lengths[i];
		}

		printf("\n%7.2f", lengths[i]);
	}

	remainder_diff = fabs(remainder - (2 * longest));

	printf("  [%.2f]\n", remainder_diff);

	total_diff += remainder_diff;

	printf("\nTotal diff: %.2f\n", total_diff);

	return 0;
}
