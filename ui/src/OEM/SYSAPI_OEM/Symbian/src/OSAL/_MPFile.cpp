#include "_MPFile.h"


CMPFile::CMPFile()
{
	
	CleanupClosePushL(mSession);
	
	User::LeaveIfError(mSession.Connect());
	User::LeaveIfError(mSession.ShareAuto());
	
    reset();
    
    
    CleanupStack::Pop();
}

CMPFile::~CMPFile()
{		
	//CleanupStack::PopAndDestroy(&mSession);
	if(mDir != NULL)
	{
		delete mDir;
	}
	
	mSession.Close();
}


void CMPFile::reset()
{
	mReadMode = 0;
	mWriteMode = 0;
	
	mAlwaysCreate = FALSE;
	
	mDir = NULL;
	mDirIndex = 0;

	
}

TBool CMPFile::CreateFile(const TUint16 *filePath, TInt32 mode)
{
	TInt err = KErrNone;
	
	TBuf16<MAX_PATH_LEN> filePathBuf = (const TUint16*)filePath;	
	TInt accessMode = analyzeAccessMode(mode);
	
	if(mAlwaysCreate)
	{	
#if 0
		User::LeaveIfError( mFileWriteStream.Replace(mSession, filePathBuf, accessMode) );
#endif
		err = mFileWriteStream.Replace(mSession, filePathBuf, accessMode);
		if(err != KErrNone)
		{
			return FALSE;
		}		
		
		if(mReadMode != 0)
		{
#if 0
			User::LeaveIfError( mFileReadStream.Open(mSession, filePathBuf, accessMode) );
#endif
			err = mFileReadStream.Open(mSession, filePathBuf, accessMode);
			if(err != KErrNone)
			{
				return FALSE;
			}
			
		}				
		
		return TRUE;
	}
		
	if(mFileWriteStream.Create(mSession, filePathBuf, accessMode) == KErrNone)
	{			
		if(mReadMode != 0)
		{
#if 0
			User::LeaveIfError( mFileReadStream.Open(mSession, filePathBuf, accessMode) );
#endif
			err = mFileReadStream.Open(mSession, filePathBuf, accessMode);
			if(err != KErrNone)
			{
				return FALSE;
			}
		}
		return TRUE;
		
	}
	else
	{		
		if( OpenFile(filePath, mode) )	
			return TRUE;
	}
	
	return FALSE;	
	
}

TBool CMPFile::OpenFile(const TUint16 *filePath, TInt32 mode)
{
	TInt err = KErrNone;
	
	TBuf16<MAX_PATH_LEN> filePathBuf = (const TUint16*)filePath;	
	
	//User::LeaveIfError( mFile.Open(mSession, filePathBuf, analyzeAccessMode(mode)));
	TInt accessMode = analyzeAccessMode(mode);	
	
	if(mReadMode != 0)
	{
#if 0
		User::LeaveIfError( mFileReadStream.Open(mSession, filePathBuf, accessMode) );
#endif
		err = mFileReadStream.Open(mSession, filePathBuf, accessMode);
		if(err != KErrNone)
		{
			return FALSE;
		}
		
		return TRUE;
	}
	if(mWriteMode != 0)
	{
#if 0
		User::LeaveIfError( mFileWriteStream.Open(mSession, filePathBuf, accessMode) );
#endif
		err = mFileWriteStream.Open(mSession, filePathBuf, accessMode);
		if(err != KErrNone)
		{
			return FALSE;
		}
		
		
#if 0
		CleanupStack::PushL(&mFileWriteStream);
		mFileWriteStream.Sink()->SeekL(mFileWriteStream.Sink()->EWrite, EStreamEnd, 0);		
		CleanupStack::Pop(&mFileWriteStream);
#endif
		
		return TRUE;
	}
	
	return FALSE;
	

}

