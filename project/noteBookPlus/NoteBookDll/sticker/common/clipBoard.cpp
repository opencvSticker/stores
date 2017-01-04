#include "clipBoard.h"
#include <tchar.h>
#include "../bmp/BMP.h"

namespace stickerEditor
{


	int SaveHBITMAP(HBITMAP hB, char * lpsz_FileName) {

		BITMAP csBitmap;
		int nRetValue = GetObject(hB, sizeof(csBitmap), &csBitmap);
		unsigned long n_BPP, n_Width, n_Height;

		if (nRetValue) {
			n_Width = (long)csBitmap.bmWidth;
			n_Height = (long)csBitmap.bmHeight;
			n_BPP = (long)csBitmap.bmBitsPixel;
			long sz = csBitmap.bmWidth*csBitmap.bmHeight*(csBitmap.bmBitsPixel >> 3);
			csBitmap.bmBits = (void *) new BYTE[sz];
			GetBitmapBits((HBITMAP)hB, sz, csBitmap.bmBits);

			printf("Proceeding Image %dx%d, BPP=%d", n_Width, n_Height, n_BPP, csBitmap.bmBits);
		}
		else {
			printf("Invalid Object in Clipboard Buffer"); return 1;
		}

		DWORD *lp_Canvas = new DWORD[n_Width * n_Height];
		if (n_BPP == 32) {
			for (unsigned long y = 0; y < n_Height; y++) {
				for (unsigned long x = 0; x < n_Width; x++) {
					RGBQUAD * rgb = ((RGBQUAD *)((char*)(csBitmap.bmBits)
						+ csBitmap.bmWidthBytes*y + x * sizeof(DWORD)));
					lp_Canvas[(n_Height - 1 - y)*n_Width + x] = *((DWORD *)rgb);
				}
			}
		}
		else if (n_BPP == 24) {
			for (unsigned long y = 0; y < n_Height; y++) {
				for (unsigned long x = 0; x < n_Width; x++) {

					RGBTRIPLE rgbi = *((RGBTRIPLE *)((char*)(csBitmap.bmBits)
						+ csBitmap.bmWidthBytes*y + x * 3));

					RGBQUAD rgbq;
					rgbq.rgbRed = rgbi.rgbtRed;
					rgbq.rgbGreen = rgbi.rgbtGreen;
					rgbq.rgbBlue = rgbi.rgbtBlue;
					lp_Canvas[(n_Height - 1 - y)*n_Width + x] = *((DWORD *)(&rgbq));
				}
			}
		}
		else {
			// here I could handle other resultions also, but I think it is 
			// too obvoius to add them here.... 
		}

		unsigned long n_Bits = 32;
		FILE *pFile = fopen(lpsz_FileName, "wb");
		if (pFile == NULL) { printf("File Cannot Be Written"); return 1; }

		// save bitmap file header
		BITMAPFILEHEADER fileHeader;
		fileHeader.bfType = 0x4d42;
		fileHeader.bfSize = 0;
		fileHeader.bfReserved1 = 0;
		fileHeader.bfReserved2 = 0;
		fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		fwrite((char*)&fileHeader, sizeof(fileHeader), 1, pFile);

		// save bitmap info header
		BITMAPINFOHEADER infoHeader;
		infoHeader.biSize = sizeof(infoHeader);
		infoHeader.biWidth = n_Width;
		infoHeader.biHeight = n_Height;
		infoHeader.biPlanes = 1;
		infoHeader.biBitCount = (unsigned short)(n_Bits & 0xffff);
		infoHeader.biCompression = BI_RGB;
		infoHeader.biSizeImage = 0;
		infoHeader.biXPelsPerMeter = 0;
		infoHeader.biYPelsPerMeter = 0;
		infoHeader.biClrUsed = 0;
		infoHeader.biClrImportant = 0;
		fwrite((char*)&infoHeader, sizeof(infoHeader), 1, pFile);
		fwrite((char*)lp_Canvas, 1, (n_Bits >> 3)*n_Width*n_Height, pFile);
		fclose(pFile);
		return 0;
	}
	
//http://bbs.csdn.net/topics/390008354
bool ClipBoard::getClipBoardText(HWND hWnd,vector<wchar_t> &wchs)
{
	if (!OpenClipboard(NULL))
	{
		return false;
	}

	UINT uFormt = 0;
	HANDLE hClip = NULL;
#ifdef UNICODE
	uFormt = CF_UNICODETEXT;
#else
	uFormt = CF_TEXT;
#endif
	hClip = GetClipboardData(uFormt);
	if (hClip == NULL)
	{
		return false;
	}

	TCHAR* lpszText = (TCHAR*)GlobalLock(hClip);

	for (int i = 0; i< _tcslen(lpszText); i++)
	{
		wchs.push_back(lpszText[i]);
	}

	GlobalUnlock(hClip);

	CloseClipboard();

	return true;

}

bool ClipBoard::getClipBoardBitmap(HWND hWnd, ToBmpFormat *pic)
{
	if (!OpenClipboard(NULL))
	{
		return false;
	}

	UINT uFormt = 0;
	HANDLE hClip = NULL;

	uFormt = CF_BITMAP;
	hClip = GetClipboardData(uFormt);
	if (hClip == NULL)
	{
		return false;
	}


	HBITMAP bitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
	if (bitmap != NULL)
	{
		SaveHBITMAP(bitmap, "acac.bmp");
		pic->read("acac.bmp");	
	}

	GlobalUnlock(hClip);

	CloseClipboard();

	return true;
}

}