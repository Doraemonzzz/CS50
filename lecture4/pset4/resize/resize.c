// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    //比例
    int n = atoi(argv[1]);
    if(n <= 0 || n > 100){
        fprintf(stderr, "Wrong size\n");
        return 2;
    }
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
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
        return 5;
    }


    //更新大小
    BITMAPFILEHEADER bf_out = bf;
    BITMAPINFOHEADER bi_out = bi;
    bi_out.biWidth = bi.biWidth * n;
    bi_out.biHeight = bi.biHeight * n;

    // determine padding for scanlines
    int padding_in = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding_out = (4 - (bi_out.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //更新大小
    bi_out.biSizeImage = abs(bi_out.biHeight) * (sizeof(RGBTRIPLE)*bi_out.biWidth + padding_out);
    bf_out.bfSize = bi_out.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof( BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_out, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_out, sizeof(BITMAPINFOHEADER), 1, outptr);



    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {

        //先将一行字符串重复n遍构成一个字符串
         RGBTRIPLE temp[bi_out.biWidth];
        //记录位置
        int x = 0;
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++){
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            //存入temp
            for(int k = 0; k < n; k++){
                temp[x++] = triple;
            }
        }

        //再将这个字符串重复n遍
        for(int j = 0; j < n; j++){
            fwrite(&temp, sizeof(temp), 1, outptr);

            for(int k=0; k<padding_out; k++){
                fputc(0x00, outptr);
            }
        }

        // skip over padding, if any
        fseek(inptr, padding_in, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
