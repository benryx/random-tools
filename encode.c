#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEXICON_FILENAME ".lexicon"
#define MAX_FILEPATH_SIZE 64
#define MAX_LEXICON_SIZE 0xFFFFF
#define MAX_TOKENS 64
#define MAX_TOKEN_SIZE 64
#define MAX_BIN_WIDTH 20

char lexicon[MAX_LEXICON_SIZE][MAX_TOKEN_SIZE + 1];
char line[MAX_TOKENS * (MAX_TOKEN_SIZE + 1)];
char *tokens[MAX_TOKENS];
unsigned long indices[MAX_TOKENS];
unsigned char bytes[MAX_TOKENS * MAX_BIN_WIDTH / 8];

void encode(void);
void decode(void);
int read_lexicon(void);
int read_tokens(void);
unsigned long find_indices(int n_tokens);
void print_binary(unsigned long number, int width);
int interleave(int n_tokens);
int save_lexicon(void);

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
        int n_tokens;
        int i;
        int n_bytes;

        if (!read_lexicon()) {
                fprintf(stderr, "Failed to read lexicon from file.\n");
        }

        n_tokens = read_tokens();

        printf("Tokens: %d\n", n_tokens);

        find_indices(n_tokens);

        for (i = 0; i < n_tokens; i++) {
                print_binary(indices[i], 8);
                printf(" [%3ld,%3lx]: %s\n",
                       indices[i],
                       indices[i],
                       lexicon[indices[i]]
                );
        }

        n_bytes = interleave(n_tokens);

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
                           (MAX_TOKEN_SIZE + 1) * sizeof(char), fp)) != NULL; i++) {
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

int read_tokens() {
        char *p;
        int n_tokens = 0;

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

                        /* Otherwise, we are at the start of a token */
                        tokens[n_tokens++] = p;

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

                        /* Otherwise, mark end of this token, and advance */
                        *p++ = '\0';
                }
        }

        return n_tokens;
}

unsigned long find_indices(int n_tokens) {
        unsigned long j;
        unsigned long max_index = 0;
        int i;

        for (i = 0; i < n_tokens; i++) {
                for (j = 0; j < MAX_LEXICON_SIZE; j++) {
                        if (*lexicon[j] == '\0') {
                                strcpy(lexicon[j], tokens[i]);
                                break;
                        } else if (strcmp(tokens[i], lexicon[j]) == 0) {
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

void print_binary(unsigned long number, int width) {
        int i;

        /*
        if (!number) {
                putchar('0');
        }
        */

        for (i = 0; i < width; i++) {
                putchar((number & 1) + '0');
                number >>= 1;
        }

        return;
}

int interleave(int n_tokens) {
        unsigned char *bp = bytes;
        int n_bits = 0;
        int remaining = 1;
        int shift;
        int i;

        for (shift = 0; remaining; shift++) {
                remaining = 0;
                for (i = 0; i < n_tokens; i++) {
                        if (indices[i] >> shift) {
                                remaining = 1;
                        }

                        *bp = (*bp << 1) | ((indices[i] >> shift) & 1);
                        n_bits++;

                        if (n_bits == 8) {
                                bp++;
                                n_bits = 0;
                        }
                }
        }

        return (bp - bytes) + 1;
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