TInt32 CMPFile::ReadFile(void *buffer, TInt32 bytesToRead, TInt32 bufSize)
{
	//TPtr8 a( (TUint8 *)buffer, bufSize); or
	TPtr8 a( (TUint8 *)buffer, 0, bufSize);
	
	CleanupClosePushL(mFileReadStream);	
	 
	TUint fileSize = 0;
	
	fileSize = (mFileReadStream.Source())->SizeL();
	
	TStreamPos curPos;
	curPos = mFileReadStream.Source()->TellL(mFileReadStream.Source()->ERead);
	
	//mFileReadStream.Source()->SeekL(TRead, TStreamMark, );
	
	TUint readSize = fileSize - curPos.Offset();
	
	if(readSize > bytesToRead)
		readSize = bytesToRead;	
	
	else if(readSize == 0) // EOF
	{
		CleanupStack::Pop(&mFileReadStream);
		return -1;
	}
		
	//CleanupStack::PushL(&mFileReadStream);	
	
	mFileReadStream.ReadL(a, readSize);
	
	CleanupStack::Pop();
		
	// for debug
	//TInt bb = a.Length();
	//
	
	return a.Length();
	

}

TInt32 CMPFile::WriteFile(const void * buffer, TInt32 byteToWrite)
{	
	TInt32 curSize = 0;
	TInt32 afterSize = 0;
	
	CleanupClosePushL(mFileWriteStream);
	
	curSize = GetSize();
	
	mFileWriteStream.WriteL( (TUint8 *)buffer, byteToWrite);
	mFileWriteStream.CommitL();
	
	afterSize = GetSize();
		
	CleanupStack::Pop();
	
	return afterSize - curSize;	
}

void CMPFile::CloseFile()
{	
	
	mFileReadStream.Close();
	mFileWriteStream.Close();
//	mSession.Close();
	mReadMode = 0;
	mWriteMode = 0;
}

void CMPFile::Flush()
{
	if( mFileWriteStream.Sink() != NULL )
	{
		CleanupClosePushL(mFileWriteStream);
		
		mFileWriteStream.CommitL();
		
		CleanupStack::Pop();
	}
	
}

TInt32 CMPFile::GetSize()
{
	TInt32 readFileSize = 0;
	TInt32 writeFileSize = 0;
	
	if(mReadMode == 0 && mWriteMode == 0)
		return -1;
	
	if(mReadMode)
	{
		//CleanupStack::PushL(&mFileReadStream);
		CleanupClosePushL(mFileReadStream);
		if( mFileReadStream.Source() ) 
			readFileSize = mFileReadStream.Source()->SizeL();
		else
			readFileSize = 0;
		CleanupStack::Pop();
	}
	if(mWriteMode)
	{
		CleanupClosePushL(mFileWriteStream);
		if( mFileWriteStream.Sink() )
			writeFileSize = mFileWriteStream.Sink()->SizeL();
		else
			writeFileSize = 0;
		CleanupStack::Pop();
	}
	
	if(writeFileSize > readFileSize)
		return writeFileSize;
	else 
		return readFileSize;	
}

TInt32 CMPFile::Seek(TSeekPoint seekPoint, TInt32 offset, EStream stream)
{
	if(mReadMode == 0 && mWriteMode ==0)
		return -1;
	
	if( (seekPoint == EFileStart) && (offset<0) )
	{
		return -1;
	}
	
	if( (seekPoint == EFileEnd) && (offset>0) )
	{
		return -1;
	}
	
	TStreamPos curPos;	
	
	if(mReadMode && stream == EReadStream)	
	{
		CleanupClosePushL(mFileReadStream);
		curPos = mFileReadStream.Source()->SeekL(mFileReadStream.Source()->ERead, (TStreamLocation)seekPoint, offset);
		CleanupStack::Pop();
	}
	else if(mWriteMode && stream == EWriteStream)
	{
		CleanupClosePushL(mFileWriteStream);
		curPos = mFileWriteStream.Sink()->SeekL(mFileWriteStream.Sink()->EWrite, (TStreamLocation)seekPoint, offset);
		CleanupStack::Pop();
	}
	
	//for debug
	//TInt aa = curPos.Offset();
	//
	
	return curPos.Offset();
}

