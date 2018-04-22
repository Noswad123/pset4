#include <stdio.h>
#include <stdint.h>
#include <cs50.h>
#include "bmp.h"

 const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
 {
    printf("argv*: %s\n",argv[0]);
    if (argc != 1)
    {
        fprintf(stderr, "Usage: ./recover.c\n");
        return 1;
    }

    FILE *inptr = fopen("card.raw", "r");
    //open card.raw
    if (!inptr)
    {
        printf("Error opening the file \"card.raw\"...");
        return 1;
    }

    BYTE buffer[512];

    int counter = 0;
    FILE *outptr = NULL;
    bool isFound=false;

    // Iterate over file contents and read 512 bytes until end of file
    while (fread(buffer, 1, 512, inptr) != 0x00)
    {
    // Check if the first four bytes are a JPEG signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff
            && (buffer[3] & 0xf0) == 0xe0)
        {
            if(!isFound)     //if have not found the first jpg
            {
                isFound=true;
                printf("found one\n");
                // Open a new JPEG file for writing
                char filename[8];
                sprintf(filename, "%03d.jpg", counter++);
                outptr=fopen(filename,"w");

                if(!outptr)
                    return 1;

                fwrite(buffer, 1, 512,outptr);
            }
            else
            {
                    fclose(outptr);
                     // Open a new JPEG file for writing
                char filename[8];
                sprintf(filename, "%03d.jpg", counter++);
                outptr = fopen(filename,"w");

                if(!outptr)
                    return 1;
            }
        }
        else
            {
                // Close the file, if it is opened
                if(isFound)
                {
                    fwrite(buffer, 1, 512,outptr);
                }
            }
    }
//close all files and free memory
          fclose(outptr);
    fclose(inptr);

    return 0;
 }