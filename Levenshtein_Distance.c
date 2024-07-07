#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 1024

int min(int a, int b, int c) 
{
    if (a < b && a < c) return a;
    else if (b < a && b < c) return b;
    return c;
}

int levenshtein_distance(const char *s, const char *t) 
{
    int len_s = strlen(s);
    int len_t = strlen(t);

    // Allocating memory for the distance matrix
    int **d = (int **)malloc((len_s + 1) * sizeof(int *));
    for (int i = 0; i <= len_s; i++) 
    {
        d[i] = (int *)malloc((len_t + 1) * sizeof(int));
    }

    // Initializing the distance matrix
    for (int i = 0; i <= len_s; i++) d[i][0] = i;
    for (int j = 0; j <= len_t; j++) d[0][j] = j;

    // Filling the distance matrix
    for (int i = 1; i <= len_s; i++) 
    {
        for (int j = 1; j <= len_t; j++) 
        {
            int cost = ((s[i - 1] == t[j - 1]) || (s[i - 1] == (t[j - 1] + 32)) || (s[i - 1] == (t[j - 1] - 32))) ? 0 : 1;
            d[i][j] = min(d[i - 1][j] + 1,        // deletion
                          d[i][j - 1] + 1,        // insertion
                          d[i - 1][j - 1] + cost); // substitution
        }
    }

    //Levenshtein Distance
    int distance = d[len_s][len_t];

    for (int i = 0; i <= len_s; i++) 
    {
        free(d[i]);
    }
    free(d);

    return distance;
}

int main() 
{
    FILE *file;
    char line[MAX_WORD_LENGTH];
    char *filename = "english_words.txt";
    char input_word[MAX_WORD_LENGTH];
    int min_distance = -1;
    char **closest_words = NULL;
    int closest_words_count = 0;

    printf("Enter the input word: ");
    scanf("%s", input_word);

    file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file %s for reading\n", filename);
        return 1;
    }

    while (fgets(line, sizeof(line), file)) 
    {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') 
        {
            line[len - 1] = '\0';
        }

        int distance = levenshtein_distance(input_word, line);

        if (min_distance == -1 || distance < min_distance) 
        {
            min_distance = distance;
            closest_words_count = 1;
            closest_words = realloc(closest_words, closest_words_count * sizeof(char *));
            closest_words[closest_words_count - 1] = strdup(line);
        } 
        else if (distance == min_distance) 
        {
            closest_words_count++;
            closest_words = realloc(closest_words, closest_words_count * sizeof(char *));
            closest_words[closest_words_count - 1] = strdup(line);
        }
    }

    fclose(file);

    if(min_distance == 0)
    {
        printf("The spelling is correct: %s\n", input_word);
    }
    else if (min_distance != -1 && min_distance != 0) 
    {
        printf("Did you mean:\n");
        for (int i = 0; i < closest_words_count; i++) 
        {
            printf("%s\n", closest_words[i]);
            free(closest_words[i]);
        }
    } 
    else 
    {
        printf("No words found in the file.\n");
    }

    free(closest_words);

    return 0;
}
