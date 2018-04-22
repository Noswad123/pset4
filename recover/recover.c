#include <stdio.h>
 #include <stdint.h>

 const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
 {
if (argc != 1)
    {
        fprintf(stderr, "Usage: ./recover.c\n");
        return 1;
    }
    FILE *inptr;

    if ((inptr = fopen("card.raw", "r")) == NULL)
    {
        printf("Error opening the file \"card.raw\"...");
        return 1;
    }

    uint8_t buf[512];

    int counter = 0;
    FILE *outptr = NULL;

    // Iterate over file contents
    while (fread(buf, BLOCK_SIZE, 1, inptr))
    {
        // Check if the first four bytes are a JPEG signature
        if (buf[0] == 0xff && buf[1] == 0xd8 && buf[2] == 0xff
            && (buf[3] == 0xe0 || buf[3] == 0xe1))
        {
            // Close the file, if it is opened
            if (outptr != NULL)
                fclose(outptr);

            char filename[8];
            sprintf(filename, "%03d.jpg", counter);

            // Open a new JPEG file for writing
            outptr = fopen(filename, "w");

            counter++;
        }

        if (outptr != NULL)
            fwrite(buf, BLOCK_SIZE, 1, outptr);
    }

    if (outptr != NULL)
        fclose(outptr);

    fclose(inptr);

    return 0;
 }