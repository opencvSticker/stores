
#include "bmp.h"

ToBmpFormat::ToBmpFormat():m_readBMPSuccessful(false)
{}

ToBmpFormat::ToBmpFormat(char * pFilePath):m_filePath(pFilePath),m_readBMPSuccessful(false)
{
	read(pFilePath);

}

void ToBmpFormat::read(char* fileName)
{
	m_filePath = fileName;
	if (!readFile())
	{
		return;
	}

	if (!readBMP())
	{
		return;
	}

	m_readBMPSuccessful = true;

}

bool ToBmpFormat::readFile()
{
	return m_bmpFileContent.read(m_filePath.c_str());
}

bool ToBmpFormat::readBMP()
{
	unsigned int index=0;

	if (m_bmpFileContent.getFileLength() < (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) ))
	{
		return false;
	}

	const char *fileContent = m_bmpFileContent.getFileContent();
	memcpy(&(m_bitmapheader),fileContent,sizeof BITMAPFILEHEADER);
	memcpy(&(m_bitmapinfoheader),fileContent + (sizeof BITMAPFILEHEADER),sizeof BITMAPINFOHEADER);

	unsigned char secondChar = (m_bitmapheader.bfType & 0xff00)>>8;
	unsigned char firstChar = (m_bitmapheader.bfType & 0x00ff);
	if ( firstChar != 'B' || secondChar != 'M')
	{
		return false;
	}

	if ( m_bitmapheader.bfOffBits >  (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) ))
	{
		//这个文件包括调色板的数据，所以分析不了
		return false;
	}
    
    
	return true;
}

//index is just the pixel in the bmp
DWORD ToBmpFormat::getPixel(unsigned int index)
{
	if (!m_readBMPSuccessful)
	{
		return 0xffffffff;
	}

	const char * fileContent = m_bmpFileContent.getFileContent();
	if (fileContent!=NULL)
	{
		unsigned int paddingCount = 0;
		unsigned int remainder = (getBMPWidth()*(m_bitmapinfoheader.biBitCount/8))%4;
		if (remainder>0)
		{
			paddingCount = 4 - remainder;
		}

		unsigned int counterInWidth = (index - 1) / getBMPWidth();
        
		unsigned int indexInFile = m_bitmapheader.bfOffBits +( index -1) *(m_bitmapinfoheader.biBitCount/8) 
			                                      + counterInWidth * paddingCount;
		BYTE R = fileContent[indexInFile+2];
		BYTE G = fileContent[indexInFile+1];
		BYTE B = fileContent[indexInFile+0];
		return RGB(R,G,B);
	}

	return 0xffffffff;

}

LPBYTE* ToBmpFormat::getBits()
{
	const char * fileContent = m_bmpFileContent.getFileContent();
	if (fileContent == NULL)
	{
		return NULL;
	}
	unsigned int indexInFile = m_bitmapheader.bfOffBits;

	return (LPBYTE*)&fileContent[indexInFile + 0];;

}

BITMAPINFOHEADER ToBmpFormat::getBitMapInfoHeader()
{
	return m_bitmapinfoheader;
}

unsigned int ToBmpFormat::getBMPWidth()
{
	if (m_readBMPSuccessful)
	{
		return m_bitmapinfoheader.biWidth;
	}
	return 0;

}
unsigned int ToBmpFormat::getBMPHeight()
{
	if (m_readBMPSuccessful)
	{
		return m_bitmapinfoheader.biHeight;
	}

	return 0;
}