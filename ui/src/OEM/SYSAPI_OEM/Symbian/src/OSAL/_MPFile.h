//MPFile.h (Multi Platform File)
/*
 * 목적 : Symbian과 Windows Mobile의 File Access를 지원하기 위한 Interface Claass
 * include : None
 * library : None 
 * 작성자      : PND Lab 윤병호 사원
 * History : 2008-08-08(초기 버전)
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
	 * 기능 :  파일을 생성 or Open한다.		 
	 * arg : filePath - Create File Name, mode - Create Mode
	 * Return : void
	 * 작성자 : 윤병호c
	 * History : 08-08-08(초기 버전)
	 */
	TBool CreateFile(const TUint16 *flePath, TInt32 mode);
	
	/*
	 * 기능 :  파일을 생성 or Open한다.		 
	 * arg : filePath - Create File Name, mode - Create Mode
	 * Return : void
	 * 작성자 : 윤병호
	 * History : 08-08-08(초기 버전)
	 */
	TBool OpenFile(const TUint16 *filePath, TInt32 mode);
	

	/*
	 * 기능 : 파일 Read 
	 * arg : buffer - 읽어들일 Buffer
	 * 		 bytesToRead - 읽을 byte
	 * 		 bufSize - Buffer Size 		   
	 * Return : 읽어드린  Bytes 
	 * 작성자 : 윤병호
	 * History : 08-08-08(초기 버전)
	 */
	 TInt32 ReadFile(void * buffer, TInt32 bytesToRead, TInt32 bufSize);
	
	/*
	 * 기능 : 파일 Write. 
	 * arg : buffer - write buffer
	 * 		 byteToWrite - write할 bytes 수
	 * Return : 
	 * 작성자 : 윤병호
	 * History : 08-08-08(초기 버전)
	 */
	TInt32 WriteFile(const void * buffer, TInt32 byteToWrite);
	
	/*
	 * 기능 : 파일 Close. 
	 * arg : None
	 * Return : void
	 * 작성자 : 윤병호
	 * History : 08-08-08(초기 버전)
	 */
	void CloseFile();
	
	/*
	 * 기능 : 파일 Flush. 
	 * arg : None
	 * Return : void
	 * 작성자 : 윤병호
	 * History : 08-08-08(초기 버전)
	 */
	void Flush();
	
	
	/*
	 * 기능 : Get File Size. 
	 * arg : None
	 * Return : File Size(Byte단위)
	 * 작성자 : 윤병호
	 * History : 08-08-08(초기 버전)
	 */
	TInt32 GetSize();
	
	/*
	 * 기능 : 파일 탐색. 
	 * arg : seekPoint - 기준 위치
	 * 	 	 offset - offset
	 * 		 stream - symbian 전용(Windows에서는 0)
	 * Return : void
	 * 작성자 : 윤병호
	 * History : 08-08-08(초기 버전)
	 */
	TInt32 Seek(TSeekPoint seekPoint, TInt32 offset, /*Symbian*/EStream stream);
	
	/*
	 * 기능 : Get File Pointer 
	 * arg : stream - 신비안 전용(Windows에서는 0)
	 * Return : Current File Pointer Location
	 * 작성자 : 윤병호
	 * History : 08-08-08(초기 버전)
	 */
	TInt32 GetPointer(/*Symbian*/EStream stream);
	
	/*
	 * 기능 : 첫번째 File 탐색. 
	 * arg : filePath - 탐색할 Path
	 * 		 extraArg - Windows or Symbian용 부가적인 argument
	 * 		 Windows에서는 LPWIN32_FIND_DATA 
	 * Return : Handle
	 * 작성자 : 윤병호
	 * History : 08-08-08(초기 버전)
	 */
	TBool FindFirstFile(const TUint16 *filePath, PFIND_DATA extraArg);
	
	
	/*
	 * 기능 : 다음 File 탐색. 
	 * arg : extraArg - Windows or Symbian용 부가적인 argument
	 * 		 Windows에서는 LPWIN32_FIND_DATA 
	 * Return : 성공 // 실패
	 * 작성자 : 윤병호
	 * History : 08-08-08(초기 버전)
	 */
	TBool FindNextFile(PFIND_DATA extraArg);
	
	
	/*
	 * 기능 : File 탐색 Close. 
	 * arg : None 
	 * Return : 성공 // 실패
	 * 작성자 : 윤병호
	 * History : 08-08-08(초기 버전)
	 */
	TBool FindFileColse();
	
	/*
	 * 기능 :  소멸자
	 * arg : None 
	 * Return : None
	 * 작성자 : 윤병호
	 * History : 08-08-08(초기 버전)
	 */
	~CMPFile();	
	
	 /*
	 * 기능 : Check Open Status 
	 * arg : None
	 * Return : TRUE -> Opened, FALSE -> Not Opened
	 * 작성자 : 윤병호
	 * History : 08-08-30(추가)
	 */
	TBool IsOpened(); 
	 
	 /*
 	 * 기능 : Delete File 
 	 * arg : fileName -> 삭제할 file Name (path)
 	 * Return : TRUE -> success, FALSE -> fail
 	 * 작성자 : 윤병호
 	 * History : 08-09-08(추가)
 	 */
 	static TBool fileDelete(TUint16 *fileName);	
 
	 /*
  	 * 기능 : Create Directory 
  	 * arg : dirPath -> Create할 Directory Path
  	 * Return : TRUE -> success, FALSE -> fail
  	 * 작성자 : 윤병호
  	 * History : 08-09-08(추가)
  	 */
  	 static TBool CreateDir(TUint16 *dirPath); 


	 /*
	 * 기능 : Rename Directory 
  	 * arg : 
  	 * Return : TRUE -> success, FALSE -> fail
  	 * 작성자 : Zime
  	 * History : 08-12-18(추가)
  	 */
  	 static TBool fileRename(TUint16 *OldName, TUint16 *NewName);
};


#endif /*MPFILE_H_*/
