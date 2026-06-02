#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <regex.h>

typedef struct {
    char* file; 
} StemFile;

int isStem(char* f_name) {
    regex_t rgx;
    int value;

    // Matches "<anything> - " e.g. "song - ..." or "dkwoakd2d29312 - "
    value = regcomp(&rgx, ".+ - ", REG_EXTENDED);
    if (value != 0) return 0;  // failed to compile

    value = regexec(&rgx, f_name, 0, NULL, 0);
    regfree(&rgx);

    return value == 0;  // 0 means a match was found
}

// Appends a copy of name to the stems array, growing it if needed.
// Returns 1 on success, 0 on allocation failure.
int addStem(StemFile** stems, int* count, int* capacity, char* name) {
    if (*count >= *capacity) {
        int new_capacity = *capacity * 2;
        StemFile* tmp = (StemFile *)realloc(*stems, new_capacity * sizeof(StemFile));
        if (tmp == NULL) return 0;
        *stems = tmp;
        *capacity = new_capacity;
    }

    // Store a copy of the name; entry is reused on the next readdir.
    char* copy = strdup(name);
    if (copy == NULL) return 0;

    (*stems)[*count].file = copy;
    (*count)++;
    return 1;
}

int isAudio(char* f_name) {
    // prob a better way to do this but oh well.
    if (strstr(f_name, ".zip")) return 1;
    if (strstr(f_name, ".mp3")) return 1; 
    if (strstr(f_name, ".wav")) return 1; 
    return 0;
}

int main(void) {
    DIR *folder;
    struct dirent *entry;

    folder = opendir(".");
    if (folder == NULL) {
        perror("Unable to read directory");
        return 1;
    }
    
    int stem_capacity = 10;
    StemFile* stems = (StemFile *)malloc(stem_capacity * sizeof(StemFile));
    if (stems == NULL) {
        printf("Stem Array Failed\n");
        return 1;
    }

    int stem_count = 0;
    while ((entry = readdir(folder)) != NULL) {
        char* f_name = entry->d_name;
        if (isAudio(f_name)) {
            if (isStem(f_name)) {
                if (!addStem(&stems, &stem_count, &stem_capacity, f_name)) {
                    printf("Stem Array Add Failed\n");
                    free(stems);
                    closedir(folder);
                    return 1;
                }
            }
            printf("Audio! %s : %s\n", isStem(f_name) ? "Stem" : "Not Stem", f_name);
        }
    }

    printf("\nFound %d stem(s):\n", stem_count);
    for (int i = 0; i < stem_count; i++) {
        printf("  %s\n", stems[i].file);
        free(stems[i].file);
    }
    free(stems);

    closedir(folder);
    return 0;
}
