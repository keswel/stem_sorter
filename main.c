#include <stdio.h>
#include <string.h>
#include <dirent.h>

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
        if (isAudio(f_name)) printf("Audio! %s\n", f_name);
    }

    closedir(folder);
    return 0;
}
