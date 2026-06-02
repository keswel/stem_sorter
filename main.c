#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <regex.h>

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

    while ((entry = readdir(folder)) != NULL) {
        char* f_name = entry->d_name;
        if (isAudio(f_name)) {
           printf("Audio! %s : %s\n", isStem(f_name) ? "Stem" : "Not Stem", f_name);
           
        }
    }

    closedir(folder);
    return 0;
}
