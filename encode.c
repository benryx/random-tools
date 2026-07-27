#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEXICON_FILENAME ".lexicon"
#define MAX_FILEPATH_SIZE 64
#define MAX_LEXICON_SIZE 0xFFFFF
#define MAX_WORDS 64
#define MAX_WORD_SIZE 64

char lexicon[MAX_LEXICON_SIZE][MAX_WORD_SIZE + 1];
char line[MAX_WORDS * (MAX_WORD_SIZE + 1)];
char *words[MAX_WORDS];
size_t indices[MAX_WORDS];

void encode(void);
void decode(void);
int read_lexicon(void);
size_t read_words(void);
void find_indices(void);
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
        size_t i;

        if (read_lexicon()) {
                fprintf(stderr, "Successfully read lexicon from file!\n");
        }

        read_words();

        find_indices();

        for (i = 0; words[i] != NULL; i++) {
                fprintf(stderr, "%3ld: %s\n", indices[i], lexicon[indices[i]]);
        }

        save_lexicon();

        return;
}

void decode(void) {
        return;
}

int read_lexicon() {
        FILE *fp;
        char filepath[MAX_FILEPATH_SIZE];
        char *lp;
        size_t i;

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

size_t read_words() {
        char *bp;
        size_t n_words = 0;

        if ((fgets(line, sizeof(line), stdin)) != NULL) {
                bp = line;

                while (*bp != '\0') {
                        /* Skip leading whitespace */
                        while (
                                *bp == ' ' ||
                                *bp == '\t' ||
                                *bp == '\n'
                        ) {
                                bp++;
                        }

                        /* If at null character, then we're done */
                        if (*bp == '\0') {
                                break;
                        }

                        /* Otherwise, we are at the start of a word */
                        words[n_words++] = bp;

                        /* Skip until whitespace or null */
                        while (
                                *bp != ' ' &&
                                *bp != '\t' &&
                                *bp != '\n' &&
                                *bp != '\0'
                        ) {
                                bp++;
                        }

                        /* If at null character, then we're done */
                        if (*bp == '\0') {
                                break;
                        }

                        /* Otherwise, mark end of this word */
                        *bp++ = '\0';
                }
        }

        return n_words;
}

void find_indices(void) {
        int found;
        size_t i = 0;
        size_t j = 0;

        for (i = 0; words[i] != NULL; i++) {
                found = 0;

                for (j = 0; *lexicon[j] != '\0'; j++) {
                        if (strcmp(words[i], lexicon[j]) == 0) {
                                indices[i] = j;
                                found = 1;
                                break;
                        }
                }
                if (!found) {
                        strcpy(lexicon[j], words[i]);
                        indices[i] = j;
                }
        }

        return;
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
