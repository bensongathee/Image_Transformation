#include <stdio.h>
#include <string.h>
#include "bmp.h"

BITMAPFILEHEADER *ReadBMFileHeader(FILE *fp, FILE *fp2);
BITMAPINFOHEADER *ReadBMInfoHeader(FILE *fp, FILE *fp2);
RGB rgbtri(FILE *fp);
DWORD ReadLittleEndian4(FILE *fp,FILE *fp2);
WORD ReadLittleEndian2(FILE *fp,FILE *fp2);

int main(int argc , char * argv[]){
    FILE *fp;
    FILE *fp2;
    BITMAPFILEHEADER *bmpFileHeader = NULL;
    BITMAPINFOHEADER *bmpInfoHeader = NULL;
    RGB rgbtriple;
    HSV hsvtriple;

    if(argc != 3){
        puts("Usage: fileInputname fileOutputname");
        exit(1);
    }
    if((fp = fopen(argv[1],"r")) == NULL){
        printf("Cannot open file %s\n", argv[1]);
        exit(1);
    }
    if((fp2 = fopen(argv[2], "r+")) == NULL){
        printf("Cannot open file %s\n", argv[2]);
        exit(1);
    }

    bmpFileHeader = ReadBMFileHeader(fp , fp2);
    if(bmpFileHeader->bfType != 19778){
        puts("File not a Bitmap");
        exit(1);
    }

    bmpInfoHeader = ReadBMInfoHeader(fp, fp2);

    printf("File type          = %i\n", bmpFileHeader->bfType);
    printf("File size          = %d bytes\n", bmpFileHeader->bfSize);
    printf("Data offset        = %d bytes\n\n", bmpFileHeader->bfOffBits);
    printf("Info header size   = %d bytes\n", bmpInfoHeader->biSize);
    printf("Width              = %d pixels\n", bmpInfoHeader->biWidth);
    printf("Height             = %d pixels\n", bmpInfoHeader->biHeight);
    printf("Planes             = %d\n", bmpInfoHeader->biPlanes);
    printf("Bit count          = %d bits/pixel\n", bmpInfoHeader->biBitCount);
    printf("Compression        = %d\n", bmpInfoHeader->biCompression);
    printf("Size image         = %d bytes\n", bmpInfoHeader->biSizeImage);
    printf("X pixels per meter = %d\n", bmpInfoHeader->biXPelsPerMeter);
    printf("Y pixels per meter = %d\n", bmpInfoHeader->biYPelsPerMeter);
    printf("Color used         = %d colors\n", bmpInfoHeader->biClrUsed);

    int pixels = (int) (bmpInfoHeader->biWidth * bmpInfoHeader->biHeight);

    for(int i = 0; i < pixels; i++){
        rgbtriple = rgbtri(fp);
        hsvtriple = rgb2hsv(rgbtriple);
        if(hsvtriple.hue > 20 && hsvtriple.hue < 340){
            hsvtriple.saturation = 0;
        }
        rgbtriple = hsv2rgb(hsvtriple);
        fwrite(&rgbtriple.blue, 1, 1, fp2);
        fwrite(&rgbtriple.green, 1, 1, fp2);
        fwrite(&rgbtriple.red, 1, 1, fp2);
    }	
    free(bmpFileHeader);
    free(bmpInfoHeader);

    return 0;
}

BITMAPFILEHEADER *ReadBMFileHeader(FILE *fp, FILE *fp2){
    BITMAPFILEHEADER *header;

    WORD filetype;
    DWORD filesize;
    WORD reserved1;
    WORD reserved2;
    DWORD offset;

    fread(&filetype, 1, 2, fp); 	//incorrect
    fwrite(&filetype, 1, 2, fp2);

    filesize = ReadLittleEndian4(fp,fp2);

    fread(&reserved1, 1, 2, fp);
    fwrite(&reserved1, 1, 2, fp2);

    fread(&reserved2, 1, 2, fp);
    fwrite(&reserved2, 1, 2, fp2);

    offset = ReadLittleEndian4(fp,fp2);

    header = (BITMAPFILEHEADER *) malloc(sizeof(BITMAPFILEHEADER));
    header->bfType = filetype;
    header->bfSize = filesize;
    header->bfReserved1 = reserved1;
    header->bfReserved2 = reserved2;
    header->bfOffBits = offset;
    
    return header;
}

BITMAPINFOHEADER *ReadBMInfoHeader(FILE *fp, FILE *fp2){
    BITMAPINFOHEADER *infoheader;

    DWORD headersize;
    LONG imagewidth;
    LONG imageheight;
    WORD noofcolorplanes;
    WORD bitsperpixel;
    DWORD compression;
    DWORD imagesize;
    LONG xresolutionpixelspermeter;
    LONG yresolutionpixelspermeter;
    DWORD usedcolors;
    DWORD importantcolors;

    headersize = ReadLittleEndian4(fp,fp2);
    imagewidth = (LONG)ReadLittleEndian4(fp,fp2);
    imageheight = (LONG)ReadLittleEndian4(fp,fp2);
    noofcolorplanes = ReadLittleEndian2(fp,fp2);
    bitsperpixel = ReadLittleEndian2(fp,fp2);
    compression = ReadLittleEndian4(fp,fp2);
    imagesize = ReadLittleEndian4(fp,fp2);
    xresolutionpixelspermeter =(LONG) ReadLittleEndian4(fp,fp2);
    yresolutionpixelspermeter = (LONG)ReadLittleEndian4(fp,fp2);
    usedcolors = ReadLittleEndian4(fp,fp2);
    importantcolors =  ReadLittleEndian4(fp,fp2);

    infoheader = (BITMAPINFOHEADER *) malloc(sizeof(BITMAPINFOHEADER));
    infoheader->biSize = headersize;
    infoheader->biWidth = imagewidth;
    infoheader->biHeight = imageheight;
    infoheader->biPlanes = noofcolorplanes ;
    infoheader->biBitCount = bitsperpixel;
    infoheader->biCompression = compression;
    infoheader->biSizeImage = imagesize;
    infoheader->biXPelsPerMeter = xresolutionpixelspermeter;
    infoheader->biYPelsPerMeter = yresolutionpixelspermeter;
    infoheader->biClrUsed = usedcolors;
    infoheader->biClrImportant = importantcolors;

    return infoheader;
}

RGB rgbtri(FILE *fp){
    RGB rgbtriple;

    BYTE blue;
    BYTE green;
    BYTE red;

    fread(&blue, 1, 1, fp);
    fread(&green, 1, 1, fp); 
    fread(&red, 1, 1, fp);

    rgbtriple.blue = blue;
    rgbtriple.green = green;
    rgbtriple.red = red;

    return rgbtriple;
}

DWORD ReadLittleEndian4(FILE *fp, FILE *fp2){
    BYTE buf[4];
    DWORD result = 0;  
    int i;

    fread(buf, 1 , 4, fp);
    fwrite(buf, 1, 4, fp2);
    for(i = 3; i >= 0; i--){
        result = (result << 8) | (DWORD)buf[i];
    }   
    return result;
}

WORD ReadLittleEndian2(FILE *fp, FILE *fp2){
    BYTE buf[2];
    WORD result = 0;
    int i = 0;

    fread(buf, 1, 2, fp);
    fwrite(buf, 1, 2, fp2);
    for(i = 1; i >= 0; i--){
        result = (result << 8) | (WORD)buf[i];
    }
    return result;
}