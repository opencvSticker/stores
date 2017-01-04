
#ifndef BITMAPFILEHEADER

#include <windows.h>

typedef struct tagBITMAPFILEHEADER { // bmfh 
    WORD    bfType; 
    DWORD   bfSize; 
    WORD    bfReserved1; 
    WORD    bfReserved2; 
    DWORD   bfOffBits; 
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{ // bmih 
    DWORD  biSize; 
    LONG   biWidth; 
    LONG   biHeight; 
    WORD   biPlanes; 
    WORD   biBitCount 
    DWORD  biCompression; 
    DWORD  biSizeImage; 
    LONG   biXPelsPerMeter; 
    LONG   biYPelsPerMeter; 
    DWORD  biClrUsed; 
    DWORD  biClrImportant; 
} BITMAPINFOHEADER; 

typedef struct tagPALETTEENTRY { // pe 
    BYTE peRed; 
    BYTE peGreen; 
    BYTE peBlue; 
    BYTE peFlags; 
} PALETTEENTRY; 

typedef struct tagBITMAP_FILE{ 
      BITMAPFILEHEADER bitmapheader;
      BITMAPINFOHEADER bitmapinfoheader;
      PALETTEENTRY palette[256];
      UCHAR *buffer;   //UCHAR ��С1�ֽ�(ͬBYTE), ��VC6�� 
} BITMAP_FILE;



#endif