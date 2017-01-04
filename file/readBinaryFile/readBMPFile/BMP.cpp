#include "stdafx.h"
#include "bmp.h"


BMPContent::BMPContent()
{
	m_bytes = NULL;
}
BMPContent::~BMPContent()
{	
	dealloc();
}

void BMPContent::dealloc()
{
	if (m_bytes != NULL)
	{
		delete[] m_bytes;
		m_bytes = NULL;
	}
}

bool BMPContent::alloc(unsigned int size)
{
	dealloc();
	m_bytes = new char[size];
	if (m_bytes == NULL)
	{
		return false;
	}

	return true;
}

/************************************************************************/
/* the example to use getPixel()
for (int j=0; j<bm->getBMPHeight(); j++)
{
for (int i=0; i< bm->getBMPWidth(); i++)
{
    //SetPixel(hdc, i, j, bm->getPixel((height - j - 1)*width + (i + 1)));
    SetPixel(hdc, i, j, bm->getPixel(i,j));
}
}
*/
/************************************************************************/

//the iWidth and iHeight both are begin from 0
COLORREF BMPContent::getPixel(unsigned int iWidth, unsigned int iHeight)
{
	unsigned int width = getBMPWidth();
	unsigned int height = getBMPHeight();

	if (iWidth>(width-1) || iHeight > (height-1))
	{
		return 0xffffffff;
	}

	getPixel((height - iHeight - 1)*width + (iWidth + 1));
}

COLORREF BMPContent::getPixel(unsigned int index)
{
	const char * fileContent = m_bytes;
	if (fileContent != NULL)
	{
		unsigned int paddingCount = 0;
		unsigned int remainder = (getBMPWidth()*(m_bitmapinfoheader.biBitCount / 8)) % 4;
		if (remainder > 0)
		{
			paddingCount = 4 - remainder;
		}

		unsigned int counterInWidth = (index - 1) / getBMPWidth();

		unsigned int indexInFile = m_bitmapheader.bfOffBits + (index - 1) *(m_bitmapinfoheader.biBitCount / 8)
			+ counterInWidth * paddingCount;
		unsigned char R = fileContent[indexInFile + 2];
		unsigned char G = fileContent[indexInFile + 1];
		unsigned char B = fileContent[indexInFile + 0];
		
		return RGB(R, G, B);
	}

	return 0xffffffff;

}

unsigned int BMPContent::getBMPWidth()
{
	return m_bitmapinfoheader.biWidth;	
}

unsigned int BMPContent::getBMPHeight()
{
	return m_bitmapinfoheader.biHeight;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

BmpHelper::BmpHelper()
{

}

shared_ptr<BMPContent> BmpHelper::readBmpFile(const char *file)
{
	shared_ptr<BMPContent> bmpContent = make_shared<BMPContent>();

	shared_ptr<FileContentInBi> fileContent = ReadFileContentInBi::read(file);
	if (fileContent == NULL)
	{
		return NULL;
	}

	unsigned int fileLen = ReadFileContentInBi::getFileLength(file);
	if (fileLen < (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)))
	{
		return NULL;
	}

	memcpy(&(bmpContent->m_bitmapheader), fileContent->getBytes(), sizeof BITMAPFILEHEADER);
	memcpy(&(bmpContent->m_bitmapinfoheader), fileContent->getBytes() + (sizeof BITMAPFILEHEADER), sizeof BITMAPINFOHEADER);

	unsigned char secondChar = (bmpContent->m_bitmapheader.bfType & 0xff00) >> 8;
	unsigned char firstChar = (bmpContent->m_bitmapheader.bfType & 0x00ff);
	if (firstChar != 'B' || secondChar != 'M')
	{
		return NULL;
	}

	if (bmpContent->m_bitmapheader.bfOffBits > (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)))
	{
		//这个文件包括调色板的数据，所以分析不了
		return NULL;
	}

	if (!bmpContent->alloc(fileLen))
	{
		return NULL;
	}

	memcpy(bmpContent->m_bytes, fileContent->getBytes(), fileLen);

	return bmpContent;
}

