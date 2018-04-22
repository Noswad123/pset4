// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize n infile outfile\n");
        return 1;
    }

    // remember filenames
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

        printf("infile bitmap info header\n");
        printf("biheight: %u biwidth: %u\n", bi.biHeight, bi.biWidth);
        printf("bisize: %u bisizeImage: %u biCompression: %u\n\n", bi.biSize, bi.biSizeImage, bi.biCompression);

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


        BITMAPFILEHEADER outBf;
		BITMAPINFOHEADER outBi;

		outBf = bf;
		outBi = bi;


    bi.biHeight *= 200;
    //resize the image
    outBi.biWidth *= n;
    outBi.biHeight *= n;



    // write outfile's BITMAPFILEHEADER
    fwrite(&outBf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&outBi, sizeof(BITMAPINFOHEADER), 1, outptr);

    printf("outfile bitmap info header\n");
        printf("biheight: %u biwidth: %u\n", outBi.biHeight, outBi.biWidth);
        printf("bisize: %u bisizeImage: %u biCompression: %u\n\n", outBi.biSize, outBi.biSizeImage, outBi.biCompression);



    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        for (int m = 0; m < n; m++)     // write RGB triple to outfile
        {
            for (int j = 0; j < bi.biWidth; j++)                // iterate over pixels in scanline
            {
                RGBTRIPLE triple;       // temporary storage
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);    // read RGB triple from infile
                for (int k = 0; k < n; k++)     // write RGB triple to outfile
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);      // write RGB triple to outfile n times
                }
            }
        }
        //fseek(inptr, outBi.biWidth+padding, SEEK_CUR);
    }
    //fseek(inptr, -(bi.biWidth * 3 + padding ), SEEK_CUR);
    //fseek(inptr, bi.biWidth*3+padding, SEEK_CUR);
    printf("%i\n",padding);

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
