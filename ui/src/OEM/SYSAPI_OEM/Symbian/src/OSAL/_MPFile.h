//MPFile.h (Multi Platform File)
/*
 * ���� : Symbian�� Windows Mobile�� File Access�� �����ϱ� ���� Interface Claass
 * include : None
 * library : None 
 * �ۼ���      : PND Lab ����ȣ ���
 * History : 2008-08-08(�ʱ� ����)
 */

#ifndef MPFILE_SYMBIAN_H_
#define MPFILE_SYMBIAN_H_

#include <e32std.h>
#include <s32file.h>
#include <e32cmn.h>

#define MAX_PATH_LEN 256 

enum EStream
{
	EReadStream = 0,
	EWriteStream
};

typedef struct _FIND_DATA
{
	TInt32 att;
	TInt32 size;	
	TUint16 name[MAX_PATH_LEN]; 
}FIND_DATA, *PFIND_DATA;

#define FILE_ATT_NORMAL		0x00000000
#define FILE_ATT_HIDDEN		0x00000001
#define FILE_ATT_SYSTEM		0x00000002
#define FILE_ATT_DIRECTORY	0x00000004
#define FILE_ATT_ARCHIVE	0x00000010
#define FILE_ATT_READONLY	0x00000020


enum TAccessMode
{
	FILEMODEREAD = 0x01,
	FILEMODEWRITE = 0x02,
	FILEMODEBINARY = 0x04,	
	FILEMODETEXT = 0x08,
	FILEMODEALWAYSCREATE = 0x10
	
};


enum TSeekPoint
{
	EFileStart = 0,
	EFileCurrent,
	EFileEnd
};



class CMPFile
{
private:
	RFs mSession;
	
	RFileReadStream mFileReadStream;
	RFileWriteStream mFileWriteStream;
	
	TInt32 mReadMode;
	TInt32 mWriteMode;
	TBool mAlwaysCreate;
	
	CDir *mDir;
	TInt mDirIndex;
	
protected:
	void reset();
	

	TInt32 analyzeAccessMode(TInt32 &mode);
	

	
public:
	
	CMPFile();	
	
	/*
	 * ��� :  ������ ���� or Open�Ѵ�.		 
	 * arg : filePath - Create File Name, mode - Create Mode
	 * Return : void
	 * �ۼ��� : ����ȣc
	 * History : 08-08-08(�ʱ� ����)
	 */
	TBool CreateFile(const TUint16 *flePath, TInt32 mode);
	
	/*
	 * ��� :  ������ ���� or Open�Ѵ�.		 
	 * arg : filePath - Create File Name, mode - Create Mode
	 * Return : void
	 * �ۼ��� : ����ȣ
	 * History : 08-08-08(�ʱ� ����)
	 */
	TBool OpenFile(const TUint16 *filePath, TInt32 mode);
	

	/*
	 * ��� : ���� Read 
	 * arg : buffer - �о���� Buffer
	 * 		 bytesToRead - ���� byte
	 * 		 bufSize - Buffer Size 		   
	 * Return : �о�帰  Bytes 
	 * �ۼ��� : ����ȣ
	 * History : 08-08-08(�ʱ� ����)
	 */
	 TInt32 ReadFile(void * buffer, TInt32 bytesToRead, TInt32 bufSize);
	
	/*
	 * ��� : ���� Write. 
	 * arg : buffer - write buffer
	 * 		 byteToWrite - write�� bytes ��
	 * Return : 
	 * �ۼ��� : ����ȣ
	 * History : 08-08-08(�ʱ� ����)
	 */
	TInt32 WriteFile(const void * buffer, TInt32 byteToWrite);
	
	/*
	 * ��� : ���� Close. 
	 * arg : None
	 * Return : void
	 * �ۼ��� : ����ȣ
	 * History : 08-08-08(�ʱ� ����)
	 */
	void CloseFile();
	