shared_ptr<BMPContent> BmpHelper::readBmpFromHBitmap(const HBITMAP hBM)
{
	if (hBM == NULL)
	{
		return NULL;
	}

	BITMAP bm;
	int nRetValue = GetObject(hBM, sizeof(bm), &bm);
	if (nRetValue == NULL)
	{
		return NULL;
	}

	long size;
	size = bm.bmWidth * bm.bmHeight * ( bm.bmBitsPixel >> 3 ); // >> 3 means devided by 8

	unsigned char *buf = new unsigned char[size];
	if (buf == NULL)
	{
		return NULL;
	}

	GetBitmapBits((HBITMAP)hBM, size, buf);

	shared_ptr<BMPContent> bmContent = make_shared<BMPContent>();
	bmContent->alloc(bm.bmWidth * bm.bmHeight * 4); //it means to save the HBITMAP to 32 bits
	if (bmContent->m_bytes == NULL)
	{
		delete buf;
		buf = NULL;
		return NULL;
	}

	if (bm.bmBitsPixel != 32 && bm.bmBitsPixel != 24)// now, just support 24bits and 32bits perPixels
	{
		delete[] buf;
		buf = NULL;
		return NULL;
	}
	
	DWORD *lp_Canvas = (DWORD*)(bmContent->m_bytes);

	if (bm.bmBitsPixel == 32) {
		for (unsigned long y = 0; y < bm.bmHeight; y++) {
			for (unsigned long x = 0; x < bm.bmWidth; x++) {
				RGBQUAD * rgb = ((RGBQUAD *)((char*)(buf)
					+ bm.bmWidthBytes*y + x * sizeof(DWORD)));
				lp_Canvas[(bm.bmHeight - 1 - y)*bm.bmWidth + x] = *((DWORD *)rgb);
			}
		}
	}
	else if (bm.bmBitsPixel == 24) {
		for (unsigned long y = 0; y < bm.bmHeight; y++) {
			for (unsigned long x = 0; x < bm.bmWidth; x++) {

				RGBTRIPLE rgbi = *((RGBTRIPLE *)((char*)(buf)
					+ bm.bmWidthBytes*y + x * 3));

				RGBQUAD rgbq;
				rgbq.rgbRed = rgbi.rgbtRed;
				rgbq.rgbGreen = rgbi.rgbtGreen;
				rgbq.rgbBlue = rgbi.rgbtBlue;
				lp_Canvas[(bm.bmHeight - 1 - y)*bm.bmWidth + x] = *((DWORD *)(&rgbq));
			}
		}
	}

	// save bitmap file header
	bmContent->m_bitmapheader.bfType = 0x4d42;
	bmContent->m_bitmapheader.bfSize = 0;
	bmContent->m_bitmapheader.bfReserved1 = 0;
	bmContent->m_bitmapheader.bfReserved2 = 0;
	bmContent->m_bitmapheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// save bitmap info header
	bmContent->m_bitmapinfoheader.biSize = sizeof(BITMAPINFOHEADER);
	bmContent->m_bitmapinfoheader.biWidth = bm.bmWidth;
	bmContent->m_bitmapinfoheader.biHeight = bm.bmHeight;
	bmContent->m_bitmapinfoheader.biPlanes = 1;
	bmContent->m_bitmapinfoheader.biBitCount = 32;
	bmContent->m_bitmapinfoheader.biCompression = BI_RGB;
	bmContent->m_bitmapinfoheader.biSizeImage = 0;
	bmContent->m_bitmapinfoheader.biXPelsPerMeter = 0;
	bmContent->m_bitmapinfoheader.biYPelsPerMeter = 0;
	bmContent->m_bitmapinfoheader.biClrUsed = 0;
	bmContent->m_bitmapinfoheader.biClrImportant = 0;

	delete[] buf;
	buf = NULL;

	return bmContent;
}

bool BmpHelper::writeBmpToFile(shared_ptr<BMPContent> bmContent,const char * file)
{
	if (bmContent == NULL)
	{
		return false;
	}

	FILE *pFile = NULL;
	fopen_s(&pFile, file, "wb");
	if (pFile == NULL)
	{
		return false;
	}

	if (bmContent->m_bytes == NULL)
	{
		return false;
	}

	int width = bmContent->m_bitmapinfoheader.biWidth;
	int height = bmContent->m_bitmapinfoheader.biHeight;
	int n_Bits = bmContent->m_bitmapinfoheader.biBitCount;

	fwrite((char*)&(bmContent->m_bitmapheader), sizeof(BITMAPFILEHEADER), 1, pFile);
	fwrite((char*)&(bmContent->m_bitmapinfoheader), sizeof(BITMAPINFOHEADER), 1, pFile);
	fwrite((char*)bmContent->m_bytes, 1, (n_Bits >> 3)*width*height, pFile);
	fclose(pFile);

	return true;
}


