#include<stdio.h>
#include <windows.h>

#ifndef ReadFileContentInBi_2016_9_22
#define ReadFileContentInBi_2016_9_22
//read the file content In Binary format,
//and strore all the content to a char *
//if read failed, the read function will return -1.
//if read successfully, the read function will return the length of the file.
class ReadFileContentInBi
{
	public :
		ReadFileContentInBi();
		~ReadFileContentInBi();

		bool read(const char * file);
		const char * getFileContent();
		unsigned int getFileLength();        

	private :
		char * m_fileContent;
		unsigned int m_fileLength;

};

#endif