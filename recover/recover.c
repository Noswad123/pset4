#include <stdio.h>
#include <stdint.h>
#include <cs50.h>
typedef uint8_t  BYTE;

 const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
 {
     printf("%s\n",argv[0]);
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover.c\n");
        return 1;
    }
        char *inFile=argv[1];
    FILE *inptr = fopen(inFile, "r");
    //open card.raw
    if (!inptr)
    {
        printf("Error opening the file \"card.raw\"...");
        return 2;
    }

    BYTE buffer[BLOCK_SIZE];

    int counter = 0;
    FILE *outptr = NULL;
    bool isFound=false;

    // Iterate over file contents and read 512 bytes until end of file
    while (fread(buffer, 1,512, inptr)==512)
    {
        //!feof(inptr)

    // Check if the first four bytes are a JPEG signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff
            && (buffer[3] & 0xf0) == 0xe0)
        {
            if(isFound)
            {
                fclose(outptr);
            }
            else //if have not found the first jpg
            {
                    isFound=true;
            }

            // Open a new JPEG file for writing
                char filename[10];
                sprintf(filename, "%03i.jpg", counter++);
                outptr=fopen(filename,"w");

        }
        // Close the file, if it is opened
        if(isFound)
        {
            fwrite(buffer, 512, 1,outptr);
        }

    }
//close all files and free memory
    fclose(outptr);
    fclose(inptr);

    return 0;
 }