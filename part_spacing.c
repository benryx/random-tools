#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
        double combined_height;
        double half_kerf;
        double space_between;
        double space_edges;
        double total_space;
        int num_parts;
        int sheet_height;

        if (argc != 5) {
                fprintf(stderr, "Usage: %s %s %s %s %s\n",
                                argv[0],
                                "NUM_PARTS",
                                "COMBINED_HEIGHT",
                                "SHEET_HEIGHT",
                                "HALF_KERF");
                exit(EXIT_SUCCESS);
        }

        num_parts = strtol(argv[1], NULL, 10);
        combined_height = strtod(argv[2], NULL);
        sheet_height = strtol(argv[3], NULL, 10);
        half_kerf = strtod(argv[4], NULL);

        total_space = sheet_height - combined_height;

        /*
         * The total number of spaces is num_parts + 1, but I want the spaces on
         * the edges to be 1.5 times larger than the spaces between the parts.
         * There will be 1.5 times space on one edge, then num_parts - 1 single
         * spaces in between the parts, then another 1.5 times space on the
         * other edge.
         */
        space_between = total_space * 1.0 / (1.5 + (num_parts - 1) + 1.5);
        space_edges = total_space * 1.5 / (1.5 + (num_parts - 1) + 1.5);

        printf("\n%13s:%7.3f\n", "Total space", total_space);
        printf("%13s:%8.4f\t(adjusted: %.4f)\n",
               "Space between",
               space_between,
               space_between - (2.0 * half_kerf));
        printf("%13s:%8.4f\t(adjusted: %.4f)\n\n",
               "Space @ edges",
               space_edges,
               space_edges - half_kerf);

        exit(EXIT_SUCCESS);
}
