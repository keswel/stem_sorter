#include <stdio.h>
#include <dirent.h>

int main(void) {
    DIR *folder;
    struct dirent *entry;

    folder = opendir(".");
    if (folder == NULL) {
        perror("Unable to read directory");
        return 1;
    }

    while ((entry = readdir(folder)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(folder);
    return 0;
}
