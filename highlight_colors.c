#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_COLORS 6

int main() {
	size_t j;
	char *selected; 
	char *colors[NUM_COLORS] = {
		"Blue", 
		"Green", 
		"Orange",
		"Pink",
		"Purple",
		"Yellow",
	};

	srand(time(NULL));

	printf("\n");
	for (size_t i = 0; i < NUM_COLORS; i++) {
		j = i + (rand() % (NUM_COLORS - i));
		selected = colors[j];
		colors[j] = colors[i];
		colors[i] = selected;

		printf("%ld: %s\n", i, colors[i]);
	}
	printf("\n");

	return 0;
}
