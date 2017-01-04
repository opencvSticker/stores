

#include"ReadFileContentInBi.h"

ReadFileContentInBi::ReadFileContentInBi():m_fileContent(NULL),m_fileLength(-1)
{
}

ReadFileContentInBi::~ReadFileContentInBi()
{
	if( m_fileContent != NULL)
	{
	    delete [] m_fileContent;
	}
}

const char * ReadFileContentInBi::getFileContent()
{
	return m_fileContent;
}

unsigned int ReadFileContentInBi::getFileLength()
{
    return m_fileLength;
}

bool ReadFileContentInBi::read(const char * file)
{
    FILE *pFile = fopen(file, "rb");
    if (pFile == NULL)
    {
     printf("open file failed.\n");
     return false;
    }
   
    fseek(pFile,0,SEEK_END);
   
    unsigned int length = ftell(pFile);
    printf("file length is %d.\n",length);

    m_fileLength = length;
   
    m_fileContent = new char[length +1];
   
    fseek(pFile,0,SEEK_SET);
    fread(m_fileContent,1,length,pFile);
   
    m_fileContent[length] = 0;
   
    fclose(pFile);

    return true;
}