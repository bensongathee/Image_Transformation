// BMP-related data types based on Microsoft's own wingdi.h
// Used from HarvardX CS50 with modifications by Varun Khatri
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
double nfmod(double a, double b);
// aliases for C/C++ primitive data types
// https://msdn.microsoft.com/en-us/library/cc230309.aspx
typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

// information about the type, size, and layout of a file
// https://msdn.microsoft.com/en-us/library/dd183374(v=vs.85).aspx
typedef struct
{
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
}BITMAPFILEHEADER;

// information about the dimensions and color format
// https://msdn.microsoft.com/en-us/library/dd183376(v=vs.85).aspx
typedef struct
{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
}BITMAPINFOHEADER;

// relative intensities of red, green, and blue
// https://msdn.microsoft.com/en-us/library/dd162939(v=vs.85).aspx
typedef struct
{
    BYTE blue;
    BYTE green;
    BYTE red;
}RGB;

// Alternative representation of RGB model in terms of hue, saturation and value
// http://learn.leighcotnoir.com/artspeak/elements-color/hue-value-saturation/
typedef struct
{
    double hue;
    double saturation;
    double value;
}HSV;

// to covert one pixel's data from RGB to HSV domain
// https://www.rapidtables.com/convert/color/rgb-to-hsv.html
HSV rgb2hsv(RGB in)
{   
    HSV hsv;
    // your code here
    double Rprime = ((double) in.red/255);
    double Gprime = ((double) in.green/255);
    double Bprime = ((double) in.blue/255);
    double rgbprimevals[] = {Bprime, Gprime, Rprime};
    double Cmax = -1000;
    double Cmin = 1000;
    double delta;
    double hue;
    double saturation;
    double value;
    int Cmaxrgbprimeval;

    for(int i = 0; i < 3; i++){
        if(rgbprimevals[i] > Cmax){
            Cmax = rgbprimevals[i];
            Cmaxrgbprimeval = i;
        }
        if(rgbprimevals[i] < Cmin){
            Cmin = rgbprimevals[i];
        }
    }

    delta = Cmax - Cmin;

    if(delta == 0){
        hue = 0;
    }else if(Cmaxrgbprimeval == 2){
        hue = 60 * nfmod((Gprime - Bprime)/delta, 6);
    }else if(Cmaxrgbprimeval == 1){
        hue = 60 * ((Bprime - Rprime)/delta+2);
    }else if(Cmaxrgbprimeval == 0){
        hue = 60 * ((Rprime - Gprime)/delta+4);
    }

    if(Cmax == 0){
        saturation = 0;
    }else if(Cmax != 0){
        saturation = delta/Cmax;
    }

    hsv.hue = hue;
    hsv.saturation = saturation;
    hsv.value = Cmax;

    return hsv;
}

// to covert one pixel's data from HSV to RGB domain
// https://www.rapidtables.com/convert/color/hsv-to-rgb.html
RGB hsv2rgb(HSV in)
{
    RGB rgb;
    // your code here
    double C = in.value * in.saturation;
    double X = C * (1- fabs(nfmod(in.hue/60,2)-1));
    double m = in.value - C;
    double Rprime;
    double Gprime;
    double Bprime;

    if(in.hue>=0 && in.hue<60){
        Rprime = C; 
        Gprime = X;
        Bprime = 0;
    }else if(in.hue>=60 && in.hue<120){
        Rprime = X; 
        Gprime = C;
        Bprime = 0;
    }else if(in.hue>=120 && in.hue<180){
        Rprime = 0; 
        Gprime = C;
        Bprime = X;
    }else if(in.hue>=180 && in.hue<240){
        Rprime = 0; 
        Gprime = X;
        Bprime = C;
    }else if(in.hue>=240 && in.hue<300){
        Rprime = X; 
        Gprime = 0;
        Bprime = C;
    }else if(in.hue>=300 && in.hue<360){
        Rprime = C; 
        Gprime = 0;
        Bprime = X;
    }

    rgb.red = (BYTE)((Rprime+m)*255);
    rgb.green = (BYTE)((Gprime+m)*255);
    rgb.blue = (BYTE)((Bprime+m)*255);

    return rgb;
}

BITMAPFILEHEADER *ReadBMFileHeader(FILE *fp, FILE *fp2);
BITMAPINFOHEADER *ReadBMInfoHeader(FILE *fp, FILE *fp2);
RGB rgbtri(FILE *fp);

double nfmod(double a, double b){
    return a - b * floor(a/b);
}