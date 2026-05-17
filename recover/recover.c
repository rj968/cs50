#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_if_jpeg(uint8_t *buffer);
int main(int argc, char *argv[])
{
    // checking for valid input
    if (argc != 2)
    {
        printf("Enter only a single file\n");
        return 1;
    }
    // opening source file
    FILE *src = fopen(argv[1], "r");
    if (src == NULL)
    {
        printf("Out of memory\n");
        return 1;
    }

    uint8_t ptr[512];
    int filenum = 0;

    char filename[8];
    FILE *jpg = NULL;
    while (fread(ptr, sizeof(uint8_t), 512, src) == 512)

    {
        if (check_if_jpeg(ptr))
        {
            if (filenum > 0)
                fclose(jpg);
            sprintf(filename, "%03i.jpg", filenum);
            jpg = fopen(filename, "w");
            fwrite(ptr, sizeof(uint8_t), 512, jpg);
            filenum++;
        }
        else if (jpg != NULL)
            fwrite(ptr, sizeof(uint8_t), 512, jpg);
    }

    if (jpg != NULL)
        fclose(jpg);
    fclose(src);
}
// if not a jpeg then try doing fseek to the next block
int check_if_jpeg(uint8_t *buffer)
{
    return buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
           (buffer[3] & 0xf0) == 0xe0;
}
