#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
void patchExecutable(const char* executablePath, int frameRate) {
    // Open the executable file in read and write mode
    FILE* fp = fopen(executablePath, "r+b");
    if (!fp) {
        printf("Failed to open the executable file.\n");
        return;
    }
 
    // Get the size of the executable file
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    rewind(fp);
 
    // Allocate memory for the buffer to hold the executable contents
    unsigned char* buffer = (unsigned char*)malloc(fileSize);
    if (!buffer) {
        printf("Failed to allocate memory for the executable.\n");
        fclose(fp);
        return;
    }
 
    // Read the executable file into the buffer
    if (fread(buffer, 1, fileSize, fp) != fileSize) {
        printf("Failed to read the executable file.\n");
        fclose(fp);
        free(buffer);
        return;
    }
 
    // Define the pattern to search for in the executable
    const char* pattern = "\xc7\x43\x20\x89\x88\x88\x3c\xeb\x43\x89\x73\x18\xeb\xca\x89\x73\x18";
    const char* replacement = "\xc7\x43\x20\x89\x88\x88\x3c\xeb\x43\x89\x73\x18\xeb\xca\x89\x73\x18";
 
    // Search for the pattern in the buffer
    unsigned char* match = memmem(buffer, fileSize, pattern, strlen(pattern));
    if (!match) {
        printf("Pattern not found in the executable.\n");
        fclose(fp);
        free(buffer);
        return;
    }
 
    // Replace the pattern with the replacement in the buffer
    memcpy(match, replacement, strlen(replacement));
 
    // Seek back to the beginning of the file
    fseek(fp, 0, SEEK_SET);
 
    // Write the modified buffer back to the file
    if (fwrite(buffer, 1, fileSize, fp) != fileSize) {
        printf("Failed to write the patched executable.\n");
    }
 
    // Close the file and free the allocated memory
    fclose(fp);
    free(buffer);
    printf("Patched binary saved.\n");
}
 
int main() {
    const char* executablePath = "eldenring.exe";
    int frameRate = 30;
 
    // Patch the executable with the desired frame rate
    patchExecutable(executablePath, frameRate);
 
    return 0;
}
 
