#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEXICON_FILENAME ".lexicon"
#define MAX_FILEPATH_SIZE 64
#define MAX_LEXICON_SIZE 0xFFFFF
#define MAX_WORDS 64
#define MAX_WORD_SIZE 64
#define MAX_BIN_WIDTH 20
#define MAX_RESULT_SIZE 32

char lexicon[MAX_LEXICON_SIZE][MAX_WORD_SIZE + 1];
char line[MAX_WORDS * (MAX_WORD_SIZE + 1)];
char *words[MAX_WORDS];
size_t indices[MAX_WORDS];
char binary[MAX_WORDS][MAX_BIN_WIDTH + 1];
char combined[MAX_WORDS * MAX_BIN_WIDTH + 1];
char result[MAX_RESULT_SIZE];
unsigned char bytes[MAX_WORDS * MAX_BIN_WIDTH / 8];

void encode(void);
void decode(void);
int read_lexicon(void);
int read_words(void);
size_t find_indices(int n_words);
int save_lexicon(void);
void fill_binary(int n_words, int width);
int bit_width(size_t number);
void interleave(int n_words, int width);
int result_to_bytes(void);

int main(void) {
        /*int choice;

        fprintf(stderr, "Enter message prefixed by (e)ncode or (d)ecode: ");
        choice = getchar();

        switch (tolower(choice)) {
        case 'e':
                encode();
                break;
        case 'd':
                decode();
                break;
        default:
                break;
        }
        */

        encode();

        exit(EXIT_SUCCESS);
}

void encode(void) {
        int n_words;
        size_t max_index;
        int max_width;
        int i;
        int n_bytes;

        if (!read_lexicon()) {
                fprintf(stderr, "Failed to read lexicon from file.\n");
        }

        n_words = read_words();

        printf("Tokens: %d\n", n_words);

        max_index = find_indices(n_words);
        max_width = bit_width(max_index);
        printf("Max Bit Width: %d\n", max_width);

        fill_binary(n_words, max_width);

        for (i = 0; i < n_words; i++) {
                printf("[%3ld,%3lx] %s: %s\n",
                       indices[i],
                       indices[i],
                       binary[i],
                       lexicon[indices[i]]
                );
        }

        interleave(n_words, max_width);

        printf("Combined binary:\n\t%s\n", combined);

        n_bytes = result_to_bytes();

        printf("Final result:\n\t");
        for (i = 0; i < n_bytes; i++) {
                printf("%02x ", bytes[i]);
        }
        printf("\n");

        if (!save_lexicon()) {
                fprintf(stderr, "Failed to write lexicon to file.\n");
        }

        return;
}

void decode(void) {
        return;
}

int read_lexicon() {
        FILE *fp;
        char filepath[MAX_FILEPATH_SIZE];
        char *lp;
        int i;

        sprintf(filepath, "%s/%s", getenv("HOME"), LEXICON_FILENAME);

        if ((fp = fopen(filepath, "r")) == NULL) {
                fprintf(stderr, "Failed to open %s for reading.\n", filepath);
                return 0;
        }

        for (i = 0; (fgets(lexicon[i],
                           (MAX_WORD_SIZE + 1) * sizeof(char), fp)) != NULL; i++) {
                lp = lexicon[i];

                /* We don't want blank entries, overwrite these. */
                if (*lp == '\n' || *lp == ' ') {
                        *lp = '\0';
                        i--;
                        continue;
                }

                while (*lp != '\n' && *lp != '\0') {
                        lp++;
                };
                *lp = '\0';
        }

        fclose(fp);

        return 1;
}

int read_words() {
        char *p;
        int n_words = 0;

        if ((fgets(line, sizeof(line), stdin)) != NULL) {
                p = line;

                while (*p != '\0') {
                        /* Skip leading whitespace */
                        while (
                                *p == ' ' ||
                                *p == '\t' ||
                                *p == '\n'
                        ) {
                                p++;
                        }

                        /* If at null character, then we're done */
                        if (*p == '\0') {
                                break;
                        }

                        /* Otherwise, we are at the start of a word */
                        words[n_words++] = p;

                        /* Skip until whitespace or null */
                        while (
                                *p != ' ' &&
                                *p != '\t' &&
                                *p != '\n' &&
                                *p != '\0'
                        ) {
                                p++;
                        }

                        /* If at null character, then we're done */
                        if (*p == '\0') {
                                break;
                        }

                        /* Otherwise, mark end of this word, and advance */
                        *p++ = '\0';
                }
        }

        return n_words;
}

size_t find_indices(int n_words) {
        int i;
        size_t j;
        size_t max_index = 0;

        for (i = 0; i < n_words; i++) {
                for (j = 0; j < MAX_LEXICON_SIZE; j++) {
                        if (*lexicon[j] == '\0') {
                                strcpy(lexicon[j], words[i]);
                                break;
                        } else if (strcmp(words[i], lexicon[j]) == 0) {
                                break;
                        }
                }

                indices[i] = j;

                if (j > max_index) {
                        max_index = j;
                }
        }

        return max_index;
}

int save_lexicon(void) {
        FILE *fp;
        char filepath[MAX_FILEPATH_SIZE];
        size_t i;

        sprintf(filepath, "%s/%s", getenv("HOME"), LEXICON_FILENAME);

        if ((fp = fopen(filepath, "w")) == NULL) {
                fprintf(stderr, "Failed to open %s for writing.\n", filepath);
                return 0;
        }

        for (i = 0; *lexicon[i] != '\0'; i++) {
                fprintf(fp, "%s\n", lexicon[i]);
        }

        fclose(fp);

        return 1;
}

void fill_binary(int n_words, int width) {
        size_t number;
        int i;
        int j;

        for (i = 0; i < n_words; i++) {
                number = indices[i];

                binary[i][width] = '\0';
                for (j = width - 1; j >= 0; j--) {
                        binary[i][j] = (number & 1) + '0';
                        number >>= 1;
                }
        }

        return;
}

int bit_width(size_t number) {
        int width;

        for (width = 0; number != 0; width++) {
                number >>= 1;
        }

        return width;
}

void interleave(int n_words, int width) {
        char *cp = combined;
        int x;
        int r;
        int c;

        /*
         * Pad with zeros in the front until it's a multiple of 8.
         * So then we can split it up into bytes.
         */
        x = n_words * width;
        while (x++ % 8 != 0) {
                *cp++ = '0';
        }

        for (c = 0; c < width; c++) {
                for (r = 0; r < n_words; r++) {
                        *cp++ = binary[r][c];
                }
        }

        *cp = '\0';
}

int result_to_bytes(void) {
        char *cp = combined;
        unsigned char byte;
        int i;
        int n_bytes = 0;

        while (*cp != '\0') {
                byte = 0;

                for (i = 0; i < 8; i++) {
                        if (*cp == '\0') {
                                break;
                        }
                        byte = (byte << 1) | (*cp++ - '0');
                }

                bytes[n_bytes++] = byte;
        }

        return n_bytes;
}
