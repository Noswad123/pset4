// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
void info(BITMAPINFOHEADER bi, BITMAPFILEHEADER bf);
int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize n infile outfile\n");
        return 1;
    }

    // Store filenames & scale factor
    char *infile = argv[2];
    char *outfile = argv[3];
    int n = atoi(argv[1]);

    // Check the resize factor
	if(n < 0 || n > 50)
	{
			return 1;
	}

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

   // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //modify headers
    BITMAPFILEHEADER outBf = bf;
	BITMAPINFOHEADER outBi = bi;


        outBi.biWidth = bi.biWidth * n;
        outBi.biHeight = bi.biHeight * n;
        printf("bisizeimage: %i\n", bi.biSizeImage);
        printf("bfsize: %i\n",bf.bfSize);

        int newPadding =(4 - (outBi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
        outBi.biSizeImage = (3*abs(outBi.biWidth)+ newPadding)*abs(outBi.biHeight) ;
        printf("outbisizeimage: %i\n",outBi.biSizeImage);
        outBf.bfSize = outBi.biSizeImage + sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
        printf("outbfsize: %lu\n",sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER));

    // write outfile's BITMAPFILEHEADER
    fwrite(&outBf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&outBi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        for(int repeat = 0; repeat < n; repeat++)
        {
            fseek(inptr, 54+(bi.biWidth * 3 + padding) * i, SEEK_SET);

            for (int j = 0; j < bi.biWidth; j++)                // iterate over pixels in scanline
            {
                RGBTRIPLE triple;                               // temporary storage
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);    // read RGB triple from infile
                for (int k = 0; k < n; k++)                         // write RGB triple to outfile n times
                {
                  fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }

            }
            for (int r = 0; r < newPadding; r++)
            {
                fputc(0x00, outptr);
            }
        }
    }
    printf("padding: %i\n",padding);
     printf("padding: %i\n",newPadding);
    fclose(inptr);// close infile
    fclose(outptr); // close outfile
    return 0;// success
}


void info(BITMAPINFOHEADER bi, BITMAPFILEHEADER bf)
{
    printf("width: %i\n",bi.biWidth);
    printf("height: %i\n",bi.biHeight);
    printf("bisize: %i\n",bi.biSizeImage);
    printf("bfsize: %i\n",bf.bfSize);

}