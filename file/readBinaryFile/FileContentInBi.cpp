
#include "stdafx.h"
#include"FileContentInBi.h"

#define  _CRT_SECURE_NO_WARNINGS


//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
ReadFileContentInBi::ReadFileContentInBi()
{
}

ReadFileContentInBi::~ReadFileContentInBi()
{
}

shared_ptr<FileContentInBi> ReadFileContentInBi::getFileContent(const char * file)
{
	return read(file);
}

unsigned int ReadFileContentInBi::getFileLength(const char * file)
{
	FILE *pFile = NULL;
	fopen_s(&pFile, file, "rb");
    if (pFile == NULL)
    {
     printf("open file failed.\n");
     return -1;
    }
	
	shared_ptr<FileContentInBi> fileContent = make_shared<FileContentInBi>();
   
    fseek(pFile,0,SEEK_END);
   
    unsigned int length = ftell(pFile);
	
	fclose(pFile);
    return length;
}

shared_ptr<FileContentInBi> ReadFileContentInBi::read(const char * file)
{
	FILE *pFile = NULL;
	fopen_s(&pFile, file, "rb");
    if (pFile == NULL)
    {
     printf("open file failed.\n");
     return NULL;
    }
	
	shared_ptr<FileContentInBi> fileContent = make_shared<FileContentInBi>();
   
    fseek(pFile,0,SEEK_END);
   
    unsigned int length = ftell(pFile);
    printf("file length is %d.\n",length);
   
    fileContent->alloc(length);
   
    fseek(pFile,0,SEEK_SET);
    fread(fileContent->m_bytes,1,length,pFile);
      
    fclose(pFile);

    return fileContent;
}

//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//

bool WriteFileContentInBi::write(shared_ptr<FileContentInBi> fileContent,const unsigned fileLen, const char * file)
{
	if (fileContent == NULL)
	{
		return false;
	}

	FILE *pFile = NULL;
	fopen_s(&pFile, file, "wb");
	if (pFile == NULL)
	{
		return false;
	}

	if (fileContent->getBytes() != NULL)
	{
		fwrite(fileContent->getBytes(), fileLen, 1, pFile);
	}

	fclose(pFile);
	
	return true;
}