TInt32 CMPFile::GetPointer(EStream stream)
{
	if(mReadMode == 0 && mWriteMode ==0)
			return -1;
	
	TStreamPos curPos;
	
	if(mReadMode && stream == EReadStream)	
	{
		CleanupClosePushL(mFileReadStream);
		curPos = mFileReadStream.Source()->TellL(MStreamBuf::ERead);
		CleanupStack::Pop();
	}
	else if(mWriteMode && stream == EWriteStream)
	{
		CleanupClosePushL(mFileWriteStream);
		curPos = mFileWriteStream.Sink()->TellL(MStreamBuf::EWrite);
		CleanupStack::Pop();
	}
	
	//for debug
	//TInt aa = curPos.Offset();
	//
	
	return curPos.Offset();
	
	
}

TBool CMPFile::FindFirstFile(const TUint16 *filePath, PFIND_DATA extraArg)
{
	TBuf16<MAX_PATH_LEN> filePathBuf = (const TUint16*)filePath;
	if(mDir != NULL)
	{
		return FALSE;
	}

	CleanupStack::PushL((TAny*)mDir);
	
	Mem::FillZ(extraArg, sizeof(PFIND_DATA));
	
#if 0
	User::LeaveIfError( mSession.GetDir(
										   filePathBuf,
	                        			   KEntryAttMaskSupported,	    		 			
	                        			   ESortByName,
	                        			   mDir
	                        			   )
	                   );
#else
	TInt32 ret = mSession.GetDir(
								filePathBuf,
								KEntryAttMaskSupported,	    		 			
								ESortByName,
								mDir
    			   			   );	
	if(ret != KErrNone)
	{
		return FALSE;
	}
#endif

	int  iCnt = mDir[mDirIndex].Count(); // 폴더안에 list개수.(값이 없으면.False)
	if(iCnt == 0)
		return FALSE;

	
	if((*mDir)[mDirIndex].IsHidden())
	{
		((PFIND_DATA)extraArg)->att |= FILE_ATT_HIDDEN;
	}
	
	if((*mDir)[mDirIndex].IsSystem())
	{
		((PFIND_DATA)extraArg)->att |= FILE_ATT_SYSTEM;
	}
	
	if((*mDir)[mDirIndex].IsDir())
	{
		((PFIND_DATA)extraArg)->att |= FILE_ATT_DIRECTORY;
	}
	
	if((*mDir)[mDirIndex].IsArchive())
	{
		((PFIND_DATA)extraArg)->att |= FILE_ATT_ARCHIVE;
	}
	
	if((*mDir)[mDirIndex].IsReadOnly())
	{
		((PFIND_DATA)extraArg)->att |= FILE_ATT_READONLY;
	}
	
	Mem::Copy( (TAny *)(((PFIND_DATA)extraArg)->name), (TAny *)((*mDir)[mDirIndex]).iName.Ptr(), ((*mDir)[mDirIndex]).iName.Length() * sizeof(TUint16));
		
	((PFIND_DATA)extraArg)->size = (TInt32)((*mDir)[mDirIndex].iSize);	
	
	mDirIndex++;
	
//	CleanupStack::Pop((TAny*)mDir);
	CleanupStack::Pop();
	return TRUE;
	
}

TBool CMPFile::FindNextFile(PFIND_DATA extraArg)
{

	if(mDir == NULL)
	{
		return FALSE;		
	}

	CleanupStack::PushL((TAny*)mDir);
	
	if( (mDir->Count()) == mDirIndex )
	{
		return FALSE;
	}	
	
	
	Mem::FillZ(extraArg, sizeof(FIND_DATA));
		
		
	if((*mDir)[mDirIndex].IsHidden())
	{
		((PFIND_DATA)extraArg)->att |= FILE_ATT_HIDDEN;
	}
	
	if((*mDir)[mDirIndex].IsSystem())
	{
		((PFIND_DATA)extraArg)->att |= FILE_ATT_SYSTEM;
	}
	
	if((*mDir)[mDirIndex].IsDir())
	{
		((PFIND_DATA)extraArg)->att |= FILE_ATT_DIRECTORY;
	}
	
	if((*mDir)[mDirIndex].IsArchive())
	{
		((PFIND_DATA)extraArg)->att |= FILE_ATT_ARCHIVE;
	}
	
	if((*mDir)[mDirIndex].IsReadOnly())
	{
		((PFIND_DATA)extraArg)->att |= FILE_ATT_READONLY;
	}
	
	Mem::Copy( (TAny *)(((PFIND_DATA)extraArg)->name), (TAny *)((*mDir)[mDirIndex]).iName.Ptr(), ((*mDir)[mDirIndex]).iName.Length() * sizeof(TUint16));
		
	((PFIND_DATA)extraArg)->size = (TInt32)((*mDir)[mDirIndex].iSize);	
	
	mDirIndex++;
	
//	CleanupStack::Pop((TAny*)mDir);
	CleanupStack::Pop();
	return true;	
	
}

