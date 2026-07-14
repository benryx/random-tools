#include <stdio.h>
#include <math.h>

int main() {
	float gap_between_pieces = 0.125;
	float edge_gaps = 0.625;

	int n_parts;
	float part_width;
	float plate_width;

	int parts_per_col;
	float waste_per_col;

	int columns;
	int extra;

	printf("Number of parts, part width, plate width: ");
	scanf("%d %f %f", &n_parts, &part_width, &plate_width);


	parts_per_col = (plate_width - edge_gaps)
			/ (part_width + gap_between_pieces);
	waste_per_col = fmod((plate_width - edge_gaps),
			(part_width + gap_between_pieces));

	printf("\nParts per full column: %d\n", parts_per_col);
	printf("Wasted space per full column: %.2f\n", waste_per_col);

	columns = n_parts / parts_per_col;
	extra = n_parts % parts_per_col;

	printf("Columns: %d full", columns);

	if (extra > 0) {
		printf(" + %d/%d partial\n", extra, parts_per_col);
	} else {
		printf("\n");
	}
}
