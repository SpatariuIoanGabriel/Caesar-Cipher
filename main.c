#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define NUM_LETTERS 26

/* Read the frequency distribution of letters from a file */
void read_distribution(float *dist) {
    FILE *fp;
    fp = fopen("distribution.txt", "r");
    if (fp == NULL) {
        printf("Error: distribution file not found.\n");
        exit(1);
    }
    for (int i = 0; i < NUM_LETTERS; i++) {
        fscanf(fp, "%f", &dist[i]);
    }
    fclose(fp);
}

/* Compute the frequency histogram of a text */
void compute_histogram(char *text, float *hist) {
    int len = strlen(text);
    int count[NUM_LETTERS] = {0};
    for (int i = 0; i < len; i++) {
        if (isalpha(text[i])) {
            count[tolower(text[i]) - 'a']++;
        }
    }
    for (int i = 0; i < NUM_LETTERS; i++) {
        hist[i] = (float)count[i] / len;
    }
}

/* Compute the Chi-square distance between two histograms */
float chi_square_distance(float *hist1, float *hist2) {
    float dist = 0;
    for (int i = 0; i < NUM_LETTERS; i++) {
        float diff = hist1[i] - hist2[i];
        dist += diff * diff / hist2[i];
    }
    return dist;
}

/* Decrypt a Caesar's cipher using frequency analysis */
void decrypt_caesar(char *ciphertext, char *plaintext, int key) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i++) {
        if (isalpha(ciphertext[i])) {
            if (islower(ciphertext[i]))
                plaintext[i] = 'a' + (tolower(ciphertext[i]) - 'a' - key + NUM_LETTERS) % NUM_LETTERS;
            else {
                plaintext[i] = 'A' + (toupper(ciphertext[i]) - 'A' - key + NUM_LETTERS) % NUM_LETTERS;
            }
        }else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[len] = '\0';
}


int main() {
    float english_dist[NUM_LETTERS];
    char ciphertext[1000], plaintext[1000];
    read_distribution(english_dist);
    printf("Enter ciphertext:\n");
    fgets(ciphertext, 1000, stdin);
    int len = strlen(ciphertext);
    if (ciphertext[len-1] == '\n') {
        ciphertext[len-1] = '\0';
    }
    float min_dist = INFINITY;
    int best_key = 0;
    for (int key = 0; key < NUM_LETTERS; key++) {
        char candidate[1000];
        decrypt_caesar(ciphertext, candidate, key);
        float candidate_hist[NUM_LETTERS];
        compute_histogram(candidate, candidate_hist);
        float dist = chi_square_distance(candidate_hist, english_dist);
        if (dist < min_dist) {
            min_dist = dist;
            best_key = key;
            strcpy(plaintext, candidate);
        }
    }
    printf("Plaintext:\n%s\n", plaintext);
    return 0;
}
