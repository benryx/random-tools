#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEXICON_FILENAME ".lexicon"
#define MAX_LEXICON_SIZE 0xFFFF
#define MAX_WORDS 64

char *lexicon[MAX_LEXICON_SIZE];

void encode(void);
void decode(void);
int read_lexicon(void);
int write_lexicon(void);
int store_word(char *word, long index);
void get_binary(size_t n, int width, char *binary);
int bit_length(size_t n);

int main(void) {
        char dump_buf[256];
        char choice;

        while (1) {
                fprintf(stderr,
                        "Enter message prefixed by (E)ncode or (D)ecode? ");
                choice = getchar();

                switch (tolower(choice)) {
                case 'e':
                        encode();
                        break;
                case 'd':
                        decode();
                        break;
                case 'q':
                        exit(EXIT_SUCCESS);
                case EOF:
                        exit(EXIT_SUCCESS);
                default:
                        fprintf(stderr,
                                "Invalid input. "
                                "Prefix message with E or D.\n");
                        fgets(dump_buf, sizeof(dump_buf), stdin);
                        break;
                }
        }

        exit(EXIT_SUCCESS);
}

void encode(void) {
        char buf[256];
        char *bp = buf;
        char *words[MAX_WORDS];
        char *word;
        char *interleaved;
        char *binary;
        size_t n_words = 0;
        size_t i;
        size_t j;
        size_t indices[MAX_WORDS];
        size_t max_index;
        size_t bits_per_word;
        size_t total_bits;

        if (lexicon[0] == NULL) {
                if (!read_lexicon()) {
                        fprintf(stderr, "Failed to read lexicon.\n");
                }
        }

        fgets(buf, sizeof(buf), stdin);

        while (n_words < MAX_WORDS) {
                /* Skip all leading whitespace */
                bp += strspn(bp, " \t\n");

                /* If there's nothing than break */
                if (*bp == '\0') {
                        break;
                }

                /* Otherwise save the word */
                words[n_words++] = bp;

                /* Skip over text until we reach whitespace */
                bp += strcspn(bp, " \t\n");

                /* If were at null character, then we're done */
                if (*bp == '\0') {
                        break;
                }

                /* Otherwise replace whitespace character with null */
                *bp++ = '\0';
        }

        for (i = 0; i < n_words; i++) {
                word = words[i];

                for (j = 0; j < MAX_LEXICON_SIZE; j++) {
                        if (lexicon[j] == NULL) {
                                store_word(word, j);
                                break;
                        } else if (strcmp(word, lexicon[j]) == 0) {
                                break;
                        }
                }
                indices[i] = j;

                if (j > max_index) {
                        max_index = j;
                }
        }

        if (!write_lexicon()) {
                fprintf(stderr, "Failed to write lexicon.\n");
        }

        bits_per_word = bit_length(max_index);
        total_bits = bits_per_word * n_words;

        if ((interleaved = malloc((total_bits + 1) * sizeof(char))) == NULL) {
                fprintf(stderr, "Failed to allocate memory for interleaving.\n");
                return;
        }

        if ((binary = malloc((bits_per_word + 1) * sizeof(char))) == NULL) {
                fprintf(stderr,
                  "Failed to allocate memory for binary string representation\n.");
                return;
        }
        for (i = 0; i < n_words; i++) {
                get_binary(indices[i], bits_per_word, binary);
                printf("%s - %3ld: %s\n", binary, indices[i], words[i]);
        }
        printf("\n");

        return;
}

void decode(void) {
        char buf[256];

        fprintf(stderr, "DECODING...\n");
        fgets(buf, sizeof(buf), stdin);

        printf(buf);

        return;
}

int read_lexicon() {
        FILE *fp;
        char filepath[64];
        char line[256];
        int i;

        sprintf(filepath, "%s/%s", getenv("HOME"), LEXICON_FILENAME);

        if ((fp = fopen(filepath, "r")) == NULL) {
                fprintf(stderr, "Failed to open %s for reading.\n", filepath);
                return 0;
        }

        for (i = 0; (fgets(line, sizeof(line), fp) != NULL) &&
                    (i < MAX_LEXICON_SIZE); i++) {
                /* Replace the newline character with '\0' */
                line[strcspn(line, "\n")] = '\0';
                if (!store_word(line, i)) {
                        return 0;
                }
        }

        fclose(fp);

        return 1;
}

int write_lexicon() {
        FILE *fp;
        char filepath[64];
        int i;

        sprintf(filepath, "%s/%s", getenv("HOME"), LEXICON_FILENAME);

        if ((fp = fopen(filepath, "w")) == NULL) {
                fprintf(stderr, "Failed to open %s for writing.\n", filepath);
                return 0;
        }

        for (i = 0; lexicon[i] != NULL && i < MAX_LEXICON_SIZE; i++) {
                fprintf(fp, "%s\n", lexicon[i]);
        }

        fclose(fp);

        return 1;
}

int store_word(char *word, long index) {
        if ((lexicon[index] = malloc((strlen(word) + 1) * sizeof(char))) == NULL) {
                fprintf(stderr,
                        "Failed to allocate memory for '%s' at index %ld.\n",
                        word, index);
                return 0;
        }

        strcpy(lexicon[index], word);

        return 1;
}

void get_binary(unsigned long n, int width, char *binary)
{
        int i = 0;

        for (i = width - 1; i >= 0; i--) {
                binary[(width - 1) - i] = (n & (1 << i)) ? '1' : '0';
        }
        binary[width] = '\0';
}

int bit_length(size_t n) {
        int bits = 0;

        while (n > 0) {
                bits++;
                n >>= 1;
        }

        return bits;
}