	/*
	 * ��� : ���� Flush. 
	 * arg : None
	 * Return : void
	 * �ۼ��� : ����ȣ
	 * History : 08-08-08(�ʱ� ����)
	 */
	void Flush();
	
	
	/*
	 * ��� : Get File Size. 
	 * arg : None
	 * Return : File Size(Byte����)
	 * �ۼ��� : ����ȣ
	 * History : 08-08-08(�ʱ� ����)
	 */
	TInt32 GetSize();
	
	/*
	 * ��� : ���� Ž��. 
	 * arg : seekPoint - ���� ��ġ
	 * 	 	 offset - offset
	 * 		 stream - symbian ����(Windows������ 0)
	 * Return : void
	 * �ۼ��� : ����ȣ
	 * History : 08-08-08(�ʱ� ����)
	 */
	TInt32 Seek(TSeekPoint seekPoint, TInt32 offset, /*Symbian*/EStream stream);
	
	/*
	 * ��� : Get File Pointer 
	 * arg : stream - �ź�� ����(Windows������ 0)
	 * Return : Current File Pointer Location
	 * �ۼ��� : ����ȣ
	 * History : 08-08-08(�ʱ� ����)
	 */
	TInt32 GetPointer(/*Symbian*/EStream stream);
	
	/*
	 * ��� : ù��° File Ž��. 
	 * arg : filePath - Ž���� Path
	 * 		 extraArg - Windows or Symbian�� �ΰ����� argument
	 * 		 Windows������ LPWIN32_FIND_DATA 
	 * Return : Handle
	 * �ۼ��� : ����ȣ
	 * History : 08-08-08(�ʱ� ����)
	 */
	TBool FindFirstFile(const TUint16 *filePath, PFIND_DATA extraArg);
	
	
	/*
	 * ��� : ���� File Ž��. 
	 * arg : extraArg - Windows or Symbian�� �ΰ����� argument
	 * 		 Windows������ LPWIN32_FIND_DATA 
	 * Return : ���� // ����
	 * �ۼ��� : ����ȣ
	 * History : 08-08-08(�ʱ� ����)
	 */
	TBool FindNextFile(PFIND_DATA extraArg);
	
	
	/*
	 * ��� : File Ž�� Close. 
	 * arg : None 
	 * Return : ���� // ����
	 * �ۼ��� : ����ȣ
	 * History : 08-08-08(�ʱ� ����)
	 */
	TBool FindFileColse();
	
	/*
	 * ��� :  �Ҹ���
	 * arg : None 
	 * Return : None
	 * �ۼ��� : ����ȣ
	 * History : 08-08-08(�ʱ� ����)
	 */
	~CMPFile();	
	
	 /*
	 * ��� : Check Open Status 
	 * arg : None
	 * Return : TRUE -> Opened, FALSE -> Not Opened
	 * �ۼ��� : ����ȣ
	 * History : 08-08-30(�߰�)
	 */
	TBool IsOpened(); 
	 
	 /*
 	 * ��� : Delete File 
 	 * arg : fileName -> ������ file Name (path)
 	 * Return : TRUE -> success, FALSE -> fail
 	 * �ۼ��� : ����ȣ
 	 * History : 08-09-08(�߰�)
 	 */
 	static TBool fileDelete(TUint16 *fileName);	
 
	 /*
  	 * ��� : Create Directory 
  	 * arg : dirPath -> Create�� Directory Path
  	 * Return : TRUE -> success, FALSE -> fail
  	 * �ۼ��� : ����ȣ
  	 * History : 08-09-08(�߰�)
  	 */
  	 static TBool CreateDir(TUint16 *dirPath); 


	 /*
	 * ��� : Rename Directory 
  	 * arg : 
  	 * Return : TRUE -> success, FALSE -> fail
  	 * �ۼ��� : Zime
  	 * History : 08-12-18(�߰�)
  	 */
  	 static TBool fileRename(TUint16 *OldName, TUint16 *NewName);
};


#endif /*MPFILE_H_*/
