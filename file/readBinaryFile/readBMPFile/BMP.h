
#include"FileContentInBi.h"
#include <windows.h>
#include<string>
#include <memory>
#include <wingdi.h>
using namespace std;

#ifndef To_bmp_2016_09_23
#define To_bmp_2016_09_23

//just support RGB bmp map
class BMPContent
{
public:
	BMPContent();
	~BMPContent();

	//the iWidth and iHeight both are begin from 0
	COLORREF getPixel(unsigned int index);
	COLORREF getPixel(unsigned int iWidth, unsigned int iHeight);
	unsigned int getBMPHeight();
	unsigned int getBMPWidth();

private:
	bool alloc(unsigned int size);
	void dealloc();
private:
	BITMAPFILEHEADER m_bitmapheader;
	BITMAPINFOHEADER m_bitmapinfoheader;
	char *m_bytes;
	
friend class BmpHelper;

};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------


//read a file and read out the bmp data
class BmpHelper
{
private:
	BmpHelper();
public:
	static shared_ptr<BMPContent> readBmpFile(const char *file);
	static shared_ptr<BMPContent> readBmpFromHBitmap(const HBITMAP hBM);
	static bool writeBmpToFile(shared_ptr<BMPContent> bmpContent, const char * file);
};

#endif