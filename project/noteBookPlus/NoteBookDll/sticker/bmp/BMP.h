
#include"ReadFileContentInBi.h"
#include<string>
#include <windows.h>

#ifndef To_bmp_2016_09_23
#define To_bmp_2016_09_23


//read a file and read out the bmp data
class ToBmpFormat
{
public:
	ToBmpFormat();
	ToBmpFormat(char *);

	void read(char*);

	unsigned int getBMPWidth();
	unsigned int getBMPHeight();
	LPBYTE* ToBmpFormat::getBits();
	BITMAPINFOHEADER getBitMapInfoHeader();
	DWORD getPixel(unsigned int index);

	

private:
	bool readFile();
	bool readBMP();

	std::string m_filePath;
	ReadFileContentInBi m_bmpFileContent;

	BITMAPFILEHEADER m_bitmapheader;
	BITMAPINFOHEADER m_bitmapinfoheader;

	bool m_readBMPSuccessful;

};

#endif