#include<stdio.h>
#include<memory>

using namespace std;

#ifndef ReadFileContentInBi_2016_9_22
#define ReadFileContentInBi_2016_9_22
//read the file content In Binary format,
//and strore all the content to a char *
//if read failed, the read function will return -1.
//if read successfully, the read function will return the length of the file.

struct FileContentInBi
{
	FileContentInBi(){m_bytes = NULL;}
	~FileContentInBi()
	{
		dealloc();
	}
	
	void alloc(unsigned int size )
	{
		dealloc();
		m_bytes = new char[size];
	}
	
	const char * getBytes()
	{ 
	    return m_bytes;
	}
private :
    void dealloc()
	{
		if(m_bytes != NULL) 
			delete [] m_bytes;	
		m_bytes = NULL;
	}
	
private:	
	char * m_bytes;
	
friend class ReadFileContentInBi;
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
class ReadFileContentInBi
{
	private :
		ReadFileContentInBi();
		~ReadFileContentInBi();
	public:
		static shared_ptr<FileContentInBi> read(const char * file);		
		static unsigned int getFileLength(const char * file);

    private :
        static shared_ptr<FileContentInBi> getFileContent(const char * file);

};

//------------------------------------------------------------------------
//------------------------------------------------------------------------

class WriteFileContentInBi
{
public:
	//the write will re-write all the file even though the file existed.
	static bool write(shared_ptr<FileContentInBi> fileContent, const unsigned fileLen, const char * file);

private:
	WriteFileContentInBi() {}
};

#endif