TBool CMPFile::FindFileColse()
{
	if(mDir == NULL)
	{
		return FALSE;
	}
	else
	{
		delete mDir;
		mDir = NULL;
	}
	
	return TRUE;
}

TInt32 CMPFile::analyzeAccessMode(TInt32 &mode)
{
	TUint32 ret = 0;
		
	if( (mode & FILEMODEREAD) == FILEMODEREAD)
	{
		ret |= EFileRead;
		mReadMode = 1;
	}
	if( (mode & FILEMODEWRITE) == FILEMODEWRITE)
	{
		ret |= EFileWrite;
		mWriteMode = 1;
	}
	if( (mode & FILEMODEALWAYSCREATE) == FILEMODEALWAYSCREATE)
	{
		mAlwaysCreate = TRUE;
	}
	
	if( (mode & FILEMODEBINARY) == FILEMODEBINARY)
	{
		ret |= EFileStream;
		ret |= EFileShareAny;
		return ret;
	}
	if( (mode & FILEMODETEXT) == FILEMODETEXT)
	{
		ret |= EFileStreamText;
	}	
	
	ret |= EFileShareAny;		
	
	
	return ret;
}

TBool CMPFile::IsOpened()
{
	if(mReadMode || mWriteMode)
		return TRUE;
	else 
		return FALSE;
}

TBool CMPFile::fileDelete(TUint16 *fileName)
{	
	TBuf16<MAX_PATH_LEN> fileNameBuf = (const TUint16*)fileName;
	
	RFs fSession;
	
	CleanupClosePushL(fSession);		
	
	if( (fSession.Connect() == KErrNone) && (fSession.Delete(fileNameBuf) == KErrNone) )
	{
		CleanupStack::Pop();
		fSession.Close();
		
		return TRUE;
	}
    
	/*
	if( fSession.Delete() == KErrNone )
	{
		CleanupStack::Pop();
		fSession.Close();
		
		return TRUe;
	}
	*/
    
    CleanupStack::Pop();
    fSession.Close();
    
    return FALSE;
}

TBool CMPFile::CreateDir(TUint16 *dirPath)
{
	TBuf16<MAX_PATH_LEN> dirPathBuf = (const TUint16*)dirPath;
		
	RFs fSession;
	
	CleanupClosePushL(fSession);		
	
	if( (fSession.Connect() == KErrNone) && (fSession.MkDirAll(dirPathBuf) == KErrNone) )
	{
		CleanupStack::Pop();
		fSession.Close();
		
		return TRUE;
	}
    
	/*
	if( fSession.Delete() == KErrNone )
	{
		CleanupStack::Pop();
		fSession.Close();
		
		return TRUe;
	}
	*/
    
    CleanupStack::Pop();
    fSession.Close();
    
    return FALSE;
}

TBool CMPFile::fileRename(TUint16 *OldName, TUint16 *NewName)
{	
	TBuf16<MAX_PATH_LEN> OldNameBuf = (const TUint16*)OldName;
	TBuf16<MAX_PATH_LEN> NewNameBuf = (const TUint16*)NewName;
	
	RFs fSession;
	
	CleanupClosePushL(fSession);		
	
	if(fSession.Connect() == KErrNone)
	{
		if(fSession.Rename(OldNameBuf, NewNameBuf) == KErrNone)
		{
			CleanupStack::Pop();
			fSession.Close();
			return TRUE;
		}
	}
	
	
	CleanupStack::Pop();
	fSession.Close();
	
	return FALSE;
}

